#include "IApp.h"

// Class Property Implementation //
Status IApp::S_APP_EXIT				= CStatus::registerState(_T("IApp exit"));
Status IApp::S_UNREGISTERED_EVT		= CStatus::registerState(_T("No components registered for this event"));
std::unordered_multimap<int, EventDelegate> IApp::evtRegistry;

// Static Function Implementation //
Status IApp::eventHandler(int evtId, const IEventArgs& evtArgs)
{
	STATE res = S_SUCCESS.state;

	if (evtRegistry.find(evtId) == evtRegistry.end())
		return S_UNREGISTERED_EVT;

	auto registeredCmps = evtRegistry.equal_range(evtId);

	for (auto iter = registeredCmps.first; iter != registeredCmps.second; iter++) {
		Status tmp = iter->second(evtArgs);
		if (!tmp)
			res = tmp.state;
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