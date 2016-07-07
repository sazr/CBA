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

#ifndef CBA_IMGBTNCMP_H
#define CBA_IMGBTNCMP_H

#include "../CBA.h"
#include "../Component.h"
#include "Platforms/Win32App.h"

class ImageBtnComponent : public Component
{
public:
	friend class Component;

	// Static Variables //
	static const tstring PROP_CENTRE_BMP;
	static const tstring PROP_UP_BMP;
	static const tstring PROP_DOWN_BMP;
	static const tstring PROP_HOVER_BMP;

	// Static Methods //

	// Class Variables //

	// Class Methods //
	virtual ~ImageBtnComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

	Status registerBtnImages(HWND hwnd, HBITMAP upBmp, HBITMAP downBmp, HBITMAP hoverBmp);

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	ImageBtnComponent(const std::weak_ptr<IApp>& app);

	Status registerEvents();
	
private:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //

};

#endif // CBA_IMGBTNCMP_H