#include "App.h"

App::App() 
	: POSIXConsoleApp()
{
	
}

App::~App()
{

}

Status App::init(const IEventArgs& evtArgs)
{

	return S_SUCCESS;
}

Status App::terminate(const IEventArgs& evtArgs)
{
	return S_SUCCESS;
}