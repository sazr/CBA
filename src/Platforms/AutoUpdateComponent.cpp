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

#include "AutoUpdateComponent.h"

// Class Property Implementation //
Status AutoUpdateComponent::S_VERSION_UP_TO_DATE = Status::registerState(_T("Application version is the latest."));;

// Static Function Implementation //


// Function Implementation //
AutoUpdateComponent::AutoUpdateComponent(const std::weak_ptr<IApp>& app, const std::string& versionCheckUrl, const std::string& dwnldFileUrl, const std::string& absOutputFilePath, const std::string& INIFilePath)
	: Component(app), versionCheckUrl(versionCheckUrl), dwnldFileUrl(dwnldFileUrl), absOutputFilePath(absOutputFilePath),
	INIFilePath(INIFilePath), getVersionId(Status::registerState(_T("Unique Download ID")).state), getExeId(Status::registerState(_T("Unique Download ID")).state)
{
	dldCmp = addComponent<DownloadComponent>(app);
	registerEvents();
}

AutoUpdateComponent::~AutoUpdateComponent()
{

}

Status AutoUpdateComponent::preInit(const IEventArgs& evtArgs)
{
	const Win32AppInit& initArgs = static_cast<const Win32AppInit&>(evtArgs);
	tstring cmdArgs = initArgs.cmdLine;

	if (cmdArgs.find(_T("/updated")) != tstring::npos) {
		output(_T("Updated\n"));
		char exePath[MAX_PATH];
		GetModuleFileNameA(GetModuleHandle(NULL), exePath, MAX_PATH);
		std::string tmpName = std::string(exePath) + std::string(".dep");
		DeleteFileA(tmpName.c_str());
		return S_SUCCESS;
	}
	
	auto checkVersionRes = dldCmp->download(versionCheckUrl, getVersionId);
	
	return S_SUCCESS;
}

Status AutoUpdateComponent::init(const IEventArgs& evtArgs)
{
	//const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	return S_SUCCESS;
}

Status AutoUpdateComponent::terminate(const IEventArgs& evtArgs)
{

	return S_SUCCESS;
}

Status AutoUpdateComponent::registerEvents()
{
	registerEvent(Win32App::WM_CUSTOM_PRE_CREATE, &AutoUpdateComponent::preInit);
	registerEvent(DispatchWindowComponent::translateMessage(getVersionId, DownloadComponent::WM_DOWNLOAD_COMPLETE), &AutoUpdateComponent::onDownloadComplete);
	registerEvent(DispatchWindowComponent::translateMessage(getExeId, DownloadComponent::WM_DOWNLOAD_COMPLETE), &AutoUpdateComponent::onDownloadFileComplete);
	return S_SUCCESS;
}

Status AutoUpdateComponent::onDownloadFileComplete(const IEventArgs& evtArgs)
{
	const DownloadEvtArgs& args = static_cast<const DownloadEvtArgs&>(evtArgs);

	char exePath[MAX_PATH];
	GetModuleFileNameA(GetModuleHandle(NULL), exePath, MAX_PATH);

	std::string tmpName = std::string(exePath) + std::string(".dep");
	DeleteFileA(tmpName.c_str());
	MoveFileA(exePath, tmpName.c_str());
	MoveFileA(args.data.c_str(), exePath);

	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcessA(exePath, "/updated",
		NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL,
		NULL, &si, &pi)) {
		return S_SUCCESS;
	}

	WritePrivateProfileStringA("config", "version", newVersionStr.c_str(),
		INIFilePath.c_str());

	ExitProcess(0);
	return S_SUCCESS;
}

Status AutoUpdateComponent::onDownloadComplete(const IEventArgs& evtArgs)
{
	STATE res = S_VERSION_UP_TO_DATE.state;
	const DownloadEvtArgs& args = static_cast<const DownloadEvtArgs&>(evtArgs);
	//unregisterEvent(DispatchWindowComponent::translateMessage(getVersionId, DownloadComponent::WM_DOWNLOAD_COMPLETE), &AutoUpdateComponent::onDownloadComplete);

	newVersionStr = args.data;
	double latestVersion = atof(args.data.c_str());

	char curVersionStr[MAX_PATH];
	GetPrivateProfileStringA("config", "version", "0.00", curVersionStr,
		MAX_PATH, INIFilePath.c_str());

	double curVersion = atof(curVersionStr);
	OutputDebugStringA(curVersionStr);

	if (curVersion < latestVersion)
		auto res = dldCmp->downloadFile(dwnldFileUrl, absOutputFilePath, getExeId, false);

	return Status(res);
}