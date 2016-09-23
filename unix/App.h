#ifndef POSIX_DEMO_H
#define POSIX_DEMO_H

#include "CBA.h"
#include "Platforms/POSIXConsoleApp.h"

class App : public POSIXConsoleApp
{
public:
	friend class IApp;

	virtual ~App();
	
	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

protected:

	App();

private:

};

#endif // POSIX_DEMO_H