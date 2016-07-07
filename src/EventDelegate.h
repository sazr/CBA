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

#ifndef CBA_EVENTDELEGATE_H
#define CBA_EVENTDELEGATE_H

#include "CBA.h"
#include "GUID.h"
#include "Status.h"

#pragma message("TODO: Similar to WinForms passing event parameters as a void pointer. Any ideas of a better solution that maintains polymorphism?")

struct IEventArgs
{
protected:
	IEventArgs() = default;
	~IEventArgs() = default;

public:
	IEventArgs(const IEventArgs& other) = delete;
	IEventArgs(IEventArgs&& other) = delete;
	IEventArgs& operator=(const IEventArgs& other) = delete;
	IEventArgs& operator=(IEventArgs&& other) = delete;
};

struct NullArgs : public IEventArgs { };
#pragma message("Should 'const NullArgs NULL_ARGS' be a static variable?")
const NullArgs NULL_ARGS;

class EventDelegate
{
public:
	typedef std::function < Status (const IEventArgs&) > EDelegate;

	EventDelegate(EDelegate delegate, GUID nGUidContext);
	EventDelegate(const EventDelegate& other) = default;
	EventDelegate(EventDelegate&& other) = delete;
	EventDelegate& operator=(const EventDelegate& other) = delete;
	EventDelegate& operator=(EventDelegate&& other) = delete;

	bool operator== (const EventDelegate& other) const;
	bool operator!= (const EventDelegate& other) const;

	Status operator()(const IEventArgs& evtArgs);

private:
	GUID gUidContext;
	EDelegate delegate;
};

#endif // CBA_EVENTDELEGATE_H
