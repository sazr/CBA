/*
Copyright (c) 2016 Sam Zielke-Ryner All rights reserved.

For job opportunities or to work together on projects please contact
myself via Github:   https://github.com/sazr

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. The source code, API or snippets cannot be used for commercial 
purposes without written consent from the author.

THIS SOFTWARE IS PROVIDED ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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