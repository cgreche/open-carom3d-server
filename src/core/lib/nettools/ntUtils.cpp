
#include "ntUtils.h"

#include <WS2TCPIP.h>
#include <process.h>

#include <wininet.h>
#pragma comment(lib,"wininet.lib")

namespace nettools {

	//get host by name is blocking only
	unsigned int __stdcall thread_get_host_info(void* param)
	{
		
		/*
		ntHostInfo *p = (ntHostInfo*)param;

		p->hEvent = CreateEvent(NULL, false, false, NULL);
		if (!p->hEvent) return 1;

		SetEvent(p->hEvent);
		*/
		return 0;
	}

	bool ntUtils::GetHostInfo(const char *hostname, ntHostInfo *pHostInfo)
	{
		return false;
		/*
		memset(pHostInfo, 0, sizeof(ntHostInfo));

		pHostInfo->hostname = (char *)hostname;
		if ((pHostInfo->host_addr = inet_addr(hostname)) == INADDR_NONE)
			pHostInfo->hostname = (char *)hostname;

		hostent	*host;
		if (hostname && *hostname)
			host = gethostbyname(hostname);
		else
			host = gethostbyaddr((char *)&host_addr, 4, AF_INET);

		if (!host)
		{
			p->result = WSAGetLastError();
			p->host_addr = INADDR_NONE;
		}
		else
		{
			p->host_addr = *(DWORD*)host->h_addr;
		}
		/*
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, thread_get_host_info,
			(void *)pHostInfo, 0, NULL);
		if (hThread == INVALID_HANDLE_VALUE)
			return false;

		SetThreadPriority(hThread, GetThreadPriority(GetCurrentThread()));
		*/
		return true;
	}

	bool ntUtils::IsConnectedToInternet()
	{
		DWORD flags;
		bool res;
		res = ::InternetGetConnectedState(&flags, NULL) == TRUE;
		return res;
	}

	const char *ntUtils::LongToIPv4(unsigned long rawip)
	{
		struct in_addr addr;
		addr.S_un.S_addr = rawip;
		static char ret[16] = "";
		::inet_ntop(AF_INET, &addr, ret, sizeof(ret));
		return (const char*)::inet_ntop(AF_INET, &addr, ret, sizeof(ret));
	}

}