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

#ifndef CBA_AUTOUPDATECMP_H
#define CBA_AUTOUPDATECMP_H

#include "../CBA.h"
#include "../Component.h"
#include "Win32App.h"
#include "Platforms/Win32App.h"
#include "DownloadComponent.h"

class AutoUpdateComponent : public Component
{
public:
	friend class Component;

	// Static Variables //
	static Status S_VERSION_UP_TO_DATE;

	// Static Methods //


	// Class Variables //


	// Class Methods //
	virtual ~AutoUpdateComponent();

	Status preInit(const IEventArgs& evtArgs);
	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	STATE getVersionId;
	STATE getExeId;
	const std::string versionCheckUrl;
	const std::string dwnldFileUrl;
	const std::string absOutputFilePath;
	const std::string INIFilePath;
	std::string newVersionStr;
	std::shared_ptr<DownloadComponent> dldCmp;

	// Class Methods //
	AutoUpdateComponent(const std::weak_ptr<IApp>& app, const std::string& versionCheckUrl, const std::string& dwnldFileUrl, const std::string& absOutputFilePath, const std::string& INIFilePath);

	Status registerEvents();
	Status onDownloadFileComplete(const IEventArgs& evtArgs);
	Status onDownloadComplete(const IEventArgs& evtArgs);

private:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	
	// Class Methods //

};

#endif // CBA_AUTOUPDATECMP_H