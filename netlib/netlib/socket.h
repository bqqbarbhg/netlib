#ifndef _NETLIB_SOCKET_H
#define _NETLIB_SOCKET_H

#include "net.h"

class Address;

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

	// Creates a socket in the address family `af` with the type `type`
	Socket(int af, const SocketType& type);

	// Create and bind a socket to `addr` with the type `type`
	// The socket is closed if the binding fails
	Socket(const Address& addr, const SocketType& type);

	// Movable
	Socket(Socket&& s);
	Socket& operator=(Socket s);

	// Closes the socket if it's open
	~Socket();

	// Closes the socket
	void close();

	// Returns whether the socket is open or not
	bool is_open() const;

	// Bind the socket to address `addr`
	int bind(const Address& addr);

	// Connects the socket to the address `addr`
	int connect(const Address& addr);

	// Starts listening for connections to the socket
	int listen(int backlog=SOMAXCONN);

	// Set the socket as blocking (default true)
	int set_blocking(bool blocking);

	// Accepts a client (should be called after `listen`)
	Socket accept();

	// Accepts a client (should be called after `listen`)
	// Stores the address of the sender to out_addr (must be large enough to
	// hold the address)
	Socket accept(Address& out_addr);

	// Receive data from the socket to `dest`
	// `dest` should be at least `length` bytes long
	// returns the number of bytes received
	int receive(char *dest, int length, int flags=0);
	// Store the sender to `from` (`from` must be large enough to hold the
	// address)
	int receive_from(Address& from, char *dest, int length, int flags=0);

	// Send data from the socket
	// `src` should contain `length` bytes to send
	// returns the number of bytes sent
	int send(const char *src, int length, int flags=0);
	// Send the packet to the address `to`
	int send_to(const Address& to, const char *src, int length, int flags=0);
	
private:
	// Disable copying
	Socket(const Socket&);

	// Create a socket from a internal socket
	explicit Socket(socket_handle_t fd);

	// Internal socket handle passed to OS functions
	socket_handle_t m_socket;
};

#endif