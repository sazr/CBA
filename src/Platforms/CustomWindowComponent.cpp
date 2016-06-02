#include "CBA_Build.h"
#include "CustomWindowComponent.h"
#include "Win32App.h"

// Class Property Implementation //
const tstring CustomWindowComponent::PROP_HWND_CMP = _T("PROP_HWND_CMP");


// Static Function Implementation //


// Function Implementation //
CustomWindowComponent::CustomWindowComponent(const std::weak_ptr<IApp>& app, const tstring wndClassName, WNDPROC wndCallback) : Component(app), WND_CLASS_NAME(wndClassName), wndCallback(wndCallback)
{
	registerEvents();
}

CustomWindowComponent::~CustomWindowComponent()
{

}

Status CustomWindowComponent::init(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	
	registerWindowClass(args);

	return S_SUCCESS;
}

Status CustomWindowComponent::terminate(const IEventArgs& evtArgs)
{

	return S_SUCCESS;
}

Status CustomWindowComponent::registerEvents()
{
	registerEvent(WM_CREATE, &CustomWindowComponent::init);
	registerEvent(WM_CLOSE, &CustomWindowComponent::terminate);
	return S_SUCCESS;
}

Status CustomWindowComponent::registerWindowClass(const WinEventArgs& evtArgs)
{
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndCallback;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = evtArgs.hinstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszClassName = WND_CLASS_NAME.c_str();
	//wcex.lpszMenuName = NULL; // MAKEINTRESOURCE(IDC_WT);
	//wcex.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(iconID));
	//wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(smallIconID));

	if (RegisterClassEx(&wcex) == 0)
		return S_UNDEFINED_ERROR;

	return S_SUCCESS;
}