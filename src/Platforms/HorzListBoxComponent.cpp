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
	int childX = 0; // hMargin;
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

	//long maxDragValue = childX - (horzContainerDim.right - horzContainerDim.left);
	long widthIncrease = hMargin + (dim.right - dim.left);
	children.push_back(child);

	const WinEventArgs args = { NULL, listBox, MAKEWPARAM(widthIncrease, 0), (LPARAM)child };
	IApp::eventHandler(DispatchWindowComponent::translateMessage(listBox, WM_CUSTOM_LB_ADD_CHILD), args);

	return S_SUCCESS;
}

Status HorzListBoxComponent::insertChild(HWND child, unsigned int insertionIndex)
{
	if (insertionIndex >= children.size())
		return S_UNDEFINED_ERROR;

	setupListener(child);
	
	RECT horzContainerDim, dim, cDim;
	int childX = 0; // hMargin;
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

	//long maxDragValue = childX - (horzContainerDim.right - horzContainerDim.left);
	long widthIncrease = hMargin + (dim.right - dim.left);
	children.insert(children.begin() + insertionIndex, child);

	const WinEventArgs args = { NULL, listBox, MAKEWPARAM(widthIncrease, 0), (LPARAM)child };
	IApp::eventHandler(DispatchWindowComponent::translateMessage(listBox, WM_CUSTOM_LB_ADD_CHILD), args);

	return S_SUCCESS;
}

Status HorzListBoxComponent::removeLastChild()
{
	if (children.empty())
		return S_UNDEFINED_ERROR;

	RECT dim;
	HWND child = children[children.size() - 1];
	GetClientRect(child, &dim);
	long widthDecrease = hMargin + (dim.right - dim.left);
	children.pop_back();
	DestroyWindow(child);

	const WinEventArgs args = { NULL, listBox, (WPARAM)widthDecrease, 0 };
	IApp::eventHandler(DispatchWindowComponent::translateMessage(listBox, WM_CUSTOM_LB_REMOVE_CHILD), args);

	return S_SUCCESS;
}