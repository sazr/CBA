#ifndef CBA_HORZLBCMP_H
#define CBA_HORZLBCMP_H

#include "../CBA.h"
#include "ListBoxComponent.h"
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

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	HorzListBoxComponent(const std::weak_ptr<IApp>& app, const RECT& wndDim, HBRUSH bkCol, 
		unsigned int hMargin, unsigned int vMargin, bool dpiAware = false, DWORD flags = WS_CHILD);

private:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	Status appendChild(HWND child) override;
	Status insertChild(HWND child, unsigned int insertionIndex = UINT_MAX) override;

};

#endif // CBA_HORZLBCMP_H