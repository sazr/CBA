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

#include "WinUtilityComponent.h"
#include "Win32App.h"

// Class Property Implementation //

// Static Function Implementation //
Status WinUtilityComponent::getProcessFilePath(DWORD processId, tstring& filePath, bool handleElevatedProcesses)
{
	DWORD flags = PROCESS_QUERY_INFORMATION;
	if (handleElevatedProcesses)
		flags = PROCESS_QUERY_LIMITED_INFORMATION;

	HANDLE hProcess = OpenProcess(flags, FALSE, processId);

	if (hProcess == NULL) {
		output(_T("Failed to retrieve process handle: %s\n"), getLastErrorAsString().c_str());
		return S_UNDEFINED_ERROR;
	}

	//HANDLE hModule;
	//DWORD cbNeeded;
	DWORD len = MAX_PATH;
	TCHAR exePath[MAX_PATH];

	//if (GetModuleFileNameEx(hProcess, NULL, exePath, MAX_PATH) == 0){
	if (QueryFullProcessImageName(hProcess, 0, exePath, &len) == 0) {
		//if (GetProcessImageFileName(hProcess, exePath, MAX_PATH) == 0) {
		output(_T("Failed to get module file name: %s, %s\n"), getLastErrorAsString().c_str(), exePath);
		return S_UNDEFINED_ERROR;
	}

	filePath = tstring(exePath);
	CloseHandle(hProcess);
	return S_SUCCESS;
}

Status WinUtilityComponent::getINISectionNames(const tstring& absINIPath, std::vector<tstring>& sectionNames)
{
	TCHAR sectionNamesBuf[1024];

	if (!GetPrivateProfileSectionNames(sectionNamesBuf, 1024, absINIPath.c_str()))
		return S_UNDEFINED_ERROR;

	for (TCHAR* p = sectionNamesBuf; *p; ++p)
	{
		tstring sectionName(p);
		sectionNames.push_back(sectionName);
		p += sectionName.size();

		output(_T("%s\n"), sectionName.c_str());

		//if (*p == 0)
		//	break;
	}

	return S_SUCCESS;
}

Status WinUtilityComponent::getINISectionNames(const tstring& absINIPath, tstring& sectionNames)
{
	tstringstream ss;
	TCHAR sectionNamesBuf[1024];

	if (!GetPrivateProfileSectionNames(sectionNamesBuf, 1024, absINIPath.c_str()))
		return S_UNDEFINED_ERROR;

	for (TCHAR* p = sectionNamesBuf; *p; ++p)
	{
		tstring sectionName(p);
		ss << sectionName << _T(",");
		p += sectionName.size();

		//if (*p == 0)
		//	break;
	}

	ss >> sectionNames;
	return S_SUCCESS;
}

Status WinUtilityComponent::getINISectionKeyValues(const tstring& absINIPath, const tstring& sectionName, std::vector<tstring>& keys, std::vector<tstring>& values)
{
	TCHAR sectionBuf[10000];

	if (!GetPrivateProfileSection(sectionName.c_str(), sectionBuf, 10000, absINIPath.c_str()))
		return S_UNDEFINED_ERROR;

	for (TCHAR* p = sectionBuf; *p; ++p) {

		tstring keyValuePair(p);
		std::size_t pos = keyValuePair.find('=');
		tstring key = keyValuePair.substr(0, pos);
		tstring value = keyValuePair.substr(pos + 1);
		keys.push_back(key);
		values.push_back(value);

		p += keyValuePair.size();
	}

	return S_SUCCESS;
}

std::string WinUtilityComponent::wstrtostr(const std::wstring &wstr)
{
	// Convert a Unicode string to an ASCII string
	std::string strTo;
	char *szTo = new char[wstr.length() + 1];
	szTo[wstr.size()] = '\0';
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
	strTo = szTo;
	delete[] szTo;
	return strTo;
}

std::wstring WinUtilityComponent::strtowstr(const std::string &str)
{
	// Convert an ASCII string to a Unicode String
	std::wstring wstrTo;
	wchar_t *wszTo = new wchar_t[str.length() + 1];
	wszTo[str.size()] = L'\0';
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszTo, (int)str.length());
	wstrTo = wszTo;
	delete[] wszTo;
	return wstrTo;
}

time_t WinUtilityComponent::getTimeRemote()
{
	DWORD connState;
	if (!InternetGetConnectedState(&connState, 0) || connState & INTERNET_CONNECTION_OFFLINE)
		return NULL;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	time_t res = NULL;
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("200.20.186.76");
	server_addr.sin_port = htons(123);

	protoent *proto = getprotobyname("udp");
	int s = socket(PF_INET, SOCK_DGRAM, proto->p_proto);
	if (s == INVALID_SOCKET) {
		res = NULL;
		goto Cleanup;
	}

	char msg[48] = { 0x08, 0, 0, 0, 0, 0, 0, 0, 0 };
	int iResult = sendto(s, msg, sizeof(msg), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (iResult == SOCKET_ERROR) {
		res = NULL;
		goto Cleanup;
	}

	memset(msg, 0, sizeof(msg));
	struct sockaddr saddr;
	socklen_t saddr_l = sizeof(saddr);
	iResult = recvfrom(s, msg, 48, 0, &saddr, &saddr_l);
	if (iResult == SOCKET_ERROR) {
		res = NULL;
		goto Cleanup;
	}

	time_t rawtime = ntohl(*(u_long *)&msg[40]) - 2208988800U;

Cleanup:
	closesocket(s);
	WSACleanup();

	return /*1000000000 * */rawtime;
}

//Status WinUtilityComponent::getOSVersionString(tstring& versionStr)
//{
//	WCHAR version[MAX_PATH]; 
//	size_t maxlen = MAX_PATH;
//	WCHAR path[_MAX_PATH];
//	if (!GetSystemDirectoryW(path, _MAX_PATH))
//		return S_UNDEFINED_ERROR;
//
//	wcscat_s(path, L"\\kernel32.dll");
//
//	//
//	// Based on example code from this article
//	// http://support.microsoft.com/kb/167597
//	//
//
//	DWORD handle;
//#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
//	DWORD len = GetFileVersionInfoSizeExW(FILE_VER_GET_NEUTRAL, path, &handle);
//#else
//	DWORD len = GetFileVersionInfoSizeW(path, &handle);
//#endif
//	if (!len)
//		return S_UNDEFINED_ERROR;
//
//	std::unique_ptr<uint8_t> buff(new (std::nothrow) uint8_t[len]);
//	if (!buff)
//		return S_UNDEFINED_ERROR;
//
//#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
//	if (!GetFileVersionInfoExW(FILE_VER_GET_NEUTRAL, path, 0, len, buff.get()))
//#else
//	if (!GetFileVersionInfoW(path, 0, len, buff.get()))
//#endif
//		return S_UNDEFINED_ERROR;
//
//	VS_FIXEDFILEINFO *vInfo = nullptr;
//	UINT infoSize;
//
//	if (!VerQueryValueW(buff.get(), L"\\", reinterpret_cast<LPVOID*>(&vInfo), &infoSize))
//		return S_UNDEFINED_ERROR;
//
//	if (!infoSize)
//		return S_UNDEFINED_ERROR;
//
//	swprintf_s(version, maxlen, L"%u.%u.%u.%u",
//		HIWORD(vInfo->dwFileVersionMS),
//		LOWORD(vInfo->dwFileVersionMS),
//		HIWORD(vInfo->dwFileVersionLS),
//		LOWORD(vInfo->dwFileVersionLS));
//	
//	versionStr = version;
//	return S_SUCCESS;
//}


// Function Implementation //
WinUtilityComponent::WinUtilityComponent(const std::weak_ptr<IApp>& app) : Component(app)
{
	registerEvents();
}

WinUtilityComponent::~WinUtilityComponent()
{

}

Status WinUtilityComponent::init(const IEventArgs& evtArgs)
{
	//const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	if (!SystemParametersInfo(SPI_GETWORKAREA, NULL, &clientArea, NULL))
		return S_UNDEFINED_ERROR;

	return S_SUCCESS;
}

Status WinUtilityComponent::terminate(const IEventArgs& evtArgs)
{

	return S_SUCCESS;
}

Status WinUtilityComponent::registerEvents()
{
	registerEvent(WM_CREATE, &WinUtilityComponent::init);
	registerEvent(WM_CLOSE, &WinUtilityComponent::terminate);
	registerEvent(WM_DISPLAYCHANGE, &WinUtilityComponent::onDisplayChange);
	return S_SUCCESS;
}

Status WinUtilityComponent::onDisplayChange(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	int newW = LOWORD(args.lParam);
	int newH = HIWORD(args.lParam);

	if (!SystemParametersInfo(SPI_GETWORKAREA, NULL, &clientArea, NULL))
		return S_UNDEFINED_ERROR;

	output(_T("New Dims: %d, %d, %d, %d, %d, %d, T: %d, T: %d\n"), newW, newH, clientArea.left, clientArea.right, clientArea.top, clientArea.bottom,
		clientArea.right - clientArea.left, clientArea.bottom - clientArea.top);

	return S_SUCCESS;
}

Status WinUtilityComponent::getClientRect(RECT& clientRect)
{
	if (!SystemParametersInfo(SPI_GETWORKAREA, 0, &clientArea, 0))
		return S_UNDEFINED_ERROR;

	clientRect = clientArea;
	return S_SUCCESS;
}

