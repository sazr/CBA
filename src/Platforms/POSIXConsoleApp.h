#ifndef CBA_POSIXCONSOLEAPP_H
#define CBA_POSIXCONSOLEAPP_H

#include "../CBA.h"
#include "../Status.h"
#include "../Interfaces/IApp.h"

/*struct POSIXConsoleAppInit : public IEventArgs
{
	POSIXConsoleAppInit() = delete; // default;
	POSIXConsoleAppInit(const HINSTANCE& hinstance, const LPTSTR& cmdLine, const int& cmdShow) :
		hinstance(hinstance), cmdLine(cmdLine), cmdShow(cmdShow)
	{}

	const HINSTANCE hinstance;
	const LPTSTR cmdLine;
	const int cmdShow;
};

struct WinEventArgs : public IEventArgs
{
	WinEventArgs() = delete; // default;
	WinEventArgs(const HINSTANCE& hinstance, const HWND& hwnd, const WPARAM& wParam, const LPARAM& lParam) :
		hinstance(hinstance), hwnd(hwnd), wParam(wParam), lParam(lParam)
	{}

	const HINSTANCE hinstance;
	const HWND hwnd;
	const WPARAM wParam;
	const LPARAM lParam;
};*/

#pragma message("TODO: How to force all inheriters of IApp to only instantiate by static IApp::create() method")
class POSIXConsoleApp : public IApp, public Component
{
public:
	//friend class IApp;

	virtual ~POSIXConsoleApp();
	POSIXConsoleApp(const POSIXConsoleApp& other) = delete;
	POSIXConsoleApp(POSIXConsoleApp&& other) = delete;
	POSIXConsoleApp& operator=(const POSIXConsoleApp& other) = delete;
	POSIXConsoleApp& operator=(POSIXConsoleApp&& other) = delete;

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

protected:

	POSIXConsoleApp();

	Status update();
	Status registerEvents();

private:

};

#endif // CBA_POSIXCONSOLEAPP_H