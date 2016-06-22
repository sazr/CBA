#ifndef CBA_DISPATCHWNDCMP_H
#define CBA_DISPATCHWNDCMP_H

#include "../CBA.h"
#include "../Component.h"
#include "Win32App.h"

struct DispatchEventArgs : public IEventArgs
{
	DispatchEventArgs() = delete; // default;
	DispatchEventArgs(const HWND& hwnd, const HWND& catalystHwnd, const UINT& message, const WPARAM& wParam, const LPARAM& lParam) :
		hwnd(hwnd), catalystHwnd(catalystHwnd), message(message), wParam(wParam), lParam(lParam)
	{}

	const HWND hwnd;
	const HWND catalystHwnd;
	const UINT message;
	const WPARAM wParam;
	const LPARAM lParam;
};

class DispatchWindowComponent : public Component
{
public:
	friend class Component;

	// Static Variables //
	static Status WM_DISPATCH_MSG;
	static Status WM_STOP_PROPAGATION_MSG;
	static const tstring PROP_ORIG_CALLBACK;
	static const tstring PROP_ID;

	// Static Methods //
	static LRESULT CALLBACK dispatchCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK customWndCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static int translateMessage(int hwndID, UINT message);
	static int translateMessage(HWND hwnd, UINT message);

	// Class Variables //
	std::unordered_map<int, WNDPROC> originalWndProcs;

	// Class Methods //
	virtual ~DispatchWindowComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

	Status addDispatcher(HWND child);

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	DispatchWindowComponent(const std::weak_ptr<IApp>& app);

	Status registerEvents();
	
private:
	// Static Variables //

	// Static Methods //
	
	// Class Variables //

	// Class Methods //

};

#endif // CBA_DISPATCHWNDCMP_H