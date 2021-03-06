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

#include "DragScrollerComponent.h"

// Class Property Implementation //
const unsigned int DragScrollerComponent::LBUTTONDOWN_DELAY = 100;
const STATE DragScrollerComponent::LBUTTONDOWN_TIMER = Status::registerState(_T("LBUTTONDOWN_TIMER")).state;

// Static Function Implementation //


// Function Implementation //
DragScrollerComponent::DragScrollerComponent(const std::weak_ptr<IApp>& app, STATE hwndId, ScrollDirection scrollDir)
	: IScrollerComponent(app, hwndId, scrollDir), xPos(0), yPos(0), 
	lButtonDown(false), isDragging(false), killOnCommand(false),
	lButtonDownWParam(0), lButtonDownLParam(0), lButtonDownHwnd(nullptr)
{
	registerEvents();
}

DragScrollerComponent::~DragScrollerComponent()
{

}

Status DragScrollerComponent::init(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	RECT wndDim;
	GetClientRect(args.hwnd, &wndDim);

	if (scrollDir == SCROLL_VERT)
		maxScrollPos -= wndDim.bottom + wndDim.top;
	else maxScrollPos -= wndDim.right + wndDim.left;

	return S_SUCCESS;
}

Status DragScrollerComponent::terminate(const IEventArgs& evtArgs)
{

	return S_SUCCESS;
}

Status DragScrollerComponent::enable()
{
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, WM_CREATE), &DragScrollerComponent::init);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, WM_COMMAND), &DragScrollerComponent::onCommand);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, ListBoxComponent::WM_CUSTOM_LB_ADD_CHILD), &DragScrollerComponent::onLBAddChild);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, ListBoxComponent::WM_CUSTOM_LB_REMOVE_CHILD), &DragScrollerComponent::onLBRemoveChild);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, WM_MOUSEMOVE), &DragScrollerComponent::onMouseMove);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, WM_LBUTTONDOWN), &DragScrollerComponent::onLButtonDown);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, WM_LBUTTONUP), &DragScrollerComponent::onLButtonUp);
	registerEvent(DispatchWindowComponent::translateMessage(LBUTTONDOWN_TIMER, WM_TIMER), &DragScrollerComponent::onTimer);
	return S_SUCCESS;
}

Status DragScrollerComponent::disable()
{
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, WM_CREATE), &DragScrollerComponent::init);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, WM_COMMAND), &DragScrollerComponent::onCommand);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, ListBoxComponent::WM_CUSTOM_LB_ADD_CHILD), &DragScrollerComponent::onLBAddChild);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, ListBoxComponent::WM_CUSTOM_LB_REMOVE_CHILD), &DragScrollerComponent::onLBRemoveChild);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, WM_MOUSEMOVE), &DragScrollerComponent::onMouseMove);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, WM_LBUTTONDOWN), &DragScrollerComponent::onLButtonDown);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, WM_LBUTTONUP), &DragScrollerComponent::onLButtonUp);
	unregisterEvent(DispatchWindowComponent::translateMessage(LBUTTONDOWN_TIMER, WM_TIMER), &DragScrollerComponent::onTimer);
	return S_SUCCESS;
}

Status DragScrollerComponent::registerEvents()
{
	enable();
	return S_SUCCESS;
}

Status DragScrollerComponent::onCommand(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	 
	if (killOnCommand) {
		killOnCommand = false;
		return DispatchWindowComponent::WM_STOP_PROPAGATION_MSG;
	}

	return S_SUCCESS;
}

Status DragScrollerComponent::onLBAddChild(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	maxScrollPos += (long)LOWORD(args.wParam);
	return S_SUCCESS;
}

Status DragScrollerComponent::onLBRemoveChild(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	long decrease = (long)args.wParam;
	maxScrollPos -= decrease;
	
	if (clientScrollPos < maxScrollPos)
		return S_SUCCESS;

	if (scrollDir == SCROLL_VERT)
		return dragScroll(args.hwnd, yPos - decrease, yPos, true);
	else return dragScroll(args.hwnd, xPos - decrease, xPos, true);

	return S_SUCCESS;
}

Status DragScrollerComponent::onMouseMove(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	if (scrollDir == SCROLL_VERT)
		return dragScroll(args.hwnd, HIWORD(args.lParam), yPos);
	else return dragScroll(args.hwnd, LOWORD(args.lParam), xPos);
}

Status DragScrollerComponent::onLButtonDown(const IEventArgs& evtArgs)
{
	STATE res = S_SUCCESS.state;
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	lButtonDown = true;
	isDragging = false;
	killOnCommand = false;
	xPos = LOWORD(args.lParam);
	yPos = HIWORD(args.lParam);
	dragDistance = 0;

	lButtonDownHwnd = (HWND)args.user;
	lButtonDownWParam = args.wParam;
	lButtonDownLParam = args.lParam;
	return S_SUCCESS;  
}

Status DragScrollerComponent::onLButtonUp(const IEventArgs& evtArgs)
{
	STATE res = S_SUCCESS.state;
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	if (isDragging) {
		killOnCommand = true;
	}

	lButtonDown = false;
	isDragging = false;
	xPos = LOWORD(args.lParam);
	yPos = HIWORD(args.lParam);
	dragDistance = 0;
	return Status(res);
}

Status DragScrollerComponent::onTimer(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	
	if (!isDragging) {
		const WinEventArgs lButtonDownArgs{ NULL, lButtonDownHwnd, lButtonDownWParam, lButtonDownLParam };
		Win32App::eventHandler(DispatchWindowComponent::translateMessage(lButtonDownArgs.hwnd, WM_LBUTTONDOWN), lButtonDownArgs);
	}

	KillTimer(args.hwnd, LBUTTONDOWN_TIMER);
	return S_SUCCESS;
}

Status DragScrollerComponent::dragScroll(HWND hwnd, long pos, long& prevScrollPos, bool alwaysScroll)
{
	if (!alwaysScroll && GetKeyState(VK_LBUTTON) >= 0)
		return S_UNDEFINED_ERROR;

	if (dragDistance > DRAG_THRESHOLD)
		isDragging = true;

	long distToScroll = prevScrollPos - pos;
	dragDistance += abs(distToScroll);
	distToScroll *= 0.9; // move 90% of the distance the user scrolled

	/*output(_T("pos: %d, prevScrollPos: %d, distToScroll: %d, clientScrollPos: %d, maxScrollPos: %d\n"), 
		pos, prevScrollPos, distToScroll, clientScrollPos, maxScrollPos);*/

	if (clientScrollPos > maxScrollPos) {
		distToScroll = maxScrollPos - clientScrollPos;

		if (maxScrollPos <= 0)
			distToScroll = -clientScrollPos;
	}
	else if (clientScrollPos + distToScroll < 0 || clientScrollPos + distToScroll >= maxScrollPos)
		return S_UNDEFINED_ERROR;

	/*output(_T("AFTER pos: %d, prevScrollPos: %d, distToScroll: %d, clientScrollPos: %d, maxScrollPos: %d\n"),
		pos, prevScrollPos, distToScroll, clientScrollPos, maxScrollPos);*/

	clientScrollPos += distToScroll;
	distToScroll = -distToScroll;
	prevScrollPos = pos;

	ScrollWindowEx(hwnd, (scrollDir == SCROLL_VERT) ? 0 : distToScroll,
		(scrollDir == SCROLL_VERT) ? distToScroll : 0, 
		NULL, NULL, NULL, NULL,
		SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
	UpdateWindow(hwnd);

	return S_SUCCESS;
}


