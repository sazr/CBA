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

#include "DispatchWindowComponent.h"

// Class Property Implementation //
Status DispatchWindowComponent::WM_DISPATCH_MSG = CStatus::registerState(_T("Custom event. A parent receives child events child event."));
Status DispatchWindowComponent::WM_STOP_PROPAGATION_MSG = Status(IApp::S_KILL_PROPAGATION.state);
const tstring DispatchWindowComponent::PROP_ORIG_CALLBACK = _T("PROP_ORIG_CALLBACK");
const tstring DispatchWindowComponent::PROP_ID = _T("PROP_ID");

// Static Function Implementation //
LRESULT CALLBACK DispatchWindowComponent::dispatchCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int id = (int)GetProp(hwnd, PROP_ID.c_str());
	WNDPROC origProc = static_cast<WNDPROC>(GetProp(hwnd, PROP_ORIG_CALLBACK.c_str()));
	//output(_T("DispatchWindowComponent::dispatchCallback message: %d, id: %d\n"), message, id);
	const WinEventArgs args{ NULL, hwnd, wParam, lParam };
	
	if (Win32App::eventHandler(DispatchWindowComponent::translateMessage(id, message), args) == WM_STOP_PROPAGATION_MSG) {
		output(_T("Child Killing propagation: %d\n"), message);
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		//int wmEvent = HIWORD(wParam);
		output(_T("Dispatch WM_COMMAND: %d\n"), wmId);

		if (wmId > 0)
			Win32App::eventHandler(wmId, args);
	}
	break;
	case WM_DRAWITEM:
	{
		int wmId = LOWORD(wParam);
		output(_T("WM_DRAWITEM: %d\n"), wmId);

		if (wmId > 0)
			Win32App::eventHandler(translateMessage(wmId, WM_DRAWITEM), args);
	}
	break;
	case WM_TIMER:
	{
		Win32App::eventHandler(translateMessage(wParam, WM_TIMER), args);
	}
	break;
	default: {} break;
	}

	return CallWindowProc(origProc, hwnd, message, wParam, lParam);
}

LRESULT CALLBACK DispatchWindowComponent::customWndCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const WinEventArgs args{ NULL, hwnd, wParam, lParam };

	int id = (int)GetProp(hwnd, PROP_ID.c_str());
	//output(_T("DispatchWindowComponent::customWndCallback message: %d, id: %d\n"), message, id);
	//Win32App::eventHandler(DispatchWindowComponent::translateMessage(id, message), args);

	if (Win32App::eventHandler(DispatchWindowComponent::translateMessage(id, message), args) == WM_STOP_PROPAGATION_MSG) {
		output(_T("Killing propagation: %d\n"), message);
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	switch (message)
	{
	case WM_NCCREATE:
	{
		int id = (int)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetProp(hwnd, PROP_ID.c_str(), (HANDLE)id);

		/*registerEventLambda<Component>(translateMessage(id, WM_NCDESTROY), [&](const IEventArgs& args)->Status {
			if (!RemoveProp(hwnd, PROP_ID.c_str()))
			return S_UNDEFINED_ERROR;

			return S_SUCCESS;
			});*/
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		//int wmEvent = HIWORD(wParam);
		output(_T("Dispatch WM_COMMAND: %d\n"), wmId);

		if (wmId > 0)
			Win32App::eventHandler(wmId, args);
	}
	break;
	case WM_DRAWITEM:
	{
		int wmId = LOWORD(wParam);
		output(_T("WM_DRAWITEM: %d\n"), wmId);

		if (wmId > 0)
			Win32App::eventHandler(translateMessage(wmId, WM_DRAWITEM), args);
	}
	break;
	case WM_TIMER:
	{
		Win32App::eventHandler(translateMessage(wParam, WM_TIMER), args);
	}
	break;
	default: {} break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

int DispatchWindowComponent::translateMessage(int hwndID, UINT message)
{
	return WM_DISPATCH_MSG * 2 * hwndID + message;
}

int DispatchWindowComponent::translateMessage(HWND hwnd, UINT message)
{
	int id = (int)GetProp(hwnd, PROP_ID.c_str());
	return translateMessage(id /*GetDlgCtrlID(hwnd)*/, message);
}


// Function Implementation //
DispatchWindowComponent::DispatchWindowComponent(const std::weak_ptr<IApp>& app) : Component(app)
{
	registerEvents();
}

DispatchWindowComponent::~DispatchWindowComponent()
{

}

Status DispatchWindowComponent::init(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	return S_SUCCESS;
}

Status DispatchWindowComponent::terminate(const IEventArgs& evtArgs)
{
	return S_SUCCESS;
}

Status DispatchWindowComponent::registerEvents()
{
	return S_SUCCESS;
}

Status DispatchWindowComponent::addDispatcher(HWND hwnd)
{
	int hwndId = GetDlgCtrlID(hwnd);

	if (hwndId == 0) {
		hwndId = (int)GetProp(hwnd, PROP_ID.c_str());
		if (hwndId == 0) {
			hwndId = Status::registerState(_T("DispatchWindowComponent::addDispatcher: Auto-generated id"));
			SetWindowLongPtr(hwnd, GWLP_ID, (LONG)hwndId);
		}
	}

	if (!SetProp(hwnd, PROP_ID.c_str(), (HANDLE)hwndId))
		return S_UNDEFINED_ERROR;

	if (!SetProp(hwnd, PROP_ORIG_CALLBACK.c_str(), (HANDLE)SetWindowLong(hwnd, GWL_WNDPROC, (LONG)dispatchCallback)))
		return S_UNDEFINED_ERROR;

	registerEventLambda<Component>(translateMessage(hwndId, WM_NCDESTROY), [&](const IEventArgs& args)->Status {
		
		if (!RemoveProp(hwnd, PROP_ORIG_CALLBACK.c_str()))
			return S_UNDEFINED_ERROR;
		if (!RemoveProp(hwnd, PROP_ID.c_str()))
			return S_UNDEFINED_ERROR;

		return S_SUCCESS;
	});

	return S_SUCCESS;
}