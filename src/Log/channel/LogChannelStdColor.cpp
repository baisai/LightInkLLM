

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


#include "Log/channel/LogChannelStdColor.h"
#include "Log/helper/FormatHelper.h"

namespace LightInk
{
	LogChannelStdOutColor::LogChannelStdOutColor(const string & format, uint32 level, uint32 flushLevel, LogChannelPtr channel) : 
		LogChannel(format, level, flushLevel),
		m_channel(channel)
	{
		for (int i = 0; i < LogLevel::LogMsg_Count; i++)
		{
#ifndef _WIN32
			m_colors[i] = NULL;
			m_bgColors[i] = NULL;
			m_code[i] = NULL;
#else
			m_colors[i] = 7;
			m_bgColors[i] = 0;
#endif
		}
		set_color(LogLevel::LogMsg_Trace, Color_White);
		set_color(LogLevel::LogMsg_Debug, Color_White);
		set_color(LogLevel::LogMsg_Message, Color_Green);
		set_color(LogLevel::LogMsg_Warning, Color_Yellow);
		set_color(LogLevel::LogMsg_Error, Color_Magenta);
		set_color(LogLevel::LogMsg_Fatal, Color_Red);

		set_code(LogLevel::LogMsg_Message, Code_Bold);
		set_code(LogLevel::LogMsg_Warning, Code_Bold);
		set_code(LogLevel::LogMsg_Error, Code_Bold);
		set_code(LogLevel::LogMsg_Fatal, Code_Bold);
	}

	void LogChannelStdOutColor::set_color(LogLevel::LEVEL level, Color color)
	{
		m_colors[FormatHelper::level_index(level)] = s_Code[color];
	}
	void LogChannelStdOutColor::clear_color(LogLevel::LEVEL level)
	{
#ifndef _WIN32
		m_colors[FormatHelper::level_index(level)] = NULL;
#else
		m_colors[FormatHelper::level_index(level)] = 7;
#endif
	}
	void LogChannelStdOutColor::set_bg_color(LogLevel::LEVEL level, BGColor color)
	{
		m_bgColors[FormatHelper::level_index(level)] = s_Code[color];
	}
	void LogChannelStdOutColor::clear_bg_color(LogLevel::LEVEL level)
	{
#ifndef _WIN32
		m_bgColors[FormatHelper::level_index(level)] = NULL;
#else
		m_bgColors[FormatHelper::level_index(level)] = 0;
#endif
	}
	void LogChannelStdOutColor::set_code(LogLevel::LEVEL level, Code code)
	{
#ifndef _WIN32
		m_code[FormatHelper::level_index(level)] = s_Code[code];
#endif
	}
	void LogChannelStdOutColor::clear_code(LogLevel::LEVEL level)
	{
#ifndef _WIN32
		m_code[FormatHelper::level_index(level)] = NULL;
#endif
	}

#ifdef _WIN32
	RuntimeError LogChannelStdOutColor::do_log(const LogItem & item)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), MAKEWORD(m_colors[FormatHelper::level_index(item.m_level)], m_bgColors[FormatHelper::level_index(item.m_level)]));
		RuntimeError err = m_channel->log(item);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), MAKEWORD(7, 0));
		return err;
	}
#else
	RuntimeError LogChannelStdOutColor::do_log(const LogItem & item)
	{
		LogItem i(item.m_name, item.m_level);
		i.m_threadID = item.m_threadID;
		i.m_time = item.m_time;
		const string & s = item.m_format.str();
		const char * stuffix = s_Code[Code_Reset];
		const char * prefix = m_colors[FormatHelper::level_index(i.m_level)];
		if (prefix) { i.m_format << prefix; }
		prefix = m_bgColors[FormatHelper::level_index(i.m_level)];
		if (prefix) { i.m_format << prefix; }
		prefix = m_code[FormatHelper::level_index(i.m_level)];
		if (prefix) { i.m_format << prefix; }

		i.m_format << s << stuffix;
		
		return m_channel->log(i);
	}
#endif

	RuntimeError LogChannelStdOutColor::do_flush()
	{
		return m_channel->flush(LogLevel::LogMsg_Fatal);
	}

#ifdef _WIN32
	uint16 LogChannelStdOutColor::s_Code[] = {
		0, 0, 0,
		0, 0, 0,
		0, // Code

		8, 4, 2,
		6, 1, 12,
		3, 7, //Color

		8, 4, 2,
		6, 1, 12,
		3, 7, //BG Color
	};
#else
	const char * LogChannelStdOutColor::s_Code[] = {
		"\033[00m", "\033[1m", "\033[2m",
		"\033[4m", "\033[5m", "\033[7m",
		"\033[8m", // Code

		"\033[30m", "\033[31m", "\033[32m",
		"\033[33m", "\033[34m", "\033[35m",
		"\033[36m", "\033[37m", //Color

		"\033[40m", "\033[41m", "\033[42m",
		"\033[43m", "\033[44m", "\033[45m",
		"\033[46m", "\033[47m" //BG Color
	};
#endif
}