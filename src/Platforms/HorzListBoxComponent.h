#ifndef CBA_HORZLBCMP_H
#define CBA_HORZLBCMP_H

#include "../CBA.h"
#include "ListBoxComponent.h"
#include "DragScrollHandler.h"
#include "Win32App.h"

class HorzListBoxComponent : public ListBoxComponent
{
public:
	friend class Component;

	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	virtual ~HorzListBoxComponent();

	Status onMouseMove(const WinEventArgs& args) override;
	Status hoverScroll(int xPos, int yPos, long& scrollPos);
	Status dragScroll(long pos, long& prevScrollPos, long& distToScroll);

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	long scrollSpeed;
	RECT scrollPrevRect;
	RECT scrollNextRect;

	// Class Methods //
	HorzListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol, 
		unsigned int hMargin, unsigned int vMargin, bool dpiAware = false, DWORD flags = WS_CHILD);

	Status setMaxDragValue() override;
	
private:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	Status appendChild(HWND child) override;
	Status insertChild(HWND child, unsigned int insertionIndex = UINT_MAX) override;

};

#endif // CBA_HORZLBCMP_H