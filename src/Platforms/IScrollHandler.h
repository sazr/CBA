#ifndef CBA_ISCROLLHANDLER_H
#define CBA_ISCROLLHANDLER_H

#include "../CBA.h"
#include "Win32App.h"

class IScrollHandler
{
public:

	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	virtual Status onScroll(const WinEventArgs& evtArgs)
	{
		return S_UNDEFINED_ERROR;
	}
	
	virtual Status onMouseDown(const WinEventArgs& evtArgs)
	{
		output(_T("F: %s, %d\n"), __FUNCTIONW__, ++i);
		return S_UNDEFINED_ERROR;
	}

	virtual Status onMouseUp(const WinEventArgs& evtArgs) 
	{
		output(_T("F: %s, %d\n"), __FUNCTIONW__, ++i);
		return S_UNDEFINED_ERROR;
	}

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	int i;

	// Class Methods //
	
private:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //

};

#endif // CBA_ISCROLLHANDLER_H