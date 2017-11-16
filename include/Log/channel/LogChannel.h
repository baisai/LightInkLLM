

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

#ifndef LIGHTINK_LOG_LOGCHANNEL_H_
#define LIGHTINK_LOG_LOGCHANNEL_H_

#include "Common/RuntimeError.h"
#include "Log/LogItem.h"

namespace LightInk
{
	struct LogItem;
	class LIGHTINK_DECL LogChannel : public SmallObject
	{
	public:
		LogChannel() : m_level(~0u) {  }
		virtual ~LogChannel() {  }

		virtual RuntimeError log(const LogItem & item) { return do_log(item); }
		virtual RuntimeError flush() { return do_flush(); }

		bool should_log(LogLevel::LEVEL level);
		void add_level(LogLevel::LEVEL level);
		void remove_level(LogLevel::LEVEL level);
		void reset_level(uint32 level);

	protected:
		virtual RuntimeError do_log(const LogItem & item) = 0;
		virtual RuntimeError do_flush() = 0;

	private:
		uint32 m_level;

	LIGHTINK_DISABLE_COPY(LogChannel)
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline bool LogChannel::should_log(LogLevel::LEVEL level) { return (level & m_level) != 0;}
	inline void LogChannel::add_level(LogLevel::LEVEL level) { m_level |= level; }
	inline void LogChannel::remove_level(LogLevel::LEVEL level) { m_level &= (~level); }
	inline void LogChannel::reset_level(uint32 level) { m_level = level; }

}

#endif