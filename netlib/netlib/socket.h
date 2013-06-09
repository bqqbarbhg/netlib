#ifndef _NETLIB_SOCKET_H
#define _NETLIB_SOCKET_H

#include "net.h"

// Specifies a way to create a `Socket`
struct SocketType
{
public:
	// Creates a new socket type from two enums corresponding to the two last
	// arguments of the `socket` function
	SocketType(int type, int protocol);

	// User datagram protocol
	// Usually `SOCK_DGRAM`, `IPPROTO_UDP`
	static SocketType UDP;

	// Transmission control protocol
	// Usually `SOCK_STREAM`, `IPPROTO_TCP`
	static SocketType TCP;

	int type;
	int protocol;
};

// An internet socket
// Default constructible, Movable
class Socket
{
public:
	// Creates an unitialized (closed) socket
	Socket();

	// Movable
	Socket(Socket&& s);
	Socket& operator=(Socket s);

	// Closes the socket if it's open
	~Socket();

private:
	// Disable copying
	Socket(const Socket&);

	// Internal socket handle passed to OS functions
	socket_handle_t m_socket;
};

#endif