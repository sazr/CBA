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