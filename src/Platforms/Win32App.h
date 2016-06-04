#ifndef CBA_WIN32APP_H
#define CBA_WIN32APP_H

#include "../CBA.h"
#include "../Component.h"
#include "../Interfaces/IApp.h"
#include "WinUtilityComponent.h" 

struct Win32AppInit : public IEventArgs
{
	Win32AppInit() = delete; // default;
	Win32AppInit(const HINSTANCE& hinstance, const LPTSTR& cmdLine, const int& cmdShow) :
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
};

#pragma message("TODO: How to force all inheriters of IApp to only instantiate by static IApp::create() method")
class Win32App : public IApp, public Component
{
public:
	//friend class IApp;

	static Status S_WND_REGISTER_FAIL;
	static Status S_WND_CREATE_FAIL;
	static Status WM_CUSTOM_PRE_CREATE;
	static const tstring WND_CLASS_NAME;
	static HINSTANCE hinstance;

	virtual ~Win32App();
	Win32App(const Win32App& other) = delete;
	Win32App(Win32App&& other) = delete;
	Win32App& operator=(const Win32App& other) = delete;
	Win32App& operator=(Win32App&& other) = delete;

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

protected:
	HWND hwnd;
	HACCEL hAccelTable;
	tstring wndTitle;
	LPTSTR cmdLine;
	int cmdShow;
	HBRUSH bkColour;
	RECT wndDimensions;
	DWORD wndFlags;
	int iconID;
	int smallIconID;
	std::shared_ptr <WinUtilityComponent> util;

	Win32App();

	Status update();
	Status registerEvents();
	Status registerWindowClass(const Win32AppInit& evtArgs);
	Status initInstance(const Win32AppInit& evtArgs);
	static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

};

#endif // CBA_WIN32APP_H