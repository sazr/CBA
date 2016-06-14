#include "VertListBoxComponent.h"

// Class Property Implementation //

// Static Function Implementation //


// Function Implementation //
VertListBoxComponent::VertListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol,
	unsigned int hMargin, unsigned int vMargin, bool dpiAware, DWORD flags)
	: ListBoxComponent(app, wndDim, bkCol, hMargin, vMargin, IScrollHandler(), dpiAware, flags | WS_VSCROLL)
{
	
}

VertListBoxComponent::~VertListBoxComponent()
{
	//DeleteObject(bkColour);
}

Status VertListBoxComponent::onMouseMove(const WinEventArgs& args)
{
	if (!scrollEnabled)
		return S_SUCCESS;

	if (GetKeyState(VK_LBUTTON) & 0x100) {
		long curX = LOWORD(args.lParam);
		long curY = HIWORD(args.lParam);

		if (dragDistance > DRAG_THRESHOLD)
			isDragging = true;

		long diffX = xPos - curX;
		long diffY = yPos - curY;
		dragDistance += abs(diffY);
		diffY *= 0.9; // move 90% of the distance the user scrolled
		
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
		}*/

		output(_T("CTop: %d\n"), clientDragPos);
		if (clientDragPos + diffY < 0 || clientDragPos + diffY >= maxDragValue)
			return S_SUCCESS;

		clientDragPos += diffY;

		ScrollWindowEx(args.hwnd, 0, -diffY, (CONST RECT *) NULL,
			(CONST RECT *) NULL, (HRGN)NULL, (PRECT)NULL,
			SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
		UpdateWindow(args.hwnd);

		xPos = curX;
		yPos = curY;
	}

	return S_SUCCESS;
}

Status VertListBoxComponent::appendChild(HWND child)
{
	setupListener(child);

	RECT vertContainerDim, dim;
	int childY = vMargin;
	GetWindowRect(listBox, &vertContainerDim);

	if (!children.empty()) {
		RECT neighbourDim;
		//GetWindowRect(*children.end(), &neighbourDim);
		GetWindowRect(children.at( children.size()-1 ), &neighbourDim);
		childY = neighbourDim.bottom + vMargin - vertContainerDim.top;
	}

	BOOL res1 = SetWindowPos(child, 0, hMargin, childY, 0, 0, SWP_NOSIZE);
	BOOL res2 = GetClientRect(child, &dim);
	childY += vMargin + (dim.bottom - dim.top);

	maxDragValue = childY - (vertContainerDim.bottom - vertContainerDim.top);
	children.push_back(child);

	return S_SUCCESS;
}

Status VertListBoxComponent::insertChild(HWND child, unsigned int insertionIndex)
{
	if (insertionIndex >= children.size())
		return S_UNDEFINED_ERROR;

	setupListener(child);

	RECT vertContainerDim, dim, cDim;
	int childY = vMargin;
	GetWindowRect(listBox, &vertContainerDim);
	
	if (!children.empty()) {
		RECT neighbourDim;
		GetWindowRect(children.at(insertionIndex), &neighbourDim);
		childY = neighbourDim.top - vertContainerDim.top;
	}
	else insertionIndex = 0;

	BOOL res1 = SetWindowPos(child, 0, hMargin, childY, 0, 0, SWP_NOSIZE);
	BOOL res2 = GetClientRect(child, &dim);
	childY += vMargin + (dim.bottom - dim.top);
	
	for (int i = insertionIndex; i < children.size(); i++) {
		SetWindowPos(children.at(i), 0, hMargin, childY, 0, 0, SWP_NOSIZE);
		GetClientRect(child, &cDim);
		childY += vMargin + (cDim.bottom - cDim.top);
	}

	maxDragValue = childY - (vertContainerDim.bottom - vertContainerDim.top);
	children.insert(children.begin() + insertionIndex, child);

	return S_SUCCESS;
}

Status VertListBoxComponent::setMaxDragValue()
{
	maxDragValue = wndDim.bottom + wndDim.top;
	return S_SUCCESS;
}