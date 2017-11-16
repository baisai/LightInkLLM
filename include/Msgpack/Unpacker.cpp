


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
#ifndef LIGHTINK_MSGPACK_UNPACKER_CPP_
#define LIGHTINK_MSGPACK_UNPACKER_CPP_

#include "Unpacker.h"

namespace LightInk
{
	template <typename TBuffer>
	inline Unpacker<TBuffer>::Unpacker() : m_buffer(NULL) 
	{
		LogTraceStepCall("Unpacker<TBuffer>::Unpacker()");
		LogTraceStepReturnVoid;
	}

	template <typename TBuffer>
	inline Unpacker<TBuffer>::Unpacker(TBuffer* b) : m_buffer(b) 
	{
		LogTraceStepCall("Unpacker<TBuffer>::Unpacker(TBuffer* b)");
		LogTraceStepReturnVoid;
	}

	template <typename TBuffer>
	inline Unpacker<TBuffer>::Unpacker(TBuffer& b) : m_buffer(&b) 
	{
		LogTraceStepCall("Unpacker<TBuffer>::Unpacker(TBuffer& b)");
		LogTraceStepReturnVoid;
	}

	template <typename TBuffer>
	inline void Unpacker<TBuffer>::set_buffer(TBuffer * b)
	{
		LogTraceStepCall("void Unpacker<TBuffer>::set_buffer(TBuffer * b)");
		m_buffer = b;
		LogTraceStepReturnVoid;
	}
	template <typename TBuffer>
	inline void Unpacker<TBuffer>::set_buffer(TBuffer & b)
	{
		LogTraceStepCall("void Unpacker<TBuffer>::set_buffer(TBuffer & b)");
		m_buffer = &b;
		LogTraceStepReturnVoid;
	}
	template <typename TBuffer>
	inline TBuffer * Unpacker<TBuffer>::get_buffer()
	{
		LogTraceStepCall("TBuffer * Unpacker<TBuffer>::get_buffer()");
		LogTraceStepReturn(m_buffer);
	}

	template <typename TBuffer>
	inline uint8 Unpacker<TBuffer>::get_pack_type(TBuffer & buffer)
	{
		LogTraceStepCall("uint8 Unpacker<TBuffer>::get_pack_type(TBuffer & buffer)");
		LogTraceStepReturn((*reinterpret_cast<const unsigned char*>(buffer.data())));
	}

	template <typename TBuffer>
	inline uint32 Unpacker<TBuffer>::clear_pack_type(TBuffer & buffer, uint8 packType)
	{
		LogTraceStepCall("uint32 Unpacker<TBuffer>::clear_pack_type(TBuffer & buffer, uint8 packType)");
		LogTraceStepReturn(buffer.size());
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_uint8(TBuffer & buffer, uint8 & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_uint8(TBuffer & buffer, uint8 & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		e = unpack_imp_uint8<uint8>(buffer, d, t);
		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_uint16(TBuffer & buffer, uint16 & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_uint16(TBuffer & buffer, uint16 & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		e = unpack_imp_uint16<uint16>(buffer, d, t);
		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_uint32(TBuffer & buffer, uint32 & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_uint32(TBuffer & buffer, uint32 & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		e = unpack_imp_uint32<uint32>(buffer, d, t);
		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_uint64(TBuffer & buffer, uint64 & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_uint64(TBuffer & buffer, uint64 & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		e = unpack_imp_uint64<uint64>(buffer, d, t);
		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_int8(TBuffer & buffer, int8 & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_int8(TBuffer & buffer, int8 & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		e = unpack_imp_int8<int8>(buffer, d, t);
		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_int16(TBuffer & buffer, int16 & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_int16(TBuffer & buffer, int16 & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		e = unpack_imp_int16<int16>(buffer, d, t);
		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_int32(TBuffer & buffer, int32 & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_int32(TBuffer & buffer, int32 & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		e = unpack_imp_int32<int32>(buffer, d, t);
		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_int64(TBuffer & buffer, int64 & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_int64(TBuffer & buffer, int64 & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		e = unpack_imp_int64<int64>(buffer, d, t);
		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_char(TBuffer & buffer, char  & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_char(TBuffer & buffer, char  & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
#if defined(CHAR_MIN)
#if CHAR_MIN < 0
		e = unpack_imp_int8<char>(buffer, d, t);
#else
		e = unpack_imp_uint8<char>(buffer, d, t);
#endif
#else
#error CHAR_MIN is not defined
#endif
		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_signed_char(TBuffer & buffer, signed char & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_signed_char(TBuffer & buffer, signed char & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		e = unpack_imp_int8<signed char>(buffer, d, t);
		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_short(TBuffer & buffer, short & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_short(TBuffer & buffer, short & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}

#if defined(SIZEOF_SHORT)
#if SIZEOF_SHORT == 2
		e = unpack_imp_int16<short>(buffer, d, t);
#elif SIZEOF_SHORT == 4
		e = unpack_imp_int32<short>(buffer, d, t);
#else
		e = unpack_imp_int64<short>(buffer, d, t);
#endif

#elif defined(SHRT_MAX)
#if SHRT_MAX == 0x7fff
		e = unpack_imp_int16<short>(buffer, d, t);
#elif SHRT_MAX == 0x7fffffff
		e = unpack_imp_int32<short>(buffer, d, t);
#else
		e = unpack_imp_int64<short>(buffer, d, t);
#endif

#else
		if(sizeof(short) == 2) {
			e = unpack_imp_int16<short>(buffer, d, t);
		} else if(sizeof(short) == 4) {
			e = unpack_imp_int32<short>(buffer, d, t);
		} else {
			e = unpack_imp_int64<short>(buffer, d, t);
		}
#endif

		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_int(TBuffer & buffer, int & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_int(TBuffer & buffer, int & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}

#if defined(SIZEOF_INT)
#if SIZEOF_INT == 2
		e = unpack_imp_int16<int>(buffer, d, t);
#elif SIZEOF_INT == 4
		e = unpack_imp_int32<int>(buffer, d, t);
#else
		e = unpack_imp_int64<int>(buffer, d, t);
#endif

#elif defined(INT_MAX)
#if INT_MAX == 0x7fff
		e = unpack_imp_int16<int>(buffer, d, t);
#elif INT_MAX == 0x7fffffff
		e = unpack_imp_int32<int>(buffer, d, t);
#else
		e = unpack_imp_int64<int>(buffer, d, t);
#endif

#else
		if(sizeof(int) == 2) {
			e = unpack_imp_int16<int>(buffer, d, t);
		} else if(sizeof(int) == 4) {
			e = unpack_imp_int32<int>(buffer, d, t);
		} else {
			e = unpack_imp_int64<int>(buffer, d, t);
		}
#endif

		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_long(TBuffer & buffer, long & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_long(TBuffer & buffer, long & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		
#if defined(SIZEOF_LONG)
#if SIZEOF_LONG == 2
		e = unpack_imp_int16<long>(buffer, d, t);
#elif SIZEOF_LONG == 4
		e = unpack_imp_int32<long>(buffer, d, t);
#else
		e = unpack_imp_int64<long>(buffer, d, t);
#endif

#elif defined(LONG_MAX)
#if LONG_MAX == 0x7fffL
		e = unpack_imp_int16<long>(buffer, d, t);
#elif LONG_MAX == 0x7fffffffL
		e = unpack_imp_int32<long>(buffer, d, t);
#else
		e = unpack_imp_int64<long>(buffer, d, t);
#endif

#else
		if(sizeof(long) == 2) {
			e = unpack_imp_int16<long>(buffer, d, t);
		} else if(sizeof(long) == 4) {
			e = unpack_imp_int32<long>(buffer, d, t);
		} else {
			e = unpack_imp_int64<long>(buffer, d, t);
		}
#endif

		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_long_long(TBuffer & buffer, long long & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_long_long(TBuffer & buffer, long long & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		
#if defined(SIZEOF_LONG_LONG)
#if SIZEOF_LONG_LONG == 2
		e = unpack_imp_int16<long long>(buffer, d, t);
#elif SIZEOF_LONG_LONG == 4
		e = unpack_imp_int32<long long>(buffer, d, t);
#else
		e = unpack_imp_int64<long long>(buffer, d, t);
#endif

#elif defined(LLONG_MAX)
#if LLONG_MAX == 0x7fffL
		e = unpack_imp_int16<long long>(buffer, d, t);
#elif LLONG_MAX == 0x7fffffffL
		e = unpack_imp_int32<long long>(buffer, d, t);
#else
		e = unpack_imp_int64<long long>(buffer, d, t);
#endif

#else
		if(sizeof(long long) == 2) {
			e = unpack_imp_int16<long long>(buffer, d, t);
		} else if(sizeof(long long) == 4) {
			e = unpack_imp_int32<long long>(buffer, d, t);
		} else {
			e = unpack_imp_int64<long long>(buffer, d, t);
		}
#endif

		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_unsigned_char(TBuffer & buffer, unsigned char & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_unsigned_char(TBuffer & buffer, unsigned char & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		
		e = unpack_imp_uint8<unsigned char>(buffer, d, t);

		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_unsigned_short(TBuffer & buffer, unsigned short & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_unsigned_short(TBuffer & buffer, unsigned short & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		
#if defined(SIZEOF_SHORT)
#if SIZEOF_SHORT == 2
		e = unpack_imp_uint16<unsigned short>(buffer, d, t);
#elif SIZEOF_SHORT == 4
		e = unpack_imp_uint32<unsigned short>(buffer, d, t);
#else
		e = unpack_imp_uint64<unsigned short>(buffer, d, t);
#endif

#elif defined(USHRT_MAX)
#if USHRT_MAX == 0xffffU
		e = unpack_imp_uint16<unsigned short>(buffer, d, t);
#elif USHRT_MAX == 0xffffffffU
		e = unpack_imp_uint32<unsigned short>(buffer, d, t);
#else
		e = unpack_imp_uint64<unsigned short>(buffer, d, t);
#endif

#else
		if(sizeof(unsigned short) == 2) {
			e = unpack_imp_uint16<unsigned short>(buffer, d, t);
		} else if(sizeof(unsigned short) == 4) {
			e = unpack_imp_uint32<unsigned short>(buffer, d, t);
		} else {
			e = unpack_imp_uint64<unsigned short>(buffer, d, t);
		}
#endif

		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_unsigned_int(TBuffer & buffer, unsigned int & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_unsigned_int(TBuffer & buffer, unsigned int & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		
#if defined(SIZEOF_INT)
#if SIZEOF_INT == 2
		e = unpack_imp_uint16<unsigned int>(buffer, d, t);
#elif SIZEOF_INT == 4
		e = unpack_imp_uint32<unsigned int>(buffer, d, t);
#else
		e = unpack_imp_uint64<unsigned int>(buffer, d, t);
#endif

#elif defined(UINT_MAX)
#if UINT_MAX == 0xffffU
		e = unpack_imp_uint16<unsigned int>(buffer, d, t);
#elif UINT_MAX == 0xffffffffU
		e = unpack_imp_uint32<unsigned int>(buffer, d, t);
#else
		e = unpack_imp_uint64<unsigned int>(buffer, d, t);
#endif

#else
		if(sizeof(unsigned int) == 2) {
			e = unpack_imp_uint16<unsigned int>(buffer, d, t);
		} else if(sizeof(unsigned int) == 4) {
			e = unpack_imp_uint32<unsigned int>(buffer, d, t);
		} else {
			e = unpack_imp_uint64<unsigned int>(buffer, d, t);
		}
#endif

		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_unsigned_long(TBuffer & buffer, unsigned long & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_unsigned_long(TBuffer & buffer, unsigned long & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}

#if defined(SIZEOF_LONG)
#if SIZEOF_LONG == 2
		e = unpack_imp_uint16<unsigned long>(buffer, d, t);
#elif SIZEOF_LONG == 4
		e = unpack_imp_uint32<unsigned long>(buffer, d, t);
#else
		e = unpack_imp_uint64<unsigned long>(buffer, d, t);
#endif

#elif defined(ULONG_MAX)
#if ULONG_MAX == 0xffffUL
		e = unpack_imp_uint16<unsigned long>(buffer, d, t);
#elif ULONG_MAX == 0xffffffffUL
		e = unpack_imp_uint32<unsigned long>(buffer, d, t);
#else
		e = unpack_imp_uint64<unsigned long>(buffer, d, t);
#endif

#else
		if(sizeof(unsigned long) == 2) {
			e = unpack_imp_uint16<unsigned long>(buffer, d, t);
		} else if(sizeof(unsigned long) == 4) {
			e = unpack_imp_uint32<unsigned long>(buffer, d, t);
		} else {
			e = unpack_imp_uint64<unsigned long>(buffer, d, t);
		}
#endif

		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_unsigned_long_long(TBuffer & buffer, unsigned long long & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_unsigned_long_long(TBuffer & buffer, unsigned long long & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		
#if defined(SIZEOF_LONG_LONG)
#if SIZEOF_LONG_LONG == 2
		e = unpack_imp_uint16<unsigned long long>(buffer, d, t);
#elif SIZEOF_LONG_LONG == 4
		e = unpack_imp_uint32<unsigned long long>(buffer, d, t);
#else
		e = unpack_imp_uint64<unsigned long long>(buffer, d, t);
#endif

#elif defined(ULLONG_MAX)
#if ULLONG_MAX == 0xffffUL
		e = unpack_imp_uint16<unsigned long long>(buffer, d, t);
#elif ULLONG_MAX == 0xffffffffUL
		e = unpack_imp_uint32<unsigned long long>(buffer, d, t);
#else
		e = unpack_imp_uint64<unsigned long long>(buffer, d, t);
#endif

#else
		if(sizeof(unsigned long long) == 2) {
			e = unpack_imp_uint16<unsigned long long>(buffer, d, t);
		} else if(sizeof(unsigned long long) == 4) {
			e = unpack_imp_uint32<unsigned long long>(buffer, d, t);
		} else {
			e = unpack_imp_uint64<unsigned long long>(buffer, d, t);
		}
#endif

		if (e != RE_Success)
		{
			clear_pack_type(buffer, t);
		}
		LogTraceStepReturn(e);
	}


	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_float(TBuffer & buffer, float & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_float(TBuffer & buffer, float & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		if (t == _msgpack_head_float32)
		{
			char buf[sizeof(d)];
			e = buffer.read(buf, sizeof(d));
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}
			union { uint32 i; float f; } mem;
			load<uint32>(mem.i, buf);
			d = mem.f;
			LogTraceStepReturn(RE_Success);
		}
		clear_pack_type(buffer, t);
		LogTraceStepReturn(RE_Msgpack_TypeError);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_double(TBuffer & buffer, double & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_double(TBuffer & buffer, double & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		if (t == _msgpack_head_float64)
		{
			char buf[sizeof(d)];
			e = buffer.read(buf, sizeof(d));
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}
			union { uint64 i; double f; } mem;
			load<uint64>(mem.i, buf);

#if defined(__arm__) && !(__ARM_EABI__) // arm-oabi
			// https://github.com/msgpack/msgpack-perl/pull/1
			mem.i = (mem.i & 0xFFFFFFFFUL) << 32UL | (mem.i >> 32UL);
#endif
			d = mem.f;
			LogTraceStepReturn(RE_Success);
		}
		clear_pack_type(buffer, t);
		LogTraceStepReturn(RE_Msgpack_TypeError);
	}


	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_nil(TBuffer & buffer)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_nil(TBuffer & buffer)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		if (t == _msgpack_head_nil)
		{
			LogTraceStepReturn(RE_Success);
		}
		clear_pack_type(buffer, t);
		LogTraceStepReturn(RE_Msgpack_TypeError);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_bool(TBuffer & buffer, bool & d)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_bool(TBuffer & buffer, bool & d)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		if (t == _msgpack_head_true)
		{
			d = true;
			LogTraceStepReturn(RE_Success);
		} 
		else if(t == _msgpack_head_false)
		{
			d = false;
			LogTraceStepReturn(RE_Success);
		}
		clear_pack_type(buffer, t);
		LogTraceStepReturn(RE_Msgpack_TypeError);
	}



	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_array(TBuffer & buffer, uint32 & l)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_array(TBuffer & buffer, uint32 & l)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		if (t >= _msgpack_head_fixarray_from && t <= _msgpack_head_fixarray_to) //fixarray
		{
			l = t & 0xfu;
			LogTraceStepReturn(RE_Success);
		}
		else if (t == _msgpack_head_array16)
		{
			uint16 len = 0;
			char buf[sizeof(len)];
			e = buffer.read(buf, sizeof(len));
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}
			load<uint16>(len, buf);
			l = len;
			LogTraceStepReturn(RE_Success);
		}
		else if (t == _msgpack_head_array32)
		{
			char buf[sizeof(l)];
			e = buffer.read(buf, sizeof(l));
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}
			load<uint32>(l, buf);
			LogTraceStepReturn(RE_Success);
		}
		clear_pack_type(buffer, t);
		LogTraceStepReturn(RE_Msgpack_TypeError);
	}


	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_map(TBuffer & buffer, uint32 & l)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_map(TBuffer & buffer, uint32 & l)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		if (t >= _msgpack_head_fixmap_from && t <= _msgpack_head_fixmap_to) //fixmap
		{
			l = t & 0xfu;
			LogTraceStepReturn(RE_Success);
		}
		else if (t == _msgpack_head_map16)
		{
			uint16 len = 0;
			char buf[sizeof(len)];
			e = buffer.read(buf, sizeof(len));
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}
			load<uint16>(len, buf);
			l = len;
			LogTraceStepReturn(RE_Success);
		}
		else if (t == _msgpack_head_map32)
		{
			char buf[sizeof(l)];
			e = buffer.read(buf, sizeof(l));
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}
			load<uint32>(l, buf);
			LogTraceStepReturn(RE_Success);
		}
		clear_pack_type(buffer, t);
		LogTraceStepReturn(RE_Msgpack_TypeError);
	}


	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_str(TBuffer & buffer, uint32 & l)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_str(TBuffer & buffer, uint32 & l)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		if (t >= _msgpack_head_fixstr_from && t <= _msgpack_head_fixstr_to) //fixstr
		{
			l = t & 0x1fu;
			LogTraceStepReturn(RE_Success);
		}
		switch (t)
		{
		case _msgpack_head_str8:
			{
				uint8 len = 0;
				e = buffer.read(&len, sizeof(len));
				if (e != RE_Success)
				{
					LogTraceStepReturn(e);
				}
				l = len;
				LogTraceStepReturn(RE_Success);
			}
		case _msgpack_head_str16:
			{
				uint16 len = 0;
				char buf[sizeof(len)];
				e = buffer.read(buf, sizeof(len));
				if (e != RE_Success)
				{
					LogTraceStepReturn(e);
				}
				load<uint16>(len, buf);
				l = len;
				LogTraceStepReturn(RE_Success);
			}
		case _msgpack_head_str32:
			{
				char buf[sizeof(l)];
				e = buffer.read(buf, sizeof(l));
				if (e != RE_Success)
				{
					LogTraceStepReturn(e);
				}
				load<uint32>(l, buf);
				LogTraceStepReturn(RE_Success);
			}
		default:
			break;
		}
		clear_pack_type(buffer, t);
		LogTraceStepReturn(RE_Msgpack_TypeError);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_str_body(TBuffer & buffer, char * b, uint32 l)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_str_body(TBuffer & buffer, char * b, uint32 l)");
		LogTraceStepReturn(buffer.read(b, l));
	}


	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_str_string(TBuffer & buffer, string & s)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_str_string(TBuffer & buffer, string & s)");
		uint32 len = 0;
		unpack_str(buffer, len);
		LogTraceStepReturn(buffer.read(s, len));
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_str_simple(TBuffer & buffer, const char ** b, uint32 & l)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_str_simple(TBuffer & buffer, const char ** b, uint32 & l)");
		unpack_str(buffer, l);
		LogTraceStepReturn(buffer.read(b, l));
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_bin(TBuffer & buffer, uint32  & l)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_bin(TBuffer & buffer, uint32  & l)");
		uint8 t = 0;
		RuntimeError e= buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		switch (t)
		{
		case _msgpack_head_bin8:
			{
				uint8 len = 0;
				e = buffer.read(&len, sizeof(len));
				if (e != RE_Success)
				{
					LogTraceStepReturn(e);
				}
				l = len;
				LogTraceStepReturn(RE_Success);
			}
		case _msgpack_head_bin16:
			{
				uint16 len = 0;
				char buf[sizeof(len)];
				e = buffer.read(buf, sizeof(len));
				if (e != RE_Success)
				{
					LogTraceStepReturn(e);
				}
				load<uint16>(len, buf);
				l = len;
				LogTraceStepReturn(RE_Success);
			}
		case _msgpack_head_bin32:
			{
				char buf[sizeof(l)];
				e = buffer.read(buf, sizeof(l));
				if (e != RE_Success)
				{
					LogTraceStepReturn(e);
				}
				load<uint32>(l, buf);
				LogTraceStepReturn(RE_Success);
			}
		default:
			break;
		}
		clear_pack_type(buffer, t);
		LogTraceStepReturn(RE_Msgpack_TypeError);
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_bin_body(TBuffer & buffer, char * b, uint32 l)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_bin_body(TBuffer & buffer, char * b, uint32 l)");
		LogTraceStepReturn(buffer.read(b, l));
	}


	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_ext(TBuffer & buffer, uint32 & l, int8 & type)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_ext(TBuffer & buffer, uint32 & l, int8 & type)");
		uint8 t = 0;
		RuntimeError e = buffer.read(&t, sizeof(t));
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		switch (t)
		{
		case _msgpack_head_fixext1:
			l = 1;
			break;
		case _msgpack_head_fixext2:
			l = 2;
			break;
		case _msgpack_head_fixext4:
			l = 4;
			break;
		case _msgpack_head_fixext8:
			l = 8;
			break;
		case _msgpack_head_fixext16:
			l = 16;
			break;
		case _msgpack_head_ext8:
			{
				uint8 len = 0;
				e = buffer.read(&len, sizeof(len));
				if (e != RE_Success)
				{
					LogTraceStepReturn(e);
				}
				l = len;
			}
			break;
		case _msgpack_head_ext16:
			{
				uint16 len = 0;
				char buf[sizeof(len)];
				e = buffer.read(buf, sizeof(len));
				if (e != RE_Success)
				{
					LogTraceStepReturn(e);
				}
				load<uint16>(len, buf);
				l = len;
			}
			
			break;
		case _msgpack_head_ext32:
			{
				char buf[sizeof(l)];
				e = buffer.read(buf, sizeof(l));
				if (e != RE_Success)
				{
					LogTraceStepReturn(e);
				}
				load<uint32>(l, buf);
			}
			break;
		default:
			clear_pack_type(buffer, t);
			LogTraceStepReturn(RE_Msgpack_TypeError);
		}
		LogTraceStepReturn(buffer.read(&type, sizeof(type)));
	}

	template <typename TBuffer>
	inline RuntimeError Unpacker<TBuffer>::unpack_ext_body(TBuffer & buffer, char * b, uint32 l)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_ext_body(TBuffer & buffer, char * b, uint32 l)");
		LogTraceStepReturn(buffer.read(b, l));
	}


	template <typename TBuffer>
	template <typename T>
	RuntimeError Unpacker<TBuffer>::unpack_imp_uint8(TBuffer & buffer, typename enable_if<sizeof(T) == sizeof(uint8), T>::type & d, uint8 header)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_imp_uint8(TBuffer & buffer, T & d, uint8 header)");
		if (header == _msgpack_head_uint8)
		{
			LogTraceStepReturn(buffer.read(&d, sizeof(d)));
		}
		else if (header <= _msgpack_head_positive_fixint_to)
		{
			memcpy(&d, &header, sizeof(header));
			LogTraceStepReturn(RE_Success);
		}
		//类型不对
		LogTraceStepReturn(RE_Msgpack_TypeError);
	}

	template <typename TBuffer>
	template <typename T>
	RuntimeError Unpacker<TBuffer>::unpack_imp_uint16(TBuffer & buffer, typename enable_if<sizeof(T) == sizeof(uint16), T>::type & d, uint8 header)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_imp_uint16(TBuffer & buffer, T & d, uint8 header)");
		if (header == _msgpack_head_uint16)
		{
			char buf[sizeof(d)];
			RuntimeError e = buffer.read(buf, sizeof(d));
			if (e == RE_Success)
			{
				load<T>(d, buf);
			}
			LogTraceStepReturn(e);
		}
		LogTraceStepReturn(unpack_imp_check_int(buffer, d, header));
		
	}

	template <typename TBuffer>
	template <typename T>
	RuntimeError Unpacker<TBuffer>::unpack_imp_uint32(TBuffer & buffer, typename enable_if<sizeof(T) == sizeof(uint32), T>::type & d, uint8 header)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_imp_uint32(TBuffer & buffer, T & d, uint8 header)");
		if (header == _msgpack_head_uint32)
		{
			char buf[sizeof(d)];
			RuntimeError e = buffer.read(buf, sizeof(d));
			if (e == RE_Success)
			{
				load<T>(d, buf);
			}
			LogTraceStepReturn(e);
		}
		LogTraceStepReturn(unpack_imp_check_int(buffer, d, header));
		
	}

	template <typename TBuffer>
	template <typename T>
	RuntimeError Unpacker<TBuffer>::unpack_imp_uint64(TBuffer & buffer, typename enable_if<sizeof(T) == sizeof(uint64), T>::type & d, uint8 header)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_imp_uint64(TBuffer & buffer, T & d, uint8 header)");
		if (header == _msgpack_head_uint64)
		{
			char buf[sizeof(d)];
			RuntimeError e = buffer.read(buf, sizeof(d));
			if (e == RE_Success)
			{
				load<T>(d, buf);
			}
			LogTraceStepReturn(e);
		}
		LogTraceStepReturn(unpack_imp_check_int(buffer, d, header));
		
	}

	template <typename TBuffer>
	template <typename T>
	RuntimeError Unpacker<TBuffer>::unpack_imp_int8(TBuffer & buffer, typename enable_if<sizeof(T) == sizeof(int8), T>::type & d, uint8 header)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_imp_int8(TBuffer & buffer, T & d, uint8 header)");
		if (header == _msgpack_head_int8)
		{
			LogTraceStepReturn(buffer.read(&d, sizeof(d)));
		}
		else if (header >= _msgpack_head_negative_fixint_from || header <= _msgpack_head_positive_fixint_to) 
		{
			memcpy(&d, &header, sizeof(header));
			LogTraceStepReturn(RE_Success);
		}
		//类型不对
		LogTraceStepReturn(RE_Msgpack_TypeError);
	}

	template <typename TBuffer>
	template <typename T>
	RuntimeError Unpacker<TBuffer>::unpack_imp_int16(TBuffer & buffer, typename enable_if<sizeof(T) == sizeof(int16), T>::type & d, uint8 header)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_imp_int16(TBuffer & buffer, T & d, uint8 header)");
		if (header == _msgpack_head_int16 || header == _msgpack_head_uint16)
		{
			char buf[sizeof(d)];
			RuntimeError e = buffer.read(buf, sizeof(d));
			if (e == RE_Success)
			{
				load<T>(d, buf);
			}
			LogTraceStepReturn(e);
		}
		LogTraceStepReturn(unpack_imp_check_int(buffer, d, header));
		
	}

	template <typename TBuffer>
	template <typename T>
	RuntimeError Unpacker<TBuffer>::unpack_imp_int32(TBuffer & buffer, typename enable_if<sizeof(T) == sizeof(int32), T>::type & d, uint8 header)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_imp_int32(TBuffer & buffer, T & d, uint8 header)");
		if (header == _msgpack_head_int32 || header == _msgpack_head_uint32)
		{
			char buf[sizeof(d)];
			RuntimeError e = buffer.read(buf, sizeof(d));
			if ( e == RE_Success)
			{
				load<T>(d, buf);
			}
			LogTraceStepReturn(e);
		}
		LogTraceStepReturn(unpack_imp_check_int(buffer, d, header));
	
	}

	template <typename TBuffer>
	template <typename T>
	RuntimeError Unpacker<TBuffer>::unpack_imp_int64(TBuffer & buffer, typename enable_if<sizeof(T) == sizeof(int64), T>::type & d, uint8 header)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_imp_int64(TBuffer & buffer, T & d, uint8 header)");
		if (header == _msgpack_head_int64 || header == _msgpack_head_uint64)
		{
			char buf[sizeof(d)];
			RuntimeError e = buffer.read(buf, sizeof(d));
			if (e == RE_Success)
			{
				load<T>(d, buf);
			}
			LogTraceStepReturn(e);
		}
		LogTraceStepReturn(unpack_imp_check_int(buffer, d, header));
	}


	template <typename TBuffer>
	template <typename T>
	RuntimeError Unpacker<TBuffer>::unpack_imp_check_int(TBuffer & buffer, T & d, uint8 header)
	{
		LogTraceStepCall("RuntimeError Unpacker<TBuffer>::unpack_imp_check_int(Tbuffer & buffer, T & d, uint8 header)");
		switch (header)
		{
		case _msgpack_head_int32:
			{
				int32 t = 0;
				RuntimeError e = unpack_imp_int32<int32>(buffer, t, header);
				if (e == RE_Success) { d = static_cast<T>(t); }
				LogTraceStepReturn(e);
			}
			break;
		case _msgpack_head_uint32:
			{
				uint32 t = 0;
				RuntimeError e = unpack_imp_uint32<uint32>(buffer, t, header);
				if (e == RE_Success) { d = static_cast<T>(t); }
				LogTraceStepReturn(e);
			}
			break;
		case _msgpack_head_int16:
			{
				int16 t = 0;
				RuntimeError e = unpack_imp_int16<int16>(buffer, t, header);
				if (e == RE_Success) { d = static_cast<T>(t); }
				LogTraceStepReturn(e);
			}
			break;
		case _msgpack_head_uint16:
			{
				uint16 t = 0;
				RuntimeError e = unpack_imp_uint16<uint16>(buffer, t, header);
				if (e == RE_Success) { d = static_cast<T>(t); }
				LogTraceStepReturn(e);
			}
			break;
		case _msgpack_head_int8:
			{
				int8 t = 0;
				RuntimeError e = buffer.read(&t, sizeof(t));
				if (e == RE_Success) { d = static_cast<T>(t); }
				LogTraceStepReturn(e);
			}
			break;
		case _msgpack_head_uint8:
			{
				uint8 t = 0;
				RuntimeError e = buffer.read(&t, sizeof(t));
				if (e == RE_Success) { d = static_cast<T>(t); }
				LogTraceStepReturn(e);
			}
			break;
		default:
			if (header >= _msgpack_head_negative_fixint_from || header <= _msgpack_head_positive_fixint_to)
			{
				int8 t = 0;
				memcpy(&t, &header, sizeof(header));
				d = static_cast<T>(t);
				LogTraceStepReturn(RE_Success);
			}
			break;
		}
		//类型不对
		LogTraceStepReturn(RE_Msgpack_TypeError);
	}

}



#endif