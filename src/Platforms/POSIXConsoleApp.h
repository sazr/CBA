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

#ifndef CBA_POSIXCONSOLEAPP_H
#define CBA_POSIXCONSOLEAPP_H

#include "../CBA.h"
#include "../Status.h"
#include "../Interfaces/IApp.h"
#include "../Component.h"

/*struct POSIXConsoleAppInit : public IEventArgs
{
	POSIXConsoleAppInit() = delete; // default;
	POSIXConsoleAppInit(const HINSTANCE& hinstance, const LPTSTR& cmdLine, const int& cmdShow) :
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
		hinstance(hinstance), hwnd(hwnd), wParam(wParam), lParam(lParam)
	{}

	const HINSTANCE hinstance;
	const HWND hwnd;
	const WPARAM wParam;
	const LPARAM lParam;
};*/

#pragma message("TODO: How to force all inheriters of IApp to only instantiate by static IApp::create() method")
class POSIXConsoleApp : public IApp, public Component
{
public:
	//friend class IApp;

	virtual ~POSIXConsoleApp();
	POSIXConsoleApp(const POSIXConsoleApp& other) = delete;
	POSIXConsoleApp(POSIXConsoleApp&& other) = delete;
	POSIXConsoleApp& operator=(const POSIXConsoleApp& other) = delete;
	POSIXConsoleApp& operator=(POSIXConsoleApp&& other) = delete;

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

protected:

	POSIXConsoleApp();

	Status update();
	Status registerEvents();

private:

};

#endif // CBA_POSIXCONSOLEAPP_H