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

#ifndef CBA_RUNAPPCMP_H
#define CBA_RUNAPPCMP_H

#include "../CBA.h"
#include "../Component.h"
#include "../Models/HwndInfo.h"
#include "Win32App.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi")

//static bool icompare_pred(TCHAR a, TCHAR b)
//{
//	return _totlower(a) == _totlower(b);
//}
//
//static bool icompare(tstring const& a, tstring const& b)
//{
//	if (a.length() == b.length()) {
//		return std::equal(b.begin(), b.end(),
//			a.begin(), icompare_pred);
//	}
//
//	return false;
//}

class RunApplicationComponent : public Component
{
public:
	friend class Component;

	// Static Variables //
	static Status RESIZE_APP;
	static const unsigned int MAX_TRIES;

	// Static Methods //
	static BOOL CALLBACK enumWindows(HWND hwnd, LPARAM lParam);

	// Class Variables //
	std::vector<HWND> openWnds;
	
	// Class Methods //
	virtual ~RunApplicationComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);
	//Status tryResizeHwnd(HWND hwnd);
	Status runApplications(const std::vector<HwndInfo>& nHwndInfos, const std::vector<HWND>& nHwndIgnoreList);
	bool isAltTabWindow(HWND hwnd);

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	unsigned int nTries;
	RECT clientRect;
	HWND mainHwnd;
	std::vector<HwndInfo> hwndInfos;
	std::vector<HWND> hwndIgnoreList;

	// Class Methods //
	RunApplicationComponent(const std::weak_ptr<IApp>& app);

	Status registerEvents();
	Status onTimer(const IEventArgs& evtArgs);
	bool isMainWindow(HWND hwnd);
	Status getProcessFilePath(DWORD processId, tstring& filePath);
	
private:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //

};

#endif // TEST_CMP_H