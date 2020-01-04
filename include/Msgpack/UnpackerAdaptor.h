


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

#ifndef LIGHTINK_MSGPACK_UNPACKERADAPTOR_H_
#define LIGHTINK_MSGPACK_UNPACKERADAPTOR_H_

#include "Unpacker.h"
#include "Common/TypeTool/TypeTool.h"
#include "Common/TypeTool/TypeListDefine.h"
#include "Common/CharPtrBridge.h"

namespace LightInk
{
	template <typename TBuffer, typename T>
	inline RuntimeError unpack(TBuffer & buffer, T & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, T & v)");
		LogTraceStepReturn(v.unpack(buffer));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, char & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, char & v)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_char(buffer, v));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, signed char & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, signed char & v)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_signed_char(buffer, v));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, short & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, short & v)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_short(buffer, v));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, int & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, int & v)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_int(buffer, v));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, long & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, long & v)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_long(buffer, v));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, long long & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, long long & v)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_long_long(buffer, v));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, unsigned char & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, unsigned char & v)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_unsigned_char(buffer, v));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, unsigned short & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, unsigned short & v)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_unsigned_short(buffer, v));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, unsigned int & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, unsigned int & v)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_unsigned_int(buffer, v));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, unsigned long & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, unsigned long & v)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_unsigned_long(buffer, v));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, unsigned long long & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, unsigned long long & v)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_unsigned_long_long(buffer, v));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, float & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, float & v)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_float(buffer, v));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, double & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, double & v)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_double(buffer, v));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, TypeEmpty &)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, TypeEmpty &)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_nil(buffer));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, bool & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, bool & v)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_bool(buffer, v));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, RuntimeError & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, RuntimeError & v)");
		int rv = 0;
		RuntimeError err = Unpacker<TBuffer>::unpack_int(buffer, rv);
		v = (RuntimeError)rv;
		LogTraceStepReturn(err);
	}

	//²»°²È«
	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, char * v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, char * v)");
		uint32 len = 0;
		RuntimeError e = Unpacker<TBuffer>::unpack_str(buffer, len);
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_str_body(buffer, v, len));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, char * v, uint32 & len)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, char * v, uint32 & len)");
		if (v)
		{
			LogTraceStepReturn(Unpacker<TBuffer>::unpack_str_body(buffer, v, len));
		}
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_str(buffer, len));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, CharPtrBridge & cpb)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, CharPtrBridge & cpb)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_str_simple(buffer, &cpb.m_charPtr, cpb.m_len));
	}

	template <typename TBuffer, uint32 len>
	inline RuntimeError unpack(TBuffer & buffer, char (& v)[len])
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, char (& v)[len])");
		uint32 size = 0;
		Unpacker<TBuffer>::unpack_str(buffer, size);
		if (size != len)
		{
			LogTraceStepReturn(RE_Msgpack_ArrayLenError);
		}
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_str_body(buffer, v, size));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, string & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, string & v)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_str_string(buffer, v));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, const char ** v, uint32 & len)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, const char ** v, uint32 & len)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_str_simple(buffer, v, len));
	}

	template <typename TBuffer>
	inline RuntimeError unpack(TBuffer & buffer, char ** v, uint32 & len)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, char ** v, uint32 & len)");
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_str_simple(buffer, const_cast<const char **>(v), len));
	}

	template <typename TBuffer, typename T, uint32 len>
	inline RuntimeError unpack(TBuffer & buffer, T (& v)[len])
	{
		LogTraceStepCall("RuntimeError pack(TBuffer & buffer, T (& v)[len])");
		uint32 size = 0;
		Unpacker<TBuffer>::unpack_array(buffer, size);
		if (size != len)
		{
			LogTraceStepReturn(RE_Msgpack_ArrayLenError);
		}
		RuntimeError e = RE_Success;
		for (uint32 i = 0; i < len; ++i)
		{
			e = unpack(buffer, *(v + i));
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer, typename T>
	inline RuntimeError unpack(TBuffer & buffer, T * v, uint32 & len)
	{
		LogTraceStepCall("RuntimeError pack(TBuffer & buffer, T * v, uint32 & len)");
		if (v)
		{
			RuntimeError e = RE_Success;
			for (uint32 i = 0; i < len; ++i)
			{
				e = unpack(buffer, *(v + i));
				if (e != RE_Success)
				{
					LogTraceStepReturn(e);
				}
			}
			LogTraceStepReturn(e);
		}
		LogTraceStepReturn(Unpacker<TBuffer>::unpack_array(buffer, len));
	}

	template <typename TBuffer, typename T>
	inline RuntimeError unpack(TBuffer & buffer, std::vector<T> & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, std::vector<T> & v)");
		uint32 len = 0;
		Unpacker<TBuffer>::unpack_array(buffer, len);
		RuntimeError e = RE_Success;
		for (uint32 i = 0; i < len; ++i)
		{
			v.push_back(T());
			e = unpack(buffer, v[i]);
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}
		}
		LogTraceStepReturn(e);
	}

	template <typename TBuffer, typename K, typename T>
	inline RuntimeError unpack(TBuffer & buffer, std::map<K, T> & v)
	{
		LogTraceStepCall("RuntimeError unpack(TBuffer & buffer, std::map<K, T> & v)");
		uint32 len = 0;
		Unpacker<TBuffer>::unpack_map(buffer, len);
		RuntimeError e = RE_Success;
		for (uint32 i = 0; i < len; ++i)
		{
			std::pair<K, T> elem;
			e = unpack(buffer, elem.first);
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}
			e = unpack(buffer, elem.second);
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}
			v.insert(elem);
		}
		LogTraceStepReturn(e);
	}

}

#endif