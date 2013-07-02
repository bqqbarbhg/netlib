#include "net.h"

#if NETLIB_PLATFORM == NETLIB_PLATFORM_WINDOWS

NetServiceHandle::NetServiceHandle()
{
	WSAStartup(MAKEWORD(2, 2), &m_wsadata);
}
NetServiceHandle::~NetServiceHandle()
{
	WSACleanup();
}

#else

NetServiceHandle::NetServiceHandle()
{
}
NetServiceHandle::~NetServiceHandle()
{
}

#endif