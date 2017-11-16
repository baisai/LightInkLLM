

#include "Common/TypeTool/TypeTool.h"
#include "Common/OsHelper.h"
#include "Log/helper/FormatHelper.h"
#include "fmt/fmt.h"
#include "Log/channel/DateNameStrategy.h"

namespace LightInk
{

	//////////////////////////////////////////////////////////////////////////////
	//DateNameHour
	/////////////////////////////////////////////////////////////////////////////
	 OsHelper::FileNameType DateNameHour::calc_filename(const OsHelper::FileNameType & name, time_t t, const OsHelper::FileNameType & extName)
	{
		tm lt = OsHelper::localtime(t);
		TypeSelect<IsSameType<OsHelper::FileCharType, char>::Result, fmt::MemoryWriter, fmt::WMemoryWriter>::Result w;
		w.write(LIGHTINK_LOG_FILENAME_T("{}.{:04d}.{:02d}.{:02d}.{:02d}{}"), name, lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, lt.tm_hour, extName);
		return w.str();
	}

	time_t DateNameHour::get_next_time(time_t t)
	{
		tm lt = OsHelper::localtime(t + 3600);
		lt.tm_min = 0;
		lt.tm_sec = 0;
		return mktime(&lt);
	}



	//////////////////////////////////////////////////////////////////////////////
	//DateNameDay
	/////////////////////////////////////////////////////////////////////////////
	OsHelper::FileNameType DateNameDay::calc_filename(const OsHelper::FileNameType & name, time_t t, const OsHelper::FileNameType & extName)
	{
		tm lt = OsHelper::localtime(t);
		TypeSelect<IsSameType<OsHelper::FileCharType, char>::Result, fmt::MemoryWriter, fmt::WMemoryWriter>::Result w;
		w.write(LIGHTINK_LOG_FILENAME_T("{}.{:04d}.{:02d}.{:02d}{}"), name, lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, extName);
		return w.str();
	}

	time_t DateNameDay::get_next_time(time_t t)
	{
		tm lt = OsHelper::localtime(t + 24 * 3600);
		lt.tm_hour = 0;
		lt.tm_min = 0;
		lt.tm_sec = 0;
		return mktime(&lt);
	}


	//////////////////////////////////////////////////////////////////////////////
	//DateNameWeek
	/////////////////////////////////////////////////////////////////////////////
	OsHelper::FileNameType DateNameWeek::calc_filename(const OsHelper::FileNameType & name, time_t t, const OsHelper::FileNameType & extName)
	{
		tm lt = OsHelper::localtime(t);
		TypeSelect<IsSameType<OsHelper::FileCharType, char>::Result, fmt::MemoryWriter, fmt::WMemoryWriter>::Result w;
		w.write(LIGHTINK_LOG_FILENAME_T("{}.{:04d}.{:02d}.{:02d}.{}{}"), name, lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, FormatHelper::wdays[lt.tm_wday], extName);
		return w.str();
	}

	time_t DateNameWeek::get_next_time(time_t t)
	{
		tm lt = OsHelper::localtime(t + 7 * 24 * 3600);
		lt.tm_hour = 0;
		lt.tm_min = 0;
		lt.tm_sec = 0;
		return mktime(&lt);
	}


	//////////////////////////////////////////////////////////////////////////////
	//DateNameMonth
	/////////////////////////////////////////////////////////////////////////////
	OsHelper::FileNameType DateNameMonth::calc_filename(const OsHelper::FileNameType & name, time_t t, const OsHelper::FileNameType & extName)
	{
		tm lt = OsHelper::localtime(t);
		TypeSelect<IsSameType<OsHelper::FileCharType, char>::Result, fmt::MemoryWriter, fmt::WMemoryWriter>::Result w;
		w.write(LIGHTINK_LOG_FILENAME_T("{}.{:04d}.{:02d}{}"), name, lt.tm_year + 1900, lt.tm_mon + 1, extName);
		return w.str();
	}

	time_t DateNameMonth::get_next_time(time_t t)
	{
		tm lt = OsHelper::localtime(t + 31 * 24 * 3600);
		lt.tm_mday = 1;
		lt.tm_hour = 0;
		lt.tm_min = 0;
		lt.tm_sec = 0;
		return mktime(&lt);
	}


}