

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

#ifndef LIGHTINK_LOG_LOGTHREADTYPE_H_
#define LIGHTINK_LOG_LOGTHREADTYPE_H_

#include "Common/OsHelper.h"
#include "Common/TypeTool/TypeTool.h"
#include "Common/Ptr/SharedPtr.h"
#include "Common/Ptr/AutoPtr.h"
#include "Log/LogConfig.h"

namespace LightInk
{
#ifdef LIGHTINK_LOG_MULTITHREAD
	class LIGHTINK_DECL LogLock
	{
	public:
		LogLock();
		~LogLock();

		void lock();
		bool try_lock();
		void unlock();

	private:
#ifdef _WIN32
		CRITICAL_SECTION m_handle;
#else
		pthread_mutex_t m_handle;
#endif

	LIGHTINK_DISABLE_COPY(LogLock)
	};

	class LIGHTINK_DECL LogThread : public SmallObject
	{
	public:
		LogThread();
		virtual ~LogThread();

		int32 run();
		virtual void work();

		bool is_running();

#ifdef _WIN32
		static DWORD WINAPI work_thread(void * arg);
#else
		static void * work_thread(void * arg);
#endif

		static uint32 thread_self();

	protected:
		bool m_running;
	private:
#ifdef _WIN32
		HANDLE m_handle;
#else
		pthread_t m_handle;
#endif

	LIGHTINK_DISABLE_COPY(LogThread)
	};

#endif

	class LIGHTINK_DECL LogEmptyLock
	{
	public:
		LogEmptyLock() {}
		~LogEmptyLock() {}
		void lock() {}
		bool try_lock() { return true; }
		void unlock() {}

	};

	template <typename T>
	struct LogSharedPtrTS
	{
		typedef SharedPtrWrapper<T, RefCounterTS<SmallObject>, PtrDelStrategy, SmallObject> type;
	};
	template <typename T>
	struct LogAutoPtr
	{
		typedef AutoPtrWrapper<T, PtrDelStrategy, SmallObject> type;
	};

	template <typename T>
	struct LogSharedPtr
	{
		typedef SharedPtrWrapper<T, RefCounter<SmallObject>, PtrDelStrategy, SmallObject> type;
	};

#define LogSleepMillis turf_sleepMillis

#ifdef LIGHTINK_LOG_MULTITHREAD
#define LogSharedPtrAuto LightInk::LogSharedPtrTS
#define LogLockAuto LightInk::LogLock
#else
#define LogSharedPtrAuto LightInk::LogSharedPtr
#define LogLockAuto LightInk::LogEmptyLock
#endif
	
#define SelectSharedPtr(M, T) typename TypeSelect<IsSameType<M, LogEmptyLock>::Result, LogSharedPtr<T>::type, LogSharedPtrTS<T>::type>::Result
	
}




#endif