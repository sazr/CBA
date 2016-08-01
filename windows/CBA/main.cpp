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
#include "Component.h"
#include "Platforms/HorzListBoxComponent.h"
#include "Platforms/Win32App.h"

class S : public Component
{
public:
	S() : Component(std::weak_ptr<IApp>())
	{

	}

	Status init(const IEventArgs& args)
	{
		return S_SUCCESS;
	}

	Status terminate(const IEventArgs& args)
	{
		return S_SUCCESS;
	}

	Status registerEvents()
	{
		return S_SUCCESS;
	}

protected:
};

class App : public Win32App
{
public:
	App() : Win32App()
	{
		
	}

	Status init(const IEventArgs& evtArgs)
	{
		printf("Abc\n");
		addComponent<HorzListBoxComponent>(app, RECT{ 10, 10, 300, 300 }, (HBRUSH)GetStockObject(DKGRAY_BRUSH), 10, 10);
		return S_SUCCESS;
	}
protected:

};

int main(int argc, char** argv)
{
	Status s = Status::registerState(_T("a"));
	output(_T("a: %s, %s\n"), _T("a"), s.toString());

	GUID g;
	generateGUID(g);
	std::function<Status(const IEventArgs&)> f = [](const IEventArgs& args)->Status {
		return S_SUCCESS;
	};
	EventDelegate e(f, g);
	e(NULL_ARGS);

	S d;
	//d.a<int>(4);
	auto fc = d.addComponent<S>();

	App a;
	a.init(NULL_ARGS);
	system("pause");
	return 0;
}

