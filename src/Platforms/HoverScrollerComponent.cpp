#include "HoverScrollerComponent.h"

// Class Property Implementation //


// Static Function Implementation //


// Function Implementation //
HoverScrollerComponent::HoverScrollerComponent(const std::weak_ptr<IApp>& app, STATE hwndId, ScrollDirection scrollDir)
	: IScrollerComponent(app, hwndId, scrollDir), scrollSpeed(1)
{
	registerEvents();
}

HoverScrollerComponent::~HoverScrollerComponent()
{

}

Status HoverScrollerComponent::init(const IEventArgs& evtArgs)
{
	//const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	return S_SUCCESS;
}

Status HoverScrollerComponent::terminate(const IEventArgs& evtArgs)
{

	return S_SUCCESS;
}

Status HoverScrollerComponent::enable()
{
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, ListBoxComponent::WM_CUSTOM_LB_ADD_CHILD), &HoverScrollerComponent::onLBAddChild);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, WM_MOUSEMOVE), &HoverScrollerComponent::onMouseMove);

	return S_SUCCESS;
}

Status HoverScrollerComponent::disable()
{
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, ListBoxComponent::WM_CUSTOM_LB_ADD_CHILD), &HoverScrollerComponent::onLBAddChild);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, WM_MOUSEMOVE), &HoverScrollerComponent::onMouseMove);

	return S_SUCCESS;
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
	maxScrollPos = (long)LOWORD(args.wParam);

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

Status HoverScrollerComponent::onMouseMove(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	return hoverScroll(args.hwnd, LOWORD(args.lParam), HIWORD(args.lParam));
}

Status HoverScrollerComponent::onLButtonDown(const IEventArgs& evtArgs)
{
	//const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	return S_SUCCESS;
}

Status HoverScrollerComponent::onLButtonUp(const IEventArgs& evtArgs)
{
	//const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

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

#pragma message("for slow speed only scroll every 5th hover message received")

	if (PtInRect(&scrollPrevRect, p)) {
		output(_T("In prev\n"));
		distToScroll = scrollSpeed;
	}
	else if (PtInRect(&scrollNextRect, p)) {
		output(_T("In next\n"));
		distToScroll = -scrollSpeed;
	}
	else {
		output(_T("In neither\n")); return S_SUCCESS;
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
