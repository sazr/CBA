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

#ifndef CBA_CUSTOMWNDCMP_H
#define CBA_CUSTOMWNDCMP_H

#include "../CBA.h"
#include "Win32App.h"

class CustomWindowComponent : public Component
{
public:
	friend class Component;

	// Static Variables //
	static const tstring PROP_HWND_CMP;

	// Static Methods //
	template <typename T>
	static Status setHWNDComponent(HWND hwnd, T* cmp)
	{
		if (!SetProp(hwnd, PROP_HWND_CMP.c_str(), cmp))
			return S_UNDEFINED_ERROR;

		std::function<Status(const IEventArgs&)> anon = [&hwnd](const IEventArgs& evtArgs) -> Status {
			RemoveProp(hwnd, CustomWindowComponent::PROP_HWND_CMP.c_str());
			return S_SUCCESS;
		};

		cmp->registerEventLambda<T>(WM_DESTROY, anon);

		return S_SUCCESS;
	}

	template <typename T>
	static T* getHWNDComponent(HWND hwnd)
	{
		T* cmp = static_cast<T*>(GetProp(hwnd, PROP_HWND_CMP.c_str()));
		return cmp;
	}

	// Class Variables //

	// Class Methods //
	virtual ~CustomWindowComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	WNDCLASSEX wcex;
	WNDPROC wndCallback;
	const tstring WND_CLASS_NAME;

	// Class Methods //
	CustomWindowComponent(const std::weak_ptr<IApp>& app, const tstring wndClassName, WNDPROC wndCallback, HBRUSH bkColour = NULL);

	Status registerEvents();

private:
	// Static Variables //

	// Static Methods //
	Status registerWindowClass(const WinEventArgs& evtArgs);

	// Class Variables //
	HBRUSH bkColour;

	// Class Methods //

};

#endif // CBA_CUSTOMWNDCMP_H