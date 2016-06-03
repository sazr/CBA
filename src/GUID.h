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
