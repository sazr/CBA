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

#ifndef CBA_DISPATCHWNDCMP_H
#define CBA_DISPATCHWNDCMP_H

#include "../CBA.h"
#include "../Component.h"
#include "Win32App.h"

struct DispatchEventArgs : public IEventArgs
{
	DispatchEventArgs() = delete; // default;
	DispatchEventArgs(const HWND& hwnd, const HWND& catalystHwnd, const UINT& message, const WPARAM& wParam, const LPARAM& lParam) :
		hwnd(hwnd), catalystHwnd(catalystHwnd), message(message), wParam(wParam), lParam(lParam)
	{}

	const HWND hwnd;
	const HWND catalystHwnd;
	const UINT message;
	const WPARAM wParam;
	const LPARAM lParam;
};

class DispatchWindowComponent : public Component
{
public:
	friend class Component;

	// Static Variables //
	static Status WM_DISPATCH_MSG;
	static Status WM_STOP_PROPAGATION_MSG;
	static const tstring PROP_ORIG_CALLBACK;
	static const tstring PROP_ID;

	// Static Methods //
	static LRESULT CALLBACK dispatchCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK customWndCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static int translateMessage(int hwndID, UINT message);
	static int translateMessage(HWND hwnd, UINT message);

	// Class Variables //
	std::unordered_map<int, WNDPROC> originalWndProcs;

	// Class Methods //
	virtual ~DispatchWindowComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

	Status addDispatcher(HWND child);

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	DispatchWindowComponent(const std::weak_ptr<IApp>& app);

	Status registerEvents();
	
private:
	// Static Variables //

	// Static Methods //
	
	// Class Variables //

	// Class Methods //

};

#endif // CBA_DISPATCHWNDCMP_H