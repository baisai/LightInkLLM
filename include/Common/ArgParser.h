


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

#ifndef LIGHTINK_COMMON_ARGPARSER_H_
#define LIGHTINK_COMMON_ARGPARSER_H_

#include "Common/STLType.h"

namespace LightInk
{
	struct LIGHTINK_DECL Arg
	{
		Arg() : m_index(0), m_value("") {  }
		Arg(const Arg & cp) : m_index(cp.m_index), m_option(cp.m_option), m_value(cp.m_value) {  }
		Arg & operator = (const Arg & right);
		~Arg() {  }
		uint32 m_index;
		string m_option;
		const char * m_value;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline Arg & Arg::operator = (const Arg & right) 
	{
		m_index = right.m_index; m_option = right.m_option; 
		m_value = right.m_value; return *this; 
	}

	class LIGHTINK_DECL ArgParser
	{
	public:
		ArgParser();
		~ArgParser();
		void parser(int argc, const char ** argv);
		size_t size();
		const Arg & get_arg(size_t index);
		Arg & operator[](int index);

	private:
		vector<Arg>::type m_args;
	LIGHTINK_DISABLE_COPY(ArgParser)
	};
}


#endif