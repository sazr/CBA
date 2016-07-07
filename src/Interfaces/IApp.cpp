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

#include "IApp.h"
//#include "../Component.h"

// Class Property Implementation //
Status IApp::S_APP_EXIT				= CStatus::registerState(_T("IApp exit"));
Status IApp::S_UNREGISTERED_EVT		= CStatus::registerState(_T("No components registered for this event"));
Status IApp::S_KILL_PROPAGATION		= CStatus::registerState(_T("Stop propagating message"));
std::unordered_multimap<int, EventDelegate> IApp::evtRegistry;

// Static Function Implementation //
Status IApp::eventHandler(int evtId, const IEventArgs& evtArgs)
{
	STATE res = S_SUCCESS.state;

	if (evtRegistry.find(evtId) == evtRegistry.end())
		return S_UNREGISTERED_EVT;

	auto registeredCmps = evtRegistry.equal_range(evtId);

	for (auto iter = registeredCmps.first; iter != registeredCmps.second; iter++) {
		res = iter->second(evtArgs).state;

		if (res == S_KILL_PROPAGATION.state)
			return S_KILL_PROPAGATION;
	}

	return Status(res);
}

// Function Implementation //
IApp::IApp()
{

}

IApp::~IApp()
{

}

Status IApp::exec()
{
	while (true) {
		Status res = update();

		if (res == S_APP_EXIT)
			break;
		if (!res)
			return res;
	}

	return S_SUCCESS;
}

Status IApp::registerForEvent(int eventId, const EventDelegate& delegate)
{
	// Remove component from existing registry for this event
	unregisterForEvent(eventId, delegate);

	evtRegistry.emplace(eventId, delegate);
	return S_SUCCESS;
}

Status IApp::unregisterForEvent(int eventId, const EventDelegate& delegate)
{
	auto registeredCmps = evtRegistry.equal_range(eventId);

	auto existingDlg = std::find_if(registeredCmps.first, registeredCmps.second, [&delegate](std::pair<const int, EventDelegate>& iter) {
		return iter.second == delegate;
	});

	if (existingDlg != registeredCmps.second)
		evtRegistry.erase(existingDlg);

	/*for (auto iter = registeredCmps.first; iter != registeredCmps.second; iter++) {
		if (iter->second != delegate)
		continue;

		evtRegistry.erase(iter);
		break;
	}*/

	return S_SUCCESS;
}