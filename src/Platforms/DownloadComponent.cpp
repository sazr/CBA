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

#include "DownloadComponent.h"

// Class Property Implementation //
Status DownloadComponent::WM_DOWNLOAD_UPDATE = Status::registerState(_T("WM_DOWNLOAD_UPDATE"));
Status DownloadComponent::WM_DOWNLOAD_COMPLETE = Status::registerState(_T("WM_DOWNLOAD_COMPLETE"));

// Static Function Implementation //

// Function Implementation //
DownloadComponent::DownloadComponent(const std::weak_ptr<IApp>& app) : Component(app)
{
	registerEvents();
}

DownloadComponent::~DownloadComponent()
{

}

Status DownloadComponent::init(const IEventArgs& evtArgs)
{
	//const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);

	return S_SUCCESS;
}

Status DownloadComponent::terminate(const IEventArgs& evtArgs)
{

	return S_SUCCESS;
}

Status DownloadComponent::registerEvents()
{
	//registerEvent(WM_CREATE, &DownloadComponent::init);
	//registerEvent(WM_CLOSE, &DownloadComponent::terminate);
	return S_SUCCESS;
}

bool DownloadComponent::checkHTTPVersion(HINTERNET hIurl)
{
	// Check version >= 1.1
	char str[80];
	unsigned long len = 79;

	if (!HttpQueryInfoA(hIurl, HTTP_QUERY_VERSION, &str, &len, NULL))
		return false;

	// First, check major version number
	char *p = strchr(str, '/');
	p++;
	if (*p == '0')
		return false;       // can't use HTTP 0.x

	// Now, find start of minor HTTP version number
	p = strchr(str, '.');
	p++;

	// convert to int
	int minorVerNum = atoi(p);

	if (minorVerNum > 0)
		return true;

	return false;
}

bool DownloadComponent::getFileName(const std::string& absFilePath, std::string& fName)
{
	fName.clear();
	size_t pos = absFilePath.find_last_of('/');

	if (pos == std::string::npos)
		return false;

	fName = absFilePath.substr(pos + 1);
	return true;
}

bool DownloadComponent::isHTTPProtocol(const std::string& url)
{
	std::string protocol = url.substr(0, 7);
	std::transform(protocol.begin(), protocol.end(), protocol.begin(), tolower);
	return protocol == "http://";
}

Status DownloadComponent::downloadFile(const std::string& url, const std::string& outputFilePath, STATE uid, bool reload)
{
	STATE res = S_SUCCESS.state;
	const size_t transferSize = 10240;
	std::ofstream fout;					// output stream
	char buf[transferSize];		// input buffer
	unsigned long numrcved = ULONG_MAX;			// number of bytes read
	unsigned long filelen;			// length of the file on disk
	HINTERNET hIurl = NULL, hInet = NULL;			// internet handles
	unsigned long contentlen = ULONG_MAX;		// length of content
	unsigned long len = ULONG_MAX;				// length of contentlen
	unsigned long total = 0;		// running total of bytes received
	char header[80];				// holds Range header
	DownloadEvtArgs args = { 0, 0, std::string(outputFilePath) };

	output(_T("1\n"));
	if (!isHTTPProtocol(url)) {
		res = S_UNDEFINED_ERROR;
		goto Cleanup;
	}

	/*
	Open the file spcified by url.
	The open stream will be returned in fout. If reload is true, then any
	preexisting file will be truncated. The length of any preexisting file
	(after possible truncation) is returned.
	*/
	output(_T("2\n"));
	filelen = openfile(url, outputFilePath, reload, fout);
	if (filelen == ULONG_MAX) {
		output(_T("Failed to open file\n"));
		return S_UNDEFINED_ERROR;
	}

	// See if internet connection is available
	output(_T("3\n"));
	if (InternetAttemptConnect(0) != ERROR_SUCCESS)
		return S_UNDEFINED_ERROR;

	// Open internet connection
	output(_T("4\n"));
	hInet = InternetOpenA("downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInet == NULL) {
		res = S_UNDEFINED_ERROR;
		goto Cleanup;
	}

	// Construct header requesting range of data
	sprintf_s(header, "Range:bytes=%d-", filelen);

	// Open the URL and request range
	//hIurl = InternetOpenUrl(hInet, url, header, -1, INTERNET_FLAG_NO_CACHE_WRITE, 0);
	output(_T("5\n"));
	hIurl = InternetOpenUrlA(hInet, url.c_str(), header, strlen(header), INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if (hIurl == NULL) {
		res = S_UNDEFINED_ERROR;
		goto Cleanup;
	}

	// Confirm that HTTP/1.1 or greater is supported
	output(_T("6\n"));
	if (!checkHTTPVersion(hIurl)) {
		res = S_UNDEFINED_ERROR;
		goto Cleanup;
	}

	// Get content length
	output(_T("7\n"));
	len = sizeof contentlen;
	if (!HttpQueryInfoA(hIurl, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &contentlen, &len, NULL)) {
		//output(_T("Fail: %d, %s\n"), GetLastError(), getLastErrorAsString().c_str());
		res = S_UNDEFINED_ERROR;
		goto Cleanup;
	}

	// If existing file (if any) is not complete, then finish downloading
	output(_T("8\n"));
	if (filelen != contentlen && contentlen)
	{
		while (numrcved > 0)
		{
			// Read a buffer of info
			if (!InternetReadFile(hIurl, &buf, transferSize, &numrcved)) {
				res = S_UNDEFINED_ERROR;
				goto Cleanup;
			}

			// Write buffer to disk
			fout.write(buf, numrcved);
			if (!fout.good()) {
				res = S_UNDEFINED_ERROR;
				goto Cleanup;
			}

			// update running total
			total += numrcved;

			// Call update function, if specified
			if (/*update &&*/ numrcved > 0) {
				args.curProgress = contentlen + filelen;
				args.totalFileSize = total + filelen;
				//res = update(args); // contentlen + filelen, total + filelen);
				Win32App::eventHandler(DispatchWindowComponent::translateMessage(uid, WM_DOWNLOAD_UPDATE), args);
			}
		}
	}

Cleanup:
	output(_T("cleanup\n"));
	fout.close();
	InternetCloseHandle(hIurl);
	InternetCloseHandle(hInet);

	output(_T("9\n"));
	if (/*onComplete &&*/ (contentlen + filelen) == (total + filelen)) {
		args.curProgress = total + filelen;
		args.totalFileSize = total + filelen;
		//res = onComplete(args);
		Win32App::eventHandler(DispatchWindowComponent::translateMessage(uid, WM_DOWNLOAD_COMPLETE), args);
	}

	return Status(res);
}

Status DownloadComponent::download(const std::string& url, STATE uid)
{
	output(_T("1\n"));
	STATE res = S_SUCCESS.state;
	std::stringstream ss;
	const size_t transferSize = 10240;
	char buf[transferSize] = { 0 };		// input buffer
	unsigned long numrcved = ULONG_MAX;			// number of bytes read
	unsigned long filelen = 0;			// length of the file on disk
	HINTERNET hIurl = NULL, hInet = NULL;			// internet handles
	unsigned long contentlen = ULONG_MAX;		// length of content
	unsigned long len = ULONG_MAX;				// length of contentlen
	unsigned long total = 0;		// running total of bytes received
	char header[80];				// holds Range header
	DownloadEvtArgs args = { 0, 0, "" };

	if (!isHTTPProtocol(url)) {
		res = S_UNDEFINED_ERROR;
		goto Cleanup;
	}

	// See if internet connection is available
	if (InternetAttemptConnect(0) != ERROR_SUCCESS)
		return S_UNDEFINED_ERROR;

	// Open internet connection
	hInet = InternetOpenA("downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInet == NULL) {
		res = S_UNDEFINED_ERROR;
		goto Cleanup;
	}

	// Construct header requesting range of data
	sprintf_s(header, "Range:bytes=%d-", 0);

	// Open the URL and request range
	hIurl = InternetOpenUrlA(hInet, url.c_str(), header, strlen(header), INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if (hIurl == NULL) {
		res = S_UNDEFINED_ERROR;
		goto Cleanup;
	}

	output(_T("1\n"));
	// Confirm that HTTP/1.1 or greater is supported
	if (!checkHTTPVersion(hIurl)) {
		res = S_UNDEFINED_ERROR;
		goto Cleanup;
	}

	output(_T("2\n"));
	while (numrcved > 0)
	{
		// Read a buffer of info
		output(_T("loop 1\n"));
		if (!InternetReadFile(hIurl, &buf, transferSize, &numrcved)) {
			output(_T("loop 2 exit\n"));
			res = S_UNDEFINED_ERROR;
			goto Cleanup;
		}

		if (numrcved <= 0)
			break;

		ss << buf;
		total += numrcved;

		// Call update function, if specified
		args.curProgress = total;
		args.totalFileSize = ULONG_MAX;
		//res = update(args); // contentlen + filelen, total + filelen);
		Win32App::eventHandler(DispatchWindowComponent::translateMessage(uid, WM_DOWNLOAD_UPDATE), args);
	}

Cleanup:
	output(_T("cleanup\n"));
	InternetCloseHandle(hIurl);
	InternetCloseHandle(hInet);

	args.curProgress = total;
	args.totalFileSize = ULONG_MAX;
	ss >> args.data;
	Win32App::eventHandler(DispatchWindowComponent::translateMessage(uid, WM_DOWNLOAD_COMPLETE), args);
	
	output(_T("Exiting\n"));
	return Status(res);
}

unsigned long DownloadComponent::openfile(const std::string& url, const std::string& outputFilePath, bool reload, std::ofstream& fout)
{
	/*std::string fname;

	if (!getFileName(url, fname))
		return ULONG_MAX;*/

	if (!reload)
		fout.open(outputFilePath.c_str(), std::ios::binary | std::ios::out | std::ios::app | std::ios::ate);
	else
		fout.open(outputFilePath.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);

	if (!fout)
		return ULONG_MAX;

	return fout.tellp();
}
