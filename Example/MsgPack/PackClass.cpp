


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

#include <string>
#include <vector>
#include "Msgpack/PackDefine.h"

class PackClassBase
{
public:

	bool operator == (const PackClassBase & right)
	{
		if (m_char == right.m_char && m_short == right.m_short && m_ushort == right.m_ushort &&
			m_int == right.m_int && m_uint == right.m_uint && m_long == right.m_long && m_ulong == right.m_ulong &&
			m_longlong == right.m_longlong && m_ulonglong == right.m_ulonglong && m_string == right.m_string)
		{
			return true;
		}
		return false;
	}

	LIGHTINK_MSGPACK_DEFINE(m_char, m_short, m_ushort, m_int, m_uint, m_long, m_ulong, m_longlong, m_ulonglong, m_string, m_vec)

public:
	char m_char;
	short m_short;
	unsigned short m_ushort;
	int m_int;
	unsigned int m_uint;
	long m_long;
	unsigned m_ulong;
	long long m_longlong;
	unsigned long long m_ulonglong;
	std::string m_string;
	std::vector<char> m_vec;
};

class PackClassDiv
{
public:

	bool operator == (const PackClassDiv & right)
	{
		if (m_char == right.m_char && m_short == right.m_short && m_ushort == right.m_ushort &&
			m_int == right.m_int && m_uint == right.m_uint && m_long == right.m_long && m_ulong == right.m_ulong &&
			m_longlong == right.m_longlong && m_ulonglong == right.m_ulonglong && m_string == right.m_string &&
			m_class == right.m_class)
		{
			return true;
		}
		return false;
	}

	LIGHTINK_MSGPACK_DEFINE(m_char, m_short, m_ushort, m_int, m_uint, m_long, m_ulong, m_longlong, m_ulonglong, m_string, m_class)

public:
	char m_char;
	short m_short;
	unsigned short m_ushort;
	int m_int;
	unsigned int m_uint;
	long m_long;
	unsigned m_ulong;
	long long m_longlong;
	unsigned long long m_ulonglong;
	std::string m_string;

	PackClassBase m_class;

};


void test_pack_class()
{
	LogDebug("start...call...test_pack_class()");

	PackClassDiv pcd;
	pcd.m_char = 1;
	pcd.m_short = 2;
	pcd.m_ushort = 3;
	pcd.m_int = 4;
	pcd.m_uint = 5;
	pcd.m_long = 6;
	pcd.m_ulong = 7;
	pcd.m_longlong = 8;
	pcd.m_ulonglong = 9;
	pcd.m_string = "chen";

	pcd.m_class.m_char = 10;
	pcd.m_class.m_short = 11;
	pcd.m_class.m_ushort = 12;
	pcd.m_class.m_int = 13;
	pcd.m_class.m_uint = 14;
	pcd.m_class.m_long = 15;
	pcd.m_class.m_ulong = 16;
	pcd.m_class.m_longlong = 17;
	pcd.m_class.m_ulonglong = 18;
	pcd.m_class.m_string = "dong";



	LightInk::PackBuffer<LightInk::DataBuffer> buff;

	//pcd.pack(buff);
	buff << pcd;


	PackClassDiv pcdu;


	pcdu.unpack(buff);
	//buff >> pcdu;


	if (pcd == pcdu)
	{
		LogMessage("pack unpack class success!!!");
	}
	else
	{
		LogMessage("pack unpack class failed!!!");
	}



	LogDebug("over...call...test_pack_class()");
}