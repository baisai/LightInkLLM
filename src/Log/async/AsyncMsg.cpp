

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


#include "Log/async/AsyncMsg.h"


namespace LightInk
{
	AsyncMsg::AsyncMsg(const ChannelListPtr & channel) : m_type(Flush), m_channel(channel)
	{

	}
	AsyncMsg::AsyncMsg(const ChannelListPtr & channel, const LogFormatPtr & format, const LogItem & item) : 
		m_type(Log), m_name(*item.m_name), m_threadID(item.m_threadID), m_msg(item.m_msg.data(), item.m_msg.size()), 
		m_level(item.m_level), m_time(item.m_time), m_fl(item.m_fl), m_channel(channel), m_format(format)
	{

	}

	void AsyncMsg::fill_log_item(LogItem & item)
	{
		item.m_name = &m_name;
		item.m_threadID = m_threadID;
		item.m_msg << m_msg;
		item.m_level = m_level;
		item.m_time = m_time;
		item.m_fl = m_fl;
	}

	const ChannelListPtr & AsyncMsg::get_channel()
	{
		return m_channel;
	}
	const LogFormatPtr & AsyncMsg::get_format()
	{
		return m_format;
	}
	AsyncMsg::MsgType AsyncMsg::get_type()
	{
		return m_type;
	}

}