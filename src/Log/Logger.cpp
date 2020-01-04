

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

#include "Log/LogOption.h"
#include "Log/LoggerMgr.h"

namespace LightInk
{
	Logger::Logger() : m_deployed(false), m_format(new LogFormat("%+")), m_level(~0u), m_flushLevel(0u), m_err(NULL) 
	{  }

	bool Logger::set_option(const LogOption & op)
	{
		if (!m_deployed)
		{
			m_name = op.get_name();
			m_format.reset(new LogFormat(op.get_format()));
			m_level = op.get_level();
			m_flushLevel = op.get_flush_level();
			m_channel = op.get_channel_list();
		}
		return !m_deployed;
	}

	RuntimeError Logger::log(LogLevel::LEVEL level, const char * fmt, const fmt::ArgList & args)
	{
		if (!should_log(level)) { return RE_Success; }
		try
		{
			LogItem item(&m_name, level);
			item.m_msg.write(fmt, args);
			RuntimeError err = channel(item);
			if (err != RE_Success)
				return err;
			if (should_flush(level))
				return flush(level);
			return RE_Success;
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

	RuntimeError Logger::log_fl(const LogFileLine & fl, LogLevel::LEVEL level, const char * fmt, const fmt::ArgList & args)
	{
		if (!should_log(level)) { return RE_Success; }
		try
		{
			LogItem item(&m_name, level);
			item.m_fl = fl;
			item.m_msg.write(fmt, args);
			RuntimeError err = channel(item);
			if (err != RE_Success)
				return err;
			if (should_flush(level))
				return flush(level);
			return RE_Success;
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

	RuntimeError Logger::flush(LogLevel::LEVEL level)
	{
		//should_flush called in log and log_fl
		return LightInkLog->flush(this, level);
	}
	RuntimeError Logger::channel(LogItem & item)
	{
		//should_log called in log and log_fl
		return LightInkLog->channel(this, item);
	}

	void Logger::default_error_handle(const string & msg) 
	{ std::cerr << msg.c_str() << std::endl; }

	RuntimeError Logger::do_flush(LogLevel::LEVEL level) { return m_channel->flush(level); }

	RuntimeError Logger::do_channel(LogItem & item)
	{
		m_format->format(item);
		return m_channel->log(item);
	}
}