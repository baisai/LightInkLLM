


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

#ifndef LIGHTINK_MSGPACK_XXTEAENCRYPTER_H_

#define LIGHTINK_MSGPACK_XXTEAENCRYPTER_H_

#include "Common/Type.h"
#include "Common/RuntimeError.h"
#include "Msgpack/DataBuffer.h"

namespace LightInk
{
	class LIGHTINK_DECL XxteaEncrypter
	{
	public:
		template<typename TBuffer>
		static RuntimeError encrypt(TBuffer & src, DataBuffer * dest, const char * key, uint32 keyLen)
		{
			LogTraceStepCall("RuntimeError XxteaEncrypter::encrypt(TBuffer & src, TBuffer & dest, const char * key, uint32 keyLen)");
			src.read_pos(0);
			dest->clear();
			uint32 n = (src.size() >> 2) + 1;
			if (n <= 1)
			{
				n = 2;
			}
			n <<= 2;
			RuntimeError e = dest->resize_buffer(n);
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}
			e = encrypt(src.data(), src.size(), dest->data(), &n, key, keyLen);
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}
			dest->write_pos(n);
			LogTraceStepReturn(RE_Success);
		}

		template<typename TBuffer>
		static RuntimeError decrypt(TBuffer & src, DataBuffer * dest, const char * key, uint32 keyLen)
		{
			LogTraceStepCall("RuntimeError XxteaEncrypter::decrypt(TBuffer & src, TBuffer & dest, const char * key, uint32 keyLen)");
			src.read_pos(0);
			dest->clear();
			RuntimeError e = dest->resize_buffer(src.write_pos());
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}
			uint32 n = 0;
			e = decrypt(src.data(), src.size(), dest->data(), &n, key, keyLen);
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}
			dest->write_pos(n);
			LogTraceStepReturn(RE_Success);
		}

		static RuntimeError encrypt(const char * src, uint32 srcLen, char * dest, uint32 * destLen, const char * key, uint32 keyLen);
		static RuntimeError decrypt(const char * src, uint32 srcLen, char * dest, uint32 * destLen, const char * key, uint32 keyLen);

		static RuntimeError encrypt(int32 * data, uint32 len, int32 * key);
		static RuntimeError decrypt(int32 * data, uint32 len, int32 * key);


		static uint32 get_encrypt_max_len(uint32 srcLen);
		static RuntimeError get_decrypt_len(const char * src, uint32 srcLen, uint32 * destLen);
	};
}


#endif