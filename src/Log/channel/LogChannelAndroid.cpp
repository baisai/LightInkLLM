

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

#if defined(__ANDROID__)

#include "Log/channel/LogChannelAndroid.h"

namespace LightInk
{

	RuntimeError LogChannelAndroid::do_log(const LogItem & item)
	{
		RuntimeError err = RE_Success;
		if (should_log(item.m_level))
		{
			const android_LogPriority androidLevel = convert_to_android(item.m_level);
			if (__android_log_write(androidLevel, m_tag.c_str(), item.m_format.c_str()) < 0)
			{
				err = RE_Log_WriteChannelFailed;
			}
		}
		return err;
	}

	RuntimeError LogChannelAndroid::do_flush()
	{
		return RE_Success;
	}

	android_LogPriority LogChannelAndroid::convert_to_android(LogLevel::LEVEL level)
	{
		switch(level)
		{
		case LogLevel::LogMsg_Trace:
			return ANDROID_LOG_VERBOSE;
		case LogLevel::LogMsg_Debug:
			return ANDROID_LOG_DEBUG;
		case LogLevel::LogMsg_Message:
			return ANDROID_LOG_INFO;
		case LogLevel::LogMsg_Warning:
			return ANDROID_LOG_WARN;
		case LogLevel::LogMsg_Error:
			return ANDROID_LOG_ERROR;
		case LogLevel::LogMsg_Fatal:
			return ANDROID_LOG_FATAL;
		default:
			return ANDROID_LOG_DEFAULT;
		}
	}

}


#endif