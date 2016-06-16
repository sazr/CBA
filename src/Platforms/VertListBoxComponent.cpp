#include "VertListBoxComponent.h"

// Class Property Implementation //

// Static Function Implementation //


// Function Implementation //
VertListBoxComponent::VertListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol,
	unsigned int hMargin, unsigned int vMargin, bool dpiAware, DWORD flags)
	: ListBoxComponent(app, wndDim, bkCol, hMargin, vMargin, IScrollerComponent::ScrollDirection::SCROLL_VERT, dpiAware, flags | WS_VSCROLL)
{
	
}

VertListBoxComponent::~VertListBoxComponent()
{

}

Status VertListBoxComponent::appendChild(HWND child)
{
	setupListener(child);

	RECT vertContainerDim, dim;
	int childY = vMargin;
	GetWindowRect(listBox, &vertContainerDim);

	if (!children.empty()) {
		RECT neighbourDim;
		GetWindowRect(children.at( children.size()-1 ), &neighbourDim);
		childY = neighbourDim.bottom + vMargin - vertContainerDim.top;
	}

	BOOL res1 = SetWindowPos(child, 0, hMargin, childY, 0, 0, SWP_NOSIZE);
	BOOL res2 = GetClientRect(child, &dim);
	childY += vMargin + (dim.bottom - dim.top);

	long maxDragValue = childY - (vertContainerDim.bottom - vertContainerDim.top);
	children.push_back(child);

	const WinEventArgs args = { NULL, listBox, MAKEWPARAM(maxDragValue, 0), (LPARAM)child };
	IApp::eventHandler(DispatchWindowComponent::translateMessage(listBox, WM_CUSTOM_LB_ADD_CHILD), args);

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

	long maxDragValue = childY - (vertContainerDim.bottom - vertContainerDim.top);
	children.insert(children.begin() + insertionIndex, child);

	const WinEventArgs args = { NULL, listBox, MAKEWPARAM(maxDragValue, 0), (LPARAM)child };
	IApp::eventHandler(DispatchWindowComponent::translateMessage(listBox, WM_CUSTOM_LB_ADD_CHILD), args);

	return S_SUCCESS;
}
