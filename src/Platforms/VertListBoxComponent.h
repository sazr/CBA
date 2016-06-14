#ifndef CBA_VERTLBCMP_H
#define CBA_VERTLBCMP_H

#include "../CBA.h"
#include "ListBoxComponent.h"
#include "DragScrollHandler.h"
#include "Win32App.h"

class VertListBoxComponent : public ListBoxComponent
{
public:
	friend class Component;

	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	virtual ~VertListBoxComponent();

	Status onMouseMove(const WinEventArgs& args) override;

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	VertListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol, 
		unsigned int hMargin, unsigned int vMargin, bool dpiAware=false, DWORD flags=WS_CHILD);

	Status setMaxDragValue() override;
	
private:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	Status appendChild(HWND child) override;
	Status insertChild(HWND child, unsigned int insertionIndex = UINT_MAX) override;

};

#endif // CBA_VERTLBCMP_H