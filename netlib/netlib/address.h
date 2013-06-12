#ifndef _NETLIB_ADDRESS_H
#define _NETLIB_ADDRESS_H

#include "net.h"

#include <iterator>
#include <iosfwd>
#include <utility>
#include <cstdint>

struct SocketType;
class AddressList;

// An address in some domain
// Usually AF_INET or AF_INET6
class Address
{
public:
	// Create an address with no value
	explicit Address(int len);

	// Create an address by copying a value
	Address(const struct sockaddr* addr, int len);

	// Default constructible, Copyable, movable
	Address();
	Address(const Address& a);
	Address(Address&& a);
	Address& operator=(Address a);
	~Address();

	// Accessors
	int get_address_family() const { return m_addr->sa_family; }
	struct sockaddr *get_ptr() const { return m_addr; }
	int get_len() const { return m_addr_len; }
	int *get_len_ptr() { return &m_addr_len; }

	// Sorted by the length first and then bitwise address struct comparison
	bool operator<(const Address& rhs) const;

	// Query addresses by host name (equivalent to `getaddrinfo`)
	// name: host name
	// port: port number or symbolic port value (eg. http)
	// type (hint): Socket type of the searched address
	// af (hint): Address family of the searched address
	static AddressList find_by_name(const char* name, const char *port);
	static AddressList find_by_name(const char* name, const char *port,
		const SocketType& type, int af=AF_UNSPEC);

	// Any ipv4 address (optionally with specified port)
	static Address inet_any(uint16_t port=0);

	// Any ipv6 address (optionally with specified port)
	static Address inet6_any(uint16_t port=0);

private:
	// Pointer to the low-level representation of the address (sized `m_addr_len`)
	struct sockaddr *m_addr;

	// Length of the internal address representation
	int m_addr_len;
};
std::ostream& operator<<(std::ostream& lhs, const Address& rhs);

// List of `Address` classes
// Returned from queries of addresses
// Movable
class AddressList
{
public:
	struct iterator : public std::iterator<std::input_iterator_tag, Address>
	{
	public:
		explicit iterator(struct addrinfo *a)
			: m_ptr(a)
		{
		}
		bool operator==(const iterator& i) const
		{
			return m_ptr == i.m_ptr;
		}
		bool operator!=(const iterator& i) const
		{
			return !(*this == i);
		}
		iterator& operator++()
		{
			m_ptr = m_ptr->ai_next;
			return *this;
		}
		iterator operator++(int)
		{
			iterator old(*this);
			++*this;
			return old;
		}
		Address operator*() const
		{
			return Address(m_ptr->ai_addr, m_ptr->ai_addrlen);
		}
	private:
		struct addrinfo *m_ptr;
	};

	// Creates an empty list
	AddressList();

	// Create the list from addrinfo linked list
	explicit AddressList(struct addrinfo *infos);

	// Movable
	AddressList(AddressList&& a);
	AddressList& operator=(AddressList a);

	// Free the addresses (freeaddrinfo)
	~AddressList();

	// Container
	iterator begin() const { return iterator(m_ptr); }
	iterator end() const { return iterator(nullptr); }

private:
	// Disable copying
	AddressList(const AddressList&);

	// Pointer to C addrinfo structures
	struct addrinfo *m_ptr;

};

#endif