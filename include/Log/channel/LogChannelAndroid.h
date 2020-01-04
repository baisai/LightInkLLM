

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

#ifndef LIGHTINK_LOG_LOGCHANNELANDROID_H_
#define LIGHTINK_LOG_LOGCHANNELANDROID_H_

#if defined(__ANDROID__)

#include "Log/channel/LogChannel.h"
#include <string>
#include <android/log.h>

namespace LightInk
{
	class LIGHTINK_DECL LogChannelAndroid : public LogChannel
	{
	public:
		LogChannelAndroid(const string & format, uint32 level, uint32 flushLevel, const string & tag = "LightInk") : 
			LogChannel(format, level, flushLevel), m_tag(tag) {  }
		virtual ~LogChannelAndroid() {  }

	protected:
		virtual RuntimeError do_log(const LogItem & item);

		virtual RuntimeError do_flush();

		android_LogPriority convert_to_android(LogLevel::LEVEL level);


	protected:
		string m_tag;

	LIGHTINK_DISABLE_COPY(LogChannelAndroid)
	};
}

#endif


#endif