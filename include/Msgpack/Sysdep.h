/*
 * MessagePack system dependencies
 *
 * Copyright (C) 2008-2010 FURUHASHI Sadayuki
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#ifndef LIGHTINK_MSGPACK_SYSDEP_H_
#define LIGHTINK_MSGPACK_SYSDEP_H_

#include <stdlib.h>
#include <stddef.h>


#define _msgpack_head_positive_fixint_from	0x00u
#define _msgpack_head_positive_fixint_to	0x7fu

#define _msgpack_head_fixmap_from			0x80u
#define _msgpack_head_fixmap_to				0x8fu

#define _msgpack_head_fixarray_from			0x90u
#define _msgpack_head_fixarray_to			0x9fu

#define _msgpack_head_fixstr_from			0xa0u
#define _msgpack_head_fixstr_to				0xbfu

#define _msgpack_head_nil					0xc0u

#define _msgpack_head_neverused				0xc1u

#define _msgpack_head_false					0xc2u
#define _msgpack_head_true					0xc3u

#define _msgpack_head_bin8					0xc4u
#define _msgpack_head_bin16					0xc5u
#define _msgpack_head_bin32					0xc6u

#define _msgpack_head_ext8					0xc7u
#define _msgpack_head_ext16					0xc8u
#define _msgpack_head_ext32					0xc9u

#define _msgpack_head_float32				0xcau
#define _msgpack_head_float64				0xcbu

#define _msgpack_head_uint8					0xccu
#define _msgpack_head_uint16				0xcdu
#define _msgpack_head_uint32				0xceu
#define _msgpack_head_uint64				0xcfu

#define _msgpack_head_int8					0xd0u
#define _msgpack_head_int16					0xd1u
#define _msgpack_head_int32					0xd2u
#define _msgpack_head_int64					0xd3u

#define _msgpack_head_fixext1				0xd4u
#define _msgpack_head_fixext2				0xd5u
#define _msgpack_head_fixext4				0xd6u
#define _msgpack_head_fixext8				0xd7u
#define _msgpack_head_fixext16				0xd8u

#define _msgpack_head_str8					0xd9u
#define _msgpack_head_str16					0xdau
#define _msgpack_head_str32					0xdbu

#define _msgpack_head_array16				0xdcu
#define _msgpack_head_array32				0xddu

#define _msgpack_head_map16					0xdeu
#define _msgpack_head_map32					0xdfu

#define _msgpack_head_negative_fixint_from	0xe0u
#define _msgpack_head_negative_fixint_to	0xffu


#ifdef _WIN32

#ifdef __cplusplus
/* numeric_limits<T>::min,max */
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
#endif

#else

#include <arpa/inet.h>  /* __BYTE_ORDER */
#  if !defined(__APPLE__) && !defined(__FreeBSD__) && !defined(__OpenBSD__) && \
       !(defined(__sun) && defined(__SVR4))
#    include <byteswap.h>
#  endif

#endif

#if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define __LITTLE_ENDIAN__
#elif __BYTE_ORDER == __BIG_ENDIAN
#define __BIG_ENDIAN__
#elif _WIN32
#define __LITTLE_ENDIAN__
#endif
#endif


#ifdef __LITTLE_ENDIAN__

#ifdef _WIN32
#  if defined(ntohs)
#    define _msgpack_be16(x) ntohs(x)
#  elif defined(_byteswap_ushort) || (defined(_MSC_VER) && _MSC_VER >= 1400)
#    define _msgpack_be16(x) ((uint16)_byteswap_ushort((unsigned short)x))
#  else
#    define _msgpack_be16(x) ( \
        ((((uint16)x) <<  8) ) | \
        ((((uint16)x) >>  8) ) )
#  endif
#else
#  define _msgpack_be16(x) ntohs(x)
#endif

#ifdef _WIN32
#  if defined(ntohl)
#    define _msgpack_be32(x) ntohl(x)
#  elif defined(_byteswap_ulong) || (defined(_MSC_VER) && _MSC_VER >= 1400)
#    define _msgpack_be32(x) ((uint32)_byteswap_ulong((unsigned long)x))
#  else
#    define _msgpack_be32(x) \
        ( ((((uint32)x) << 24)               ) | \
          ((((uint32)x) <<  8) & 0x00ff0000U ) | \
          ((((uint32)x) >>  8) & 0x0000ff00U ) | \
          ((((uint32)x) >> 24)               ) )
#  endif
#else
#  define _msgpack_be32(x) ntohl(x)
#endif

#if defined(_byteswap_uint64) || (defined(_MSC_VER) && _MSC_VER >= 1400)
#  define _msgpack_be64(x) (_byteswap_uint64(x))
#elif defined(bswap_64)
#  define _msgpack_be64(x) bswap_64(x)
#elif defined(__DARWIN_OSSwapInt64)
#  define _msgpack_be64(x) __DARWIN_OSSwapInt64(x)
#else
#define _msgpack_be64(x) \
    ( ((((uint64)x) << 56)                         ) | \
      ((((uint64)x) << 40) & 0x00ff000000000000ULL ) | \
      ((((uint64)x) << 24) & 0x0000ff0000000000ULL ) | \
      ((((uint64)x) <<  8) & 0x000000ff00000000ULL ) | \
      ((((uint64)x) >>  8) & 0x00000000ff000000ULL ) | \
      ((((uint64)x) >> 24) & 0x0000000000ff0000ULL ) | \
      ((((uint64)x) >> 40) & 0x000000000000ff00ULL ) | \
      ((((uint64)x) >> 56)                         ) )
#endif

#else  /* __LITTLE_ENDIAN__ */

#define _msgpack_be16(x) (x)
#define _msgpack_be32(x) (x)
#define _msgpack_be64(x) (x)

#endif

#define _msgpack_load16(cast, from, to) do {       \
        memcpy((cast*)(to), (from), sizeof(cast)); \
        *(to) = _msgpack_be16(*(to));              \
    } while (0);

#define _msgpack_load32(cast, from, to) do {       \
        memcpy((cast*)(to), (from), sizeof(cast)); \
        *(to) = _msgpack_be32(*(to));              \
    } while (0);
#define _msgpack_load64(cast, from, to) do {       \
        memcpy((cast*)(to), (from), sizeof(cast)); \
        *(to) = _msgpack_be64(*(to));              \
    } while (0);

#define _msgpack_store16(to, num) \
    do { uint16 val = _msgpack_be16(num); memcpy(to, &val, 2); } while(0)
#define _msgpack_store32(to, num) \
    do { uint32 val = _msgpack_be32(num); memcpy(to, &val, 4); } while(0)
#define _msgpack_store64(to, num) \
    do { uint64 val = _msgpack_be64(num); memcpy(to, &val, 8); } while(0)



#if !defined(__cplusplus) && defined(_MSC_VER)
#  if !defined(FALSE)
#    define FALSE (0)
#  endif
#  if !defined(TRUE)
#    define TRUE (!FALSE)
#  endif
#  if _MSC_VER >= 1800
#    include <stdbool.h>
#  else
#    define bool int
#    define true TRUE
#    define false FALSE
#  endif
#  define inline __inline
#endif

#endif /* msgpack/sysdep.h */
