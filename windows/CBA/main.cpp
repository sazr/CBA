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

#include "CBA.h"
#include "Platforms/Win32App.h"
#include "Platforms/HorzListBoxComponent.h"

class App : public Win32App
{
public:

	virtual ~App()
	{
		
	}
	
	Status init(const IEventArgs& evtArgs)
	{
		addComponent<HorzListBoxComponent>(app, RECT{ 10, 10, 300, 300 }, (HBRUSH)GetStockObject(DKGRAY_BRUSH), 10, 10);
		return S_SUCCESS;
	}
	
	Status terminate(const IEventArgs& evtArgs)
	{
		return S_SUCCESS;
	}
	
protected:

	App() : Win32App()
	{
		
	}

};

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	auto app = IApp::create<App>();

	app->init(Win32AppInit(hInstance, lpCmdLine, nCmdShow));
	app->exec();
	app->terminate(NULL_ARGS);
	return 0;
}

