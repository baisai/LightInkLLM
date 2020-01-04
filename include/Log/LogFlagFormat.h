

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

#ifndef LIGHTINK_LOG_LOGFLAGFORMAT_H_
#define LIGHTINK_LOG_LOGFLAGFORMAT_H_

#include "Common/Config.h"
#include "Log/helper/FormatHelper.h"
#include "Log/LogThreadType.h"
#include "Log/LogItem.h"

namespace LightInk
{
	class LogFlagFormat
	{
	public:
		virtual ~LogFlagFormat() {  }
		virtual void format(LogItem & item, const tm & t) = 0;
	};


	class LogFlagFormat_Name : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << (*item.m_name); }
	};

	class LogFlagFormat_FullLevel : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << FormatHelper::full_level_string(item.m_level); }
	};

	class LogFlagFormat_Level : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << FormatHelper::level_string(item.m_level); }
	};

	class LogFlagFormat_File : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << item.m_fl.m_file; }
	};

	class LogFlagFormat_Line : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << item.m_fl.m_line; }
	};

	class LogFlagFormat_FullWeekChar : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << FormatHelper::full_wdays[t.tm_wday]; }
	};

	class LogFlagFormat_WeekChar : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << FormatHelper::wdays[t.tm_wday]; }
	};

	class LogFlagFormat_FullMonthChar : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << FormatHelper::full_months[t.tm_mon]; }
	};

	class LogFlagFormat_MonthChar : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << FormatHelper::months[t.tm_mon]; }
	};

	//DateTime like 2016-11-30 17:12:34
	class LogFlagFormat_DateTime : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t)
		{
			FormatHelper::pad_n_join(item.m_format, t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, '-') << ' ';
			FormatHelper::pad_n_join(item.m_format, t.tm_hour, t.tm_min, t.tm_sec, ':');
		}
	};

	//DateTime like Wed Nov 30 17:12:34 2016
	class LogFlagFormat_DateTimeChar : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t)
		{
			item.m_format << FormatHelper::wdays[t.tm_wday] << ' ' << FormatHelper::months[t.tm_mon] << ' ' << t.tm_mday << ' ';
			FormatHelper::pad_n_join(item.m_format, t.tm_hour, t.tm_min, t.tm_sec, ':') << ' ' << t.tm_year + 1900;
		}
	};

	//Date like MM/DD/YY
	class LogFlagFormat_Date : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { FormatHelper::pad_n_join(item.m_format, t.tm_mon + 1, t.tm_mday, t.tm_year % 100, '/'); }
	};

	class LogFlagFormat_Year : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << fmt::pad(t.tm_year % 100, 2, '0'); }
	};

	class LogFlagFormat_FullYear : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << t.tm_year + 1900; }
	};

	class LogFlagFormat_Month : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << fmt::pad(t.tm_mon + 1, 2, '0'); }
	};

	class LogFlagFormat_Day : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << fmt::pad(t.tm_mday, 2, '0'); }
	};

	class LogFlagFormat_Hour : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << fmt::pad(t.tm_hour, 2, '0'); }
	};

	class LogFlagFormat_12Hour : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << fmt::pad(FormatHelper::to_12h(t), 2, '0'); }
	};

	class LogFlagFormat_Minute : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << fmt::pad(t.tm_min, 2, '0'); }
	};

	class LogFlagFormat_Second : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << fmt::pad(t.tm_sec, 2, '0'); }
	};

	class LogFlagFormat_MilliSecond : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << fmt::pad(static_cast<int>(item.m_time.tv_usec / 1000), 3, '0'); }
	};

	class LogFlagFormat_MicroSecond : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << fmt::pad(item.m_time.tv_usec, 6, '0'); }
	};

	class LogFlagFormat_AMPM : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << FormatHelper::am_pm(t); }
	};

	class LogFlagFormat_12HTime : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) 
		{ FormatHelper::pad_n_join(item.m_format, FormatHelper::to_12h(t), t.tm_min, t.tm_sec, ':') << ' ' << FormatHelper::am_pm(t); }
	};

	class LogFlagFormat_24HTime : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) 
		{  FormatHelper::pad_n_join(item.m_format, t.tm_hour, t.tm_min, t.tm_sec, ':'); }
	};

	class LogFlagFormat_UTCZone : public LogFlagFormat
	{
	public:
		LogFlagFormat_UTCZone() : m_lastUpdate(0), m_offset(0) {  }

		virtual void format(LogItem & item, const tm & t) 
		{
#if defined(_WIN32) || defined(sun) || defined(__sun)
			int32 minutes = get_cache_offset(item, t);
#else
			int32 minutes = OsHelper::utc_minutes_offset(t);
#endif
			 bool is_negative = minutes < 0;
			char sign;
			if (is_negative)
			{
				minutes = -minutes;
				sign = '-';
			}
			else
			{
				sign = '+';
			}

			int h = minutes / 60;
			int m = minutes % 60;
			item.m_format << sign;
			FormatHelper::pad_n_join(item.m_format, h, m, ':');
		}

	private:
		 int32 get_cache_offset(const LogItem & item, const tm & t)
		{
			m_lock.lock();
			if (item.m_time.tv_sec - m_lastUpdate >= 60)
			{
				m_offset = OsHelper::utc_minutes_offset(t);
				m_lastUpdate = item.m_time.tv_sec;
			}
			m_lock.unlock();
			return m_offset;
		}

	private:
		uint32 m_lastUpdate;
		int32 m_offset;
		LogLockAuto m_lock;

	LIGHTINK_DISABLE_COPY(LogFlagFormat_UTCZone)
	};

	class LogFlagFormat_ThreadID : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) {  item.m_format << item.m_threadID; }
	};

	class LogFlagFormat_UserInfo : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t) { item.m_format << fmt::StringRef(item.m_msg.data(), item.m_msg.size()); }
	};

	class LogFlagFormat_Char : public LogFlagFormat
	{
	public:
		explicit LogFlagFormat_Char(char ch): m_char(ch) {  }
		virtual void format(LogItem & item, const tm & t) {  item.m_format << m_char; }

	private:
		char m_char;
	};

	class LogFlagFormat_String : public LogFlagFormat
	{
	public:
		void add_char(char ch) { m_string.push_back(ch); }
		virtual void format(LogItem & item, const tm & t) {  item.m_format << m_string; }

	private:
		string m_string;
	};

	class LogFlagFormat_Full : public LogFlagFormat
	{
	public:
		virtual void format(LogItem & item, const tm & t)
		{
			item.m_format << '[' << static_cast<unsigned int>(t.tm_year + 1900) << '-'
                      << fmt::pad(static_cast<unsigned int>(t.tm_mon + 1), 2, '0') << '-'
                      << fmt::pad(static_cast<unsigned int>(t.tm_mday), 2, '0') << ' '
                      << fmt::pad(static_cast<unsigned int>(t.tm_hour), 2, '0') << ':'
                      << fmt::pad(static_cast<unsigned int>(t.tm_min), 2, '0') << ':'
                      << fmt::pad(static_cast<unsigned int>(t.tm_sec), 2, '0') << '.'
					  << fmt::pad(static_cast<unsigned int>(item.m_time.tv_usec), 6, '0') << ']'
					  << '[' << item.m_threadID << ']' << '[' << (*item.m_name) << ']'
					  << '[' << FormatHelper::full_level_string(item.m_level) << "]:"
					  << fmt::StringRef(item.m_msg.data(), item.m_msg.size())
					  << '[' << item.m_fl.m_file << '(' << item.m_fl.m_line << ')' << ']';
		}
	};

}

#endif