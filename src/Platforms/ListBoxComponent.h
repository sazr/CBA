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
	//static const unsigned int DRAG_THRESHOLD = 10;
	static Status WM_CUSTOM_LB_ADD_CHILD;

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