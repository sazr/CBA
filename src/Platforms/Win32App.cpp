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

#include "Win32App.h"

// Class Property Implementation //
Status Win32App::S_WND_REGISTER_FAIL	= CStatus::registerState(_T("RegisterClassEx() failure"));
Status Win32App::S_WND_CREATE_FAIL		= CStatus::registerState(_T("CreateWindowEx() failure"));
Status Win32App::WM_CUSTOM_PRE_CREATE	= CStatus::registerState(_T("Custom event. Dispatched prior to main HWND creation."));
const tstring Win32App::WND_CLASS_NAME  = _T("Win32App");
HINSTANCE Win32App::hinstance			= nullptr;

// Static Function Implementation //
LRESULT CALLBACK Win32App::wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const WinEventArgs args { hinstance, hwnd, wParam, lParam };
	eventHandler(message, args);

	switch (message)
	{
	/*case WM_WINDOWPOSCHANGING:
	{
		DefWindowProc(hwnd, message, wParam, lParam);
	}
	break;*/
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		//int wmEvent = HIWORD(wParam);
		output(_T("WM_COMMAND: %d\n"), wmId);

		eventHandler(wmId, args);

		switch (wmId)
		{
		default:
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		break;
		}
	}
	break;
	case WM_DRAWITEM:
	{
		int wmId = LOWORD(wParam);
		output(_T("WM_DRAWITEM: %d\n"), wmId);

		if (wmId > 0)
			Win32App::eventHandler(DispatchWindowComponent::translateMessage(wmId, WM_DRAWITEM), args);
	}
	break;
	case WM_TIMER:
	{
		Win32App::eventHandler(DispatchWindowComponent::translateMessage(wParam, WM_TIMER), args);
	}
	break;
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	default:
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	break;
	}

	return 0;
}

// Function Implementation //
Win32App::Win32App() 
	: IApp(), Component(std::weak_ptr<IApp>()), hwnd(nullptr), hAccelTable(nullptr), cmdShow(1), bkColour(CreateSolidBrush(RGB(50, 50, 50))), 
	wndDimensions({ CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT }), wndFlags(WS_BORDER | WS_CAPTION)
{
	
}

Win32App::~Win32App()
{

}

Status Win32App::init(const IEventArgs& evtArgs)
{
	const Win32AppInit& initArgs = static_cast<const Win32AppInit&>(evtArgs);
	hinstance = initArgs.hinstance;
	cmdLine = initArgs.cmdLine;
	cmdShow = initArgs.cmdShow;

	util = addComponent<WinUtilityComponent>(app);

	registerWindowClass(initArgs);

	eventHandler(WM_CUSTOM_PRE_CREATE, initArgs);

	initInstance(initArgs);

	return S_SUCCESS;
}

Status Win32App::update()
{
	MSG msg;

	if (!GetMessage(&msg, NULL, 0, 0))
		return S_APP_EXIT;

	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return S_SUCCESS;
}

Status Win32App::terminate(const IEventArgs& evtArgs)
{
	return S_SUCCESS;
}

Status Win32App::registerWindowClass(const Win32AppInit& evtArgs)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hinstance;
	wcex.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(iconID));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = bkColour; 
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WND_CLASS_NAME.c_str();
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(smallIconID));

	if (RegisterClassEx(&wcex) == 0)
		return S_WND_REGISTER_FAIL;

	return S_SUCCESS;
}

Status Win32App::initInstance(const Win32AppInit& evtArgs)
{

	hwnd = CreateWindowEx(0, WND_CLASS_NAME.c_str(), wndTitle.c_str(), wndFlags,	
		wndDimensions.left, wndDimensions.top, wndDimensions.right, wndDimensions.bottom, 
		NULL, NULL, hinstance, NULL);

	if (!hwnd)
		return S_WND_CREATE_FAIL;

	ShowWindow(hwnd, evtArgs.cmdShow);
	UpdateWindow(hwnd);

	return S_SUCCESS;
}

Status Win32App::registerEvents()
{
	return S_SUCCESS;
}