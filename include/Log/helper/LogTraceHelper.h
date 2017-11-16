

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

#ifndef LIGHTINK_LOG_LOGTRACEHELPER_H_
#define LIGHTINK_LOG_LOGTRACEHELPER_H_

#include "Common/Type.h"
#include "Log/Logger.h"

namespace LightInk
{
	class LIGHTINK_DECL LogTraceHelper : public SmallObject
	{
	public:
		LogTraceHelper(const char * file, uint32 line, fmt::MemoryWriter * str) : m_file(file), m_line(line), m_str(str) 
		{ LightInkLogCpp->trace_fl(LogFileLine(file, line), "[In]:{}", fmt::StringRef(str->data(), str->size())); }
		void set_line(uint32 line);
		void log_out();
		~LogTraceHelper() { log_out(); }
	protected:
		const char * m_file;
		uint32 m_line;
		fmt::MemoryWriter * m_str;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline void LogTraceHelper::set_line(uint32 line) {m_line = line;}
	inline void LogTraceHelper::log_out()
	{ if (m_str) LightInkLogCpp->trace_fl(LogFileLine(m_file, m_line), "[Out]:{}", fmt::StringRef(m_str->data(), m_str->size())); m_str = NULL; }

}

#endif