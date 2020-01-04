

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

#ifndef LIGHTINK_LOG_ASYNCMSG_H_
#define LIGHTINK_LOG_ASYNCMSG_H_

#include "Log/channel/LogChannelList.h"
#include "Log/LogFormat.h"

namespace LightInk
{
	class AsyncMsg
	{
	public:
		enum MsgType
		{
			Log = 1,
			Flush = 2,
		};
		enum AsyncOverflow
		{
			Block = 1,
			Discard = 2,
		};

		AsyncMsg(const ChannelListPtr & channel, LogLevel::LEVEL level);
		AsyncMsg(const ChannelListPtr & channel, const LogFormatPtr & format, const LogItem & item);

		void fill_log_item(LogItem & item);

		const ChannelListPtr & get_channel() const;
		const LogFormatPtr & get_format() const;
		MsgType get_type() const;
		LogLevel::LEVEL get_level() const;

	private:
		MsgType m_type;
		const string m_name;
		size_t m_threadID;
		const string m_msg;
		LogLevel::LEVEL m_level;
		struct timeval m_time;
		LogFileLine m_fl;
		ChannelListPtr m_channel;
		LogFormatPtr m_format;

	LIGHTINK_DISABLE_COPY(AsyncMsg)
	};
}


#endif