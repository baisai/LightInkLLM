

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


#include "Log/LogFormat.h"
#include "Log/LogFlagFormat.h"

namespace LightInk
{
	LogFormat::LogFormat(const string & param)
	{
		convert_format(param);
	}

	void LogFormat::format(LogItem & item)
	{
		tm t = OsHelper::localtime(item.m_time.tv_sec);
		for (vector<LogFlagFormatPtr>::type::iterator i = m_format.begin(); i != m_format.end(); ++i)
		{
			(*i)->format(item, t);
		}
		item.m_format.write(FormatHelper::endLine, FormatHelper::endLineSize);
	}


	void LogFormat::handle_flag(char flag)
	{
		switch (flag)
		{
		case 'n':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_Name()));
			break;
		case 'l':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_Level()));
			break;
		case 'L':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_FullLevel()));
			break;
		case 'w':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_Line()));
			break;
		case 'W':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_File()));
			break;
		case 't':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_ThreadID()));
			break;
		case 'u':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_UserInfo()));
			break;
		case 'a':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_WeekChar()));
			break;
		case 'A':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_FullWeekChar()));
			break;
		case 'b':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_MonthChar()));
			break;
		case 'B':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_FullMonthChar()));
			break;
		case 'c':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_DateTime()));
			break;
		case 'C':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_DateTimeChar()));
			break;
		case 'y':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_Year()));
			break;
		case 'Y':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_FullYear()));
			break;
		case 'D':
		case 'x':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_Date()));
			break;
		case 'M':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_Month()));
			break;
		case 'd':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_Day()));
			break;
		case 'h':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_12Hour()));
			break;
		case 'H':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_Hour()));
			break;
		case 'm':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_Minute()));
			break;
		case 's':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_Second()));
			break;
		case 'e':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_MilliSecond()));
			break;
		case 'f':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_MicroSecond()));
			break;
		case 'p':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_AMPM()));
			break;
		case 'r':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_12HTime()));
			break;
		case 'R':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_24HTime()));
			break;
		case 'z':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_UTCZone()));
			break;
		case '+':
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_Full()));
			break;
		default:
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_Char('%')));
			m_format.push_back(LogFlagFormatPtr(new LogFlagFormat_Char(flag)));
			break;
		}
	}

	void LogFormat::convert_format(const string & param)
	{
		LogFlagFormat_String * p = NULL;
		string::const_iterator end = param.end();
		for (string::const_iterator i = param.begin(); i != end; ++i)
		{
			if (*i == '%')
			{
				if (p) { m_format.push_back(LogFlagFormatPtr(p)); p = NULL; }
				if (++i != end) { handle_flag(*i); }
				else { break; }
			}
			else
			{
				if (!p) { p = new LogFlagFormat_String(); }
				p->add_char(*i);
			}
		}
		if (p) { m_format.push_back(LogFlagFormatPtr(p)); p = NULL; }
	}

}
