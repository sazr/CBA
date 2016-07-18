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

#include "WinUtilityComponent.h"
#include "Win32App.h"

// Class Property Implementation //

// Static Function Implementation //

// Function Implementation //
WinUtilityComponent::WinUtilityComponent(const std::weak_ptr<IApp>& app) : Component(app)
{
	registerEvents();
}

WinUtilityComponent::~WinUtilityComponent()
{

}

Status WinUtilityComponent::init(const IEventArgs& evtArgs)
{
	//const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	if (!SystemParametersInfo(SPI_GETWORKAREA, NULL, &clientArea, NULL))
		return S_UNDEFINED_ERROR;

	return S_SUCCESS;
}

Status WinUtilityComponent::terminate(const IEventArgs& evtArgs)
{

	return S_SUCCESS;
}

Status WinUtilityComponent::registerEvents()
{
	registerEvent(WM_CREATE, &WinUtilityComponent::init);
	registerEvent(WM_CLOSE, &WinUtilityComponent::terminate);
	registerEvent(WM_DISPLAYCHANGE, &WinUtilityComponent::onDisplayChange);
	return S_SUCCESS;
}

Status WinUtilityComponent::onDisplayChange(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	int newW = LOWORD(args.lParam);
	int newH = HIWORD(args.lParam);

	if (!SystemParametersInfo(SPI_GETWORKAREA, NULL, &clientArea, NULL))
		return S_UNDEFINED_ERROR;

	output(_T("New Dims: %d, %d, %d, %d, %d, %d, T: %d, T: %d\n"), newW, newH, clientArea.left, clientArea.right, clientArea.top, clientArea.bottom,
		clientArea.right - clientArea.left, clientArea.bottom - clientArea.top);

	return S_SUCCESS;
}

#pragma message("TODO: make static method")
Status WinUtilityComponent::getProcessFilePath(DWORD processId, tstring& filePath)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processId);

	if (hProcess == NULL) {
		output(_T("Failed to retrieve process handle\n"));
		return S_UNDEFINED_ERROR;
	}

	//HANDLE hModule;
	//DWORD cbNeeded;
	TCHAR exePath[MAX_PATH];

	if (GetModuleFileNameEx(hProcess, NULL, exePath, MAX_PATH) == 0){
		output(_T("Failed to get module file name: %s, %s\n"), getLastErrorAsString().c_str(), exePath);
		return S_UNDEFINED_ERROR;
	}

	filePath = tstring(exePath);
	CloseHandle(hProcess);
	return S_SUCCESS;
}

Status WinUtilityComponent::getINISectionNames(const tstring& absINIPath, std::vector<tstring>& sectionNames)
{
	TCHAR sectionNamesBuf[1024];

	if (!GetPrivateProfileSectionNames(sectionNamesBuf, 1024, absINIPath.c_str()))
		return S_UNDEFINED_ERROR;

	for (TCHAR* p = sectionNamesBuf; *p; ++p)
	{
		tstring sectionName(p);
		sectionNames.push_back(sectionName);
		p += sectionName.size();

		output(_T("%s\n"), sectionName.c_str());

		//if (*p == 0)
		//	break;
	}

	return S_SUCCESS;
}

Status WinUtilityComponent::getINISectionNames(const tstring& absINIPath, tstring& sectionNames)
{
	tstringstream ss;
	TCHAR sectionNamesBuf[1024];

	if (!GetPrivateProfileSectionNames(sectionNamesBuf, 1024, absINIPath.c_str()))
		return S_UNDEFINED_ERROR;

	for (TCHAR* p = sectionNamesBuf; *p; ++p)
	{
		tstring sectionName(p);
		ss << sectionName << _T(",");
		p += sectionName.size();

		//if (*p == 0)
		//	break;
	}

	ss >> sectionNames;
	return S_SUCCESS;
}

Status WinUtilityComponent::getINISectionKeyValues(const tstring& absINIPath, const tstring& sectionName, std::vector<tstring>& keys, std::vector<tstring>& values)
{
	TCHAR sectionBuf[10000];

	if (!GetPrivateProfileSection(sectionName.c_str(), sectionBuf, 10000, absINIPath.c_str()))
		return S_UNDEFINED_ERROR;

	for (TCHAR* p = sectionBuf; *p; ++p) {

		tstring keyValuePair(p);
		std::size_t pos = keyValuePair.find('=');
		tstring key = keyValuePair.substr(0, pos);
		tstring value = keyValuePair.substr(pos + 1);
		keys.push_back(key);
		values.push_back(value);

		p += keyValuePair.size();
	}

	return S_SUCCESS;
}

Status WinUtilityComponent::getClientRect(RECT& clientRect)
{
	if (!SystemParametersInfo(SPI_GETWORKAREA, 0, &clientArea, 0))
		return S_UNDEFINED_ERROR;

	clientRect = clientArea;
	return S_SUCCESS;
}