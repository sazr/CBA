/*
Copyright (c) 2016 Sam Zielke-Ryner All rights reserved.

For job opportunities or to work together on projects please contact
myself via Github:   https://github.com/sazr

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. The source code, API or snippets cannot be used for commercial
purposes without written consent from the author.

THIS SOFTWARE IS PROVIDED ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "AppUsageComponent.h"

// Class Property Implementation //


// Static Function Implementation //
BOOL CALLBACK AppUsageComponent::monitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	int *nMonitors = (int*)dwData;
	(*nMonitors)++;
	return TRUE;
}

// Function Implementation //
AppUsageComponent::AppUsageComponent(const std::weak_ptr<IApp>& app, const tstring& usageLogFileDirectory, const tstring& domain, const tstring& domainScript) 
	: Component(app), domain(domain), domainScript(domainScript),
	interactionIndex(0), uploadUid(Status::registerState(_T("Upload Uid")).state)
{
	upldCmp = addComponent<UploadComponent>(app);
	registerEvents();

	TCHAR szPath[MAX_PATH];
	if (SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath) == S_OK) {
		CreateDirectory((tstring(szPath) + _T("\\") + usageLogFileDirectory).c_str(), NULL);
		usageFilePath = tstring(szPath) + _T("\\") + usageLogFileDirectory + _T("\\usage.ini");
	}
}

AppUsageComponent::~AppUsageComponent()
{

}

Status AppUsageComponent::preInit(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	// Get Usage section number of keys
	std::vector<tstring> sectionNames;
	WinUtilityComponent::getINISectionNames(usageFilePath, sectionNames);
	interactionIndex = sectionNames.size();
	
	// if INI file doesn't exist: create and write SubjectInformation to it
	bool subjectInfoCatalogued = std::find(sectionNames.begin(), sectionNames.end(), _T("SubjectInformation")) != sectionNames.end();

	if (!subjectInfoCatalogued)
		catalogueSubjectInformation();

	dumpInteractions();

	// upload usage data
	if (!sectionNames.empty())
		upldCmp->uploadFile(domain, domainScript, usageFilePath, uploadUid);

	return S_SUCCESS;
}

Status AppUsageComponent::init(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	return S_SUCCESS;
}

Status AppUsageComponent::terminate(const IEventArgs& evtArgs)
{
	
	return S_SUCCESS;
}

Status AppUsageComponent::registerEvents()
{
	registerEvent(Win32App::WM_CUSTOM_PRE_CREATE, &AppUsageComponent::preInit);
	registerEvent(WM_CLOSE, &AppUsageComponent::terminate);
	registerEvent(DispatchWindowComponent::translateMessage(uploadUid, UploadComponent::WM_UPLOAD_COMPLETE), &AppUsageComponent::onUploadComplete);
	return S_SUCCESS;
}

Status AppUsageComponent::catalogueEvent(HWND hwnd, UINT msg, const tstring& annotation)
{
	// if annotation too long
	if (annotation.length() >= USEAGE_INFO_BUF_SIZE)
		return S_UNDEFINED_ERROR;

	registerEventLambda<Component>(DispatchWindowComponent::translateMessage(hwnd, msg), 
		[hwnd, msg, annotation, this](const IEventArgs& evtArgs)->Status {
			
			return this->writeUsageLog(evtArgs, hwnd, msg, annotation);
		
			//return S_SUCCESS;
	});

	return S_SUCCESS;
}

Status AppUsageComponent::catalogueData(const tstring& annotation)
{
	const WinEventArgs args(NULL, NULL, -1, -1);
	return writeUsageLog(args, NULL, -1, annotation);
}

Status AppUsageComponent::writeUsageLog(const IEventArgs& evtArgs, HWND hwnd, UINT msg, const tstring& annotation)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	
	UsageInformation usageInfo;
	usageInfo.msg = msg;
	usageInfo.wParam = args.wParam;
	usageInfo.lParam = args.lParam;
	time(&usageInfo.timeStamp); 
	
	if (annotation.length() >= USEAGE_INFO_BUF_SIZE)
		_tcscpy_s(usageInfo.annotation, _T("Annotation too long"));
	else _tcscpy_s(usageInfo.annotation, annotation.c_str());

	TCHAR sectionName[100];
	/*_itot_s(interactionIndex, sectionName, 100);*/
	_itot(interactionIndex, sectionName, 100);

	if (!WritePrivateProfileStruct(sectionName, _T("Interaction"),
		(LPVOID)&(usageInfo), sizeof(usageInfo), usageFilePath.c_str())) {
		output(_T("Failed to write UsageInformation to INI file\n"));
		return S_UNDEFINED_ERROR;
	}

	interactionIndex++;
	return S_SUCCESS;
}

Status AppUsageComponent::catalogueSubjectInformation()
{
	tstring uid;
	std::stringstream ss;

	SubjectInformation subjInfo;
	subjInfo.versionInfo.dwOSVersionInfoSize = sizeof(subjInfo.versionInfo);
	subjInfo.nMonitors = 0;

	BOOL res1 = GetVersionEx((LPOSVERSIONINFO)&subjInfo.versionInfo);
	BOOL res2 = GetTimeZoneInformation(&subjInfo.timeZoneInfo) != TIME_ZONE_ID_INVALID;
	BOOL res3 = EnumDisplayMonitors(NULL, NULL, monitorEnumProc, (LPARAM)&subjInfo.nMonitors);

	AuthComponent::getUid(uid);
	_tcscpy_s(subjInfo.uid, uid.c_str());

	if (!WritePrivateProfileStruct(_T("SubjectInformation"), _T("Information"),
		(LPVOID)&(subjInfo), sizeof(subjInfo), usageFilePath.c_str())) {
		output(_T("Failed to write SubjectInformation to INI file\n"));
		return S_UNDEFINED_ERROR;
	}

	return S_SUCCESS;
}

Status AppUsageComponent::dumpInteractions()
{
	std::vector<tstring> sectionNames;
	
	if (!WinUtilityComponent::getINISectionNames(usageFilePath, sectionNames)) {
		output(_T("Failed to retrieve ini section names\n"));
		return S_UNDEFINED_ERROR;
	}

	for (int i = 0; i < sectionNames.size(); i++) {
		UsageInformation ui;

		if (!GetPrivateProfileStruct(sectionNames[i].c_str(), _T("Interaction"),
			&ui, sizeof(UsageInformation), usageFilePath.c_str())) {
			output(_T("Failed to read struct\n"));
			continue;
		}

		output(_T("Interaction: %s\n"), ui.annotation);
	}

	return S_SUCCESS;
}

Status AppUsageComponent::onUploadComplete(const IEventArgs& evtArgs)
{
	//const UploadEvtArgs& args = static_cast<const UploadEvtArgs&>(evtArgs);

	if (DeleteFile(usageFilePath.c_str()) == 0)
		output(_T("Failed to delete usage log file: %s\n"), getLastErrorAsString().c_str());

	return S_SUCCESS;
}
