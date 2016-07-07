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

	Status removeLastChild();

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