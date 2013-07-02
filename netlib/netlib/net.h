#ifndef _NETLIB_NET_H
#define _NETLIB_NET_H

#define NETLIB_PLATFORM_WINDOWS 1

#ifdef _WIN32

#include <WinSock2.h>
#include <WS2tcpip.h>

typedef SOCKET socket_handle_t;
#define NETLIB_INVALID_SOCKET INVALID_SOCKET

#define NETLIB_PLATFORM NETLIB_PLATFORM_WINDOWS

#else

// TODO

#endif

class NetServiceHandle
{
public:
	NetServiceHandle();
	~NetServiceHandle();

private:
#if NETLIB_PLATFORM == NETLIB_PLATFORM_WINDOWS
	WSADATA m_wsadata;
#endif
};

inline bool netlib_catch_errno(int err)
{
	// TEMPTEMP
	if (err != 0)
		__asm int 3;

	return err != 0;
}

#endif