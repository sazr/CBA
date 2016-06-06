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
	delete[] buf;
	#else
	TCHAR buf[10000];
	vsprintf (buf, format.c_str(), args);
	printf(buf);
	#endif // _WIN32
}
