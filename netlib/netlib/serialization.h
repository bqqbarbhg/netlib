#ifndef _NETLIB_SERIALIZATION_H
#define _NETLIB_SERIALIZATION_H

#include "net.h"
#include <cstring>
#include <utility>

// Copy `size` bytes from `src` to the buffer
inline char *net_write(char *p, const char* src, unsigned int size)
{
	memcpy(p, src, size);
	p += size;
	return p;
}

// Read `size` bytes from the buffer to `dst`
inline const char *net_read(const char *p, char* dst, unsigned int size)
{
	memcpy(dst, p, size);
	p += size;
	return p;
}

// Serialization helpers for endian-dependent types
// hton: Convert a value from host to network byte order
// ntoh: Convert a value from network to host byte order
template <typename T> inline
typename std::enable_if<sizeof T == 1, T>::type hton(T t) { return t; }
template <typename T> inline
typename std::enable_if<sizeof T == 1, T>::type ntoh(T t) { return t; }
template <typename T> inline typename std::enable_if<sizeof T == 2, T>::type
hton(T t) { uint16_t c = htons(*reinterpret_cast<uint16_t*>(&t));
            return *reinterpret_cast<T*>(&c); }
template <typename T> inline typename std::enable_if<sizeof T == 2, T>::type
ntoh(T t) { uint16_t c = ntohs(*reinterpret_cast<uint16_t*>(&t));
            return *reinterpret_cast<T*>(&c); }
template <typename T> inline typename std::enable_if<sizeof T == 4, T>::type
hton(T t) { uint32_t c = htonl(*reinterpret_cast<uint32_t*>(&t));
            return *reinterpret_cast<T*>(&c); }
template <typename T> inline typename std::enable_if<sizeof T == 4, T>::type
ntoh(T t) { uint32_t c = ntohl(*reinterpret_cast<uint32_t*>(&t));
            return *reinterpret_cast<T*>(&c); }
template <typename T> inline typename std::enable_if<sizeof T == 8, T>::type
hton(T t) { uint64_t c = htonll(*reinterpret_cast<uint64_t*>(&t));
            return *reinterpret_cast<T*>(&c); }
template <typename T> inline typename std::enable_if<sizeof T == 8, T>::type
ntoh(T t) { uint64_t c = ntohll(*reinterpret_cast<uint64_t*>(&t));
            return *reinterpret_cast<T*>(&c); }

#include "serialization_basic.inl"

#endif