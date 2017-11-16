

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

#ifndef LIGHTINK_LOG_LOGCHANNELSTD_H_
#define LIGHTINK_LOG_LOGCHANNELSTD_H_

#include "Log/channel/LogChannel.h"

namespace LightInk
{
	//////////////////////////////////////////////////////////////////////////////
	// LogChannelStdOut
	/////////////////////////////////////////////////////////////////////////////
	class LIGHTINK_DECL LogChannelStdOut : public LogChannel
	{
	public:
		LogChannelStdOut() {  }
		virtual ~LogChannelStdOut() {  }


	protected:
		virtual RuntimeError do_log(const LogItem & item)
		{
			size_t count = fwrite(item.m_format.data(), sizeof(char), item.m_format.size(), stdout);
			if (count == sizeof(char) * item.m_format.size()) { return RE_Success; }
			return RE_Log_WriteChannelFailed;
		}
		virtual RuntimeError do_flush()
		{
			if (fflush(stdout) == EOF) { return RE_Log_FlushChannelFailed; }
			return RE_Success;
		}


	LIGHTINK_DISABLE_COPY(LogChannelStdOut)
	};


	//////////////////////////////////////////////////////////////////////////////
	// LogChannelStdErr
	/////////////////////////////////////////////////////////////////////////////
	class LIGHTINK_DECL LogChannelStdErr : public LogChannel
	{
	public:
		LogChannelStdErr() {  }
		virtual ~LogChannelStdErr() {  }


	protected:
		virtual inline RuntimeError do_log(const LogItem & item)
		{
			size_t count = fwrite(item.m_format.data(), sizeof(char), item.m_format.size(), stderr);
			if (count == sizeof(char) * item.m_format.size()) { return RE_Success; }
			return RE_Log_WriteChannelFailed;
		}
		virtual inline RuntimeError do_flush()
		{
			if (fflush(stderr) == EOF) { return RE_Log_FlushChannelFailed; }
			return RE_Success;
		}


	LIGHTINK_DISABLE_COPY(LogChannelStdErr)
	};
}

#endif