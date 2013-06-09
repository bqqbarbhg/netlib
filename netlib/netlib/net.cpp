#include "net.h"

#if NETLIB_PLATFORM == NETLIB_PLATFORM_WINDOWS

namespace {

// Initialize WSA before the program starts
class WSAInitializer
{
public:
	WSAInitializer();
	~WSAInitializer();
	WSADATA data;
};

WSAInitializer::WSAInitializer()
{
	WSAStartup(MAKEWORD(2, 2), &data);
}
WSAInitializer::~WSAInitializer()
{
	WSACleanup();
}

WSAInitializer g_wsainit;

}

#endif