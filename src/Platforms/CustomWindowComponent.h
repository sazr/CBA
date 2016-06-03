#ifndef CBA_CUSTOMWNDCMP_H
#define CBA_CUSTOMWNDCMP_H

#include "../CBA.h"
#include "../Component.h"
#include "Win32App.h"

class CustomWindowComponent : public Component
{
public:
	friend class Component;

	// Static Variables //
	static const tstring PROP_HWND_CMP;

	// Static Methods //
	template <typename T>
	static Status setHWNDComponent(HWND hwnd, T* cmp)
	{
		if (!SetProp(hwnd, PROP_HWND_CMP.c_str(), cmp))
		return S_UNDEFINED_ERROR;
		
		std::function<Status(const IEventArgs&)> anon = [&hwnd](const IEventArgs& evtArgs) -> Status {
			RemoveProp(hwnd, CustomWindowComponent::PROP_HWND_CMP.c_str());
			return S_SUCCESS;
		};

		cmp->registerEventLambda<T>(WM_DESTROY, anon);

		return S_SUCCESS;
	}

	template <typename T>
	static T* getHWNDComponent(HWND hwnd)
	{
		T* cmp = static_cast<T*>(GetProp(hwnd, PROP_HWND_CMP.c_str()));
		return cmp;
	}

	// Class Variables //
	
	// Class Methods //
	virtual ~CustomWindowComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	WNDCLASSEX wcex;
	WNDPROC wndCallback;
	const tstring WND_CLASS_NAME;

	// Class Methods //
	CustomWindowComponent(const std::weak_ptr<IApp>& app, const tstring wndClassName, WNDPROC wndCallback);

	Status registerEvents();
	
private:
	// Static Variables //

	// Static Methods //
	Status registerWindowClass(const WinEventArgs& evtArgs);

	// Class Variables //

	// Class Methods //

};

#endif // CBA_CUSTOMWNDCMP_H