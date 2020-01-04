

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


#include "Log/LogItem.h"
#include "Log/channel/LogChannel.h"

namespace LightInk
{
	LogChannel::LogChannel(const string & format, uint32 level, uint32 flushLevel) :
		m_level(level), 
		m_flushLevel(flushLevel) 
	{
		if(!format.empty()) 
			m_format.reset(new LogFormat(format));
	}

	RuntimeError LogChannel::log(const LogItem & item) 
	{ 
		if (should_log(item.m_level)) 
		{
			if (m_format)
			{
				LogItem newItem(item);
				m_format->format(newItem);
				return do_log(newItem);
			}
			return do_log(item); 
		} 
		return RE_Success; 
	}

	RuntimeError LogChannel::flush(LogLevel::LEVEL level) 
	{
		if (should_flush(level)) 
			return do_flush();
		return RE_Success;
	}

}