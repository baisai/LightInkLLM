

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

#include <algorithm>
#include "Log/LogOption.h"

namespace LightInk
{
	LogOption::LogOption() : m_name("LightInk"), m_format("%+"), m_level(~0u), m_flushLevel(0)
	{ 
		add_flush_level(LogLevel::LogMsg_Warning);
		add_flush_level(LogLevel::LogMsg_Error);
		add_flush_level(LogLevel::LogMsg_Fatal);
	}

	LogOption::LogOption(const LogOption & cp) : m_name(cp.m_name), m_format(cp.m_format), m_level(cp.m_level), m_flushLevel(cp.m_flushLevel)
	{
		m_channelOption.assign(cp.m_channelOption.begin(), cp.m_channelOption.end());
	}
	LogOption::~LogOption()
	{
		m_channelOption.clear();
	}
	LogOption & LogOption::operator = (const LogOption & right)
	{
		m_name = right.m_name;
		m_format = right.m_format;
		m_level = right.m_level;
		m_flushLevel = right.m_flushLevel;
		m_channelOption.assign(right.m_channelOption.begin(), right.m_channelOption.end());
		return *this;
	}

	void LogOption::add_channel(ChannelOptionBase * op)
	{
		m_channelOption.push_back(ChannelOptionPtr(op));
	}
	void LogOption::remove_channel(ChannelOptionBase * op)
	{
		m_channelOption.erase(std::remove(m_channelOption.begin(), m_channelOption.end(), op));
	}
	void LogOption::clear_channel()
	{
		m_channelOption.clear();
	}
	ChannelOptionBase::ChannelListPtr LogOption::get_channel_list() const
	{
		ChannelOptionBase::ChannelListPtr p(new ChannelOptionBase::ChannelList());
		vector<ChannelOptionPtr>::type::const_iterator end = m_channelOption.end();
		for (vector<ChannelOptionPtr>::type::const_iterator i = m_channelOption.begin(); i != end; ++i)
		{
			p->add_channel((*i)->get_channel());
		}
		return p;
	}

}