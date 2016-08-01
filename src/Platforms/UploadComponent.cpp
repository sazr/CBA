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

#include "UploadComponent.h"

// Class Property Implementation //
Status UploadComponent::WM_UPLOAD_COMPLETE = Status::registerState(_T("WM_UPLOAD_COMPLETE"));

// Static Function Implementation //

// Function Implementation //
UploadComponent::UploadComponent(const std::weak_ptr<IApp>& app) : Component(app)
{
	registerEvents();
}

UploadComponent::~UploadComponent()
{

}

Status UploadComponent::init(const IEventArgs& evtArgs)
{
	//const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	return S_SUCCESS;
}

Status UploadComponent::terminate(const IEventArgs& evtArgs)
{

	return S_SUCCESS;
}

Status UploadComponent::registerEvents()
{
	return S_SUCCESS;
}

Status UploadComponent::uploadFile(const tstring& domain, const tstring& domainScript, const tstring& filePath, STATE uid)
{
	tifstream t(filePath);
	tstring fileContents((std::istreambuf_iterator<wchar_t>(t)),
		std::istreambuf_iterator<wchar_t>());

	t.close();
	
	return upload(domain, domainScript, fileContents, uid);
}

Status UploadComponent::upload(const tstring& domain, const tstring& domainScript, const tstring& postData, STATE uid)
{
	tstring authToken;
	if (!AuthComponent::getAuthToken(authToken))
		return S_UNDEFINED_ERROR;

	STATE res = S_SUCCESS.state;
	TCHAR hdrs[] = _T("Content-Type: application/x-www-form-urlencoded");
	LPSTR accept[2] = { "*/*", NULL };

	HINTERNET hConnect = NULL, hSession = NULL, hRequest = NULL;
	DWORD len = postData.length() * 2;
	tstring formattedPostData(len, ' ');
	InternetCanonicalizeUrl(postData.c_str(), &formattedPostData[0], &len, ICU_BROWSER_MODE);
	std::replace(formattedPostData.begin(), formattedPostData.end(), '=', '~');
	tstring authData = _T("usage=") + formattedPostData + _T("&auth=") + authToken;

	hSession = InternetOpen(_T("uploader"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!hSession) {
		output(_T("InternetOpen failed: %s\n"), getLastErrorAsString().c_str());
		res = S_UNDEFINED_ERROR.state;
		goto Cleanup;
	}

	hConnect = InternetConnect(hSession, domain.c_str(),
		INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
	if (!hConnect) {
		output(_T("InternetConnect failed: %s\n"), getLastErrorAsString().c_str());
		res = S_UNDEFINED_ERROR.state;
		goto Cleanup;
	}

	hRequest = HttpOpenRequest(hConnect, _T("POST"),
		domainScript.c_str(), NULL, NULL, (LPCWSTR*)&accept, INTERNET_FLAG_NO_CACHE_WRITE, 1);
	if (!hRequest) {
		output(_T("HttpOpenRequest failed: %s\n"), getLastErrorAsString().c_str());
		res = S_UNDEFINED_ERROR.state;
		goto Cleanup;
	}

	if (!HttpSendRequest(hRequest, hdrs, -1, (LPVOID)authData.c_str(), authData.size() * sizeof(TCHAR))) {
		output(_T("HttpSendRequest failed: %d, %s.\n"), GetLastError(), getLastErrorAsString().c_str());
		res = S_UNDEFINED_ERROR.state;
		goto Cleanup;
	}

Cleanup:
	InternetCloseHandle(hSession);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hRequest);

	if (res == S_SUCCESS.state)
		Win32App::eventHandler(DispatchWindowComponent::translateMessage(uid, WM_UPLOAD_COMPLETE), NULL_ARGS);

	return Status(res);
}
