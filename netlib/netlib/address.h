#ifndef _NETLIB_ADDRESS_H
#define _NETLIB_ADDRESS_H

#include "net.h"

#include <iterator>
#include <iosfwd>
#include <utility>

struct SocketType;
class AddressList;

class Address
{
public:
	Address(struct sockaddr* addr, int len);

	// Default constructible, Copyable, movable
	Address();
	Address(const Address& a);
	Address(Address&& a);
	Address& operator=(Address a);
	~Address();

	int get_address_family() const { return m_addr->sa_family; }
	struct sockaddr *get_ptr() const { return m_addr; }
	int get_len() const { return m_addr_len; }

	bool operator<(const Address& rhs);

	static AddressList find_by_name(const char* name, const char *port);
	static AddressList find_by_name(const char* name, const char *port,
		const SocketType& type, int af=AF_UNSPEC);

private:
	struct sockaddr *m_addr;
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