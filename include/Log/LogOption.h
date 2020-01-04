

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


#ifndef LIGHTINK_LOG_LOGOPTION_H_
#define LIGHTINK_LOG_LOGOPTION_H_

#include "Common/Config.h"
#include <string>
#include <vector>
#include "Log/LogConfig.h"
#include "Log/LogFormat.h"
#include "Log/channel/LogChannelNull.h"
#include "Log/channel/LogChannelFile.h"
#include "Log/channel/LogChannelStd.h"
#include "Log/channel/LogChannelStdColor.h"
#include "Log/channel/LogChannelMsvc.h"
#include "Log/channel/LogChannelAndroid.h"
#include "Log/channel/DateNameStrategy.h"
#include "Log/channel/LogChannelList.h"


namespace LightInk
{
	namespace LogChannelType
	{
		enum TYPE
		{
			NullChannel = 0,
			FileChannel = 1,
			BlockFileChannel = 2,
			DateFileChannel = 3,
			StdOutChannel = 4,
			StdErrChannel = 5,
			MsvcChannel = 6,
			AndroidChannel = 7,
			CustomChannel = 8,
		};
	}

	class LIGHTINK_DECL ChannelOptionBase
	{
	public:
		typedef LogChannelList<LogLockAuto> ChannelList;
		typedef LogSharedPtrAuto<ChannelList>::type ChannelListPtr;

		ChannelOptionBase() : m_level(~0u), m_flushLevel(~0u) { ; }
		virtual ~ChannelOptionBase() { ; }

		void set_format(const string & format);
		void add_level(LogLevel::LEVEL level);
		void remove_level(LogLevel::LEVEL level);
		void reset_level(uint32 level);

		void add_flush_level(LogLevel::LEVEL level);
		void remove_flush_level(LogLevel::LEVEL level);
		void reset_flush_level(uint32 level);

		virtual ChannelList::LogChannelPtr get_channel() = 0;

	protected:
		ChannelList::LogChannelPtr m_channel;
		string m_format;
		uint32 m_level;
		uint32 m_flushLevel;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline void ChannelOptionBase::set_format(const string & format) { m_format = format; }
	inline void ChannelOptionBase::add_level(LogLevel::LEVEL level) { m_level |= level; }
	inline void ChannelOptionBase::remove_level(LogLevel::LEVEL level) { m_level &= (~level); }
	inline void ChannelOptionBase::reset_level(uint32 level) { m_level = level; }
	inline void ChannelOptionBase::add_flush_level(LogLevel::LEVEL level) { m_flushLevel |= level; }
	inline void ChannelOptionBase::remove_flush_level(LogLevel::LEVEL level) { m_flushLevel &= (~level); }
	inline void ChannelOptionBase::reset_flush_level(uint32 level) { m_flushLevel = level; }

	typedef LogAutoPtr<ChannelOptionBase>::type ChannelOptionPtr;

	template <typename M, LogChannelType::TYPE ct>
	class LIGHTINK_TEMPLATE_DECL ChannelOption;


	template <typename M>
	class LIGHTINK_TEMPLATE_DECL ChannelOption<M, LogChannelType::NullChannel> : public ChannelOptionBase
	{
	public:
		ChannelList::LogChannelPtr get_channel();
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <typename M>
	ChannelList::LogChannelPtr ChannelOption<M, LogChannelType::NullChannel>::get_channel() 
	{ if (!m_channel) { m_channel.reset(new LogChannelNull(m_format, m_level, m_flushLevel)); } return m_channel; }


	template <typename M>
	class LIGHTINK_TEMPLATE_DECL ChannelOption<M, LogChannelType::FileChannel> : public ChannelOptionBase
	{
	public:
		ChannelOption() : m_cover(false) {  }
		ChannelList::LogChannelPtr get_channel();

		void set_filename(const OsHelper::FileNameType & name);
		void set_cover(bool cover);

	protected:
		OsHelper::FileNameType m_file;
		bool m_cover;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <typename M>
	inline ChannelList::LogChannelPtr ChannelOption<M, LogChannelType::FileChannel>::get_channel() 
	{ if (!m_channel) { m_channel.reset(new LogChannelFile<M>(m_format, m_level, m_flushLevel, m_file, m_cover)); } return m_channel; }
	template <typename M>
	inline void ChannelOption<M, LogChannelType::FileChannel>::set_filename(const OsHelper::FileNameType & name) 
	{ if (m_channel) { m_channel.reset(); } m_file = name; }
	template <typename M>
	inline void ChannelOption<M, LogChannelType::FileChannel>::set_cover(bool cover)
	{ if (m_channel) { m_channel.reset(); } m_cover = cover; }



	template <typename M>
	class LIGHTINK_TEMPLATE_DECL ChannelOption<M, LogChannelType::BlockFileChannel> : public ChannelOptionBase
	{
	public:
		ChannelOption() : m_maxSize(1024*1024), m_maxFiles(10) {  }
		ChannelList::LogChannelPtr get_channel();

		void set_filename(const OsHelper::FileNameType & name);
		void set_max_size(size_t s);
		void set_max_files(size_t fs);

	protected:
		OsHelper::FileNameType m_file;
		size_t m_maxSize;
		size_t m_maxFiles;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <typename M>
	inline ChannelList::LogChannelPtr ChannelOption<M, LogChannelType::BlockFileChannel>::get_channel() 
	{ if (!m_channel) { m_channel.reset(new LogChannelBlockFile<M>(m_format, m_level, m_flushLevel, m_file, m_maxSize, m_maxFiles)); } return m_channel; }
	template <typename M>
	inline void ChannelOption<M, LogChannelType::BlockFileChannel>::set_filename(const OsHelper::FileNameType & name) 
	{ if (m_channel) { m_channel.reset(); } m_file = name; }
	template <typename M>
	inline void ChannelOption<M, LogChannelType::BlockFileChannel>::set_max_size(size_t s) 
	{ if (m_channel) { m_channel.reset(); } m_maxSize = s; }
	template <typename M>
	inline void ChannelOption<M, LogChannelType::BlockFileChannel>::set_max_files(size_t fs) 
	{ if (m_channel) { m_channel.reset(); } m_maxFiles = fs; }



	template <typename M>
	class LIGHTINK_TEMPLATE_DECL ChannelOption<M, LogChannelType::DateFileChannel> : public ChannelOptionBase
	{
	public:
		ChannelOption() : m_hours(1) {  }
		ChannelList::LogChannelPtr get_channel();

		void set_filename(const OsHelper::FileNameType & name);
		void set_hours(size_t hs);

	protected:
		OsHelper::FileNameType m_file;
		size_t m_hours;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <typename M>
	inline ChannelList::LogChannelPtr ChannelOption<M, LogChannelType::DateFileChannel>::get_channel() 
	{
		if (!m_channel)
		{
			switch (m_hours)
			{
			case 1:
				m_channel.reset(new LogChannelDateFile<M, DateNameHour>(m_format, m_level, m_flushLevel, m_file));
				break;
			case 24:
				m_channel.reset(new LogChannelDateFile<M, DateNameDay>(m_format, m_level, m_flushLevel, m_file));
				break;
			case 7*24:
				m_channel.reset(new LogChannelDateFile<M, DateNameWeek>(m_format, m_level, m_flushLevel, m_file));
				break;
			case 30 * 24:
			default:
				m_channel.reset(new LogChannelDateFile<M, DateNameMonth>(m_format, m_level, m_flushLevel, m_file));
			}
		}
		return m_channel;
	}
	template <typename M>
	inline void ChannelOption<M, LogChannelType::DateFileChannel>::set_filename(const OsHelper::FileNameType & name) 
	{ if (m_channel) { m_channel.reset(); } m_file = name; }
	template <typename M>
	inline void ChannelOption<M, LogChannelType::DateFileChannel>::set_hours(size_t hs) 
	{ if (m_channel) { m_channel.reset(); } m_hours = hs; }


	template <typename M>
	class LIGHTINK_TEMPLATE_DECL ChannelOption<M, LogChannelType::StdOutChannel> : public ChannelOptionBase
	{
	public:
		ChannelOption() : m_color(false) {  }
		ChannelList::LogChannelPtr get_channel();

		void set_color(bool color);

	protected:
		bool m_color;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <typename M>
	inline ChannelList::LogChannelPtr ChannelOption<M, LogChannelType::StdOutChannel>::get_channel()
	{ 
		if (!m_channel)
		{
			if (m_color) 
			{
				LogSharedPtr<LogChannelStdOut>::type real(new LogChannelStdOut(string(), ~0u, ~0u));
				m_channel.reset(new LogChannelStdOutColor(m_format, m_level, m_flushLevel, real)); 
			}
			else 
			{ m_channel.reset(new LogChannelStdOut(m_format, m_level, m_flushLevel)); }
		}
		return m_channel;
	}
	template <typename M>
	inline void ChannelOption<M, LogChannelType::StdOutChannel>::set_color(bool color) 
	{ if (m_channel) { m_channel.reset(); } m_color = color; }



	template <typename M>
	class LIGHTINK_TEMPLATE_DECL ChannelOption<M, LogChannelType::StdErrChannel> : public ChannelOptionBase
	{
	public:
		ChannelOption() : m_color(false) {  }
		ChannelList::LogChannelPtr get_channel();

		void set_color(bool color);

	protected:
		bool m_color;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <typename M>
	inline ChannelList::LogChannelPtr ChannelOption<M, LogChannelType::StdErrChannel>::get_channel()
	{ 
		if (!m_channel)
		{
			if (m_color) 
			{ 
				LogSharedPtr<LogChannelStdErr>::type real(new LogChannelStdErr(string(), ~0u, ~0u));
				m_channel.reset(new LogChannelStdOutColor(m_format, m_level, m_flushLevel, real)); 
			}
			else 
			{ m_channel.reset(new LogChannelStdErr(m_format, m_level, m_flushLevel)); }
		}
		
		return m_channel;
	}
	template <typename M>
	inline void ChannelOption<M, LogChannelType::StdErrChannel>::set_color(bool color) 
	{ if (m_channel) { m_channel.reset(); } m_color = color; }

	template <typename M>
	class LIGHTINK_TEMPLATE_DECL ChannelOption<M, LogChannelType::MsvcChannel> : public ChannelOptionBase
	{
	public:
		ChannelList::LogChannelPtr get_channel();
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <typename M>
	inline ChannelList::LogChannelPtr ChannelOption<M, LogChannelType::MsvcChannel>::get_channel() 
	{ if (!m_channel) { m_channel.reset(new LogChannelMsvc(m_format, m_level, m_flushLevel)); } return m_channel; }

#if defined(__ANDROID__)
	template <typename M>
	class LIGHTINK_TEMPLATE_DECL ChannelOption<M, LogChannelType::AndroidChannel> : public ChannelOptionBase
	{
	public:
		ChannelList::LogChannelPtr get_channel();

		void set_tag(const std::string & tag);

    protected:
	    std::string m_tag;

	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <typename M>
	inline ChannelList::LogChannelPtr ChannelOption<M, LogChannelType::AndroidChannel>::get_channel()
	{ if (!m_channel) { m_channel.reset(new LogChannelAndroid(m_format, m_level, m_flushLevel, m_tag)); } return m_channel; }

	template <typename M>
    inline void ChannelOption<M, LogChannelType::AndroidChannel>::set_tag(const std::string & tag)
    { if (m_channel) { m_channel.reset(); } m_tag = tag; }
#endif

	template <typename M>
	class LIGHTINK_TEMPLATE_DECL ChannelOption<M, LogChannelType::CustomChannel> : public ChannelOptionBase
	{
	public:
		ChannelOption(const ChannelList::LogChannelPtr & channel) { m_channel = channel; }
		ChannelList::LogChannelPtr get_channel();

	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <typename M>
	inline ChannelList::LogChannelPtr ChannelOption<M, LogChannelType::CustomChannel>::get_channel() 
	{ return m_channel; }

	class LIGHTINK_DECL LogOption
	{
	public:
		LogOption();
		LogOption(const LogOption & cp);
		~LogOption();
		LogOption & operator = (const LogOption & right);

		void set_name(const string & name);
		const string & get_name() const;

		void set_format(const string & format);
		const string & get_format() const;

		void add_level(LogLevel::LEVEL level);
		void remove_level(LogLevel::LEVEL level);
		void reset_level(uint32 level);
		uint32 get_level() const;

		void add_flush_level(LogLevel::LEVEL level);
		void remove_flush_level(LogLevel::LEVEL level);
		void reset_flush_level(uint32 level);
		uint32 get_flush_level() const;

		void add_channel(ChannelOptionBase * op);
		void remove_channel(ChannelOptionBase * op);
		void clear_channel();
		ChannelOptionBase::ChannelListPtr get_channel_list() const;

	protected:
		string m_name;
		vector<ChannelOptionPtr>::type m_channelOption;
		string m_format;
		uint32 m_level;
		uint32 m_flushLevel;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline void LogOption::set_name(const string & name) { m_name = name; }
	inline const string & LogOption::get_name() const { return m_name; }
	inline void LogOption::set_format(const string & format) { m_format = format; }
	inline const string & LogOption::get_format() const { return m_format; }
	inline void LogOption::add_level(LogLevel::LEVEL level) { m_level |= level; }
	inline void LogOption::remove_level(LogLevel::LEVEL level) { m_level &= (~level); }
	inline void LogOption::reset_level(uint32 level) { m_level = level; }
	inline uint32 LogOption::get_level() const { return m_level; }
	inline void LogOption::add_flush_level(LogLevel::LEVEL level) { m_flushLevel |= level; }
	inline void LogOption::remove_flush_level(LogLevel::LEVEL level) { m_flushLevel &= (~level); }
	inline void LogOption::reset_flush_level(uint32 level) { m_flushLevel = level; }
	inline uint32 LogOption::get_flush_level() const { return m_flushLevel; }
}


#endif