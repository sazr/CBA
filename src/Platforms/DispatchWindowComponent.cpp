#include "DispatchWindowComponent.h"

// Class Property Implementation //
Status DispatchWindowComponent::WM_DISPATCH_MSG = CStatus::registerState(_T("Custom event. A parent receives child events child event."));
Status DispatchWindowComponent::WM_STOP_PROPAGATION_MSG = CStatus::registerState(_T("Custom event. Don't propagate message to the windows default callback."));
const tstring DispatchWindowComponent::PROP_ORIG_CALLBACK = _T("PROP_ORIG_CALLBACK");
const tstring DispatchWindowComponent::PROP_ID = _T("PROP_ID");

// Static Function Implementation //
LRESULT CALLBACK DispatchWindowComponent::dispatchCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int id = (int)GetProp(hwnd, PROP_ID.c_str());
	WNDPROC origProc = static_cast<WNDPROC>(GetProp(hwnd, PROP_ORIG_CALLBACK.c_str()));
	//output(_T("DispatchWindowComponent::dispatchCallback message: %d, id: %d\n"), message, id);
	const WinEventArgs args{ NULL, hwnd, wParam, lParam };
	Win32App::eventHandler(DispatchWindowComponent::translateMessage(id, message), args);

	return CallWindowProc(origProc, hwnd, message, wParam, lParam);
}

LRESULT CALLBACK DispatchWindowComponent::customWndCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const WinEventArgs args{ NULL, hwnd, wParam, lParam };
	
	if (message == WM_NCCREATE) {
		int id = (int)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetProp(hwnd, PROP_ID.c_str(), (HANDLE)id);

		/*registerEventLambda<Component>(translateMessage(id, WM_NCDESTROY), [&](const IEventArgs& args)->Status {
			if (!RemoveProp(hwnd, PROP_ID.c_str()))
				return S_UNDEFINED_ERROR;

			return S_SUCCESS;
		});*/
	}
	else if (message == WM_COMMAND) {
		int wmId = LOWORD(wParam);
		//int wmEvent = HIWORD(wParam);
		output(_T("WM_COMMAND: %d\n"), wmId);

		if (wmId > 0)
			Win32App::eventHandler(wmId, args);
	}

	int id = (int)GetProp(hwnd, PROP_ID.c_str());
	//output(_T("DispatchWindowComponent::customWndCallback message: %d, id: %d\n"), message, id);
	Win32App::eventHandler(DispatchWindowComponent::translateMessage(id, message), args);

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