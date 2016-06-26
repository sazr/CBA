#include "DragScrollerComponent.h"

// Class Property Implementation //


// Static Function Implementation //


// Function Implementation //
DragScrollerComponent::DragScrollerComponent(const std::weak_ptr<IApp>& app, STATE hwndId, ScrollDirection scrollDir)
	: IScrollerComponent(app, hwndId, scrollDir), xPos(0), yPos(0), lButtonDown(false), isDragging(false)
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
		maxScrollPos = wndDim.bottom + wndDim.top;
	else maxScrollPos = wndDim.right + wndDim.left;

	return S_SUCCESS;
}

Status DragScrollerComponent::terminate(const IEventArgs& evtArgs)
{

	return S_SUCCESS;
}

Status DragScrollerComponent::enable()
{
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, WM_CREATE), &DragScrollerComponent::init);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, ListBoxComponent::WM_CUSTOM_LB_ADD_CHILD), &DragScrollerComponent::onLBAddChild);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, WM_MOUSEMOVE), &DragScrollerComponent::onMouseMove);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, WM_LBUTTONDOWN), &DragScrollerComponent::onLButtonDown);
	registerEvent(DispatchWindowComponent::translateMessage(hwndId, WM_LBUTTONUP), &DragScrollerComponent::onLButtonUp);
	return S_SUCCESS;
}

Status DragScrollerComponent::disable()
{
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, WM_CREATE), &DragScrollerComponent::init);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, ListBoxComponent::WM_CUSTOM_LB_ADD_CHILD), &DragScrollerComponent::onLBAddChild);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, WM_MOUSEMOVE), &DragScrollerComponent::onMouseMove);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, WM_LBUTTONDOWN), &DragScrollerComponent::onLButtonDown);
	unregisterEvent(DispatchWindowComponent::translateMessage(hwndId, WM_LBUTTONUP), &DragScrollerComponent::onLButtonUp);
	return S_SUCCESS;
}

Status DragScrollerComponent::registerEvents()
{
	enable();
	return S_SUCCESS;
}

Status DragScrollerComponent::onLBAddChild(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	output(_T("[%d, %d]\n"), maxScrollPos, (long)LOWORD(args.wParam));
	maxScrollPos = (long)LOWORD(args.wParam);
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
	
	//res = DispatchWindowComponent::WM_STOP_PROPAGATION_MSG.state;
	lButtonDown = true;
	isDragging = false;
	xPos = LOWORD(args.lParam);
	yPos = HIWORD(args.lParam);
	dragDistance = 0;
	return Status(res);
}

Status DragScrollerComponent::onLButtonUp(const IEventArgs& evtArgs)
{
	output(_T("DragScrollerComponent::onLButtonUp\n"));
	STATE res = S_SUCCESS.state;
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	if (isDragging) {
		output(_T("Killing propagation\n"));
		res = DispatchWindowComponent::WM_STOP_PROPAGATION_MSG.state;
	}

	lButtonDown = false;
	isDragging = false;
	xPos = LOWORD(args.lParam);
	yPos = HIWORD(args.lParam);
	dragDistance = 0;
	return Status(res);
}

Status DragScrollerComponent::dragScroll(HWND hwnd, long pos, long& prevScrollPos)
{
	if (GetKeyState(VK_LBUTTON) >= 0)
		return S_UNDEFINED_ERROR;

	if (dragDistance > DRAG_THRESHOLD)
		isDragging = true;

	long distToScroll = prevScrollPos - pos;
	dragDistance += abs(distToScroll);
	distToScroll *= 0.9; // move 90% of the distance the user scrolled

	if (clientScrollPos + distToScroll < 0 || clientScrollPos + distToScroll >= maxScrollPos)
		return S_UNDEFINED_ERROR;

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
