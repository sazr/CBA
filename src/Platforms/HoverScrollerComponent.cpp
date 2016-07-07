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
