#include "ListBoxComponent.h"

// Class Property Implementation //
Status ListBoxComponent::WM_CUSTOM_LB_ADD_CHILD = Status::registerState(_T("Custom CBA message: add child to CBA::ListBox"));

// Static Function Implementation //

// Function Implementation //
ListBoxComponent::ListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol,
	unsigned int hMargin, unsigned int vMargin, bool dpiAware, DWORD flags)
	: Component(app),
	wndDim(wndDim), dpiAware(dpiAware), wndFlags(flags | WS_CHILD),
	bkColour(bkCol), hMargin(hMargin), vMargin(vMargin),
	listBoxHwndId(Status::registerState(_T("Listbox hwnd id")))
{
	addComponent<CustomWindowComponent>(app, _T("CBA_ListBox"), &DispatchWindowComponent::customWndCallback /*&ListBoxComponent::wndCallback*/, bkColour);
	dispatchCmp = addComponent<DispatchWindowComponent>(app);
	registerEvents();
}

ListBoxComponent::ListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol,
	unsigned int hMargin, unsigned int vMargin, IScrollerComponent::ScrollDirection scrollDir, bool dpiAware, DWORD flags)
	: ListBoxComponent(app, wndDim, bkCol, hMargin, vMargin, dpiAware, flags)
{
	scrollerCmp = addComponent<HoverScrollerComponent>(app, listBoxHwndId, scrollDir);
}

ListBoxComponent::~ListBoxComponent()
{

}

Status ListBoxComponent::init(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	bool isPopup = (wndFlags & WS_POPUP) != 0;
	HMENU menuId = (HMENU)((isPopup) ? 0 : listBoxHwndId.state);

	if (dpiAware) {
		DPIAwareComponent::scaleRect(wndDim);
		hMargin = DPIAwareComponent::scaleUnits(hMargin);
		vMargin = DPIAwareComponent::scaleUnits(vMargin);
	}

	listBox = CreateWindowEx(0, _T("CBA_ListBox"), _T(""),
		WS_VISIBLE | wndFlags,
		wndDim.left, wndDim.top, wndDim.right, wndDim.bottom, 
		args.hwnd, menuId, args.hinstance, (LPVOID)listBoxHwndId.state);

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
	//const DispatchEventArgs& args = static_cast<const DispatchEventArgs&>(evtArgs);
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	POINT p = { LOWORD(args.lParam), HIWORD(args.lParam) };
	int res = MapWindowPoints(args.hwnd, listBox, (LPPOINT)&p, 1);

	const WinEventArgs& translatedArgs{ NULL, listBox, args.wParam, MAKELPARAM(p.x, p.y) };
	return IApp::eventHandler(DispatchWindowComponent::translateMessage(listBox, WM_MOUSEMOVE), translatedArgs);
}

Status ListBoxComponent::onChildLButtonDown(const IEventArgs& evtArgs)
{
	output(_T("CHILD DOWN\n"));
	//const DispatchEventArgs& args = static_cast<const DispatchEventArgs&>(evtArgs);
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	POINT p = { LOWORD(args.lParam), HIWORD(args.lParam) };
	int res = MapWindowPoints(args.hwnd, listBox, (LPPOINT)&p, 1);

	const WinEventArgs& translatedArgs{ NULL, listBox, args.wParam, MAKELPARAM(p.x, p.y) };
	return IApp::eventHandler(DispatchWindowComponent::translateMessage(listBox, WM_LBUTTONDOWN), translatedArgs);
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
	dispatchCmp->addDispatcher(child);

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
	if (scrollerCmp == NULL)
		return;

	scrollerCmp->enable();
}

void ListBoxComponent::disableScroll()
{
	if (scrollerCmp == NULL)
		return;

	scrollerCmp->disable();
}