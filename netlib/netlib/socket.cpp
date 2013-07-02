#include "socket.h"
#include "address.h"
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

Socket::Socket(socket_handle_t fd)
	: m_socket(fd)
{
}

Socket::Socket(int af, const SocketType& type)
	: m_socket(::socket(af, type.type, type.protocol))
{
}

Socket::Socket(const Address& addr, const SocketType& type)
	: m_socket(::socket(addr.get_address_family(), type.type, type.protocol))
{
	if (bind(addr) < 0)
		close();
}

Socket::Socket(Socket&& s)
	: m_socket(s.m_socket)
{
	// Invalidate the moved resource
	s.m_socket = NETLIB_INVALID_SOCKET;
}

Socket& Socket::operator=(Socket s)
{
	std::swap(m_socket, s.m_socket);
	return *this;
}

Socket::~Socket()
{
	close();
}

void Socket::close()
{
	if (is_open()) {
#if NETLIB_PLATFORM == NETLIB_PLATFORM_WINDOWS
		closesocket(m_socket);
#else
		close(m_socket);
#endif
	}
}

bool Socket::is_open() const
{
#if NETLIB_PLATFORM == NETLIB_PLATFORM_WINDOWS
	return m_socket != INVALID_SOCKET;
#else
	return m_socket >= 0;
#endif
}

int Socket::bind(const Address& addr)
{
	return ::bind(m_socket, addr.get_ptr(), addr.get_len());
}

int Socket::connect(const Address& addr)
{
	return ::connect(m_socket, addr.get_ptr(), addr.get_len());
}

int Socket::listen(int backlog)
{
	return ::listen(m_socket, backlog);
}

int Socket::set_blocking(bool blocking)
{
#if NETLIB_PLATFORM == NETLIB_PLATFORM_WINDOWS
	DWORD dw = blocking ? 0 : 1;
	return ioctlsocket(m_socket, FIONBIO, &dw);
#else
	return fcntl(handle, F_SETFL, O_NONBLOCK, blocking ? 0 : 1);
#endif
}

Socket Socket::accept()
{
	return Socket(::accept(m_socket, nullptr, nullptr));
}

Socket Socket::accept(Address& addr)
{
	return Socket(::accept(m_socket, addr.get_ptr(), addr.get_len_ptr()));
}

int Socket::receive(char *dest, int length, int flags)
{
	return ::recv(m_socket, dest, length, flags);
}

int Socket::receive_from(Address& from, char *dest, int length, int flags)
{
	return ::recvfrom(m_socket, dest, length, flags, from.get_ptr(), from.get_len_ptr());
}

int Socket::send(const char *src, int length, int flags)
{
	return ::send(m_socket, src, length, flags);
}

int Socket::send_to(const Address& to, const char *src, int length, int flags)
{
	return ::sendto(m_socket, src, length, flags, to.get_ptr(), to.get_len());
}