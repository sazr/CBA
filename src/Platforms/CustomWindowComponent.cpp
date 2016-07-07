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

#include "CustomWindowComponent.h"

// Class Property Implementation //
const tstring CustomWindowComponent::PROP_HWND_CMP = _T("PROP_HWND_CMP");


// Static Function Implementation //


// Function Implementation //
CustomWindowComponent::CustomWindowComponent(const std::weak_ptr<IApp>& app,
	const tstring wndClassName, WNDPROC wndCallback, HBRUSH bkColour)
	: Component(app), WND_CLASS_NAME(wndClassName), wndCallback(wndCallback), bkColour(bkColour)
{
	registerEvents();
}

CustomWindowComponent::~CustomWindowComponent()
{
	DeleteObject(bkColour);
}

Status CustomWindowComponent::init(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	if (!registerWindowClass(args))
		output(_T("Failed to register: %s\n"), WND_CLASS_NAME.c_str());

	return S_SUCCESS;
}

Status CustomWindowComponent::terminate(const IEventArgs& evtArgs)
{

	return S_SUCCESS;
}

Status CustomWindowComponent::registerEvents()
{
	registerEvent(WM_CREATE, &CustomWindowComponent::init);
	registerEvent(WM_CLOSE, &CustomWindowComponent::terminate);
	return S_SUCCESS;
}

Status CustomWindowComponent::registerWindowClass(const WinEventArgs& evtArgs)
{
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndCallback;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = evtArgs.hinstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = bkColour; // (HBRUSH)(COLOR_WINDOW);
	wcex.lpszClassName = WND_CLASS_NAME.c_str();
	//wcex.lpszMenuName = NULL; // MAKEINTRESOURCE(IDC_WT);
	//wcex.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(iconID));
	//wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(smallIconID));

	if (RegisterClassEx(&wcex) == 0)
		return S_UNDEFINED_ERROR;

	return S_SUCCESS;
}