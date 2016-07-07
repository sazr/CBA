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

#include "SystemTrayComponent.h"
#include "../Component.h"
#include "Win32App.h"

// Class Property Implementation //
const Status SystemTrayComponent::WM_TRAY_ICON = Status::registerState(_T("System Tray Icon Message"));

// Static Function Implementation //


// Function Implementation //
SystemTrayComponent::SystemTrayComponent(const std::weak_ptr<IApp>& app, const tstring smallIconPath, const tstring tooltip)
	: Component(app), smallIconPath(smallIconPath),
	tooltip(tooltip), trayIconID(Status::registerState(_T("Unique System Tray icon id")))
{
	registerEvents();
}

SystemTrayComponent::~SystemTrayComponent()
{

}

Status SystemTrayComponent::init(const IEventArgs& evtArgs)
{
	output(_T(" SystemTrayComponent::init\n"));
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	memset(&notifyIconData, 0, sizeof(NOTIFYICONDATA));

	notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	notifyIconData.hWnd = args.hwnd;
	notifyIconData.uID = trayIconID.state;
	notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; //  | NIF_GUID;
	notifyIconData.uCallbackMessage = WM_TRAY_ICON.state;
	notifyIconData.hIcon = (HICON)LoadImage(NULL, smallIconPath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	//generateGUID(&notifyIconData.guidItem);
	//notifyIconData.guidItem = gUid;
	_tcscpy_s(notifyIconData.szTip, tooltip.c_str());
	Shell_NotifyIcon(NIM_ADD, &notifyIconData);

	identifier = { 0 };
	identifier.cbSize = sizeof(NOTIFYICONIDENTIFIER);
	identifier.hWnd = args.hwnd;
	identifier.uID = notifyIconData.uID;

	return S_SUCCESS;
}

Status SystemTrayComponent::terminate(const IEventArgs& evtArgs)
{
	Shell_NotifyIcon(NIM_DELETE, &notifyIconData);

	return S_SUCCESS;
}

Status SystemTrayComponent::registerEvents()
{
	registerEvent(WM_CREATE, &SystemTrayComponent::init);
	registerEvent(WM_CLOSE, &SystemTrayComponent::terminate);
	return S_SUCCESS;
}


