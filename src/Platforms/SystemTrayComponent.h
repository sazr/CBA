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

#ifndef CBA_SYSTRAYCMP_H
#define CBA_SYSTRAYCMP_H

#include "../CBA.h"
#include "../Component.h"
#include "Win32App.h"
#include <shellapi.h>

class SystemTrayComponent : public Component
{
public:
	friend class Component;

	// Static Variables //
	static const Status WM_TRAY_ICON;

	// Static Methods //

	// Class Variables //
	Status ID_TRAY_ICON = Status::registerState(_T("System Tray Icon ID"));
	Status trayIconID;
	NOTIFYICONIDENTIFIER identifier;

	// Class Methods //
	virtual ~SystemTrayComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	NOTIFYICONDATA notifyIconData;
	const tstring smallIconPath;
	const tstring tooltip;

	// Class Methods //
	SystemTrayComponent(const std::weak_ptr<IApp>& app, const tstring smallIconPath, const tstring tooltip);

	Status registerEvents();

private:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //

};

#endif // CBA_SYSTRAYCMP_H