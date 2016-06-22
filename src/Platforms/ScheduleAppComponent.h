#ifndef CBA_SCHEDULEAPPCMP_H
#define CBA_SCHEDULEAPPCMP_H

#include "../CBA.h"
#include "../Component.h"
#include "Win32App.h"
#include "../Utility/EasyTaskScheduler.h"
//#include <algorithm>

class ScheduleAppComponent : public Component
{
public:
	friend class Component;

	// Static Variables //

	// Static Methods //

	// Class Variables //
	const tstring TASK_NAME;

	// Class Methods //
	virtual ~ScheduleAppComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);
	Status unregisterScheduledTask();

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	ScheduleAppComponent(const std::weak_ptr<IApp>& app, const tstring taskName);

	Status registerEvents();
	
private:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //

};

#endif // CBA_SCHEDULEAPPCMP_H