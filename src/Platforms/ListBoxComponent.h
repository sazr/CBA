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

#ifndef CBA_LBCMP_H
#define CBA_LBCMP_H

#include "../CBA.h"
#include "CustomWindowComponent.h"
#include "DispatchWindowComponent.h"
#include "DPIAwareComponent.h"
#include "DragScrollerComponent.h"
#include "HoverScrollerComponent.h"
#include "Win32App.h"

class ListBoxComponent : public Component
{
public:
	friend class Component;

	// Static Variables //
	static Status WM_CUSTOM_LB_ADD_CHILD;
	static Status WM_CUSTOM_LB_REMOVE_CHILD;

	// Static Methods //
	
	// Class Variables //

	// Class Methods //
	virtual ~ListBoxComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

	Status onChildMouseMove(const IEventArgs& evtArgs);
	Status onChildLButtonDown(const IEventArgs& evtArgs);
	Status onChildLButtonUp(const IEventArgs& evtArgs);

	Status addChild(HWND child, unsigned int insertionIndex = UINT_MAX);
	Status setListBoxClippingRect(const SIZE& windowDim, const SIZE& clientScroll);

	void enableScroll();
	void disableScroll();

	HWND getHwnd();
	void setHwnd(HWND hwnd);
	HBRUSH getBkColour();
	unsigned int size();

	template <typename T>
	Status setScroller(IScrollerComponent::ScrollDirection scrollDir)
	{
		if (scrollerCmp != NULL) {
			scrollerCmp->disable();
			#pragma message("TODO: implement removeComponent() to delete old ScrollerCmp")
		}

		scrollerCmp = addComponent<T>(app, listBoxHwndId, scrollDir);
		//scrollerCmp->init(NULL_ARGS);
		return S_SUCCESS;
	}

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	Status listBoxHwndId;
	HWND listBox;
	HBRUSH bkColour;
	DWORD wndFlags;
	bool dpiAware;
	unsigned int hMargin;
	unsigned int vMargin;
	RECT wndDim;
	std::shared_ptr<IScrollerComponent> scrollerCmp;
	std::shared_ptr<DispatchWindowComponent> dispatchCmp;
	std::vector<HWND> children;

	// Class Methods //
	ListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol,
		unsigned int hMargin, unsigned int vMargin, bool dpiAware=false, DWORD flags=WS_CHILD);
	
	ListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol,
		unsigned int hMargin, unsigned int vMargin, IScrollerComponent::ScrollDirection scrollDir, bool dpiAware = false, DWORD flags = WS_CHILD);

	Status registerEvents();
	Status setupListener(HWND child);
	
	virtual Status appendChild(HWND child) = 0;
	virtual Status insertChild(HWND child, unsigned int insertionIndex = UINT_MAX) = 0;
	
private:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	
	// Class Methods //

};

#endif // CBA_LBCMP_H