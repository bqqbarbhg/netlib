#ifndef _NETLIB_SERIALIZATION_BASIC_INL
#define _NETLIB_SERIALIZATION_BASIC_INL

#include "serialization.h"
#include <cstdint>
#include <utility>

// Serialization functions for integral types (with endian conversion)

#define _NETLIB_SERIALIZATION_BASIC_INL_OVERLOAD(M_Type) \
inline char *net_write(char *p, M_Type v) { \
	M_Type m = hton(v); \
	p = net_write(p, (const char*)&m, sizeof(M_Type)); \
	return p; \
} \
inline const char *net_read(const char *p, M_Type& v) { \
	M_Type m; \
	p = net_read(p, (char*)&m, sizeof(M_Type)); \
	v = ntoh(m); \
	return p; \
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

inline char *net_write(char *p, bool b) {
	*p = b ? 1 : 0;
	return p + sizeof(bool);
}
inline const char *net_read(const char *p, bool b) { \
	b = *p != 0;
	return p + sizeof(bool);
}

#endif