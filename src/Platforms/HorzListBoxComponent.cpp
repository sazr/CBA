#include "HorzListBoxComponent.h"

// Class Property Implementation //

// Static Function Implementation //


// Function Implementation //
HorzListBoxComponent::HorzListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol,
	unsigned int hMargin, unsigned int vMargin, bool dpiAware, DWORD flags)
	: ListBoxComponent(app, wndDim, bkCol, hMargin, vMargin, IScrollerComponent::ScrollDirection::SCROLL_HORZ, dpiAware, flags | WS_HSCROLL)
{
	
}

HorzListBoxComponent::~HorzListBoxComponent()
{
	
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

	long maxDragValue = childX - (horzContainerDim.right - horzContainerDim.left);
	children.push_back(child);

	const WinEventArgs args = { NULL, listBox, MAKEWPARAM(maxDragValue, 0), (LPARAM)child };
	IApp::eventHandler(DispatchWindowComponent::translateMessage(listBox, WM_CUSTOM_LB_ADD_CHILD), args);

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

	long maxDragValue = childX - (horzContainerDim.right - horzContainerDim.left);
	children.insert(children.begin() + insertionIndex, child);

	const WinEventArgs args = { NULL, listBox, MAKEWPARAM(maxDragValue, 0), (LPARAM)child };
	IApp::eventHandler(DispatchWindowComponent::translateMessage(listBox, WM_CUSTOM_LB_ADD_CHILD), args);

	return S_SUCCESS;
}
