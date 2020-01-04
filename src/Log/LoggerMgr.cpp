

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
#include "Log/async/AsyncThread.h"
#include "Log/LoggerMgr.h"

namespace LightInk
{
	LoggerMgr::~LoggerMgr()
	{
		release();
	}
	
	RuntimeError LoggerMgr::init_async(uint32 size, AsyncMsg::AsyncOverflow aof)
	{
		if(!m_thread)
		{
			m_thread = new AsyncThread(aof, size);
			if (m_thread && m_thread->run())
				return RE_Success;
		}
		return RE_Log_LogFailed;
	}

	void LoggerMgr::release() 
	{
		if (m_thread)
		{
			m_thread->release();
			delete m_thread;
			m_thread = NULL;
		}
		{
			Guard<LogLockAuto> l(m_lock);
			m_userLogger.clear();
			for (FixedLoggerPtrMapIter i = m_fixedLogger.begin(); i != m_fixedLogger.end(); ++i)
			{
				i->second->destroy();
			}
			m_fixedLogger.clear();
		}
	}

	RuntimeError LoggerMgr::flush(Logger * logger, LogLevel::LEVEL level)
	{ 
		if (m_thread) 
			return m_thread->async_flush(logger->m_channel, level);
		return logger->do_flush(level);
	}
	RuntimeError LoggerMgr::channel(Logger * logger, LogItem & item)
	{ 
		if (m_thread)
			return m_thread->async_channel(logger->m_channel, logger->m_format, item);
		return logger->do_channel(item);
	}

	RuntimeError LoggerMgr::register_logger(LoggerPtr log)
	{
		Guard<LogLockAuto> l(m_lock);
		std::map<string, LoggerPtr>::iterator i = m_userLogger.find(log->get_name());
		if (i != m_userLogger.end())
		{
			return RE_Log_LoggerNameExist;
		}
		m_userLogger.insert(std::pair<string, LoggerPtr>(log->get_name(), log));
		return RE_Success;
	}
	RuntimeError LoggerMgr::register_logger(Logger * log)
	{
		Guard<LogLockAuto> l(m_lock);
		std::map<string, LoggerPtr>::iterator i = m_userLogger.find(log->get_name());
		if (i != m_userLogger.end())
		{
			return RE_Log_LoggerNameExist;
		}
		m_userLogger.insert(std::pair<string, LoggerPtr>(log->get_name(), LoggerPtr(log)));
		return RE_Success;
	}

	bool LoggerMgr::unregister_logger(const string & name)
	{
		Guard<LogLockAuto> l(m_lock);
		std::map<string, LoggerPtr>::iterator i = m_userLogger.find(name);
		if (i != m_userLogger.end())
		{
			m_userLogger.erase(i);
			return true;
		}
		return false;
	}
	void LoggerMgr::clear_user_logger()
	{
		Guard<LogLockAuto> l(m_lock);
		m_userLogger.clear();
	}

	LoggerPtr LoggerMgr::get_logger(const LogOption & op)
	{
		Guard<LogLockAuto> l(m_lock);
		std::map<string, LoggerPtr>::iterator i = m_userLogger.find(op.get_name());
		if (i != m_userLogger.end())
		{
			return LoggerPtr();
		}
		LoggerPtr pLog(new Logger);
		pLog->set_option(op);
		pLog->deploy();
		m_userLogger.insert(std::pair<string, LoggerPtr>(pLog->get_name(), pLog));
		return pLog;
	}
}