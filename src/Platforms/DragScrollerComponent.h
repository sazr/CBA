#ifndef CBA_DRAGSCROLLERCMP_H
#define CBA_DRAGSCROLLERCMP_H

#include "../CBA.h"
#include "../Component.h"
#include "IScrollerComponent.h"
#include "DispatchWindowComponent.h"
#include "ListBoxComponent.h"
#include "Win32App.h"

class DragScrollerComponent : public IScrollerComponent
{
public:
	friend class Component;

	// Static Variables //

	// Static Methods //

	// Class Variables //
	
	// Class Methods //
	virtual ~DragScrollerComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

	Status enable() override;
	Status disable() override;

protected:
	// Static Variables //
	static const unsigned int DRAG_THRESHOLD = 10;

	// Static Methods //

	// Class Variables //
	bool lButtonDown;
	bool isDragging;
	long xPos;
	long yPos;
	long dragDistance;

	// Class Methods //
	DragScrollerComponent(const std::weak_ptr<IApp>& app, STATE hwndId, ScrollDirection scrollDir);

	Status registerEvents();
	Status onMouseMove(const IEventArgs& evtArgs);
	Status onLButtonDown(const IEventArgs& evtArgs);
	Status onLButtonUp(const IEventArgs& evtArgs);
	Status dragScroll(HWND hwnd, long pos, long& prevScrollPos);
	Status onLBAddChild(const IEventArgs& evtArgs);

private:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //

};

#endif // CBA_DRAGSCROLLERCMP_H