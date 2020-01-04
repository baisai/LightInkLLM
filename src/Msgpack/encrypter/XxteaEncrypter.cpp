


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

#define XXTEA_DELTA 0x9e3779b9
#define XXTEA_MX ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (key[p & 3 ^ e] ^ z))

#include <memory.h>
#include "Log/Log.h"
#include "Msgpack/encrypter/XxteaEncrypter.h"

namespace LightInk
{
	RuntimeError XxteaEncrypter::encrypt(const char * src, uint32 srcLen, char * dest, uint32 * destLen, const char * key, uint32 keyLen)
	{
		LogTraceStepCall("RuntimeError XxteaEncrypter::encrypt(const char * src, uint32 srcLen, char * dest, uint32 * destLen, const char * key, uint32 keyLen)");
		int32 n = 0;
		uint32 & ret_length = *destLen;
		if (keyLen != 16 || srcLen < 1 || !src || !key)
		{
			LogTraceStepReturn(RE_Msgpack_DataEncryptError);
		}

		n = (srcLen >> 2) + 1;
		if (n <= 1)
		{
			n = 2;
		}
		ret_length = n << 2;
		memcpy(dest, src, srcLen);
		memset(dest + srcLen, ret_length - srcLen, ret_length - srcLen);
		LogTraceStepReturn(encrypt((int32 *)dest, n, (int32 *)key));
	}

	RuntimeError XxteaEncrypter::decrypt(const char * src, uint32 srcLen, char * dest, uint32 * destLen, const char * key, uint32 keyLen)
	{
		LogTraceStepCall("RuntimeError XxteaEncrypter::decrypt(const char * src, uint32 srcLen, char * dest, uint32 * destLen, const char * key, uint32 keyLen)");
		uint32 & ret_length = *destLen;
		if (keyLen != 16 || srcLen < 1 || (srcLen & 3U) != 0 || !src || !key)
		{
			LogTraceStepReturn(RE_Msgpack_DataEncryptError);
		}
		memcpy(dest, src, srcLen);
		RuntimeError e = decrypt((int32 *)dest, srcLen >> 2, (int32 *)key);
		if(e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		int32 n = dest[srcLen - 1];
		if (n > 4)
		{
			LogTraceStepReturn(RE_Msgpack_DataEncryptError);
		}
		ret_length = srcLen - n;
		LogTraceStepReturn(RE_Success);
	}

	RuntimeError XxteaEncrypter::encrypt(int32 * data, uint32 len, int32 * key)
	{
		LogTraceStepCall("RuntimeError XxteaEncrypter::encrypt(int32 * data, uint32 len, int32 * key)");
		int32 n = len - 1;
		int32 z = data[n], y = data[0], p, q = 6 + 52 / len, sum = 0, e;
		if (n < 1) {
			LogTraceStepReturn(RE_Msgpack_DataEncryptError);
		}
		while (0 < q--) {
			sum += XXTEA_DELTA;
			e = (sum >> 2) & 3;
			for (p = 0; p < n; p++) {
				y = data[p + 1];
				z = data[p] += XXTEA_MX;
			}
			y = data[0];
			z = data[n] += XXTEA_MX;
		}
		LogTraceStepReturn(RE_Success);
	}

	RuntimeError XxteaEncrypter::decrypt(int32 * data, uint32 len, int32 * key)
	{
		LogTraceStepCall("RuntimeError XxteaEncrypter::decrypt(int32 * data, uint32 len, int32 * key)");
		int32 n = len - 1;
		int32 z = data[n], y = data[0], p, q = 6 + 52 / len, sum = q * XXTEA_DELTA, e;
		if (n < 1) {
			LogTraceStepReturn(RE_Msgpack_DataEncryptError);
		}
		while (sum != 0) {
			e = (sum >> 2) & 3;
			for (p = n; p > 0; p--) {
				z = data[p - 1];
				y = data[p] -= XXTEA_MX;
			}
			z = data[n];
			y = data[0] -= XXTEA_MX;
			sum -= XXTEA_DELTA;
		}
		LogTraceStepReturn(RE_Success);
	}

	uint32 XxteaEncrypter::get_encrypt_max_len(uint32 srcLen)
	{
		LogTraceStepCall("uint32 XxteaEncrypter::get_encrypt_max_len(uint32 srcLen)");
		uint32 n = (srcLen >> 2) + 1;
		if (n <= 1)
		{
			n = 2;
		}
		n <<= 2;
		LogTraceStepReturn(n);
	}

	RuntimeError XxteaEncrypter::get_decrypt_len(const char * src, uint32 srcLen, uint32 * destLen)
	{
		LogTraceStepCall("RuntimeError XxteaEncrypter::get_decrypt_len(const char * src, uint32 srcLen, uint32 * destLen)");
		int32 n = src[srcLen - 1];
		if (n > 4)
		{
			LogTraceStepReturn(RE_Msgpack_DataEncryptError);
		}
		*destLen = srcLen - n;
		LogTraceStepReturn(RE_Success);
	}

}


#undef XXTEA_DELTA
#undef XXTEA_MX
