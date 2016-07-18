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

#include "RunApplicationComponent.h"

// Class Property Implementation //
Status RunApplicationComponent::RESIZE_APP = Status::registerState(_T("RESIZE_APP"));
const unsigned int RunApplicationComponent::MAX_TRIES = 250;

// Static Function Implementation //
BOOL CALLBACK RunApplicationComponent::enumWindows(HWND hwnd, LPARAM lParam)
{
	RunApplicationComponent* runCmp = (RunApplicationComponent*)lParam;

	if (runCmp->isAltTabWindow(hwnd))
		runCmp->openWnds.push_back(hwnd);

	return TRUE;
}

// Function Implementation //
RunApplicationComponent::RunApplicationComponent(const std::weak_ptr<IApp>& app) 
	: Component(app), nTries(0)
{
	registerEvents();
}

RunApplicationComponent::~RunApplicationComponent()
{

}

Status RunApplicationComponent::init(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	mainHwnd = args.hwnd;

	return S_SUCCESS;
}

Status RunApplicationComponent::terminate(const IEventArgs& evtArgs)
{
	
	return S_SUCCESS;
}

Status RunApplicationComponent::registerEvents()
{
	registerEvent(WM_CREATE, &RunApplicationComponent::init);
	//registerEvent(WM_CLOSE, &RunApplicationComponent::terminate);
	registerEvent(DispatchWindowComponent::translateMessage(RESIZE_APP, WM_TIMER), &RunApplicationComponent::onTimer);
	return S_SUCCESS;
}

Status RunApplicationComponent::runApplications(const std::vector<HwndInfo>& nHwndInfos, const std::vector<HWND>& nHwndIgnoreList)
{
	KillTimer(mainHwnd, RESIZE_APP);
	nTries = 0;
	hwndIgnoreList.clear();
	hwndInfos.clear();
	hwndIgnoreList = nHwndIgnoreList;
	hwndInfos = nHwndInfos;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &clientRect, 0);

	for (auto it = hwndInfos.begin(); it != hwndInfos.end(); ) {
		// open app
		// create timer for 100ms to check for process and resize

		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		ZeroMemory(&pi, sizeof(pi));
		
		si.cb = sizeof(si);
		si.dwX = (clientRect.right - clientRect.left) * (*it).wndDimScaled.left;
		si.dwY = (clientRect.bottom - clientRect.top) * (*it).wndDimScaled.top;
		si.dwXSize = (clientRect.right - clientRect.left) * (*it).wndDimScaled.right;
		si.dwYSize = (clientRect.bottom - clientRect.top) * (*it).wndDimScaled.bottom;

		if (!CreateProcess((*it).exePath, _T(""), //_T(" -new-window "),
			NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL,
			NULL, &si, &pi)) {
			output(_T("Failed create process\n"));
			it = hwndInfos.erase(it);
			continue;
		}
		
		it++;
	}

	SetTimer(mainHwnd, RESIZE_APP, 100, NULL);
	return S_SUCCESS;
}

Status RunApplicationComponent::onTimer(const IEventArgs& evtArgs)
{
	output(_T("RunApplicationComponent::onTimer: %d\n"), hwndInfos.size());
	nTries++;

	// Catalogue windows
	openWnds.clear();
	EnumWindows(RunApplicationComponent::enumWindows, (LPARAM)this);

	for (int i = 0; i < openWnds.size(); i++) {

		DWORD pid;
		tstring exePath;
		GetWindowThreadProcessId(openWnds.at(i), &pid);
		bool inIgnoreList = std::find(hwndIgnoreList.begin(), hwndIgnoreList.end(), openWnds[i]) != hwndIgnoreList.end();

		if (inIgnoreList)
			continue;

		getProcessFilePath(pid, exePath);

		for (int j = 0; j < hwndInfos.size(); j++) {
			if (_tcscmp(hwndInfos.at(j).exePath, exePath.c_str()) != 0)
				continue;

			RECT wndDim;
			wndDim.left = (clientRect.right - clientRect.left) * hwndInfos[j].wndDimScaled.left;
			wndDim.top = (clientRect.bottom - clientRect.top) * hwndInfos[j].wndDimScaled.top;
			wndDim.right = (clientRect.right - clientRect.left) * hwndInfos[j].wndDimScaled.right;
			wndDim.bottom = (clientRect.bottom - clientRect.top) * hwndInfos[j].wndDimScaled.bottom;

			WINDOWPLACEMENT wndPlacement;
			wndPlacement.length = sizeof(WINDOWPLACEMENT);
			wndPlacement.showCmd = hwndInfos[j].showState; // SW_SHOWNORMAL;
			wndPlacement.rcNormalPosition = wndDim;
			SetWindowPlacement(openWnds.at(i), &wndPlacement);

			// Bring to front
			SetWindowPos(openWnds.at(i), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

			hwndInfos.erase(hwndInfos.begin() + j);
			hwndIgnoreList.push_back(openWnds[i]);
			break;
		}
	}

	if (hwndInfos.empty() || nTries >= MAX_TRIES)
		KillTimer(mainHwnd, RESIZE_APP);

	return S_SUCCESS;
}

bool RunApplicationComponent::isMainWindow(HWND hwnd)
{
	return GetWindow(hwnd, GW_OWNER) == (HWND)0 && IsWindowVisible(hwnd);
}

bool RunApplicationComponent::isAltTabWindow(HWND hwnd)
{
	#pragma message("Bug: currently misses some top level windows incl Piriform CCleaner.")
	TITLEBARINFO ti;
	HWND hwndTry, hwndWalk = NULL;

	if (!IsWindowVisible(hwnd))
		return false;

	hwndTry = GetAncestor(hwnd, GA_ROOTOWNER);
	while (hwndTry != hwndWalk)
	{
		hwndWalk = hwndTry;
		hwndTry = GetLastActivePopup(hwndWalk);
		if (IsWindowVisible(hwndTry))
			break;
	}

	if (hwndWalk != hwnd)
		return false;

	// the following removes some task tray programs and "Program Manager"
	ti.cbSize = sizeof(ti);
	GetTitleBarInfo(hwnd, &ti);
	if (ti.rgstate[0] & STATE_SYSTEM_INVISIBLE)
		return false;

	// Tool windows should not be displayed either, these do not appear in the
	// task bar.
	if (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)
		return false;

	return true;
}


Status RunApplicationComponent::getProcessFilePath(DWORD processId, tstring& filePath)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processId);

	if (hProcess == NULL) {
		output(_T("Failed to retrieve process handle\n"));
		return S_UNDEFINED_ERROR;
	}

	TCHAR exePath[MAX_PATH];

	if (GetModuleFileNameEx(hProcess, NULL, exePath, MAX_PATH) == 0){
		output(_T("Failed to get module file name: %s\n"), getLastErrorAsString().c_str());
		return S_UNDEFINED_ERROR;
	}

	filePath = tstring(exePath);
	CloseHandle(hProcess);
	return S_SUCCESS;
}


