#ifndef CBA_LBCMP_H
#define CBA_LBCMP_H

#include "../CBA.h"
#include "CustomWindowComponent.h"
#include "DispatchWindowComponent.h"
#include "DPIAwareComponent.h"
#include "IScrollHandler.h"
#include "Win32App.h"

class ListBoxComponent : public Component
{
public:
	friend class Component;

	// Static Variables //
	static const unsigned int DRAG_THRESHOLD = 10;

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
	Status onLButtonDown(const WinEventArgs& args);
	Status onLButtonUp(const WinEventArgs& args);
	Status onChildLButtonDown(const IEventArgs& evtArgs);
	Status onChildLButtonUp(const IEventArgs& evtArgs);

	virtual Status onMouseMove(const WinEventArgs& args) = 0;
	Status addChild(HWND child, unsigned int insertionIndex = UINT_MAX);
	Status setListBoxClippingRect(const SIZE& windowDim, const SIZE& clientScroll);

	HWND getHwnd();
	void setHwnd(HWND hwnd);
	HBRUSH getBkColour();

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	HWND listBox;
	HBRUSH bkColour;
	DWORD wndFlags;
	bool scrollEnabled;
	bool lButtonDown;
	bool dpiAware;
	bool isDragging;
	long xPos;
	long yPos;
	long maxDragValue;
	unsigned int hMargin;
	unsigned int vMargin;
	long clientDragPos;
	long dragDistance;
	RECT wndDim;
	IScrollHandler scrollHandler;
	std::shared_ptr<DispatchWindowComponent> dispatchCmp;
	std::vector<HWND> children;

	// Class Methods //
	ListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol,
		unsigned int hMargin, unsigned int vMargin, IScrollHandler scrollHandler, bool dpiAware=false, DWORD flags=WS_CHILD);

	Status registerEvents();
	Status setupListener(HWND child);
	
	virtual Status setMaxDragValue() = 0;
	virtual Status appendChild(HWND child) = 0;
	virtual Status insertChild(HWND child, unsigned int insertionIndex = UINT_MAX) = 0;
	
private:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	
	// Class Methods //

};

#endif // CBA_LBCMP_H