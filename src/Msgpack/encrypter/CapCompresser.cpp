


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


#include "Log/Log.h"
#include "Msgpack/encrypter/CapCompresser.h"

namespace LightInk
{
	RuntimeError CapCompresser::compress(const char * src, uint32 srcLen, char * dest, uint32 * destLen)
	{
		LogTraceStepCall("RuntimeError CapCompresser::compress(const char * src, uint32 srcLen, char * dest, uint32 * destLen)");
		if (srcLen == 0)
		{
			LogTraceStepReturn(RE_Msgpack_EmptyData);
		}

		uint32 modLen = srcLen & 0x00000007;
		uint32 len = srcLen >> 3;
		uint32 & dl = *destLen;
		dl = 0;
		char * flag = NULL;
		uint32 idx = 0;
		for (uint32 i = 0; i < len; ++i)
		{
			flag = dest + dl;
			*flag = 0;
			++dl;
			for (uint32 j = 0; j < 8; ++j)
			{
				if (*(src + idx) != 0)
				{
					*(dest + dl) = *(src + idx);
					++dl;
					*flag |= 1u << j;
				}
				++idx;
			}
		}

		if (modLen > 0)
		{
			flag = dest + dl;
			*flag = 0;
			++dl;
			for (uint32 j = 0; j < modLen; ++j)
			{
				if (*(src + idx) != 0)
				{
					*(dest + dl) = *(src + idx);
					++dl;
					*flag |= 1u << j;
				}
				++idx;
			}

			*(dest + dl) = modLen;
			++dl;
		}
		LogTraceStepReturn(RE_Success);
	}

	RuntimeError CapCompresser::uncompress(const char * src, uint32 srcLen, char * dest, uint32 * destLen)
	{
		LogTraceStepCall("RuntimeError CapCompresser::uncompress(const char * src, uint32 srcLen, char * dest, uint32 * destLen)");
		if (srcLen == 0)
		{
			LogTraceStepReturn(RE_Msgpack_EmptyData);
		}

		uint32 pos = srcLen - 1;

		uint8 idx = 0;
		uint32 & dl = *destLen;
		dl = 0;
		char flag = *src;
		for (uint32 i = 1; i < pos; )
		{
			while (idx < 8) //0填充
			{
				if (flag & 1u << idx && i < pos)
				{
					*(dest + dl) = *(src + i);
					++i;
				}
				else
				{
					*(dest + dl) = 0;
				}
				++dl;
				++idx;
			}
			idx = 0;
			flag = *(src + i);
			++i;
		}
		dl -= (8 - *(src + pos));
		LogTraceStepReturn(RE_Success);
	}


	uint32 CapCompresser::get_compress_max_len(uint32 srcLen)
	{
		LogTraceStepCall("uint32 CapCompresser::get_compress_max_len(uint32 srcLen)");
		LogTraceStepReturn(((srcLen >> 3) + srcLen + 2));
	}

	RuntimeError CapCompresser::get_uncompress_len(const char * src, uint32 srcLen, uint32 * destLen)
	{
		LogTraceStepCall("RuntimeError CapCompresser::get_uncompress_len(const char * src, uint32 srcLen, uint32 * destLen)");
		if (srcLen == 0)
		{
			LogTraceStepReturn(RE_Msgpack_EmptyData);
		}

		uint32 pos = srcLen - 1;

		uint8 idx = 0;
		uint32 & dl = *destLen;
		dl = 0;
		char flag = *src;
		for (uint32 i = 1; i < pos; )
		{
			while (idx < 8) //0填充
			{
				if (flag & 1u << idx && i < pos)
				{
					++i;
				}
				++dl;
				++idx;
			}
			idx = 0;
			flag = *(src + i);
			++i;
		}
		dl -= (8 - *(src + pos));
		LogTraceStepReturn(RE_Success);
	}


}
