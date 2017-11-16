

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


#ifndef LIGHTINK_LOG_LOGGER_H_
#define LIGHTINK_LOG_LOGGER_H_

#include <iostream>
#include "Log/channel/LogChannelList.h"
#include "Log/LogFormat.h"

namespace LightInk
{
	class LogOption;
	class LIGHTINK_DECL Logger : public SmallObject
	{
	public:
		friend class LoggerMgr;

		Logger(const LogOption & op);
		Logger(const string & name, ChannelListPtr cl);
		Logger(const string & name, ChannelList::LogChannelPtr c);
		Logger(const string & name, ChannelListPtr cl, const string & format);
		Logger(const string & name, ChannelList::LogChannelPtr c, const string & format);
		Logger(const string & name, ChannelListPtr cl, LogFormatPtr format);
		Logger(const string & name, ChannelList::LogChannelPtr c, LogFormatPtr format);
		virtual ~Logger() {  }


		RuntimeError log(LogLevel::LEVEL level, const char * fmt, const fmt::ArgList & args);
		RuntimeError trace(const char * fmt, const fmt::ArgList & args);
		RuntimeError debug(const char * fmt, const fmt::ArgList & args);
		RuntimeError message(const char * fmt, const fmt::ArgList & args);
		RuntimeError warning(const char * fmt, const fmt::ArgList & args);
		RuntimeError error(const char * fmt, const fmt::ArgList & args);
		RuntimeError fatal(const char * fmt, const fmt::ArgList & args);
		FMT_VARIADIC(RuntimeError, log, LogLevel::LEVEL, const char *)
		FMT_VARIADIC(RuntimeError, trace, const char *)
		FMT_VARIADIC(RuntimeError, debug, const char *)
		FMT_VARIADIC(RuntimeError, message, const char *)
		FMT_VARIADIC(RuntimeError, warning, const char *)
		FMT_VARIADIC(RuntimeError, error, const char *)
		FMT_VARIADIC(RuntimeError, fatal, const char *)


		RuntimeError log_fl(const LogFileLine & fl, LogLevel::LEVEL level, const char * fmt, const fmt::ArgList & args);
		RuntimeError trace_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args);
		RuntimeError debug_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args);
		RuntimeError message_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args);
		RuntimeError warning_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args);
		RuntimeError error_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args);
		RuntimeError fatal_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args);
		FMT_VARIADIC(RuntimeError, log_fl, const LogFileLine &, LogLevel::LEVEL, const char *)
		FMT_VARIADIC(RuntimeError, trace_fl, const LogFileLine &, const char *)
		FMT_VARIADIC(RuntimeError, debug_fl, const LogFileLine &, const char *)
		FMT_VARIADIC(RuntimeError, message_fl, const LogFileLine &, const char *)
		FMT_VARIADIC(RuntimeError, warning_fl, const LogFileLine &, const char *)
		FMT_VARIADIC(RuntimeError, error_fl, const LogFileLine &, const char *)
		FMT_VARIADIC(RuntimeError, fatal_fl, const LogFileLine &, const char *)


		template <typename T>
		RuntimeError log(LogLevel::LEVEL level, const T & t);

		template <typename T>
		RuntimeError trace(const T & t);
		template <typename T>
		RuntimeError debug(const T & t);
		template <typename T>
		RuntimeError message(const T & t);
		template <typename T>
		RuntimeError warning(const T & t);
		template <typename T>
		RuntimeError error(const T & t);
		template <typename T>
		RuntimeError fatal(const T & t);

		template <typename T>
		RuntimeError log_fl(const LogFileLine & fl, LogLevel::LEVEL level, const T & t);
		
		template <typename T>
		RuntimeError trace_fl(const LogFileLine & fl,const T & t);
		template <typename T>
		RuntimeError debug_fl(const LogFileLine & fl,const T & t);
		template <typename T>
		RuntimeError message_fl(const LogFileLine & fl,const T & t);
		template <typename T>
		RuntimeError warning_fl(const LogFileLine & fl,const T & t);
		template <typename T>
		RuntimeError error_fl(const LogFileLine & fl,const T & t);
		template <typename T>
		RuntimeError fatal_fl(const LogFileLine & fl,const T & t);

		RuntimeError flush();
		RuntimeError channel(LogItem & item);

		bool should_log(LogLevel::LEVEL level) const;
		bool should_flush(LogLevel::LEVEL level) const;

		void add_level(LogLevel::LEVEL level);
		void remove_level(LogLevel::LEVEL level);
		void reset_level(uint32 level);

		void add_flush_level(LogLevel::LEVEL level);
		void remove_flush_level(LogLevel::LEVEL level);
		void reset_flush_level(uint32 level);

		uint32 get_level() const;
		uint32 get_flush_level() const;

		const string & get_name() const;
		void set_error_handle(OsHelper::log_err_handle err);
		OsHelper::log_err_handle get_error_handle();

	protected:
		virtual void default_error_handle(const string & msg);
		virtual RuntimeError do_flush();
		virtual RuntimeError do_channel(LogItem & item);

	protected:
		const string m_name;
		LogFormatPtr m_format;
		uint32 m_level;
		uint32 m_flushLevel;
		OsHelper::log_err_handle m_err;
		ChannelListPtr m_channel;

	LIGHTINK_DISABLE_COPY(Logger)

	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline RuntimeError Logger::trace(const char * fmt, const fmt::ArgList & args)
	{ return log(LogLevel::LogMsg_Trace, fmt, args); }

	inline RuntimeError Logger::debug(const char * fmt, const fmt::ArgList & args) 
	{ return log(LogLevel::LogMsg_Debug, fmt, args); }

	inline RuntimeError Logger::message(const char * fmt, const fmt::ArgList & args) 
	{ return log(LogLevel::LogMsg_Message, fmt, args); }

	inline RuntimeError Logger::warning(const char * fmt, const fmt::ArgList & args) 
	{ return log(LogLevel::LogMsg_Warning, fmt, args); }

	inline RuntimeError Logger::error(const char * fmt, const fmt::ArgList & args) 
	{ return log(LogLevel::LogMsg_Error, fmt, args); }

	inline RuntimeError Logger::fatal(const char * fmt, const fmt::ArgList & args) 
	{ return log(LogLevel::LogMsg_Fatal, fmt, args); }

	inline RuntimeError Logger::trace_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args)
	{ return log_fl(fl, LogLevel::LogMsg_Trace, fmt, args); }

	inline RuntimeError Logger::debug_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args) 
	{ return log_fl(fl, LogLevel::LogMsg_Debug, fmt, args); }

	inline RuntimeError Logger::message_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args) 
	{ return log_fl(fl, LogLevel::LogMsg_Message, fmt, args); }

	inline RuntimeError Logger::warning_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args) 
	{ return log_fl(fl, LogLevel::LogMsg_Warning, fmt, args); }

	inline RuntimeError Logger::error_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args) 
	{ return log_fl(fl, LogLevel::LogMsg_Error, fmt, args); }

	inline RuntimeError Logger::fatal_fl(const LogFileLine & fl, const char * fmt, const fmt::ArgList & args) 
	{ return log_fl(fl, LogLevel::LogMsg_Fatal, fmt, args); }

	template <typename T>
	RuntimeError Logger::log(LogLevel::LEVEL level, const T & t)
	{
		if (!should_log(level)) { return RE_Success; }
		try
		{
			LogItem item(&m_name, level);
			item.m_msg << t;
			return channel(item);
		}
		catch (const std::exception & e)
		{
			if (m_err) { m_err(e.what()); }
			else { default_error_handle(e.what()); }
			return RE_Log_LogFailed;
		}
		catch (...)
		{
			if (m_err) { m_err("unknown error!!!"); }
			else { default_error_handle("unknown error!!!"); }
			return RE_Log_LogFailed;
		}
	}

	template <typename T>
	inline RuntimeError Logger::trace(const T & t) { return log(LogLevel::LogMsg_Trace, t); }
	template <typename T>
	inline RuntimeError Logger::debug(const T & t) { return log(LogLevel::LogMsg_Debug, t); }
	template <typename T>
	inline RuntimeError Logger::message(const T & t) { return log(LogLevel::LogMsg_Message, t); }
	template <typename T>
	inline RuntimeError Logger::warning(const T & t) { return log(LogLevel::LogMsg_Warning, t); }
	template <typename T>
	inline RuntimeError Logger::error(const T & t) { return log(LogLevel::LogMsg_Error, t); }
	template <typename T>
	inline RuntimeError Logger::fatal(const T & t) { return log(LogLevel::LogMsg_Fatal, t); }



	template <typename T>
	RuntimeError Logger::log_fl(const LogFileLine & fl, LogLevel::LEVEL level, const T & t)
	{
		if (!should_log(level)) { return RE_Success; }
		try
		{
			LogItem item(&m_name, level);
			item.m_fl = fl;
			item.m_msg << t;
			return channel(item);
		}
		catch (const std::exception & e)
		{
			if (m_err) { m_err(e.what()); }
			else { default_error_handle(e.what()); }
			return RE_Log_LogFailed;
		}
		catch (...)
		{
			if (m_err) { m_err("unknown error!!!"); }
			else { default_error_handle("unknown error!!!"); }
			return RE_Log_LogFailed;
		}
	}

	template <typename T>
	inline RuntimeError Logger::trace_fl(const LogFileLine & fl,const T & t) 
	{ return log_fl(fl, LogLevel::LogMsg_Trace, t); }
	
	template <typename T>
	inline RuntimeError Logger::debug_fl(const LogFileLine & fl,const T & t) 
	{ return log_fl(fl, LogLevel::LogMsg_Debug, t); }
	
	template <typename T>
	inline RuntimeError Logger::message_fl(const LogFileLine & fl,const T & t) 
	{ return log_fl(fl, LogLevel::LogMsg_Message, t); }
	
	template <typename T>
	inline RuntimeError Logger::warning_fl(const LogFileLine & fl,const T & t) 
	{ return log_fl(fl, LogLevel::LogMsg_Warning, t); }
	
	template <typename T>
	inline RuntimeError Logger::error_fl(const LogFileLine & fl,const T & t) 
	{ return log_fl(fl, LogLevel::LogMsg_Error, t); }
	
	template <typename T>
	inline RuntimeError Logger::fatal_fl(const LogFileLine & fl,const T & t) 
	{ return log_fl(fl, LogLevel::LogMsg_Fatal, t); }

	inline bool Logger::should_log(LogLevel::LEVEL level) const { return (level & m_level) != 0;}

	inline bool Logger::should_flush(LogLevel::LEVEL level) const { return (level & m_flushLevel) != 0;}

	inline void Logger::add_level(LogLevel::LEVEL level) { m_level |= level; }

	inline void Logger::remove_level(LogLevel::LEVEL level) { m_level &= (~level); }

	inline void Logger::reset_level(uint32 level) { m_level = level; }

	inline void Logger::add_flush_level(LogLevel::LEVEL level) { m_flushLevel |= level; }

	inline void Logger::remove_flush_level(LogLevel::LEVEL level) { m_flushLevel &= (~level); }

	inline void Logger::reset_flush_level(uint32 level) { m_flushLevel = level; }

	inline uint32 Logger::get_level() const { return m_level; }

	inline uint32 Logger::get_flush_level() const { return m_flushLevel; }

	inline const string & Logger::get_name() const { return m_name; }

	inline void Logger::set_error_handle(OsHelper::log_err_handle err) { m_err = err; }

	inline OsHelper::log_err_handle Logger::get_error_handle() { return m_err; }

	typedef LogSharedPtrAuto<Logger>::type LoggerPtr;

}


#endif