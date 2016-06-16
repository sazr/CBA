#include "ListBoxComponent.h"

// Class Property Implementation //
Status ListBoxComponent::WM_CUSTOM_LB_ADD_CHILD = Status::registerState(_T("Custom CBA message: add child to CBA::ListBox"));

// Static Function Implementation //
LRESULT CALLBACK ListBoxComponent::wndCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const WinEventArgs args { NULL, hwnd, wParam, lParam };
	IApp::eventHandler(DispatchWindowComponent::translateMessage(hwnd, message), args);
	
	return DefWindowProc(hwnd, message, wParam, lParam);
}

// Function Implementation //
ListBoxComponent::ListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol,
	unsigned int hMargin, unsigned int vMargin, bool dpiAware, DWORD flags)
	: Component(app),
	wndDim(wndDim), dpiAware(dpiAware), wndFlags(flags),
	bkColour(bkCol), hMargin(hMargin), vMargin(vMargin),
	listBoxHwndId(Status::registerState(_T("Listbox hwnd id")))
{
	addComponent<CustomWindowComponent>(app, _T("CBA_ListBox"), &ListBoxComponent::wndCallback, bkColour);
	dispatchCmp = addComponent<DispatchWindowComponent>(app);
	registerEvents();
}

ListBoxComponent::ListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol,
	unsigned int hMargin, unsigned int vMargin, IScrollerComponent::ScrollDirection scrollDir, bool dpiAware, DWORD flags)
	: ListBoxComponent(app, wndDim, bkCol, hMargin, vMargin, dpiAware, flags)
	/*Component(app),
	wndDim(wndDim), dpiAware(dpiAware), wndFlags(flags),
	bkColour(bkCol), hMargin(hMargin), vMargin(vMargin),
	listBoxHwndId(Status::registerState(_T("Listbox hwnd id")))*/
{
	addComponent<CustomWindowComponent>(app, _T("CBA_ListBox"), &ListBoxComponent::wndCallback, bkColour);
	dispatchCmp = addComponent<DispatchWindowComponent>(app);
	scrollerCmp = addComponent<DragScrollerComponent>(app, listBoxHwndId, scrollDir);
	registerEvents();
}

ListBoxComponent::~ListBoxComponent()
{

}

Status ListBoxComponent::init(const IEventArgs& evtArgs)
{
	output(_T("ListBoxComponent::init\n"));
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	
	if (dpiAware) {
		DPIAwareComponent::scaleRect(wndDim);
		hMargin = DPIAwareComponent::scaleUnits(hMargin);
		vMargin = DPIAwareComponent::scaleUnits(vMargin);
	}

	listBox = CreateWindowEx(0, _T("CBA_ListBox"), _T(""),
		WS_VISIBLE | wndFlags,
		wndDim.left, wndDim.top, wndDim.right, wndDim.bottom, 
		args.hwnd, (HMENU)listBoxHwndId.state, args.hinstance, 0);

	ShowScrollBar(listBox, SB_HORZ, false);
	ShowScrollBar(listBox, SB_VERT, false);

	return S_SUCCESS;
}

Status ListBoxComponent::terminate(const IEventArgs& evtArgs)
{
	return S_SUCCESS;
}

Status ListBoxComponent::registerEvents()
{
	registerEvent(WM_CREATE, &ListBoxComponent::init);
	return S_SUCCESS;
}

Status ListBoxComponent::onChildMouseMove(const IEventArgs& evtArgs)
{
	const DispatchEventArgs& args = static_cast<const DispatchEventArgs&>(evtArgs);

	POINT p = { LOWORD(args.lParam), HIWORD(args.lParam) };
	int res = MapWindowPoints(args.catalystHwnd, args.hwnd, (LPPOINT)&p, 1);

	const WinEventArgs& translatedArgs{ NULL, args.hwnd, args.wParam, MAKELPARAM(p.x, p.y) };
	return IApp::eventHandler(DispatchWindowComponent::translateMessage(args.hwnd, WM_MOUSEMOVE), translatedArgs);
}

Status ListBoxComponent::onChildLButtonDown(const IEventArgs& evtArgs)
{
	output(_T("CHILD DOWN\n"));
	const DispatchEventArgs& args = static_cast<const DispatchEventArgs&>(evtArgs);

	POINT p = { LOWORD(args.lParam), HIWORD(args.lParam) };
	int res = MapWindowPoints(args.catalystHwnd, args.hwnd, (LPPOINT)&p, 1);

	const WinEventArgs& translatedArgs{ NULL, args.hwnd, args.wParam, MAKELPARAM(p.x, p.y) };
	return IApp::eventHandler(DispatchWindowComponent::translateMessage(args.hwnd, WM_LBUTTONDOWN), translatedArgs);
}

Status ListBoxComponent::onChildLButtonUp(const IEventArgs& evtArgs)
{
	output(_T("CHILD UP\n"));
	/*if (isDragging) {
		outputStr("Killing propagation\n");
		const DispatchEventArgs& args = static_cast<const DispatchEventArgs&>(evtArgs);

		POINT p = { LOWORD(args.lParam), HIWORD(args.lParam) };
		int res = MapWindowPoints(args.catalystHwnd, args.hwnd, (LPPOINT)&p, 1);

		const WinEventArgs& translatedArgs{ NULL, args.hwnd, args.wParam, MAKELPARAM(p.x, p.y) };
		//onLButtonUp(translatedArgs);
		IApp::eventHandler(DispatchWindowComponent::translateMessage(args.hwnd, WM_LBUTTONUP), translatedArgs);
		return DispatchWindowComponent::WM_STOP_PROPAGATION_MSG;
	}*/

	return S_SUCCESS;
}

Status ListBoxComponent::addChild(HWND child, unsigned int insertionIndex)
{
	if (insertionIndex == UINT_MAX || insertionIndex >= children.size())
		return appendChild(child);

	return insertChild(child, insertionIndex);
}

Status ListBoxComponent::setupListener(HWND child)
{
	SetParent(child, listBox);
	dispatchCmp->addDispatcher(child, true);

	registerEvent(DispatchWindowComponent::translateMessage(child, WM_MOUSEMOVE), &ListBoxComponent::onChildMouseMove);
	registerEvent(DispatchWindowComponent::translateMessage(child, WM_LBUTTONDOWN), &ListBoxComponent::onChildLButtonDown);
	registerEvent(DispatchWindowComponent::translateMessage(child, WM_LBUTTONUP), &ListBoxComponent::onChildLButtonUp);

	return S_SUCCESS;
}

HWND ListBoxComponent::getHwnd()
{
	return listBox;
}

void ListBoxComponent::setHwnd(HWND hwnd)
{
	listBox = hwnd;
}

HBRUSH ListBoxComponent::getBkColour()
{
	return bkColour;
}

Status ListBoxComponent::setListBoxClippingRect(const SIZE& windowDim, const SIZE& clientScroll)
{
	SetWindowPos(listBox, 0, 0, 0, windowDim.cx, windowDim.cy, SWP_NOMOVE);
	
	int res = ScrollWindowEx(listBox, -clientScroll.cx, -clientScroll.cy, NULL,
		NULL, NULL, NULL,
		SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
	UpdateWindow(listBox);

	output(_T("RESULT: %d\n"), res == ERROR);
	
	return S_SUCCESS;
}

void ListBoxComponent::enableScroll()
{
	
}

void ListBoxComponent::disableScroll()
{
	
}