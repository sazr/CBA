#ifndef CBA_H
#define CBA_H

#define _CRT_NON_CONFORMING_SWPRINTFS

// C RunTime Header Files
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>
#include <memory.h>
#include <string>
#include <memory>

#ifdef _WIN32
#include <tchar.h>
#else
#define TCHAR PCHAR
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
typedef char PCHAR;
#else
typedef std::wstring tstring;
typedef std::wstringstream tstringstream;
typedef wchar_t PCHAR;
#endif

//#pragma message("Parameter cannot be reference because it causes undefined behaviour of variadic argument list (according to c++ standard) \nTODO: Wrap output() in namespace")
void output(const tstring format, ...);

#define outputStr(x) output(_T( x ))
#define outputArgs(x, y) output(_T( x ), y)

//#include "GUID.h"
//#include "Status.h"
//#include "EventDelegate.h"

/*#include "Component.h"
#include "Interfaces/IApp.h"*/

#endif // CBA_H
