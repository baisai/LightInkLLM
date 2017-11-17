


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
#include "Msgpack/PackBuffer.h"
#include "Msgpack/encrypter/CapCompresser.h"
#include "Msgpack/encrypter/XxteaEncrypter.h"


void test_compress_encrypt()
{
	LogDebug("start...call...test_compress_encrypt()");

	LightInk::PackBuffer<LightInk::DataBuffer> buff;

	for (int i = 0; i < 100; i++)
	{
		buff.pack(0);
	}


	LightInk::PackBuffer<LightInk::DataBuffer> compressBuff;
	LightInk::PackBuffer<LightInk::DataBuffer> uncompressBuff;
	LightInk::PackBuffer<LightInk::DataBuffer> xxteaBuff;
	LightInk::PackBuffer<LightInk::DataBuffer> unxxteaBuff;
	bool success = true;

	buff.compress<LightInk::CapCompresser>(compressBuff.get_buffer());
	LogMessage("pre size = %u, compress size = %u", buff.size(), compressBuff.size());
	compressBuff.uncompress<LightInk::CapCompresser>(uncompressBuff.get_buffer());
	if (buff.size() != uncompressBuff.size())
	{
		success = false;
	}
	else
	{
		char * bd = buff.data();
		char * ucbd = uncompressBuff.data();
		for (size_t i = 0; i < buff.size(); i++)
		{
			if (*bd != * ucbd)
			{
				success = false;
				break;
			}
		}
	}
	if (success)
	{
		LogMessage("compress uncompress success!!!");
	}
	else
	{
		LogMessage("compress uncompress failed!!!");
	}

	success = true;
	buff.encrypt<LightInk::XxteaEncrypter>(xxteaBuff.get_buffer(), "chendongchendong", 16);
	xxteaBuff.decrypt<LightInk::XxteaEncrypter>(unxxteaBuff.get_buffer(), "chendongchendong", 16);
	if (buff.size() != unxxteaBuff.size())
	{
		success = false;
	}
	else
	{
		char * bd = buff.data();
		char * uxbd = unxxteaBuff.data();
		for (size_t i = 0; i < buff.size(); i++)
		{
			if (*(bd + i) != *(uxbd + i))
			{
				success = false;
				break;
			}
		}
	}
	if (success)
	{
		LogMessage("encrypt decrypt success!!!");
	}
	else
	{
		LogMessage("encrypt decrypt failed!!!");
	}


	LogDebug("over...call...test_compress_encrypt()");
}