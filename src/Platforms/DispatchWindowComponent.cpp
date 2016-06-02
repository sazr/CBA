#include "CBA_Build.h"
#include "DispatchWindowComponent.h"
#include "Win32App.h"

// Class Property Implementation //
Status DispatchWindowComponent::WM_DISPATCH_MSG = CStatus::registerState(_T("Custom event. A parent receives child events child event."));
const tstring DispatchWindowComponent::PROP_DISPATCH_LISTENER = _T("PROP_DISPATCH_LISTENER");

// Static Function Implementation //
LRESULT CALLBACK DispatchWindowComponent::dispatchCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int id = GetDlgCtrlID(hwnd);
	HWND parent = GetParent(hwnd);
	DispatchWindowComponent* dispatchCmp = static_cast<DispatchWindowComponent*>(GetProp(parent, PROP_DISPATCH_LISTENER.c_str()));
	
	const DispatchEventArgs dispatchArgs { parent, hwnd, message, wParam, lParam };
	Win32App::eventHandler(DispatchWindowComponent::translateMessage(message), dispatchArgs);
	
	if (message == WM_NCDESTROY)
		RemoveProp(hwnd, PROP_DISPATCH_LISTENER.c_str());

	return CallWindowProc(dispatchCmp->originalWndProcs[id], hwnd, message, wParam, lParam);
}

int DispatchWindowComponent::translateMessage(UINT message)
{
	return WM_DISPATCH_MSG + message;
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

Status DispatchWindowComponent::addDispatcher(HWND child, bool addChildHMENUId)
{
	int hwndId = GetDlgCtrlID(child);
	HWND parent = GetParent(child);
	
	if (parent == NULL)
		return S_UNDEFINED_ERROR;
	if (GetProp(parent, PROP_DISPATCH_LISTENER.c_str()) == NULL)
		if (!SetProp(parent, PROP_DISPATCH_LISTENER.c_str(), this))
			return S_UNDEFINED_ERROR;

	if (hwndId == 0) {
		if (!addChildHMENUId)
			return S_UNDEFINED_ERROR;

		hwndId = Status::registerState(_T("DispatchWindowComponent::addDispatcher: Auto-generated id"));
		SetWindowLong(child, GWL_ID, hwndId);
	}

	auto res = originalWndProcs.emplace(hwndId, (WNDPROC)SetWindowLong(child, GWL_WNDPROC, (LONG)dispatchCallback));

	if (!res.second)
		return S_UNDEFINED_ERROR;

	return S_SUCCESS;
}