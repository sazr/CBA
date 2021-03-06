/*
Copyright (c) 2016 Sam Zielke-Ryner All rights reserved.

For job opportunities or to work together on projects please contact
myself via Github:   https://github.com/sazr

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. The source code, API or snippets cannot be used for commercial 
purposes without written consent from the author.

THIS SOFTWARE IS PROVIDED ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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