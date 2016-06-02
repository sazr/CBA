#ifdef _WIN32
	#include "CBA_BUILD.h"
#endif // _WIN32
#include "EventDelegate.h"

EventDelegate::EventDelegate(EDelegate delegate, GUID nGUidContext) : delegate(delegate)
{
	copyGUID(nGUidContext, gUidContext);
}

bool EventDelegate::operator== (const EventDelegate& other) const
{
	return compareGUID(gUidContext, other.gUidContext);
}

bool EventDelegate::operator!= (const EventDelegate& other) const
{
	return compareGUID(gUidContext, other.gUidContext) == false;
}

Status EventDelegate::operator()(const IEventArgs& evtArgs)
{
	return delegate(evtArgs);
}
