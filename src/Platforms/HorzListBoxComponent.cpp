#include "HorzListBoxComponent.h"

// Class Property Implementation //

// Static Function Implementation //


// Function Implementation //
HorzListBoxComponent::HorzListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol,
	unsigned int hMargin, unsigned int vMargin, bool dpiAware, DWORD flags)
	: ListBoxComponent(app, wndDim, bkCol, hMargin, vMargin, IScrollHandler(), dpiAware, flags | WS_HSCROLL)
{
	
}

HorzListBoxComponent::~HorzListBoxComponent()
{
	//DeleteObject(bkColour);
}

Status HorzListBoxComponent::onMouseMove(const WinEventArgs& args)
{
	long distToScroll;
	if (!dragScroll(LOWORD(args.lParam), xPos, distToScroll))
		return S_SUCCESS;

	ScrollWindowEx(args.hwnd, distToScroll, 0, (CONST RECT *) NULL,
		(CONST RECT *) NULL, (HRGN)NULL, (PRECT)NULL,
		SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
	UpdateWindow(args.hwnd);

	/*if (!scrollEnabled)
		return S_SUCCESS;

	if (GetKeyState(VK_LBUTTON) & 0x100) {
		long curX = LOWORD(args.lParam);
		long curY = HIWORD(args.lParam);

		if (dragDistance > DRAG_THRESHOLD)
			isDragging = true;

		long diffX = xPos - curX;
		long diffY = yPos - curY;
		dragDistance += abs(diffX);
		diffX *= 0.9; // move 90% of the distance the user scrolled
		//output(_T("A: %ld, %ld, %ld, %ld\n"), curX, curY, diffX, diffY);

		/*if (inverseScrollDirection) {
			if (clientDragPos - diffX < 0 || clientDragPos - diffX > maxDragValue)
				return S_SUCCESS;

			clientDragPos -= diffX;

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
		}* /

		output(_T("CLeft: %d\n"), clientDragPos);
		if (clientDragPos + diffX < 0 || clientDragPos + diffX >= maxDragValue)
			return S_SUCCESS;

		clientDragPos += diffX;

		ScrollWindowEx(args.hwnd, -diffX, 0, (CONST RECT *) NULL,
			(CONST RECT *) NULL, (HRGN)NULL, (PRECT)NULL,
			SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
		UpdateWindow(args.hwnd);

		xPos = curX;
		yPos = curY;
	}

	return S_SUCCESS;*/
}

Status HorzListBoxComponent::hoverScroll(int xPos, int yPos, long& scrollPos)
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
	if (!scrollEnabled)
		return S_SUCCESS;

	POINT p{ xPos, yPos };

	if (PtInRect(&scrollPrevRect, p))
		scrollPos = -scrollSpeed;
	else if (PtInRect(&scrollNextRect, p))
		scrollPos = scrollSpeed;
	else return S_SUCCESS;

	return S_SUCCESS;
}

Status HorzListBoxComponent::dragScroll(long pos, long& prevScrollPos, long& distToScroll)
{
	if (!scrollEnabled)
		return S_UNDEFINED_ERROR;

	if (GetKeyState(VK_LBUTTON) >= 0)
		return S_UNDEFINED_ERROR;

	if (dragDistance > DRAG_THRESHOLD)
		isDragging = true;

	distToScroll = prevScrollPos - pos;
	dragDistance += abs(distToScroll);
	distToScroll *= 0.9; // move 90% of the distance the user scrolled

	if (clientDragPos + distToScroll < 0 || clientDragPos + distToScroll >= maxDragValue)
		return S_UNDEFINED_ERROR;

	clientDragPos += distToScroll;
	distToScroll = -distToScroll;
	prevScrollPos = pos;

	return S_SUCCESS;
}

Status HorzListBoxComponent::appendChild(HWND child)
{
	setupListener(child);

	RECT horzContainerDim, dim;
	int childX = hMargin;
	GetWindowRect(listBox, &horzContainerDim);

	if (!children.empty()) {
		RECT neighbourDim;
		//GetWindowRect(*children.end(), &neighbourDim);
		GetWindowRect(children.at( children.size()-1 ), &neighbourDim);
		childX = neighbourDim.right + hMargin - horzContainerDim.left;
	}

	BOOL res1 = SetWindowPos(child, 0, childX, vMargin, 0, 0, SWP_NOSIZE);
	BOOL res2 = GetClientRect(child, &dim);
	childX += hMargin + (dim.right - dim.left);

	maxDragValue = childX - (horzContainerDim.right - horzContainerDim.left);
	children.push_back(child);

	return S_SUCCESS;
}

Status HorzListBoxComponent::insertChild(HWND child, unsigned int insertionIndex)
{
	if (insertionIndex >= children.size())
		return S_UNDEFINED_ERROR;

	setupListener(child);
	
	RECT horzContainerDim, dim, cDim;
	int childX = hMargin;
	GetWindowRect(listBox, &horzContainerDim);
	
	if (!children.empty()) {
		RECT neighbourDim;
		GetWindowRect(children.at(insertionIndex), &neighbourDim);
		childX = neighbourDim.left - horzContainerDim.left;
	}
	else insertionIndex = 0;

	BOOL res1 = SetWindowPos(child, 0, childX, vMargin, 0, 0, SWP_NOSIZE);
	BOOL res2 = GetClientRect(child, &dim);
	childX += hMargin + (dim.right - dim.left);
	
	for (int i = insertionIndex; i < children.size(); i++) {
		SetWindowPos(children.at(i), 0, childX, vMargin, 0, 0, SWP_NOSIZE);
		GetClientRect(child, &cDim);
		childX += hMargin + (cDim.right - cDim.left);
	}

	maxDragValue = childX - (horzContainerDim.right - horzContainerDim.left);
	children.insert(children.begin() + insertionIndex, child);

	return S_SUCCESS;
}

Status HorzListBoxComponent::setMaxDragValue()
{
	maxDragValue = wndDim.right + wndDim.left;
	return S_SUCCESS;
}