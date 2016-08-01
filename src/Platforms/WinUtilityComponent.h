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

#ifndef CBA_WinUtilityComponentCMP_H
#define CBA_WinUtilityComponentCMP_H

#include "../CBA.h"
#include "../Component.h"
#include <Psapi.h>
#include <Wininet.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <time.h>
#pragma comment(lib, "Wininet")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Psapi")
#pragma comment(lib, "Mincore")

class WinUtilityComponent : public Component
{
public:
	friend class Component;

	// Static Variables //
	

	// Static Methods //
	static Status getProcessFilePath(DWORD processId, tstring& filePath, bool handleElevatedProcesses = false);
	static Status getINISectionNames(const tstring& absINIPath, std::vector<tstring>& sectionNames);
	static Status getINISectionNames(const tstring& absINIPath, tstring& sectionNames);
	static Status getINISectionKeyValues(const tstring& absINIPath, const tstring& sectionName, std::vector<tstring>& keys, std::vector<tstring>& values);
	static std::wstring WinUtilityComponent::strtowstr(const std::string &str);
	static std::string WinUtilityComponent::wstrtostr(const std::wstring &wstr);
	static time_t WinUtilityComponent::getTimeRemote();
	//static Status getOSVersionString(tstring& versionStr);

	// Class Variables //


	// Class Methods //
	virtual ~WinUtilityComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

	Status getClientRect(RECT& clientRect);

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	WinUtilityComponent(const std::weak_ptr<IApp>& app);

	Status registerEvents();
	Status onDisplayChange(const IEventArgs& evtArgs);

private:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	RECT clientArea;

	// Class Methods //

};

#endif // CBA_WinUtilityComponentCMP_H