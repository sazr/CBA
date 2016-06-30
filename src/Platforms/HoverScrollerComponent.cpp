#include "HoverScrollerComponent.h"

// Class Property Implementation //
unsigned int HoverScrollerComponent::SCROLL_DELAY = 500;

// Static Function Implementation //


// Function Implementation //
HoverScrollerComponent::HoverScrollerComponent(const std::weak_ptr<IApp>& app, STATE hwndId, ScrollDirection scrollDir)
	: IScrollerComponent(app, hwndId, scrollDir), scrollSpeed(1), timerSet(false), canScroll(false)
{
	registerEvents();
}

HoverScrollerComponent::~HoverScrollerComponent()
{

}

Status HoverScrollerComponent::init(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	RECT wndDim;
	GetClientRect(args.hwnd, &wndDim);

	if (scrollDir == SCROLL_VERT)
		maxScrollPos -= wndDim.bottom + wndDim.top;
	else maxScrollPos -= wndDim.right + wndDim.left;

	return S_SUCCESS;
}

Status HoverScrollerComponent::terminate(const IEventArgs& evtArgs)
{

	return S_SUCCESS;
}

Status HoverScrollerComponent::enable()
{
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, WM_CREATE), &HoverScrollerComponent::init);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, ListBoxComponent::WM_CUSTOM_LB_ADD_CHILD), &HoverScrollerComponent::onLBAddChild);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, WM_MOUSEMOVE), &HoverScrollerComponent::onMouseMove);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, WM_MOUSELEAVE), &HoverScrollerComponent::onMouseLeave);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, WM_SHOWWINDOW), &HoverScrollerComponent::onShow);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, WM_TIMER), &HoverScrollerComponent::onTimer);

	return S_SUCCESS;
}

Status HoverScrollerComponent::disable()
{
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, WM_CREATE), &HoverScrollerComponent::init);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, ListBoxComponent::WM_CUSTOM_LB_ADD_CHILD), &HoverScrollerComponent::onLBAddChild);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, WM_MOUSEMOVE), &HoverScrollerComponent::onMouseMove);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, WM_MOUSELEAVE), &HoverScrollerComponent::onMouseLeave);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, WM_SHOWWINDOW), &HoverScrollerComponent::onShow);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, WM_TIMER), &HoverScrollerComponent::onTimer);
	
	return S_SUCCESS;
}

void HoverScrollerComponent::reset(HWND hwnd)
{
	KillTimer(hwnd, GetDlgCtrlID(hwnd));
	canScroll = false;
	timerSet = false;
}

Status HoverScrollerComponent::registerEvents()
{
	enable();

	return S_SUCCESS;
}

Status HoverScrollerComponent::onLBAddChild(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	RECT wndDim;
	GetClientRect(args.hwnd, &wndDim);
	long w = wndDim.right - wndDim.left;
	long h = wndDim.bottom - wndDim.top;
	maxScrollPos += (long)LOWORD(args.wParam);

	if (scrollDir == SCROLL_VERT) {
		scrollPrevRect = { wndDim.left, wndDim.top, wndDim.right, wndDim.top + h*0.25 };
		scrollNextRect = { wndDim.left, wndDim.bottom - h*0.25, 
			wndDim.right, wndDim.bottom };
	}
	else {
		scrollPrevRect = { wndDim.left, wndDim.top, wndDim.left + w*0.25, wndDim.bottom };
		scrollNextRect = { wndDim.right - w*0.25, wndDim.top,
			wndDim.right, wndDim.bottom };
	}

	return S_SUCCESS;
}

Status HoverScrollerComponent::onShow(const IEventArgs& evtArgs)
{
	outputStr("SHOW\n");
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	reset(args.hwnd);

	return S_SUCCESS;
}

Status HoverScrollerComponent::onMouseMove(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	return hoverScroll(args.hwnd, LOWORD(args.lParam), HIWORD(args.lParam));
}

Status HoverScrollerComponent::onMouseLeave(const IEventArgs& evtArgs)
{
	outputStr("MOUSE_LEAVE\n");
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	reset(args.hwnd);

	return S_SUCCESS;
}

Status HoverScrollerComponent::onLButtonDown(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	reset(args.hwnd);

	return S_SUCCESS;
}

Status HoverScrollerComponent::onLButtonUp(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	reset(args.hwnd);

	return S_SUCCESS;
}

Status HoverScrollerComponent::onTimer(const IEventArgs& evtArgs)
{
	outputStr("onTimer\n");
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	
	//reset(args.hwnd);
	KillTimer(args.hwnd, GetDlgCtrlID(args.hwnd));
	canScroll = true;

	return S_SUCCESS;
}

Status HoverScrollerComponent::hoverScroll(HWND hwnd, int xPos, int yPos)
{
	/*if (!scrollEnabled)
	return S_SUCCESS;

	long xScroll = 0, yScroll = 0;
	POINT p{ LOWORD(args.lParam), HIWORD(args.lParam) };

	if (PtInRect(&scrollPrevRect, p))
	xScroll = -scrollSpeed;
	else if (PtInRect(&scrollNextRect, p))
	xScroll = scrollSpeed;
	else return S_SUCCESS;

	ScrollWindowEx(args.hwnd, xScroll, yScroll, (CONST RECT *) NULL,
	(CONST RECT *) NULL, (HRGN)NULL, (PRECT)NULL,
	SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
	UpdateWindow(args.hwnd);*/

	// Generic technique
	POINT p { xPos, yPos };
	long distToScroll;

	if (PtInRect(&scrollPrevRect, p)) {
		if (!timerSet) {
			SetTimer(hwnd, GetDlgCtrlID(hwnd), SCROLL_DELAY, NULL);
			timerSet = true;
		}

		if (!canScroll)
			return S_SUCCESS;

		//output(_T("In prev\n"));
		distToScroll = scrollSpeed;
	}
	else if (PtInRect(&scrollNextRect, p)) {
		if (!timerSet) {
			SetTimer(hwnd, GetDlgCtrlID(hwnd), SCROLL_DELAY, NULL);
			timerSet = true;
		}

		if (!canScroll)
			return S_SUCCESS;

		//output(_T("In next\n"));
		distToScroll = -scrollSpeed;
	}
	else {
		//output(_T("In neither\n")); 
		KillTimer(hwnd, GetDlgCtrlID(hwnd));
		canScroll = false;
		timerSet = false;
		return S_SUCCESS;
	}

	if (clientScrollPos + distToScroll < -maxScrollPos || clientScrollPos + distToScroll >= 0) //maxScrollPos)
		return S_UNDEFINED_ERROR;

	clientScrollPos += distToScroll;

	/*output(_T("Scrolling [%d,%d], %d\n"), (scrollDir == SCROLL_VERT) ? 0 : distToScroll,
		(scrollDir == SCROLL_VERT) ? distToScroll : 0, clientScrollPos);*/
	ScrollWindowEx(hwnd, (scrollDir == SCROLL_VERT) ? 0 : distToScroll,
		(scrollDir == SCROLL_VERT) ? distToScroll : 0,
		NULL, NULL, NULL, NULL,
		SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
	UpdateWindow(hwnd);

	return S_SUCCESS;
}
