#include "address.h"
#include "net.h"

#include "socket.h"

#include <cstring>
#include <utility>
#include <ostream>

Address::Address(struct sockaddr* addr, int len)
	: m_addr((struct sockaddr*)malloc(len))
	, m_addr_len(len)
{
	memcpy(m_addr, addr, m_addr_len);
}

Address::Address()
	: m_addr(nullptr)
{
}
Address::Address(const Address& a)
	: m_addr((struct sockaddr*)malloc(a.m_addr_len))
	, m_addr_len(a.m_addr_len)
{
	memcpy(m_addr, a.m_addr, m_addr_len);
}
Address::Address(Address&& a)
	: m_addr(a.m_addr)
	, m_addr_len(a.m_addr_len)
{
	a.m_addr = nullptr;
}
Address& Address::operator=(Address a)
{
	std::swap(m_addr, a.m_addr);
	std::swap(m_addr_len, a.m_addr_len);
	return *this;
}
Address::~Address()
{
	free(m_addr);
}

bool Address::operator<(const Address& rhs)
{
	if (m_addr_len < rhs.m_addr_len)
		return true;
	if (m_addr_len > rhs.m_addr_len)
		return false;
	if (m_addr == rhs.m_addr)
		return false;
	return memcmp(m_addr, rhs.m_addr, m_addr_len) < 0;
}

static AddressList find_by_name_impl(const char *name, const char *port,
	struct addrinfo* hints)
{
	struct addrinfo *out;

	if (netlib_catch_errno(
		getaddrinfo(name, port, hints, &out)
	)) {
		// If the `getaddrinfo` call fails return an empty list
		return AddressList();
	}

	return AddressList(out);
}

AddressList Address::find_by_name(const char *name, const char *port)
{
	return find_by_name_impl(name, port, nullptr);
}

AddressList Address::find_by_name(const char *name, const char *port,
	const SocketType& type, int af)
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = af;
	hints.ai_socktype = type.type;
	hints.ai_protocol = type.protocol;
	return find_by_name_impl(name, port, &hints);
}

std::ostream& operator<<(std::ostream& lhs, const Address& rhs)
{
	// Try to print the address (fall back to <af> format)
	if (rhs.get_address_family() == AF_INET) {
		char buffer[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, (void*)&((const sockaddr_in*)rhs.get_ptr())->sin_addr, buffer, sizeof(buffer));
		lhs << buffer << ':' << ntohs(((const sockaddr_in*)rhs.get_ptr())->sin_port);
	} else if (rhs.get_address_family() == AF_INET6) {
		char buffer[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, (void*)&((const sockaddr_in6*)rhs.get_ptr())->sin6_addr, buffer, sizeof(buffer));
		lhs << '[' << buffer << "]:" << ntohs(((const sockaddr_in6*)rhs.get_ptr())->sin6_port);
	} else {
		lhs << '<' << rhs.get_address_family() << '>';
	}

	return lhs;
}



AddressList::AddressList()
	: m_ptr(nullptr)
{
}
AddressList::AddressList(struct addrinfo* infos)
	: m_ptr(infos)
{
}
AddressList::AddressList(AddressList&& a)
	: m_ptr(a.m_ptr)
{
	a.m_ptr = nullptr;
}
AddressList& AddressList::operator=(AddressList a)
{
	std::swap(m_ptr, a.m_ptr);
	return *this;
}
AddressList::~AddressList()
{
	if (m_ptr)
		freeaddrinfo(m_ptr);
}