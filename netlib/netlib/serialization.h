#ifndef _NETLIB_SERIALIZATION_H
#define _NETLIB_SERIALIZATION_H

#include "net.h"
#include <cstring>
#include <utility>

// Read data to network using this class
// Converts endianness and handles struct packing
class NetWriter
{
public:
	NetWriter(char *buffer, unsigned int size)
		: m_buffer(buffer)
		, m_ptr(buffer)
		, m_data_left(size)
	{
	}

	template <typename T>
	bool write(const T& t);

	bool write(const void *source, unsigned int size)
	{
		if (m_data_left < size)
			return false;
		memcpy(m_ptr, source, size);
		m_ptr += size;
		m_data_left -= size;
		return true;
	}

	// Advance the buffer `count` elements of T and return the pointer to
	// the start of that block
	template <typename T>
	inline T* write_ptr(unsigned int count=1) {
		unsigned int size = sizeof(T) * count;
		if (m_data_left < size)
			return nullptr;
		T *ret = reinterpret_cast<T*>(m_ptr);
		m_ptr += size;
		m_data_left -= size;
		return ret;
	}
private:
	char *m_buffer;
	char *m_ptr;
	unsigned int m_data_left;
};

// Read data from network using this class
// Converts endianness and handles struct packing
class NetReader
{
public:
	NetReader(const char *buffer, unsigned int size)
		: m_buffer(buffer)
		, m_ptr(buffer)
		, m_data_left(size)
	{
	}

	template <typename T>
	bool read(T& t);

	bool read(void *target, unsigned int size)
	{
		if (m_data_left < size)
			return false;
		memcpy(target, m_ptr, size);
		m_ptr += size;
		m_data_left -= size;
		return true;
	}

	// Advance the buffer `count` elements of T and return the pointer to
	// the start of that block
	template <typename T>
	inline const T* read_ptr(unsigned int count=1) {
		unsigned int size = sizeof(T) * count;
		if (m_data_left < size)
			return nullptr;
		const T *ret = reinterpret_cast<const T*>(m_ptr);
		m_ptr += size;
		m_data_left -= size;
		return ret;
	}

private:
	const char *m_buffer;
	const char *m_ptr;
	unsigned int m_data_left;
};

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