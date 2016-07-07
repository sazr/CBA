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

#ifndef CBA_IAPP_H
#define CBA_IAPP_H

#include "../CBA.h"
#include "../Status.h"
#include "../EventDelegate.h"
//#include "../Component.h"

class Component;

class IApp //: protected Component
{
public:

	static Status S_APP_EXIT;
	static Status S_UNREGISTERED_EVT;
	static Status S_KILL_PROPAGATION;

	template<typename T>
	static std::shared_ptr<T> create()
	{
		std::shared_ptr<T> app(new T);
		app->app = app;
		return app;
	}

	static Status eventHandler(int evtId, const IEventArgs& evtArgs);

	IApp(const IApp& other) = delete;
	IApp(IApp&& other) = delete;
	IApp& operator=(const IApp& other) = delete;
	IApp& operator=(IApp&& other) = delete;

	Status exec();
	Status registerForEvent(int eventId, const EventDelegate& delegate);
	Status unregisterForEvent(int eventId, const EventDelegate& delegate);

protected:
	static std::unordered_multimap<int, EventDelegate> evtRegistry;

	IApp();
	virtual ~IApp();

	virtual Status update() = 0;

private:

};

#endif // CBA_IAPP_H