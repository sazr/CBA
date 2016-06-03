#include "TopMostWindowComponent.h"
#include "Win32App.h"

// Class Property Implementation //


// Static Function Implementation //


// Function Implementation //
TopMostWindowComponent::TopMostWindowComponent(const std::weak_ptr<IApp>& app) : Component(app)
{
	registerEvents();
}

TopMostWindowComponent::~TopMostWindowComponent()
{

}

Status TopMostWindowComponent::init(const IEventArgs& evtArgs)
{
	return S_SUCCESS;
}

Status TopMostWindowComponent::terminate(const IEventArgs& evtArgs)
{
	return S_SUCCESS;
}

Status TopMostWindowComponent::registerEvents()
{
	registerEvent(WM_WINDOWPOSCHANGING, &TopMostWindowComponent::onWindowZOrderChange);

	return S_SUCCESS;
}

Status TopMostWindowComponent::onWindowZOrderChange(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	
	SetWindowPos(args.hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//OutputDebugString(_T("TopMostWindowComponent::onWindowZOrderChange\n"));
	
	return S_SUCCESS;
}

