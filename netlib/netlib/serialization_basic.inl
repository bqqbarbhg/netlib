#ifndef _NETLIB_SERIALIZATION_BASIC_INL
#define _NETLIB_SERIALIZATION_BASIC_INL

#include "serialization.h"
#include <cstdint>
#include <utility>
#include <cstring>
#include <string>

// Serialization functions for integral types (with endian conversion)

#define _NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD(M_Type) \
template <> inline bool NetWriter::write<M_Type>(const M_Type& m) { \
	M_Type *v = write_ptr<M_Type>(); \
	if (v == nullptr) return false; \
	*v = hton(m); \
	return true; \
} \
template <> inline bool NetReader::read<M_Type>(M_Type& m) { \
	const M_Type *v = read_ptr<M_Type>(); \
	if (v == nullptr) return false; \
	m = ntoh(*v); \
	return true; \
}

_NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD(unsigned char)
_NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD(unsigned short)
_NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD(unsigned int)
_NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD(unsigned long)
_NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD(unsigned long long)
_NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD(signed char)
_NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD(signed short)
_NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD(signed int)
_NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD(signed long)
_NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD(signed long long)
_NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD(float)
_NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD(double)
_NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD(long double)

#undef _NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD

// Booleans
template <> inline bool NetWriter::write<bool>(const bool& b)
{
	if (!write<uint8_t>(b ? 1 : 0)) return false;
	return true;
}
template <> inline bool NetReader::read<bool>(bool& b)
{
	uint8_t c;
	if (!read<uint8_t>(c)) return false;
	b = c != 0;
	return true;
}

// Strings
template <> inline bool NetWriter::write<std::string>(const std::string& str)
{
	uint32_t len = str.size();
	if (!write(len)) return false;
	if (!write(str.data(), len)) return false;
	return true;
}
template <> inline bool NetReader::read<std::string>(std::string& str)
{
	uint32_t len;
	if (!read(len)) return false;
	const char *ptr = read_ptr<char>(len);
	if (ptr == nullptr) return false;
	str.assign(ptr, len);
	return true;
}

#endif