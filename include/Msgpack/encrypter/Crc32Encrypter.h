


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

#ifndef LIGHTINK_MSGPACK_CRC32ENCRYPTER_H_

#define LIGHTINK_MSGPACK_CRC32ENCRYPTER_H_

#include "Common/Type.h"
#include "Common/RuntimeError.h"
#include "Common/CharPtrBridge.h"
#include "Log/Log.h"

namespace LightInk
{
	class LIGHTINK_DECL Crc32Encrypter
	{
	public:
		template<typename TBuffer>
		inline static uint32 encrypt(TBuffer & src, uint32 key)
		{
			LogTraceStepCall("uint32 Crc32Encrypter::encrypt(TBuffer & src, uint32 key)");
			src.read_pos(0);
			LogTraceStepReturn(encrypt(src.data(), src.size(), key));
		}

		static uint32 encrypt(void * data, uint32 len, uint32 key);

		static uint32 encrypt(const CharPtrBridge * cpb, uint32 count, uint32 key);

	private:
		static const uint32 s_crc32Table[256];

	};
}


#endif