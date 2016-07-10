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

#ifndef CBA_H
#define CBA_H

#ifndef OUTPUT_TO_FILE
#define OUTPUT_TO_FILE	0
#endif // OUTPUT_TO_FILE
#define _CRT_NON_CONFORMING_SWPRINTFS

// C RunTime Header Files
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>
#include <memory.h>
#include <string>
#include <memory>
#include <fstream>

#ifdef _WIN32
#include <SDKDDKVer.h>
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <tchar.h>
#else
#define TCHAR cba_char
#define _T(x) x
#endif // _WIN32

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>

#ifndef UNICODE
typedef std::string tstring;
typedef std::stringstream tstringstream;
typedef std::ofstream tofstream;
typedef char cba_char;
#else
typedef std::wstring tstring;
typedef std::wstringstream tstringstream;
typedef std::wofstream tofstream;
typedef wchar_t cba_char;
#endif

//#pragma message("Parameter cannot be reference because it causes undefined behaviour of variadic argument list (according to c++ standard) \nTODO: Wrap output() in namespace")
void output(const tstring format, ...);

#define outputStr(x) output(_T( x ))
#define outputArgs(x, ...) output(_T( x ), ...)

#endif // CBA_H
