#include "ListBoxComponent.h"

// Class Property Implementation //

// Static Function Implementation //
LRESULT CALLBACK ListBoxComponent::wndCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ListBoxComponent* cmp = CustomWindowComponent::getHWNDComponent<ListBoxComponent>(hwnd);
	const WinEventArgs args{ NULL, hwnd, wParam, lParam };

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		cmp->onLButtonDown(args);
	}
	break;
	case WM_LBUTTONUP:
	{

		cmp->onLButtonUp(args);
	}
	break;
	case WM_MOUSEMOVE:
	{

		cmp->onMouseMove(args);
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		Win32App::eventHandler(wmId, args);
	}
	break;
	//case WM_CTLCOLORDLG:
	//	return (INT_PTR)CreateSolidBrush(RGB(10, 10, 10));  //cmp->getBkColour();
	//break;
	//case WM_CTLCOLORSTATIC:
	//	return (INT_PTR)CreateSolidBrush(RGB(10, 10, 10)); //cmp->getBkColour();
	//break;
	default:
	{
	}
	break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

// Function Implementation //
ListBoxComponent::ListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol,
	unsigned int hMargin, unsigned int vMargin, IScrollHandler scrollHandler, bool dpiAware, DWORD flags)
	: Component(app),
	wndDim(wndDim), scrollHandler(scrollHandler), dpiAware(dpiAware), wndFlags(flags), xPos(0), yPos(0), maxDragValue(0), clientDragPos(0),
	lButtonDown(false), isDragging(false), scrollEnabled(true), bkColour(bkCol), hMargin(hMargin), vMargin(vMargin)
{
	addComponent<CustomWindowComponent>(app, _T("CBA_ListBox"), &ListBoxComponent::wndCallback, bkColour);
	dispatchCmp = addComponent<DispatchWindowComponent>(app);
	registerEvents();
}

ListBoxComponent::~ListBoxComponent()
{
	//DeleteObject(bkColour);
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
		args.hwnd, 0, args.hinstance, 0);

	setMaxDragValue(); // maxDragValue = wndDim.right + wndDim.left;
	
	CustomWindowComponent::setHWNDComponent<ListBoxComponent>(listBox, this);

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
	//output(_T("P: %ld, %ld\n"), p.x, p.y);
	int res = MapWindowPoints(args.catalystHwnd, args.hwnd, (LPPOINT)&p, 1);

	const WinEventArgs& translatedArgs{ NULL, args.hwnd, args.wParam, MAKELPARAM(p.x, p.y) };
	onMouseMove(translatedArgs);

	return S_SUCCESS;
}

Status ListBoxComponent::onLButtonDown(const WinEventArgs& args)
{
	output(_T("DOWN\n"));
	lButtonDown = true;
	isDragging = false;
	xPos = LOWORD(args.lParam);
	yPos = HIWORD(args.lParam);
	dragDistance = 0;

	scrollHandler.onMouseDown(args);

	return S_SUCCESS;
}

Status ListBoxComponent::onLButtonUp(const WinEventArgs& args)
{
	output(_T("UP\n"));
	lButtonDown = false;
	isDragging = false;
	xPos = LOWORD(args.lParam);
	yPos = HIWORD(args.lParam);
	dragDistance = 0;

	scrollHandler.onMouseUp(args);

	return S_SUCCESS;
}

Status ListBoxComponent::onChildLButtonDown(const IEventArgs& evtArgs)
{
	output(_T("CHILD DOWN\n"));
	const DispatchEventArgs& args = static_cast<const DispatchEventArgs&>(evtArgs);

	POINT p = { LOWORD(args.lParam), HIWORD(args.lParam) };
	int res = MapWindowPoints(args.catalystHwnd, args.hwnd, (LPPOINT)&p, 1);

	const WinEventArgs& translatedArgs{ NULL, args.hwnd, args.wParam, MAKELPARAM(p.x, p.y) };
	onLButtonDown(translatedArgs);

	return S_SUCCESS;
}

Status ListBoxComponent::onChildLButtonUp(const IEventArgs& evtArgs)
{
	output(_T("CHILD UP\n"));
	if (isDragging) {
		outputStr("Killing propagation\n");
		const DispatchEventArgs& args = static_cast<const DispatchEventArgs&>(evtArgs);

		POINT p = { LOWORD(args.lParam), HIWORD(args.lParam) };
		int res = MapWindowPoints(args.catalystHwnd, args.hwnd, (LPPOINT)&p, 1);

		const WinEventArgs& translatedArgs{ NULL, args.hwnd, args.wParam, MAKELPARAM(p.x, p.y) };
		onLButtonUp(translatedArgs);
		return DispatchWindowComponent::WM_STOP_PROPAGATION_MSG;
	}

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
	scrollEnabled = true;
	lButtonDown = false;
	isDragging = false;
}

void ListBoxComponent::disableScroll()
{
	scrollEnabled = false;
}