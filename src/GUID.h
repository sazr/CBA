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

#ifndef CBA_GUID
#define CBA_GUID

#include "CBA.h"

#ifdef _WIN32
#pragma comment(lib, "rpcrt4.lib")
#include <Rpc.h>
#include <windows.h>
#else
#include <uuid/uuid.h>
#endif // _WIN32


#ifdef _WIN32
#define GUID UUID
#define generateGUID(x)		  UuidCreate(&x)					// returns RPC_S_OK, RPC_S_UUID_LOCAL_ONLY, RPC_S_UUID_NO_ADDRESS
#define copyGUID(gFrom, gTo)  gTo = gFrom
#define compareGUID(g1, g2)   IsEqualGUID(g1, g2) == TRUE
#define hashGUID(gUid)		  hash<long>()(gUid.Data1) ^ (hash<short>()(gUid.Data2) << 1) >> 1 ^ hash<short>()(gUid.Data3) << 1 ^ hash<char>()(gUid.Data4[0]) << 1
#define getNullGUID(gUid)	  UuidCreateNil(gUid)
#else
#define GUID uuid_t
#define generateGUID(x)		  uuid_generate_random(x)
#define copyGUID(gFrom, gTo)  uuid_copy(gFrom, gTo)
#define compareGUID(g1, g2)   uuid_compare(g1, g2) == 0
#define hashGUID(gUid)		  gUid[0] ^ gUid[1]
#define getNullGUID(gUid)	  uuid_clear(gUid)
#endif // _WIN32


namespace std
{
	template<>
	struct hash<GUID>
	{
		std::size_t operator()(const GUID& gUid) const
		{
			using std::size_t;
			using std::hash;

			return hashGUID(gUid);
		}
	};
}

#endif // CBA_GUID
