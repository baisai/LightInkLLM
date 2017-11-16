

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


#ifndef LIGHTINK_LOG_LOGITEM_H_
#define LIGHTINK_LOG_LOGITEM_H_

#include <string>
#include "Common/SmallObject.h"
#include "fmt/fmt.h"
#include "Log/LogThreadType.h"
#include "Common/OsHelper.h"
#include "Log/helper/FormatHelper.h"

namespace LightInk
{
	struct LIGHTINK_DECL LogFileLine : public SmallObject
	{
		LogFileLine() : m_file(FormatHelper::file_line), m_line(0) {  }
		LogFileLine(const char * file, uint32 line) : m_file(file), m_line(line) {  }
		const char * m_file;
		uint32 m_line;
	};
#define LightInkLogFileLine LightInk::LogFileLine(__FILE__, __LINE__)

	struct LogItem : public SmallObject
	{
		LogItem(const string * name, LogLevel::LEVEL level) : m_name(name), m_level(level), m_fl()
		{
#ifdef LIGHTINK_LOG_MULTITHREAD
			m_threadID = LogThread::thread_self();
#else
			m_threadID = 0;
#endif
			OsHelper::gettimeofday(&m_time);
		}

		const string * m_name;
		size_t m_threadID;
		fmt::MemoryWriter m_msg;
		fmt::MemoryWriter m_format;
		LogLevel::LEVEL m_level;
		struct timeval m_time;
		LogFileLine m_fl;

	LIGHTINK_DISABLE_COPY(LogItem)
	};
}


#endif