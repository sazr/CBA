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

#include "CBA.h"

// Class Property Implementation //


// Static Function Implementation //


// Function Implementation //
//#pragma message("Parameter cannot be reference because it causes undefined behaviour of variadic argument list (according to c++ standard)")
void output(const tstring format, ...)
{
	va_list args;
	va_start(args, format);

	#ifdef _WIN32
	const int len = _vsctprintf(format.c_str(), args) + 1;
	TCHAR* buf = new TCHAR[len];
	_vstprintf_s(buf, len, format.c_str(), args);
	OutputDebugString(buf);

	/*tofstream fout;
	fout.open(_T("cba.log"), std::fstream::out | std::fstream::app);
	fout.write(buf, len);
	fout.write(_T("\n"), 1);
	fout.close();*/

	delete[] buf;
	#else
	TCHAR buf[10000];
	vsprintf (buf, format.c_str(), args);
	printf(buf);
	#endif // _WIN32
}

tstring getLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return tstring(); //No error message has been recorded

	LPTSTR messageBuffer = nullptr;
	size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&messageBuffer, 0, NULL);

	tstring message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}