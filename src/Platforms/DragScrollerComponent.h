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
	static const unsigned int LBUTTONDOWN_DELAY;
	static const STATE LBUTTONDOWN_TIMER;

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
	static const unsigned int DRAG_THRESHOLD = 100;

	// Static Methods //

	// Class Variables //
	bool lButtonDown;
	bool isDragging;
	bool killOnCommand;
	long xPos;
	long yPos;
	long dragDistance;
	HWND lButtonDownHwnd;
	WPARAM lButtonDownWParam;
	LPARAM lButtonDownLParam;

	// Class Methods //
	DragScrollerComponent(const std::weak_ptr<IApp>& app, STATE hwndId, ScrollDirection scrollDir);

	Status registerEvents();
	Status onCommand(const IEventArgs& evtArgs);
	Status onMouseMove(const IEventArgs& evtArgs);
	Status onLButtonDown(const IEventArgs& evtArgs);
	Status onLButtonUp(const IEventArgs& evtArgs);
	Status onTimer(const IEventArgs& evtArgs);
	Status dragScroll(HWND hwnd, long pos, long& prevScrollPos);
	Status onLBAddChild(const IEventArgs& evtArgs);
	Status onLBRemoveChild(const IEventArgs& evtArgs);

private:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //

};

#endif // CBA_DRAGSCROLLERCMP_H