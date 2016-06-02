#include "CBA_BUILD.h"
#include "Win32App.h"

// Class Property Implementation //
Status Win32App::S_WND_REGISTER_FAIL	= CStatus::registerState(_T("RegisterClassEx() failure"));
Status Win32App::S_WND_CREATE_FAIL		= CStatus::registerState(_T("CreateWindowEx() failure"));
Status Win32App::WM_CUSTOM_PRE_CREATE	= CStatus::registerState(_T("Custom event. Dispated prior to main HWND creation."));
const tstring Win32App::WND_CLASS_NAME  = _T("Win32App");
HINSTANCE Win32App::hinstance			= nullptr;

// Static Function Implementation //
LRESULT CALLBACK Win32App::wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const WinEventArgs args { hinstance, hwnd, wParam, lParam };
	eventHandler(message, args);

	switch (message)
	{
	case WM_WINDOWPOSCHANGING:
	{
		DefWindowProc(hwnd, message, wParam, lParam);
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		//int wmEvent = HIWORD(wParam);
		output(_T("WM_COMMAND: %d\n"), wmId);

		eventHandler(wmId, args);

		switch (wmId)
		{
		default:
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		break;
		}
	}
	break;
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	default:
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	break;
	}

	return 0;
}

// Function Implementation //
Win32App::Win32App() 
	: IApp(), hwnd(nullptr), hAccelTable(nullptr), cmdShow(1), bkColour(CreateSolidBrush(RGB(50, 50, 50))), 
	wndDimensions({ CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT }), wndFlags(WS_BORDER | WS_CAPTION)
{
	
}

Win32App::~Win32App()
{

}

Status Win32App::init(const IEventArgs& evtArgs)
{
	const Win32AppInit& initArgs = static_cast<const Win32AppInit&>(evtArgs);
	hinstance = initArgs.hinstance;
	cmdLine = initArgs.cmdLine;
	cmdShow = initArgs.cmdShow;

	GUID id = addComponent<WinUtilityComponent>(app);
	util = getComponent<WinUtilityComponent>(id);

	registerWindowClass(initArgs);

	eventHandler(WM_CUSTOM_PRE_CREATE, NULL_ARGS);

	initInstance(initArgs);

	return S_SUCCESS;
}

Status Win32App::update()
{
	MSG msg;

	if (!GetMessage(&msg, NULL, 0, 0))
		return S_APP_EXIT;

	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return S_SUCCESS;
}

Status Win32App::terminate(const IEventArgs& evtArgs)
{
	return S_SUCCESS;
}

Status Win32App::registerWindowClass(const Win32AppInit& evtArgs)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hinstance;
	wcex.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(iconID));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = bkColour; 
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WND_CLASS_NAME.c_str();
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(smallIconID));

	if (RegisterClassEx(&wcex) == 0)
		return S_WND_REGISTER_FAIL;

	return S_SUCCESS;
}

Status Win32App::initInstance(const Win32AppInit& evtArgs)
{

	hwnd = CreateWindowEx(0, WND_CLASS_NAME.c_str(), wndTitle.c_str(), wndFlags,	
		wndDimensions.left, wndDimensions.top, wndDimensions.right, wndDimensions.bottom, 
		NULL, NULL, hinstance, NULL);

	if (!hwnd)
		return S_WND_CREATE_FAIL;

	ShowWindow(hwnd, evtArgs.cmdShow);
	UpdateWindow(hwnd);

	return S_SUCCESS;
}

Status Win32App::registerEvents()
{
	return S_SUCCESS;
}