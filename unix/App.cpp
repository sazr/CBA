#ifdef _WIN32
	#include "CBA_BUILD.h"
#endif // _WIN32
#include "App.h"

// Class Property Implementation //		;


// Static Function Implementation //


// Function Implementation //
App::App() 
	: POSIXConsoleApp()
{
	
}

App::~App()
{

}

Status App::init(const IEventArgs& evtArgs)
{
//	const AppInit& initArgs = static_cast<const AppInit&>(evtArgs);
//	cmdLine = initArgs.cmdLine;
//	cmdShow = initArgs.cmdShow;
//
//	eventHandler(WM_CUSTOM_PRE_CREATE, NULL_ARGS);


	return S_SUCCESS;
}

Status App::terminate(const IEventArgs& evtArgs)
{
	return S_SUCCESS;
}

/*Status App::registerEvents()
{
	return S_SUCCESS;
}*/