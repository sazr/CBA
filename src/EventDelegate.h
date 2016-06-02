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
