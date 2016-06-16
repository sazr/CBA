#include "HoverScrollerComponent.h"

// Class Property Implementation //


// Static Function Implementation //


// Function Implementation //
HoverScrollerComponent::HoverScrollerComponent(const std::weak_ptr<IApp>& app, STATE hwndId, ScrollDirection scrollDir)
	: IScrollerComponent(app, hwndId, scrollDir), scrollSpeed(5)
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
	
	return S_SUCCESS;
}

Status HoverScrollerComponent::disable()
{
	
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
	POINT p{ xPos, yPos };
	long distToScroll;

	if (PtInRect(&scrollPrevRect, p))
		distToScroll = -scrollSpeed;
	else if (PtInRect(&scrollNextRect, p))
		distToScroll = scrollSpeed;
	else return S_SUCCESS;

	// TODO: check whether we are scrolling outside the max
	if (clientScrollPos + distToScroll < 0 || clientScrollPos + distToScroll >= maxScrollPos)
		return S_UNDEFINED_ERROR;

	ScrollWindowEx(hwnd, (scrollDir == SCROLL_VERT) ? 0 : distToScroll,
		(scrollDir == SCROLL_VERT) ? distToScroll : 0,
		NULL, NULL, NULL, NULL,
		SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
	UpdateWindow(hwnd);

	return S_SUCCESS;
}
