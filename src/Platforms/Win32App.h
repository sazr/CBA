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

#ifndef CBA_WIN32APP_H
#define CBA_WIN32APP_H

#include "../CBA.h"
#include "../Component.h"
#include "../Interfaces/IApp.h"
#include "WinUtilityComponent.h" 
#include "DispatchWindowComponent.h"

struct Win32AppInit : public IEventArgs
{
	Win32AppInit() = delete; // default;
	Win32AppInit(const HINSTANCE& hinstance, const LPTSTR& cmdLine, const int& cmdShow) :
		hinstance(hinstance), cmdLine(cmdLine), cmdShow(cmdShow)
	{}

	const HINSTANCE hinstance;
	const LPTSTR cmdLine;
	const int cmdShow;
};

struct WinEventArgs : public IEventArgs
{
	WinEventArgs() = delete; // default;
	WinEventArgs(const HINSTANCE& hinstance, const HWND& hwnd, const WPARAM& wParam, const LPARAM& lParam) :
		hinstance(hinstance), hwnd(hwnd), wParam(wParam), lParam(lParam), user(nullptr)
	{}
	WinEventArgs(const HINSTANCE& hinstance, const HWND& hwnd, const WPARAM& wParam, const LPARAM& lParam, void* user) :
		hinstance(hinstance), hwnd(hwnd), wParam(wParam), lParam(lParam), user(user)
	{}

	/*WinEventArgs(const WinEventArgs& other) :
		hinstance(other.hinstance), hwnd(other.hwnd), wParam(other.wParam), lParam(other.lParam)
	{}*/

	const HINSTANCE hinstance;
	const HWND hwnd;
	const WPARAM wParam;
	const LPARAM lParam;
	void* user;
};

static std::string WStringToString(const std::wstring& s)
{
	/*std::string temp(s.length(), ' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp;*/

	std::string result;
	for (char x : s)
		result += x;

	return result;
}

#pragma message("TODO: How to force all inheriters of IApp to only instantiate by static IApp::create() method")
class Win32App : public IApp, public Component
{
public:
	//friend class IApp;

	static Status S_WND_REGISTER_FAIL;
	static Status S_WND_CREATE_FAIL;
	static Status WM_CUSTOM_PRE_CREATE;
	static const tstring WND_CLASS_NAME;
	static HINSTANCE hinstance;

	virtual ~Win32App();
	Win32App(const Win32App& other) = delete;
	Win32App(Win32App&& other) = delete;
	Win32App& operator=(const Win32App& other) = delete;
	Win32App& operator=(Win32App&& other) = delete;

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

protected:
	HWND hwnd;
	HACCEL hAccelTable;
	tstring wndTitle;
	LPTSTR cmdLine;
	int cmdShow;
	HBRUSH bkColour;
	RECT wndDimensions;
	DWORD wndFlags;
	int iconID;
	int smallIconID;
	std::shared_ptr <WinUtilityComponent> util;

	Win32App();

	Status update();
	Status registerEvents();
	Status registerWindowClass(const Win32AppInit& evtArgs);
	Status initInstance(const Win32AppInit& evtArgs);
	static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

};

#endif // CBA_WIN32APP_H