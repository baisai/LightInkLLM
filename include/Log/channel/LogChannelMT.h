

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

#ifndef LIGHTINK_LOG_LOGCHANNELMT_H_
#define LIGHTINK_LOG_LOGCHANNELMT_H_

#include "Log/channel/LogChannel.h"
#include "Atomic/Guard.h"

namespace LightInk
{
	template <typename M>
	class LIGHTINK_TEMPLATE_DECL LogChannelMT : public LogChannel
	{
	public:
		LogChannelMT(const string & format, uint32 level, uint32 flushLevel);
		virtual ~LogChannelMT();

		virtual RuntimeError log(const LogItem & item);
		virtual RuntimeError flush(LogLevel::LEVEL level);

	protected:
		M m_lock;

	LIGHTINK_DISABLE_COPY(LogChannelMT)
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <typename M>
	LogChannelMT<M>::LogChannelMT(const string & format, uint32 level, uint32 flushLevel) :
		LogChannel(format, level, flushLevel)
	{  }

	template <typename M>
	LogChannelMT<M>::~LogChannelMT() {  }

	template <typename M>
	RuntimeError LogChannelMT<M>::log(const LogItem & item) 
	{ Guard<M> l(m_lock);return LogChannel::log(item); }

	template <typename M>
	RuntimeError LogChannelMT<M>::flush(LogLevel::LEVEL level)
	{ Guard<M> l(m_lock);return LogChannel::flush(level); }

}

#endif