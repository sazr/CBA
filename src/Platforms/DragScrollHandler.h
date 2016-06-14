#ifndef CBA_DRAGSCROLLHANDLER_H
#define CBA_DRAGSCROLLHANDLER_H

#include "../CBA.h"
#include "IScrollHandler.h"

class DragScrollHandler : public IScrollHandler
{
public:

	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	Status onScroll(const WinEventArgs& evtArgs) override;
	Status onMouseDown(const WinEventArgs& evtArgs) override;
	Status onMouseUp(const WinEventArgs& evtArgs) override;

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	
private:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //

};

#endif // CBA_DRAGSCROLLHANDLER_H