

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

#ifndef LIGHTINK_LOG_LOGCHANNELMSVC_H_
#define LIGHTINK_LOG_LOGCHANNELMSVC_H_


#include "Log/channel/LogChannel.h"

namespace LightInk
{
	class LIGHTINK_DECL LogChannelMsvc : public LogChannel
	{
	public:
		LogChannelMsvc(const string & format, uint32 level, uint32 flushLevel) : 
			LogChannel(format, level, flushLevel)
		{  }
		virtual ~LogChannelMsvc() {  }

	protected:
		virtual RuntimeError do_log(const LogItem & item)
		{
#if defined(_MSC_VER)
			int len = 0;
			while (item.m_format.size() - len >= INT16_MAX)
			{
				char tmp[1024 + 1] = { 0 };
				memcpy(tmp, item.m_format.c_str() + len, sizeof(tmp) - 1);
				len += (sizeof(tmp) - 1);
				OutputDebugString(tmp);
			}
			OutputDebugString(item.m_format.c_str() + len);
#endif
			return RE_Success;
		}
		virtual RuntimeError do_flush()
		{
			return RE_Success;
		}


	LIGHTINK_DISABLE_COPY(LogChannelMsvc)
	};
}

#endif
