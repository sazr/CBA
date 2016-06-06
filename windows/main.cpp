#include "CBA.h"
#include "Component.h"
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
		outputStr("abc\n");
		return S_SUCCESS;
	};
	EventDelegate e(f, g);
	e(NULL_ARGS);

	S d;
	//d.a<int>(4);
	auto fc = d.addComponent<S>();

	App a;
	system("pause");
	return 0;
}

