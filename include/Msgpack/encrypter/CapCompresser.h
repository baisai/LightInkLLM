


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

#ifndef LIGHTINK_MSGPACK_CAPCOMPRESSER_H_

#define LIGHTINK_MSGPACK_CAPCOMPRESSER_H_

#include "Common/Type.h"
#include "Common/RuntimeError.h"
#include "Msgpack/DataBuffer.h"

namespace LightInk
{
	class LIGHTINK_DECL CapCompresser
	{
	public:
		template <typename TBuffer>
		static RuntimeError compress(TBuffer & src, DataBuffer * dest)
		{
			LogTraceStepCall("RuntimeError CapCompresser::compress(TBuffer & src, TBuffer & dest)");
			if (src.write_pos() == 0)
			{
				LogTraceStepReturn(RE_Msgpack_EmptyData);
			}
			dest->clear();
			RuntimeError e = dest->resize_buffer((src.write_pos() >> 3) + src.write_pos() + 2);
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}

			uint32 modLen = src.write_pos() & 0x00000007;
			uint32 len = src.write_pos() >> 3;

			src.read_pos(0);
			char * buffer = src.data();
			char * destBuffer = dest->data();
			char * flag = NULL;
			uint32 idx = 0;
			uint32 destSize = 0;
			for (uint32 i = 0; i < len; ++i)
			{
				flag = destBuffer + destSize;
				*flag = 0;
				++destSize;
				for (uint32 j = 0; j < 8; ++j)
				{
					if (*(buffer + idx) != 0)
					{
						*(destBuffer + destSize) = *(buffer + idx);
						++destSize;
						*flag |= 1u << j;
					}
					++idx;
				}
			}

			if (modLen > 0)
			{
				flag = destBuffer + destSize;
				*flag = 0;
				++destSize;
				for (uint32 j = 0; j < modLen; ++j)
				{
					if (*(buffer + idx) != 0)
					{
						*(destBuffer + destSize) = *(buffer + idx);
						++destSize;
						*flag |= 1u << j;
					}
					++idx;
				}

				*(destBuffer + destSize) = modLen;
				++destSize;
			}
			dest->write_pos(destSize);
			LogTraceStepReturn(RE_Success);
		}

		template <typename TBuffer>
		static RuntimeError uncompress(TBuffer & src, DataBuffer * dest)
		{
			LogTraceStepCall("RuntimeError CapCompresser::uncompress(TBuffer & src, TBuffer & dest)");
			if (src.write_pos() == 0)
			{
				LogTraceStepReturn(RE_Msgpack_EmptyData);
			}

			uint32 pos = src.write_pos();
			dest->clear();
			RuntimeError e = dest->resize_buffer(pos + (pos >> 2) + 8);
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}

			src.read_pos(0);
			uint8 idx = 0;
			char * buffer = src.data();
			uint32 destSize = 0;
			char flag = *buffer;
			for (uint32 i = 1; i < pos; )
			{
				if ((destSize + 8) > dest->buffer_size())
				{
					e = dest->resize_buffer(destSize + (destSize >> 3) + 8);
					if (e != RE_Success)
					{
						LogTraceStepReturn(e);
					}
				}
				while (idx < 8) //0填充
				{
					if (flag & 1u << idx && i < pos)
					{
						*(dest->data() + destSize) = *(buffer + i);
						++i;
					}
					else
					{
						*(dest->data() + destSize) = 0;
					}
					++destSize;
					++idx;
				}
				idx = 0;
				flag = *(buffer + i);
				++i;
			}
			destSize -= (8 - *(buffer + pos - 1));
			dest->write_pos(destSize);
			LogTraceStepReturn(RE_Success);
		}

		static RuntimeError compress(const char * src, uint32 srcLen, char * dest, uint32 * destLen);
		static RuntimeError uncompress(const char * src, uint32 srcLen, char * dest, uint32 * destLen);

		static uint32 get_compress_max_len(uint32 srcLen);
		static RuntimeError get_uncompress_len(const char * src, uint32 srcLen, uint32 * destLen);
	};
}


#endif