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


