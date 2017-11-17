

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

#ifndef LIGHTINK_LOG_LOGGERMGR_H_
#define LIGHTINK_LOG_LOGGERMGR_H_

#include <map>
#include "Log/FixedLogger.h"
#include "fmt/fmt.h"
#include "Log/async/AsyncMsg.h"

namespace LightInk
{
	struct LogItem;
	class AsyncThread;
	class LIGHTINK_DECL LoggerMgr : public SmallObject
	{
	public:
		static LoggerMgr * get_instance();
		static bool init_cpp_log(const LogOption & op);
		static Logger * get_cpp_log();
		static bool init_lua_log(const LogOption & op);
		static Logger * get_lua_log();

		template <int32 Idx>
		bool init_fixed_logger(const LogOption & op);

		template <int32 Idx>
		Logger * get_fixed_logger();

	private:
		template <int32 Idx>
		FixedLogger<Idx> * get_raw_fixed_logger();

	public:
		LoggerMgr() : m_thread(NULL) {  }
		~LoggerMgr();

		RuntimeError init_async(uint32 size, AsyncMsg::AsyncOverflow aof);

		void release();

		RuntimeError register_logger(LoggerPtr log);
		RuntimeError register_logger(Logger * log);
		bool unregister_logger(const string & name);
		void clear_user_logger();

		LoggerPtr get_logger(const LogOption & op);
		
		RuntimeError flush(Logger * logger);

		RuntimeError channel(Logger * logger, LogItem & item);


	private:
		typedef map< int32, LogAutoPtr<SingletonBase>::type >::type::const_iterator FixedLoggerPtrMapIter;
		map< int32, LogAutoPtr<SingletonBase>::type >::type m_fixedLogger;
		map<string, LoggerPtr>::type m_userLogger;
		LogLockAuto m_lock;

		AsyncThread * m_thread;

	LIGHTINK_DISABLE_COPY(LoggerMgr)
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline LoggerMgr * LoggerMgr::get_instance() { static LoggerMgr s_lm; return &s_lm; }

	inline bool LoggerMgr::init_cpp_log(const LogOption & op) { return get_instance()->init_fixed_logger<1>(op); }
	inline Logger * LoggerMgr::get_cpp_log() { return get_instance()->get_fixed_logger<1>(); }
	inline bool LoggerMgr::init_lua_log(const LogOption & op) { return get_instance()->init_fixed_logger<2>(op); }
	inline Logger * LoggerMgr::get_lua_log() { return get_instance()->get_fixed_logger<2>(); }

	template <int32 Idx>
	inline bool LoggerMgr::init_fixed_logger(const LogOption & op) 
	{ 
		return get_raw_fixed_logger<Idx>()->init(op);
	}
	template <int32 Idx>
	inline Logger * LoggerMgr::get_fixed_logger() 
	{
		return get_raw_fixed_logger<Idx>()->get_logger(); 
	}


	template <int32 Idx>
	inline FixedLogger<Idx> * LoggerMgr::get_raw_fixed_logger() 
	{
		static FixedLogger<Idx> * s_fixedLogger = NULL;
		if (!s_fixedLogger)
		{
			Guard<LogLockAuto> l(m_lock);
			if (!s_fixedLogger)
			{
				s_fixedLogger = new FixedLogger<Idx>;
				s_fixedLogger->set_backup(&s_fixedLogger);
				m_fixedLogger.insert(std::pair<int32, LogAutoPtr<SingletonBase>::type>(Idx, 
					LogAutoPtr<SingletonBase>::type(s_fixedLogger)));
			}
		}
		return s_fixedLogger; 
	}
}


#define LightInkLog LightInk::LoggerMgr::get_instance()
#define LightInkLogCppInit(op) LightInk::LoggerMgr::init_cpp_log(op)
#define LightInkLogLuaInit(op) LightInk::LoggerMgr::init_lua_log(op)
#define LightInkLogCpp LightInk::LoggerMgr::get_cpp_log()
#define LightInkLogLua LightInk::LoggerMgr::get_lua_log()


#endif