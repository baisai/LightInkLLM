

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

#ifndef LIGHTINK_LOG_LOGCHANNELSTDCOLOR_H_
#define LIGHTINK_LOG_LOGCHANNELSTDCOLOR_H_

#include "Log/channel/LogChannel.h"

namespace LightInk
{
	class LIGHTINK_DECL LogChannelStdOutColor : public LogChannel
	{
	public:
		typedef LogSharedPtr<LogChannel>::type LogChannelPtr;

		LogChannelStdOutColor(LogChannelPtr channel);
		virtual ~LogChannelStdOutColor() {  }

		enum Code
		{
			Code_Reset = 0, Code_Bold = 1, Code_Dark = 2,
			Code_Underline = 3, Code_Blink = 4, Code_Reverse = 5,
			Code_Concealed = 6,
		};

		enum Color
		{
			Color_Grey = 7, Color_Red = 8, Color_Green = 9,
			Color_Yellow = 10, Color_Blue = 11, Color_Magenta = 12,
			Color_Cyan = 13, Color_White = 14,
		};

		enum BGColor
		{
			BGColor_Grey = 15, BGColor_Red = 16, BGColor_Green = 17,
			BGColor_Yellow = 18, BGColor_Blue = 19, BGColor_Magenta = 20,
			BGColor_Cyan = 21, BGColor_White = 22,
		};

	protected:
#ifdef _WIN32
		static uint16 s_Code[];
#else
		static const char * s_Code[];
#endif
	public:
		void set_color(LogLevel::LEVEL level, Color color);
		void clear_color(LogLevel::LEVEL level);
		void set_bg_color(LogLevel::LEVEL level, BGColor color);
		void clear_bg_color(LogLevel::LEVEL level);
		void set_code(LogLevel::LEVEL level, Code code);
		void clear_code(LogLevel::LEVEL level);

	protected:
		virtual RuntimeError do_log(const LogItem & item);

		virtual RuntimeError do_flush();

		LogChannelPtr m_channel;
#ifdef _WIN32
		uint16 m_colors[LogLevel::LogMsg_Count];
		uint16 m_bgColors[LogLevel::LogMsg_Count];
#else
		const char * m_colors[LogLevel::LogMsg_Count];
		const char * m_bgColors[LogLevel::LogMsg_Count];
		const char * m_code[LogLevel::LogMsg_Count];
#endif
	LIGHTINK_DISABLE_COPY(LogChannelStdOutColor)
	};

}

#endif