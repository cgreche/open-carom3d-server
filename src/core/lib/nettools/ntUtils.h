
#include <WinSock2.h>

namespace nettools {

	typedef struct _ntHostInfo
	{
		char *hostname; //in
		unsigned long host_addr; //in

								 //	hostent hostinfo; //out

		unsigned long result;

	} ntHostInfo;

	class ntUtils
	{
	public:
		static bool GetHostInfo(const char *hostname, ntHostInfo *pHostInfo);
		static bool IsConnectedToInternet();
		static const char *LongToIPv4(unsigned long rawip);
	};

}

