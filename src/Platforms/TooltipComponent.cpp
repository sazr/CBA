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

#include "TooltipComponent.h"

// Class Property Implementation //


// Static Function Implementation //


// Function Implementation //
TooltipComponent::TooltipComponent(const std::weak_ptr<IApp>& app) : Component(app)
{
	registerEvents();
}

TooltipComponent::~TooltipComponent()
{

}

Status TooltipComponent::init(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	
	return S_SUCCESS;
}

Status TooltipComponent::terminate(const IEventArgs& evtArgs)
{
	
	return S_SUCCESS;
}

Status TooltipComponent::registerEvents()
{
	//registerEvent(WM_CREATE, &TooltipComponent::init);
	//registerEvent(WM_CLOSE, &TooltipComponent::terminate);
	return S_SUCCESS;
}

Status TooltipComponent::addTooltip(HWND target, const tstring& tooltip)
{
	#pragma message("Get target wnd class name, if static return error")

	HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_ALWAYSTIP | TTS_USEVISUALSTYLE /* | TTS_BALLOON*/,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		GetParent(target), NULL,
		GetModuleHandle(NULL), NULL);

	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = TTTOOLINFO_V1_SIZE;
	toolInfo.hwnd = GetParent(target);
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)target;
	toolInfo.lpszText = (LPTSTR)tooltip.c_str();

	SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
	SendMessage(hwndTip, TTM_ACTIVATE, TRUE, 0);
	return S_SUCCESS;
}


