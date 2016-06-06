#include "POSIXConsoleApp.h"

// Class Property Implementation //		= nullptr;


// Static Function Implementation //


// Function Implementation //
POSIXConsoleApp::POSIXConsoleApp() 
	: IApp(), Component(std::weak_ptr<IApp>())
{
	
}

POSIXConsoleApp::~POSIXConsoleApp()
{

}

Status POSIXConsoleApp::init(const IEventArgs& evtArgs)
{
//	const POSIXConsoleAppInit& initArgs = static_cast<const POSIXConsoleAppInit&>(evtArgs);
//	cmdLine = initArgs.cmdLine;
//	cmdShow = initArgs.cmdShow;
//
//	eventHandler(WM_CUSTOM_PRE_CREATE, NULL_ARGS);


	return S_SUCCESS;
}

Status POSIXConsoleApp::update()
{
    char opt;
    std::cout << "Enter option: ";
    std::cin >> opt;

	if (opt == 'q')
		return S_APP_EXIT;

	return S_SUCCESS;
}

Status POSIXConsoleApp::terminate(const IEventArgs& evtArgs)
{
	return S_SUCCESS;
}

Status POSIXConsoleApp::registerEvents()
{
	return S_SUCCESS;
}