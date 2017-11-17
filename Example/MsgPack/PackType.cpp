


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
#include "Msgpack/DataFixBuffer.h"
#include "Msgpack/DataRefBuffer.h"

void test_pack_type()
{
	LogDebug("start...call...test_pack_type()");


	LightInk::PackBuffer<LightInk::DataBuffer> dpb;
	LightInk::PackBuffer< LightInk::DataFixBuffer<10> > dfpb;
	LightInk::PackBuffer<LightInk::DataRefBuffer> drpb;


	char charFrom = 1;
	short shortFrom = 2;
	unsigned short ushortFrom = 3;
	int intFrom = 4;
	unsigned int uintFrom = 5;
	long longFrom = 6;
	unsigned ulongFrom = 7;
	long long longlongFrom = 8;
	unsigned long long ulonglongFrom = 9;
	std::string stringFrom = "chendong";

	char charTo = 0;
	short shortTo = 0;
	unsigned short ushortTo = 0;
	int intTo = 0;
	unsigned int uintTo = 0;
	long longTo = 0;
	unsigned ulongTo = 0;
	long long longlongTo = 0;
	unsigned long long ulonglongTo = 0;
	std::string stringTo;


	dpb.pack(charFrom);
	//dpb.pack(shortFrom);
	dpb.pack(ushortFrom);
	//dpb.pack(intFrom);
	dpb.pack(uintFrom);
	//dpb.pack(longFrom);
	dpb.pack(ulongFrom);
	//dpb.pack(longlongFrom);
	dpb.pack(ulonglongFrom);
	//dpb.pack(stringFrom);

	//dpb << charFrom;
	dpb << shortFrom;
	//dpb << ushortFrom;
	dpb << intFrom;
	//dpb << uintFrom;
	dpb << longFrom;
	//dpb << ulongFrom;
	dpb << longlongFrom;
	//dpb << ulonglongFrom;
	dpb << stringFrom;


	dpb.unpack(charTo);
	//dpb.unpack(shortTo);
	dpb.unpack(ushortTo);
	//dpb.unpack(intTo);
	dpb.unpack(uintTo);
	//dpb.unpack(longTo);
	dpb.unpack(ulongTo);
	//dpb.unpack(longlongTo);
	dpb.unpack(ulonglongTo);
	//dpb.unpack(stringTo);

	//dpb >> charTo;
	dpb >> shortTo;
	//dpb >> ushortTo;
	dpb >> intTo;
	//dpb >> uintTo;
	dpb >> longTo;
	//dpb >> ulongTo;
	dpb >> longlongTo;
	//dpb >> ulonglongTo;
	dpb >> stringTo;

	if (charFrom == charTo && shortFrom == shortTo && ushortFrom == ushortTo && intFrom == intTo && uintFrom == uintTo && 
		longFrom == longTo && ulongFrom == ulongTo && longlongFrom == longlongTo && ulonglongFrom == ulonglongTo && stringFrom == stringTo)
	{
		LogMessage("pack unpack type success!!!");
	}
	else
	{
		LogMessage("pack unpack type failed!!!");
	}




	charTo = 0;
	shortTo = 0;
	ushortTo = 0;
	intTo = 0;
	uintTo = 0;
	longTo = 0;
	ulongTo = 0;
	longlongTo = 0;
	ulonglongTo = 0;
	stringTo.clear();

	
	dfpb.pack(charFrom);
	dfpb.pack(shortFrom);
	dfpb.pack(ushortFrom);
	dfpb.pack(intFrom);
	dfpb.pack(uintFrom);
	dfpb.pack(longFrom);
	dfpb.pack(ulongFrom);
	dfpb.pack(longlongFrom);
	dfpb.pack(ulonglongFrom);


	drpb.pack(charFrom);
	charTo = 0;
	drpb.unpack(charTo);


	LightInk::RuntimeError err = dfpb.pack(stringFrom);
	if (err != LightInk::RE_Success)
	{
		LogError(get_runtime_error(err));
	}
	
	try
	{
		dfpb << stringFrom;
	}
	catch(LightInk::RuntimeError err)
	{
		LogError(get_runtime_error(err));
	}

	LogDebug("over...call...test_pack_type()");
}