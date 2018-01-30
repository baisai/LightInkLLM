


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


#include "Common/ArgParser.h"
#include <algorithm>

namespace LightInk
{
	ArgParser::ArgParser() {  }
	ArgParser::~ArgParser() {  }
	void ArgParser::parser(int argc, const char ** argv)
	{
		m_args.clear();
		Arg arg;
		int idx = 1;
		for (int i = 0; i < argc; ++i)
		{
			if (argv[i][0] == '-')
			{
				if (arg.m_index != 0)
				{
					m_args.push_back(arg);
				}
				arg.m_index = idx++;
				arg.m_option = argv[i];
				arg.m_value = "";
			}
			else
			{
				if (arg.m_index == 0)
				{
					arg.m_index = idx++;
				}
				arg.m_value = argv[i];
				m_args.push_back(arg);
				arg.m_index = 0;
				arg.m_option.clear();
				arg.m_value = "";
			}
		}
		if (arg.m_index != 0)
		{
			m_args.push_back(arg);
		}
		for (size_t i = 0; i < m_args.size(); ++i)
		{
			std::transform(m_args[i].m_option.begin(), m_args[i].m_option.end(), m_args[i].m_option.begin(), ::tolower);
		}
	}

	size_t ArgParser::size()
	{
		return m_args.size();
	}

	const Arg & ArgParser::get_arg(size_t index)
	{
		return m_args.at(index);
	}
	Arg & ArgParser::operator[](int index)
	{
		return m_args[index];
	}

}