#ifndef CBA_HOVERSCROLLERCMP_H
#define CBA_HOVERSCROLLERCMP_H

#include "../CBA.h"
#include "../Component.h"
#include "IScrollerComponent.h"
#include "DispatchWindowComponent.h"
#include "ListBoxComponent.h"
#include "Win32App.h"

class HoverScrollerComponent : public IScrollerComponent
{
public:
	friend class Component;

	// Static Variables //
	static unsigned int SCROLL_DELAY;

	// Static Methods //

	// Class Variables //
	
	// Class Methods //
	virtual ~HoverScrollerComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

	Status enable() override;
	Status disable() override;

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	bool timerSet;
	bool canScroll;
	long scrollSpeed;
	RECT scrollPrevRect;
	RECT scrollNextRect;

	// Class Methods //
	HoverScrollerComponent(const std::weak_ptr<IApp>& app, STATE hwndId, ScrollDirection scrollDir);

	Status registerEvents();
	Status onShow(const IEventArgs& evtArgs);
	Status onMouseMove(const IEventArgs& evtArgs);
	Status onMouseLeave(const IEventArgs& evtArgs);
	Status onLButtonDown(const IEventArgs& evtArgs);
	Status onLButtonUp(const IEventArgs& evtArgs);
	Status onTimer(const IEventArgs& evtArgs);
	Status hoverScroll(HWND hwnd, int xPos, int yPos);
	Status onLBAddChild(const IEventArgs& evtArgs);

	void reset(HWND hwnd);

private:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //

};

#endif // CBA_HOVERSCROLLERCMP_H