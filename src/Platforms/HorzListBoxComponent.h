#ifndef CBA_HORZLBCMP_H
#define CBA_HORZLBCMP_H

#include "../CBA.h"
#include "CustomWindowComponent.h"
#include "DispatchWindowComponent.h"
#include "DPIAwareComponent.h"
#include "Win32App.h"

class HorzListBoxComponent : public Component
{
public:
	friend class Component;

	// Static Variables //
	static const unsigned int DRAG_THRESHOLD = 10;

	// Static Methods //
	static LRESULT CALLBACK HorzListBoxComponent::wndCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Class Variables //

	// Class Methods //
	virtual ~HorzListBoxComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

	Status onMouseMove(const WinEventArgs& args);
	Status onChildMouseMove(const IEventArgs& evtArgs);
	Status onLButtonDown(const WinEventArgs& args);
	Status onLButtonUp(const WinEventArgs& args);
	Status onChildLButtonDown(const IEventArgs& evtArgs);
	Status onChildLButtonUp(const IEventArgs& evtArgs);

	Status addChild(HWND child);

	HWND getHwnd();
	void setHwnd(HWND hwnd);
	HBRUSH getBkColour();

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	HorzListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol, 
		unsigned int hMargin, unsigned int vMargin, bool dpiAware=false);

	Status registerEvents();
	
private:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	HWND listBox;
	HBRUSH bkColour; 
	bool lButtonDown;
	bool dpiAware;
	bool isDragging;
	unsigned int xPos;
	unsigned int yPos;
	unsigned int maxX;
	unsigned int hMargin;
	unsigned int vMargin;
	unsigned int xCellPos;
	unsigned int clientLeftPos;
	unsigned int dragDistance;
	RECT wndDim;
	std::shared_ptr<DispatchWindowComponent> dispatchCmp;

	// Class Methods //

};

#endif // CBA_HORZLBCMP_H