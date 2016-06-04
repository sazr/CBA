#include "WinUtilityComponent.h"
#include "Win32App.h"

// Class Property Implementation //

// Static Function Implementation //

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

Status WinUtilityComponent::getProcessFilePath(DWORD processId, tstring& filePath)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processId);

	if (hProcess == NULL) {
		output(_T("Failed to retrieve process handle\n"));
		return S_UNDEFINED_ERROR;
	}

	//HANDLE hModule;
	//DWORD cbNeeded;
	TCHAR exePath[MAX_PATH];

	if (GetModuleFileNameEx(hProcess, NULL, exePath, MAX_PATH) == 0){
		output(_T("Failed to get module file name\n"));
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

Status WinUtilityComponent::getClientRect(RECT& clientRect)
{
	clientRect = clientArea;
	return S_SUCCESS;
}