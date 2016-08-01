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

#ifndef CBA_APPUSAGECMP_H
#define CBA_APPUSAGECMP_H

#pragma message("AppUsageComponent currently requires _CRT_SECURE_NO_WARNINGS and disable SDL check. Have you disabled SDL checks in project settings?")

#include "CBA.h"
#include "Component.h"
#include "Win32App.h"
#include "UploadComponent.h"
#include "DownloadComponent.h"
#include <time.h>
#include "Shlobj.h"
#pragma comment(lib, "Shell32")

#define USEAGE_INFO_BUF_SIZE 8024

struct SubjectInformation
{
	TCHAR uid[65];
	int nMonitors;
	OSVERSIONINFOEX versionInfo;
	TIME_ZONE_INFORMATION timeZoneInfo;
};

struct UsageInformation
{
	TCHAR annotation[USEAGE_INFO_BUF_SIZE];
	time_t timeStamp;
	UINT msg;
	WPARAM wParam;
	LPARAM lParam;
};

class AppUsageComponent : public Component
{
public:
	friend class Component;

	// Static Variables //

	// Static Methods //
	static BOOL CALLBACK monitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

	// Class Variables //

	// Class Methods //
	virtual ~AppUsageComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);
	Status catalogueEvent(HWND hwnd, UINT msg, const tstring& annotation);
	Status catalogueData(const tstring& annotation);

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	AppUsageComponent(const std::weak_ptr<IApp>& app, const tstring& usageLogFileDirectory, const tstring& domain, const tstring& domainScript);

	Status registerEvents();
	Status preInit(const IEventArgs& evtArgs);
	Status writeUsageLog(const IEventArgs& evtArgs, HWND hwnd, UINT msg, const tstring& annotation);
	//Status writeUsageLog(const tstring& annotation);
	Status catalogueSubjectInformation();
	Status onUploadComplete(const IEventArgs& evtArgs);

	Status dumpInteractions();
private:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	/*const*/ tstring usageFilePath;
	const tstring domain;
	const tstring domainScript;
	STATE uploadUid;
	int interactionIndex;
	std::shared_ptr<UploadComponent> upldCmp;

	// Class Methods //

};

#endif // CBA_APPUSAGECMP_H