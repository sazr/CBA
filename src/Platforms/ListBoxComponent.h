#ifndef CBA_LBCMP_H
#define CBA_LBCMP_H

#include "../CBA.h"
#include "CustomWindowComponent.h"
#include "DispatchWindowComponent.h"
#include "DPIAwareComponent.h"
#include "DragScrollerComponent.h"
#include "Win32App.h"

class ListBoxComponent : public Component
{
public:
	friend class Component;

	// Static Variables //
	static const unsigned int DRAG_THRESHOLD = 10;
	static Status WM_CUSTOM_LB_ADD_CHILD;

	// Static Methods //
	static LRESULT CALLBACK wndCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Class Variables //

	// Class Methods //
	virtual ~ListBoxComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

	void enableScroll();
	void disableScroll();

	Status onChildMouseMove(const IEventArgs& evtArgs);
	Status onChildLButtonDown(const IEventArgs& evtArgs);
	Status onChildLButtonUp(const IEventArgs& evtArgs);

	Status addChild(HWND child, unsigned int insertionIndex = UINT_MAX);
	Status setListBoxClippingRect(const SIZE& windowDim, const SIZE& clientScroll);

	HWND getHwnd();
	void setHwnd(HWND hwnd);
	HBRUSH getBkColour();

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