#ifndef TEST_APP_H
#define TEST_APP_H

#include "CBA.h"
#include "Platforms/POSIXConsoleApp.h"

#pragma message("TODO: How to force all inheriters of IApp to only instantiate by static IApp::create() method")
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

#endif // TEST_APP_H