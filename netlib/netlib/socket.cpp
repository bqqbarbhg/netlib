#include "socket.h"
#include <utility>

SocketType::SocketType(int type, int protocol)
	: type(type)
	, protocol(protocol)
{
}

// Initialize the default socket types
SocketType SocketType::UDP(SOCK_DGRAM, IPPROTO_UDP);
SocketType SocketType::TCP(SOCK_STREAM, IPPROTO_TCP);

Socket::Socket()
	: m_socket(NETLIB_INVALID_SOCKET)
{
}

Socket::Socket(Socket&& s)
	: m_socket(s.m_socket)
{
	s.m_socket = NETLIB_INVALID_SOCKET;
}

Socket& Socket::operator=(Socket s)
{
	std::swap(m_socket, s.m_socket);
	return *this;
}

Socket::~Socket()
{
#if NETLIB_PLATFORM == NETLIB_PLATFORM_WINDOWS
	if (m_socket != INVALID_SOCKET)
		closesocket(m_socket);
#else
	if (m_socket < 0)
		close(m_socket);
#endif
}