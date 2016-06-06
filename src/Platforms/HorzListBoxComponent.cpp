#include "HorzListBoxComponent.h"

// Class Property Implementation //

// Static Function Implementation //
LRESULT CALLBACK HorzListBoxComponent::wndCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HorzListBoxComponent* cmp = CustomWindowComponent::getHWNDComponent<HorzListBoxComponent>(hwnd);
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
HorzListBoxComponent::HorzListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol,
	unsigned int hMargin, unsigned int vMargin, bool dpiAware)
	: Component(app),
	wndDim(wndDim), dpiAware(dpiAware), xPos(0), yPos(0), maxX(0), clientLeftPos(0),
	lButtonDown(false), isDragging(false), bkColour(bkCol), hMargin(hMargin), vMargin(vMargin), xCellPos(hMargin)
{
	addComponent<CustomWindowComponent>(app, _T("HorzListBox"), &HorzListBoxComponent::wndCallback, bkColour);
	dispatchCmp = addComponent<DispatchWindowComponent>(app);
	registerEvents();
}

HorzListBoxComponent::~HorzListBoxComponent()
{
	//DeleteObject(bkColour);
}

Status HorzListBoxComponent::init(const IEventArgs& evtArgs)
{
	output(_T("HorzListBoxComponent::init\n"));
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	
	if (dpiAware) {
		DPIAwareComponent::scaleRect(wndDim);
		hMargin = DPIAwareComponent::scaleUnits(hMargin);
		vMargin = DPIAwareComponent::scaleUnits(vMargin);
	}

	listBox = CreateWindowEx(0, _T("HorzListBox"), _T(""),
		WS_CHILD | WS_VISIBLE | WS_HSCROLL,
		wndDim.left, wndDim.top, wndDim.right, wndDim.bottom, 
		args.hwnd, 0, args.hinstance, 0);

	maxX = wndDim.right + wndDim.left;
	
	CustomWindowComponent::setHWNDComponent<HorzListBoxComponent>(listBox, this);

	/*SCROLLINFO sInfo;
	sInfo.cbSize = sizeof(SCROLLINFO);
	sInfo.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	sInfo.nMin = 0;
	sInfo.nMax = wndDim.right;
	sInfo.nPage = wndDim.right/2;
	sInfo.nPos = 0;
	SetScrollInfo(listBox, SB_HORZ, &sInfo, true);
	SetScrollRange(listBox, SB_HORZ, 0, wndDim.right, false);*/
	ShowScrollBar(listBox, SB_HORZ, false);

	return S_SUCCESS;
}

Status HorzListBoxComponent::terminate(const IEventArgs& evtArgs)
{
	return S_SUCCESS;
}

Status HorzListBoxComponent::registerEvents()
{
	registerEvent(WM_CREATE, &HorzListBoxComponent::init);
	//registerEvent(WM_CLOSE, &HorzListBoxComponent::terminate);
	registerEvent(DispatchWindowComponent::translateMessage(WM_MOUSEMOVE), &HorzListBoxComponent::onChildMouseMove);
	registerEvent(DispatchWindowComponent::translateMessage(WM_LBUTTONDOWN), &HorzListBoxComponent::onChildLButtonDown);
	registerEvent(DispatchWindowComponent::translateMessage(WM_LBUTTONUP), &HorzListBoxComponent::onChildLButtonUp);
	return S_SUCCESS;
}

Status HorzListBoxComponent::onChildMouseMove(const IEventArgs& evtArgs)
{
	const DispatchEventArgs& args = static_cast<const DispatchEventArgs&>(evtArgs);

	POINT p = { LOWORD(args.lParam), HIWORD(args.lParam) };
	output(_T("P: %ld, %ld\n"), p.x, p.y);
	int res = MapWindowPoints(args.catalystHwnd, args.hwnd, (LPPOINT)&p, 1);

	const WinEventArgs& translatedArgs{ NULL, args.hwnd, args.wParam, MAKELPARAM(p.x, p.y) };
	onMouseMove(translatedArgs);

	return S_SUCCESS;
}

Status HorzListBoxComponent::onMouseMove(const WinEventArgs& args)
{
	if (GetKeyState(VK_LBUTTON) & 0x100) {
		long curX = LOWORD(args.lParam);
		long curY = HIWORD(args.lParam);

		if (dragDistance > DRAG_THRESHOLD)
			isDragging = true;

		long diffX = xPos - curX;
		long diffY = yPos - curY;
		dragDistance += abs(diffX);
		diffX *= 0.9; // move 90% of the distance the user scrolled
		output(_T("A: %ld, %ld, %ld, %ld\n"), curX, curY, diffX, diffY);

		/*if (inverseScrollDirection) {
			if (clientLeftPos - diffX < 0 || clientLeftPos - diffX > maxX)
				return S_SUCCESS;

			clientLeftPos -= diffX;

			ScrollWindowEx(args.hwnd, diffX, 0, (CONST RECT *) NULL,
				(CONST RECT *) NULL, (HRGN)NULL, (PRECT)NULL,
				SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
			UpdateWindow(args.hwnd);

			/*SCROLLINFO si;
			si.cbSize = sizeof(si);
			si.fMask = SIF_POS;
			si.nPos = curX;
			SetScrollInfo(args.hwnd, SB_HORZ, &si, TRUE);* /

			xPos = curX;
			yPos = curY;
		}*/

		if (clientLeftPos + diffX < 0 || clientLeftPos + diffX >= maxX)
			return S_SUCCESS;

		clientLeftPos += diffX;

		ScrollWindowEx(args.hwnd, -diffX, 0, (CONST RECT *) NULL,
			(CONST RECT *) NULL, (HRGN)NULL, (PRECT)NULL,
			SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
		UpdateWindow(args.hwnd);

		xPos = curX;
		yPos = curY;
	}

	return S_SUCCESS;
}

Status HorzListBoxComponent::onLButtonDown(const WinEventArgs& args)
{
	output(_T("DOWN\n"));
	lButtonDown = true;
	isDragging = false;
	xPos = LOWORD(args.lParam);
	yPos = HIWORD(args.lParam);
	dragDistance = 0;
	return S_SUCCESS;
}

Status HorzListBoxComponent::onLButtonUp(const WinEventArgs& args)
{
	output(_T("UP\n"));
	lButtonDown = false;
	isDragging = false;
	xPos = LOWORD(args.lParam);
	yPos = HIWORD(args.lParam);
	dragDistance = 0;
	return S_SUCCESS;
}

Status HorzListBoxComponent::onChildLButtonDown(const IEventArgs& evtArgs)
{
	output(_T("CHILD DOWN\n"));
	const DispatchEventArgs& args = static_cast<const DispatchEventArgs&>(evtArgs);

	POINT p = { LOWORD(args.lParam), HIWORD(args.lParam) };
	int res = MapWindowPoints(args.catalystHwnd, args.hwnd, (LPPOINT)&p, 1);

	const WinEventArgs& translatedArgs{ NULL, args.hwnd, args.wParam, MAKELPARAM(p.x, p.y) };
	onLButtonDown(translatedArgs);

	return S_SUCCESS;
}

Status HorzListBoxComponent::onChildLButtonUp(const IEventArgs& evtArgs)
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

Status HorzListBoxComponent::addChild(HWND child)
{
	SetParent(child, listBox);
	dispatchCmp->addDispatcher(child, true);
	
	RECT dim;
	BOOL res1 = GetWindowRect(child, &dim);
	BOOL res2 = SetWindowPos(child, 0, xCellPos, hMargin, 0, 0, SWP_NOSIZE);
	maxX = xCellPos - hMargin - (dim.right - dim.left);
	xCellPos += hMargin + (dim.right - dim.left);

	return S_SUCCESS;
}

HWND HorzListBoxComponent::getHwnd()
{
	return listBox;
}

void HorzListBoxComponent::setHwnd(HWND hwnd)
{
	listBox = hwnd;
}

HBRUSH HorzListBoxComponent::getBkColour()
{
	return bkColour;
}