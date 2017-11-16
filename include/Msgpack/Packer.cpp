


/* Copyright ChenDong(Wilbur), email <baisaichen@live.com>. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef LIGHTINK_MSGPACK_PACKER_CPP_

#define LIGHTINK_MSGPACK_PACKER_CPP_

#include <string.h>
#include "Packer.h"

namespace LightInk
{
	template <typename TBuffer>
	inline Packer<TBuffer>::Packer() : m_buffer(NULL) 
	{
		LogTraceStepCall("Packer<TBuffer>::Packer()");
		LogTraceStepReturnVoid;
	}

	template <typename TBuffer>
	inline Packer<TBuffer>::Packer(TBuffer* b) : m_buffer(b) 
	{
		LogTraceStepCall("Packer<TBuffer>::Packer(TBuffer* b)");
		LogTraceStepReturnVoid;
	}

	template <typename TBuffer>
	inline Packer<TBuffer>::Packer(TBuffer& b) : m_buffer(&b)
	{
		LogTraceStepCall("Packer<TBuffer>::Packer(TBuffer& b)");
		LogTraceStepReturnVoid;
	}

	template <typename TBuffer>
	inline void Packer<TBuffer>::set_buffer(TBuffer * b)
	{
		LogTraceStepCall("void Packer<TBuffer>::set_buffer(TBuffer * b)");
		m_buffer = b;
		LogTraceStepReturnVoid;
	}
	template <typename TBuffer>
	inline void Packer<TBuffer>::set_buffer(TBuffer & b)
	{
		LogTraceStepCall("void Packer<TBuffer>::set_buffer(TBuffer & b)");
		m_buffer = &b;
		LogTraceStepReturnVoid;
	}
	template <typename TBuffer>
	inline TBuffer * Packer<TBuffer>::get_buffer()
	{
		LogTraceStepCall("TBuffer * Packer<TBuffer>::get_buffer()");
		LogTraceStepReturn(m_buffer);
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_uint8(TBuffer & buffer, uint8 d)
	{ 
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_uint8(TBuffer & buffer, uint8 d)");
		LogTraceStepReturn(pack_imp_uint8(buffer, d)); 
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_uint16(TBuffer & buffer, uint16 d)
	{ 
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_uint16(TBuffer & buffer, uint16 d)");
		LogTraceStepReturn(pack_imp_uint16(buffer, d)); 
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_uint32(TBuffer & buffer, uint32 d)
	{ 
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_uint32(TBuffer & buffer, uint32 d)");
		LogTraceStepReturn(pack_imp_uint32(buffer, d)); 
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_uint64(TBuffer & buffer, uint64 d)
	{ 
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_uint64(TBuffer & buffer, uint64 d)");
		LogTraceStepReturn(pack_imp_uint64(buffer, d)); 
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_int8(TBuffer & buffer, int8 d)
	{ 
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_int8(TBuffer & buffer, int8 d)");
		LogTraceStepReturn(pack_imp_int8(buffer, d)); 
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_int16(TBuffer & buffer, int16 d)
	{ 
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_int16(TBuffer & buffer, int16 d)");
		LogTraceStepReturn(pack_imp_int16(buffer, d)); 
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_int32(TBuffer & buffer, int32 d)
	{ 
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_int32(TBuffer & buffer, int32 d)");
		LogTraceStepReturn(pack_imp_int32(buffer, d)); 
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_int64(TBuffer & buffer, int64 d)
	{ 
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_int64(TBuffer & buffer, int64 d)");
		LogTraceStepReturn(pack_imp_int64(buffer, d)); 
	}


	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_fix_uint8(TBuffer & buffer, uint8 d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_fix_uint8(TBuffer & buffer, uint8 d)");
		uint8 buf[2] = {static_cast<uint8>(_msgpack_head_uint8), take8_8(d)};
		LogTraceStepReturn(buffer.write(buf, 2));
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_fix_uint16(TBuffer & buffer, uint16 d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_fix_uint16(TBuffer & buffer, uint16 d)");
		uint8 buf[3];
		buf[0] = static_cast<uint8>(_msgpack_head_uint16); _msgpack_store16(&buf[1], d);
		LogTraceStepReturn(buffer.write(buf, 3));
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_fix_uint32(TBuffer & buffer, uint32 d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_fix_uint32(TBuffer & buffer, uint32 d)");
		uint8 buf[5];
		buf[0] = static_cast<uint8>(_msgpack_head_uint32); _msgpack_store32(&buf[1], d);
		LogTraceStepReturn(buffer.write(buf, 5));
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_fix_uint64(TBuffer & buffer, uint64 d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_fix_uint64(TBuffer & buffer, uint64 d)");
		uint8 buf[9];
		buf[0] = static_cast<uint8>(_msgpack_head_uint64); _msgpack_store64(&buf[1], d);
		LogTraceStepReturn(buffer.write(buf, 9));
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_fix_int8(TBuffer & buffer, int8 d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_fix_int8(TBuffer & buffer, int8 d)");
		uint8 buf[2] = {static_cast<uint8>(_msgpack_head_int8), take8_8(d)};
		LogTraceStepReturn(buffer.write(buf, 2));
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_fix_int16(TBuffer & buffer, int16 d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_fix_int16(TBuffer & buffer, int16 d)");
		uint8 buf[3];
		buf[0] = static_cast<uint8>(_msgpack_head_int16); _msgpack_store16(&buf[1], d);
		LogTraceStepReturn(buffer.write(buf, 3));
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_fix_int32(TBuffer & buffer, int32 d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_fix_int32(TBuffer & buffer, int32 d)");
		uint8 buf[5];
		buf[0] = static_cast<uint8>(_msgpack_head_int32); _msgpack_store32(&buf[1], d);
		LogTraceStepReturn(buffer.write(buf, 5));
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_fix_int64(TBuffer & buffer, int64 d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_fix_int64(TBuffer & buffer, int64 d)");
		uint8 buf[9];
		buf[0] = static_cast<uint8>(_msgpack_head_int64); _msgpack_store64(&buf[1], d);
		LogTraceStepReturn(buffer.write(buf, 9));
	}


	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_char(TBuffer & buffer, char d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_char(TBuffer & buffer, char d)");
#if defined(CHAR_MIN)
#if CHAR_MIN < 0
		LogTraceStepReturn(pack_imp_int8(buffer, d));
#else
		LogTraceStepReturn(pack_imp_uint8(buffer, d));
#endif
#else
#error CHAR_MIN is not defined
#endif
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_signed_char(TBuffer & buffer, signed char d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_signed_char(TBuffer & buffer, signed char d)");
		LogTraceStepReturn(pack_imp_int8(buffer, d));
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_short(TBuffer & buffer, short d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_short(TBuffer & buffer, short d)");
#if defined(SIZEOF_SHORT)
#if SIZEOF_SHORT == 2
		LogTraceStepReturn(pack_imp_int16(buffer, d));
#elif SIZEOF_SHORT == 4
		LogTraceStepReturn(pack_imp_int32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_int64(buffer, d));
#endif

#elif defined(SHRT_MAX)
#if SHRT_MAX == 0x7fff
		LogTraceStepReturn(pack_imp_int16(buffer, d));
#elif SHRT_MAX == 0x7fffffff
		LogTraceStepReturn(pack_imp_int32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_int64(buffer, d));
#endif

#else
		if(sizeof(short) == 2) {
			LogTraceStepReturn(pack_imp_int16(buffer, d));
		} else if(sizeof(short) == 4) {
			LogTraceStepReturn(pack_imp_int32(buffer, d));
		} else {
			LogTraceStepReturn(pack_imp_int64(buffer, d));
		}
#endif
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_int(TBuffer & buffer, int d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_int(TBuffer & buffer, int d)");
#if defined(SIZEOF_INT)
#if SIZEOF_INT == 2
		LogTraceStepReturn(pack_imp_int16(buffer, d));
#elif SIZEOF_INT == 4
		LogTraceStepReturn(pack_imp_int32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_int64(buffer, d));
#endif

#elif defined(INT_MAX)
#if INT_MAX == 0x7fff
		LogTraceStepReturn(pack_imp_int16(buffer, d));
#elif INT_MAX == 0x7fffffff
		LogTraceStepReturn(pack_imp_int32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_int64(buffer, d));
#endif

#else
		if(sizeof(int) == 2) {
			LogTraceStepReturn(pack_imp_int16(buffer, d));
		} else if(sizeof(int) == 4) {
			LogTraceStepReturn(pack_imp_int32(buffer, d));
		} else {
			LogTraceStepReturn(pack_imp_int64(buffer, d));
		}
#endif
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_long(TBuffer & buffer, long d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_long(TBuffer & buffer, long d)");
#if defined(SIZEOF_LONG)
#if SIZEOF_LONG == 2
		LogTraceStepReturn(pack_imp_int16(buffer, d));
#elif SIZEOF_LONG == 4
		LogTraceStepReturn(pack_imp_int32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_int64(buffer, d));
#endif

#elif defined(LONG_MAX)
#if LONG_MAX == 0x7fffL
		LogTraceStepReturn(pack_imp_int16(buffer, d));
#elif LONG_MAX == 0x7fffffffL
		LogTraceStepReturn(pack_imp_int32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_int64(buffer, d));
#endif

#else
		if(sizeof(long) == 2) {
			LogTraceStepReturn(pack_imp_int16(buffer, d));
		} else if(sizeof(long) == 4) {
			LogTraceStepReturn(pack_imp_int32(buffer, d));
		} else {
			LogTraceStepReturn(pack_imp_int64(buffer, d));
		}
#endif
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_long_long(TBuffer & buffer, long long d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_long_long(TBuffer & buffer, long long d)");
#if defined(SIZEOF_LONG_LONG)
#if SIZEOF_LONG_LONG == 2
		LogTraceStepReturn(pack_imp_int16(buffer, d));
#elif SIZEOF_LONG_LONG == 4
		LogTraceStepReturn(pack_imp_int32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_int64(buffer, d));
#endif

#elif defined(LLONG_MAX)
#if LLONG_MAX == 0x7fffL
		LogTraceStepReturn(pack_imp_int16(buffer, d));
#elif LLONG_MAX == 0x7fffffffL
		LogTraceStepReturn(pack_imp_int32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_int64(buffer, d));
#endif

#else
		if(sizeof(long long) == 2) {
			LogTraceStepReturn(pack_imp_int16(buffer, d));
		} else if(sizeof(long long) == 4) {
			LogTraceStepReturn(pack_imp_int32(buffer, d));
		} else {
			LogTraceStepReturn(pack_imp_int64(buffer, d));
		}
#endif
	}


	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_unsigned_char(TBuffer & buffer, unsigned char d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_unsigned_char(TBuffer & buffer, unsigned char d)");
		LogTraceStepReturn(pack_imp_uint8(buffer, d));
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_unsigned_short(TBuffer & buffer, unsigned short d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_unsigned_short(TBuffer & buffer, unsigned short d)");
#if defined(SIZEOF_SHORT)
#if SIZEOF_SHORT == 2
		LogTraceStepReturn(pack_imp_uint16(buffer, d));
#elif SIZEOF_SHORT == 4
		LogTraceStepReturn(pack_imp_uint32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_uint64(buffer, d));
#endif

#elif defined(USHRT_MAX)
#if USHRT_MAX == 0xffffU
		LogTraceStepReturn(pack_imp_uint16(buffer, d));
#elif USHRT_MAX == 0xffffffffU
		LogTraceStepReturn(pack_imp_uint32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_uint64(buffer, d));
#endif

#else
		if(sizeof(unsigned short) == 2) {
			LogTraceStepReturn(pack_imp_uint16(buffer, d));
		} else if(sizeof(unsigned short) == 4) {
			LogTraceStepReturn(pack_imp_uint32(buffer, d));
		} else {
			LogTraceStepReturn(pack_imp_uint64(buffer, d));
		}
#endif
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_unsigned_int(TBuffer & buffer, unsigned int d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_unsigned_int(TBuffer & buffer, unsigned int d)");
#if defined(SIZEOF_INT)
#if SIZEOF_INT == 2
		LogTraceStepReturn(pack_imp_uint16(buffer, d));
#elif SIZEOF_INT == 4
		LogTraceStepReturn(pack_imp_uint32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_uint64(buffer, d));
#endif

#elif defined(UINT_MAX)
#if UINT_MAX == 0xffffU
		LogTraceStepReturn(pack_imp_uint16(buffer, d));
#elif UINT_MAX == 0xffffffffU
		LogTraceStepReturn(pack_imp_uint32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_uint64(buffer, d));
#endif

#else
		if(sizeof(unsigned int) == 2) {
			LogTraceStepReturn(pack_imp_uint16(buffer, d));
		} else if(sizeof(unsigned int) == 4) {
			LogTraceStepReturn(pack_imp_uint32(buffer, d));
		} else {
			LogTraceStepReturn(pack_imp_uint64(buffer, d));
		}
#endif
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_unsigned_long(TBuffer & buffer, unsigned long d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_unsigned_long(TBuffer & buffer, unsigned long d)");
#if defined(SIZEOF_LONG)
#if SIZEOF_LONG == 2
		LogTraceStepReturn(pack_imp_uint16(buffer, d));
#elif SIZEOF_LONG == 4
		LogTraceStepReturn(pack_imp_uint32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_uint64(buffer, d));
#endif

#elif defined(ULONG_MAX)
#if ULONG_MAX == 0xffffUL
		LogTraceStepReturn(pack_imp_uint16(buffer, d));
#elif ULONG_MAX == 0xffffffffUL
		LogTraceStepReturn(pack_imp_uint32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_uint64(buffer, d));
#endif

#else
		if(sizeof(unsigned long) == 2) {
			LogTraceStepReturn(pack_imp_uint16(buffer, d));
		} else if(sizeof(unsigned long) == 4) {
			LogTraceStepReturn(pack_imp_uint32(buffer, d));
		} else {
			LogTraceStepReturn(pack_imp_uint64(buffer, d));
		}
#endif
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_unsigned_long_long(TBuffer & buffer, unsigned long long d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_unsigned_long_long(TBuffer & buffer, unsigned long long d)");
#if defined(SIZEOF_LONG_LONG)
#if SIZEOF_LONG_LONG == 2
		LogTraceStepReturn(pack_imp_uint16(buffer, d));
#elif SIZEOF_LONG_LONG == 4
		LogTraceStepReturn(pack_imp_uint32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_uint64(buffer, d));
#endif

#elif defined(ULLONG_MAX)
#if ULLONG_MAX == 0xffffUL
		LogTraceStepReturn(pack_imp_uint16(buffer, d));
#elif ULLONG_MAX == 0xffffffffUL
		LogTraceStepReturn(pack_imp_uint32(buffer, d));
#else
		LogTraceStepReturn(pack_imp_uint64(buffer, d));
#endif

#else
		if(sizeof(unsigned long long) == 2) {
			LogTraceStepReturn(pack_imp_uint16(buffer, d));
		} else if(sizeof(unsigned long long) == 4) {
			LogTraceStepReturn(pack_imp_uint32(buffer, d));
		} else {
			LogTraceStepReturn(pack_imp_uint64(buffer, d));
		}
#endif
	}


	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_float(TBuffer & buffer, float d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_float(TBuffer & buffer, float d)");
		union { float f; uint32 i; } mem;
		mem.f = d;
		uint8 buf[5];
		buf[0] = static_cast<uint8>(_msgpack_head_float32); _msgpack_store32(&buf[1], mem.i);
		LogTraceStepReturn(buffer.write(buf, 5));
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_double(TBuffer & buffer, double d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_double(TBuffer & buffer, double d)");
		union { double f; uint64 i; } mem;
		mem.f = d;
		uint8 buf[9];
		buf[0] = static_cast<uint8>(_msgpack_head_float64);
#if defined(__arm__) && !(__ARM_EABI__) // arm-oabi
		// https://github.com/msgpack/msgpack-perl/pull/1
		mem.i = (mem.i & 0xFFFFFFFFUL) << 32UL | (mem.i >> 32UL);
#endif
		_msgpack_store64(&buf[1], mem.i);
		LogTraceStepReturn(buffer.write(buf, 9));
	}


	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_nil(TBuffer & buffer)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_nil(TBuffer & buffer)");
		const uint8 d = static_cast<uint8>(_msgpack_head_nil);
		LogTraceStepReturn(buffer.write(&d, 1));
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_bool(TBuffer & buffer, bool d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_bool(TBuffer & buffer, bool d)");
		if (d)
		{
			const uint8 t = static_cast<uint8>(_msgpack_head_true);
			LogTraceStepReturn(buffer.write(&t, 1));
		}
		else
		{
			const uint8 t = static_cast<uint8>(_msgpack_head_false);
			LogTraceStepReturn(buffer.write(&t, 1));
		}
	}


	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_array(TBuffer & buffer, uint32 n)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_array(TBuffer & buffer, uint32 n)");
		if(n < 16) {
			uint8 d = static_cast<uint8>(_msgpack_head_fixarray_from | n);
			LogTraceStepReturn(buffer.write(&d, 1));
		} else if(n < 65536) {
			uint8 buf[3];
			buf[0] = static_cast<uint8>(_msgpack_head_array16); _msgpack_store16(&buf[1], static_cast<uint16>(n));
			LogTraceStepReturn(buffer.write(buf, 3));
		} else {
			uint8 buf[5];
			buf[0] = static_cast<uint8>(_msgpack_head_array32); _msgpack_store32(&buf[1], static_cast<uint32>(n));
			LogTraceStepReturn(buffer.write(buf, 5));
		}
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_map(TBuffer & buffer, uint32 n)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_map(TBuffer & buffer, uint32 n)");
		if(n < 16) {
			uint8 d = static_cast<uint8>(_msgpack_head_fixmap_from | n);
			uint8 buf = take8_8(d);
			LogTraceStepReturn(buffer.write(&buf, 1));
		} else if(n < 65536) {
			uint8 buf[3];
			buf[0] = static_cast<uint8>(_msgpack_head_map16); _msgpack_store16(&buf[1], static_cast<uint16>(n));
			LogTraceStepReturn(buffer.write(buf, 3));
		} else {
			uint8 buf[5];
			buf[0] = static_cast<uint8>(_msgpack_head_map32); _msgpack_store32(&buf[1], static_cast<uint32>(n));
			LogTraceStepReturn(buffer.write(buf, 5));
		}
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_str(TBuffer & buffer, uint32 l)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_str(TBuffer & buffer, uint32 l)");
		if(l < 32) {
			uint8 d = _msgpack_head_fixstr_from | static_cast<uint8>(l);
			uint8 buf = take8_8(d);
			LogTraceStepReturn(buffer.write(&buf, 1));
		} else if(l < 256) {
			uint8 buf[2];
			buf[0] = static_cast<uint8>(_msgpack_head_str8); buf[1] = static_cast<uint8>(l);
			LogTraceStepReturn(buffer.write(buf, 2));
		} else if(l < 65536) {
			uint8 buf[3];
			buf[0] = static_cast<uint8>(_msgpack_head_str16); _msgpack_store16(&buf[1], static_cast<uint16>(l));
			LogTraceStepReturn(buffer.write(buf, 3));
		} else {
			uint8 buf[5];
			buf[0] = static_cast<uint8>(_msgpack_head_str32); _msgpack_store32(&buf[1], static_cast<uint32>(l));
			LogTraceStepReturn(buffer.write(buf, 5));
		}
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_str_body(TBuffer & buffer, const char* b, uint32 l)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_str_body(TBuffer & buffer, const char* b, uint32 l)");
		LogTraceStepReturn(buffer.write(b, l));
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_str_string(TBuffer & buffer, const string & s)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_str_string(TBuffer & buffer, const string & s)");
		RuntimeError err = pack_str(buffer, s.size());
		if (err != RE_Success)
		{
			LogTraceStepReturn(err);
		}
		LogTraceStepReturn(buffer.write(s));
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_bin(TBuffer & buffer, uint32 l)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_bin(TBuffer & buffer, uint32 l)");
		if(l < 256) {
			uint8 buf[2];
			buf[0] = static_cast<uint8>(_msgpack_head_bin8); buf[1] = static_cast<uint8>(l);
			LogTraceStepReturn(buffer.write(buf, 2));
		} else if(l < 65536) {
			uint8 buf[3];
			buf[0] = static_cast<uint8>(_msgpack_head_bin16); _msgpack_store16(&buf[1], static_cast<uint16>(l));
			LogTraceStepReturn(buffer.write(buf, 3));
		} else {
			uint8 buf[5];
			buf[0] = static_cast<uint8>(_msgpack_head_bin32); _msgpack_store32(&buf[1], static_cast<uint32>(l));
			LogTraceStepReturn(buffer.write(buf, 5));
		}
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_bin_body(TBuffer & buffer, const char* b, uint32 l)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_bin_body(TBuffer & buffer, const char* b, uint32 l)");
		LogTraceStepReturn(buffer.write(b, l));
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_ext(TBuffer & buffer, uint32 l, int8 type)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_ext(TBuffer & buffer, uint32 l, int8 type)");
		switch(l) {
		case 1: {
			uint8 buf[2];
			buf[0] = static_cast<uint8>(_msgpack_head_fixext1);
			buf[1] = static_cast<uint8>(type);
			LogTraceStepReturn(buffer.write(buf, 2));
				} break;
		case 2: {
			uint8 buf[2];
			buf[0] = static_cast<uint8>(_msgpack_head_fixext2);
			buf[1] = static_cast<uint8>(type);
			LogTraceStepReturn(buffer.write(buf, 2));
				} break;
		case 4: {
			uint8 buf[2];
			buf[0] = static_cast<uint8>(_msgpack_head_fixext4);
			buf[1] = static_cast<uint8>(type);
			LogTraceStepReturn(buffer.write(buf, 2));
				} break;
		case 8: {
			uint8 buf[2];
			buf[0] = static_cast<uint8>(_msgpack_head_fixext8);
			buf[1] = static_cast<uint8>(type);
			LogTraceStepReturn(buffer.write(buf, 2));
				} break;
		case 16: {
			uint8 buf[2];
			buf[0] = static_cast<uint8>(_msgpack_head_fixext16);
			buf[1] = static_cast<uint8>(type);
			LogTraceStepReturn(buffer.write(buf, 2));
				 } break;
		default:
			if(l < 256) {
				uint8 buf[3];
				buf[0] = static_cast<uint8>(_msgpack_head_ext8);
				buf[1] = static_cast<uint8>(l);
				buf[2] = static_cast<uint8>(type);
				LogTraceStepReturn(buffer.write(buf, 3));
			} else if(l < 65536) {
				uint8 buf[4];
				buf[0] = static_cast<uint8>(_msgpack_head_ext16);
				_msgpack_store16(&buf[1], static_cast<uint16>(l));
				buf[3] = static_cast<uint8>(type);
				LogTraceStepReturn(buffer.write(buf, 4));
			} else {
				uint8 buf[6];
				buf[0] = static_cast<uint8>(_msgpack_head_ext32);
				_msgpack_store32(&buf[1], static_cast<uint32>(l));
				buf[5] = static_cast<uint8>(type);
				LogTraceStepReturn(buffer.write(buf, 6));
			}
			break;
		}
		LogTraceStepReturn(RE_Msgpack_TypeError);
	}

	template <typename TBuffer>
	inline RuntimeError Packer<TBuffer>::pack_ext_body(TBuffer & buffer, const char* b, uint32 l)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_ext_body(TBuffer & buffer, const char* b, uint32 l)");
		LogTraceStepReturn(buffer.write(b, l));
	}

	template <typename TBuffer>
	template <typename T>
	inline RuntimeError Packer<TBuffer>::pack_imp_uint8(TBuffer & buffer, T d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_imp_uint8(TBuffer & buffer, T d)");
		if(d < (1<<7)) {
			/* fixnum */
			uint8 buf = take8_8(d);
			LogTraceStepReturn(buffer.write(&buf, 1));
		} else {
			/* unsigned 8 */
			uint8 buf[2] = {static_cast<uint8>(_msgpack_head_uint8), take8_8(d)};
			LogTraceStepReturn(buffer.write(buf, 2));
		}
	}

	template <typename TBuffer>
	template <typename T>
	inline RuntimeError Packer<TBuffer>::pack_imp_uint16(TBuffer & buffer, T d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_imp_uint16(TBuffer & buffer, T d)");
		if(d < (1<<7)) {
			/* fixnum */
			uint8 buf = take8_16(d);
			LogTraceStepReturn(buffer.write(&buf, 1));
		} else if(d < (1<<8)) {
			/* unsigned 8 */
			uint8 buf[2] = {static_cast<uint8>(_msgpack_head_uint8), take8_16(d)};
			LogTraceStepReturn(buffer.write(buf, 2));
		} else {
			/* unsigned 16 */
			uint8 buf[3];
			buf[0] = static_cast<uint8>(_msgpack_head_uint16); _msgpack_store16(&buf[1], static_cast<uint16>(d));
			LogTraceStepReturn(buffer.write(buf, 3));
		}
	}

	template <typename TBuffer>
	template <typename T>
	inline RuntimeError Packer<TBuffer>::pack_imp_uint32(TBuffer & buffer, T d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_imp_uint32(TBuffer & buffer, T d)");
		if(d < (1<<8)) {
			if(d < (1<<7)) {
				/* fixnum */
				uint8 buf = take8_32(d);
				LogTraceStepReturn(buffer.write(&buf, 1));
			} else {
				/* unsigned 8 */
				uint8 buf[2] = {static_cast<uint8>(_msgpack_head_uint8), take8_32(d)};
				LogTraceStepReturn(buffer.write(buf, 2));
			}
		} else {
			if(d < (1<<16)) {
				/* unsigned 16 */
				uint8 buf[3];
				buf[0] = static_cast<uint8>(_msgpack_head_uint16); _msgpack_store16(&buf[1], static_cast<uint16>(d));
				LogTraceStepReturn(buffer.write(buf, 3));
			} else {
				/* unsigned 32 */
				uint8 buf[5];
				buf[0] = static_cast<uint8>(_msgpack_head_uint32); _msgpack_store32(&buf[1], static_cast<uint32>(d));
				LogTraceStepReturn(buffer.write(buf, 5));
			}
		}
	}

	template <typename TBuffer>
	template <typename T>
	inline RuntimeError Packer<TBuffer>::pack_imp_uint64(TBuffer & buffer, T d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_imp_uint64(TBuffer & buffer, T d)");
		if(d < (1ULL<<8)) {
			if(d < (1ULL<<7)) {
				/* fixnum */
				uint8 buf = take8_64(d);
				LogTraceStepReturn(buffer.write(&buf, 1));
			} else {
				/* unsigned 8 */
				uint8 buf[2] = {static_cast<uint8>(_msgpack_head_uint8), take8_64(d)};
				LogTraceStepReturn(buffer.write(buf, 2));
			}
		} else {
			if(d < (1ULL<<16)) {
				/* unsigned 16 */
				uint8 buf[3];
				buf[0] = static_cast<uint8>(_msgpack_head_uint16); _msgpack_store16(&buf[1], static_cast<uint16>(d));
				LogTraceStepReturn(buffer.write(buf, 3));
			} else if(d < (1ULL<<32)) {
				/* unsigned 32 */
				uint8 buf[5];
				buf[0] = static_cast<uint8>(_msgpack_head_uint32); _msgpack_store32(&buf[1], static_cast<uint32>(d));
				LogTraceStepReturn(buffer.write(buf, 5));
			} else {
				/* unsigned 64 */
				uint8 buf[9];
				buf[0] = static_cast<uint8>(_msgpack_head_uint64); _msgpack_store64(&buf[1], d);
				LogTraceStepReturn(buffer.write(buf, 9));
			}
		}
	}

	template <typename TBuffer>
	template <typename T>
	inline RuntimeError Packer<TBuffer>::pack_imp_int8(TBuffer & buffer, T d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_imp_int8(TBuffer & buffer, T d)");
		if(d < -(1<<5)) {
			/* signed 8 */
			uint8 buf[2] = {static_cast<uint8>(_msgpack_head_int8), take8_8(d)};
			LogTraceStepReturn(buffer.write(buf, 2));
		} else {
			/* fixnum */
			uint8 buf = take8_8(d);
			LogTraceStepReturn(buffer.write(&buf, 1));
		}
	}

	template <typename TBuffer>
	template <typename T>
	inline RuntimeError Packer<TBuffer>::pack_imp_int16(TBuffer & buffer, T d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_imp_int16(TBuffer & buffer, T d)");
		if(d < -(1<<5)) {
			if(d < -(1<<7)) {
				/* signed 16 */
				uint8 buf[3];
				buf[0] = static_cast<uint8>(_msgpack_head_int16); _msgpack_store16(&buf[1], static_cast<int16>(d));
				LogTraceStepReturn(buffer.write(buf, 3));
			} else {
				/* signed 8 */
				uint8 buf[2] = {static_cast<uint8>(_msgpack_head_int8), take8_16(d)};
				LogTraceStepReturn(buffer.write(buf, 2));
			}
		} else if(d < (1<<7)) {
			/* fixnum */
			uint8 buf = take8_16(d);
			LogTraceStepReturn(buffer.write(&buf, 1));
		} else {
			if(d < (1<<8)) {
				/* unsigned 8 */
				uint8 buf[2] = {static_cast<uint8>(_msgpack_head_uint8), take8_16(d)};
				LogTraceStepReturn(buffer.write(buf, 2));
			} else {
				/* unsigned 16 */
				uint8 buf[3];
				buf[0] = static_cast<uint8>(_msgpack_head_uint16); _msgpack_store16(&buf[1], static_cast<uint16>(d));
				LogTraceStepReturn(buffer.write(buf, 3));
			}
		}
	}

	template <typename TBuffer>
	template <typename T>
	inline RuntimeError Packer<TBuffer>::pack_imp_int32(TBuffer & buffer, T d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_imp_int32(TBuffer & buffer, T d)");
		if(d < -(1<<5)) {
			if(d < -(1<<15)) {
				/* signed 32 */
				uint8 buf[5];
				buf[0] = static_cast<uint8>(_msgpack_head_int32); _msgpack_store32(&buf[1], static_cast<int32>(d));
				LogTraceStepReturn(buffer.write(buf, 5));
			} else if(d < -(1<<7)) {
				/* signed 16 */
				uint8 buf[3];
				buf[0] = static_cast<uint8>(_msgpack_head_int16); _msgpack_store16(&buf[1], static_cast<int16>(d));
				LogTraceStepReturn(buffer.write(buf, 3));
			} else {
				/* signed 8 */
				uint8 buf[2] = { static_cast<uint8>(_msgpack_head_int8), take8_32(d)};
				LogTraceStepReturn(buffer.write(buf, 2));
			}
		} else if(d < (1<<7)) {
			/* fixnum */
			uint8 buf = take8_32(d);
			LogTraceStepReturn(buffer.write(&buf, 1));
		} else {
			if(d < (1<<8)) {
				/* unsigned 8 */
				uint8 buf[2] = { static_cast<uint8>(_msgpack_head_uint8), take8_32(d)};
				LogTraceStepReturn(buffer.write(buf, 2));
			} else if(d < (1<<16)) {
				/* unsigned 16 */
				uint8 buf[3];
				buf[0] = static_cast<uint8>(_msgpack_head_uint16); _msgpack_store16(&buf[1], static_cast<uint16>(d));
				LogTraceStepReturn(buffer.write(buf, 3));
			} else {
				/* unsigned 32 */
				uint8 buf[5];
				buf[0] = static_cast<uint8>(_msgpack_head_uint32); _msgpack_store32(&buf[1], static_cast<uint32>(d));
				LogTraceStepReturn(buffer.write(buf, 5));
			}
		}
	}

	template <typename TBuffer>
	template <typename T>
	inline RuntimeError Packer<TBuffer>::pack_imp_int64(TBuffer & buffer, T d)
	{
		LogTraceStepCall("RuntimeError Packer<TBuffer>::pack_imp_int64(TBuffer & buffer, T d)");
		if(d < -(1LL<<5)) {
			if(d < -(1LL<<15)) {
				if(d < -(1LL<<31)) {
					/* signed 64 */
					uint8 buf[9];
					buf[0] = static_cast<uint8>(_msgpack_head_int64); _msgpack_store64(&buf[1], d);
					LogTraceStepReturn(buffer.write(buf, 9));
				} else {
					/* signed 32 */
					uint8 buf[5];
					buf[0] = static_cast<uint8>(_msgpack_head_int32); _msgpack_store32(&buf[1], static_cast<int32>(d));
					LogTraceStepReturn(buffer.write(buf, 5));
				}
			} else {
				if(d < -(1<<7)) {
					/* signed 16 */
					uint8 buf[3];
					buf[0] = static_cast<uint8>(_msgpack_head_int16); _msgpack_store16(&buf[1], static_cast<int16>(d));
					LogTraceStepReturn(buffer.write(buf, 3));
				} else {
					/* signed 8 */
					uint8 buf[2] = {static_cast<uint8>(_msgpack_head_int8), take8_64(d)};
					LogTraceStepReturn(buffer.write(buf, 2));
				}
			}
		} else if(d < (1<<7)) {
			/* fixnum */
			uint8 buf = take8_64(d);
			LogTraceStepReturn(buffer.write(&buf, 1));
		} else {
			if(d < (1LL<<16)) {
				if(d < (1<<8)) {
					/* unsigned 8 */
					uint8 buf[2] = {static_cast<uint8>(_msgpack_head_uint8), take8_64(d)};
					LogTraceStepReturn(buffer.write(buf, 2));
				} else {
					/* unsigned 16 */
					uint8 buf[3];
					buf[0] = static_cast<uint8>(_msgpack_head_uint16); _msgpack_store16(&buf[1], static_cast<uint16>(d));
					LogTraceStepReturn(buffer.write(buf, 3));
				}
			} else {
				if(d < (1LL<<32)) {
					/* unsigned 32 */
					uint8 buf[5];
					buf[0] = static_cast<uint8>(_msgpack_head_uint32); _msgpack_store32(&buf[1], static_cast<uint32>(d));
					LogTraceStepReturn(buffer.write(buf, 5));
				} else {
					/* unsigned 64 */
					uint8 buf[9];
					buf[0] = static_cast<uint8>(_msgpack_head_uint64); _msgpack_store64(&buf[1], d);
					LogTraceStepReturn(buffer.write(buf, 9));
				}
			}
		}
	}
}


#endif