

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

#include "Log/LogThreadType.h"

namespace LightInk
{
//////////////////////////////////////////////////////////////////////////////////////////////
//LogLock
//////////////////////////////////////////////////////////////////////////////////////////////
	LogLock::LogLock()
	{
#ifdef _WIN32
		InitializeCriticalSection(&m_handle);
#else
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&m_handle, &attr);
#endif
	}

	LogLock::~LogLock()
	{
#ifdef _WIN32
		DeleteCriticalSection(&m_handle);
#else
		pthread_mutex_destroy(&m_handle);
#endif
	}

	void LogLock::lock()
	{
#ifdef _WIN32
		EnterCriticalSection(&m_handle);
#else
		pthread_mutex_lock(&m_handle);
#endif
	}

	bool LogLock::try_lock()
	{
#ifdef _WIN32
		return TryEnterCriticalSection(&m_handle) != FALSE;
#else
		return pthread_mutex_trylock(&m_handle) == 0;
#endif
	}

	void LogLock::unlock()
	{
#ifdef _WIN32
		LeaveCriticalSection(&m_handle);
#else
		pthread_mutex_unlock(&m_handle);
#endif
	}

//////////////////////////////////////////////////////////////////////////////////////////////
//LogThread
//////////////////////////////////////////////////////////////////////////////////////////////
	LogThread::LogThread() : m_threadID(0u)
	{

	}

	LogThread::~LogThread()
	{

	}

	bool LogThread::run()
	{
		if (is_running())
			return true;

#ifdef _WIN32
		m_handle = CreateThread(0, 0, work_thread, this, 0, 0);
		return m_handle != NULL;
#else
		pthread_attr_t type;
		pthread_attr_init(&type);
		pthread_attr_setdetachstate(&type, PTHREAD_CREATE_JOINABLE);
		return pthread_create(&m_handle, &type, work_thread, this) == 0;
#endif
	}

	void LogThread::work()
	{
#ifndef _WIN32
		pthread_detach(thread_self());
#endif
	}

	bool LogThread::is_running()
	{
		return m_threadID != 0u;
	}

	bool LogThread::join()
	{
		if (!is_running() || m_threadID == thread_self())
			return false;
#ifdef _WIN32
		return !WaitForSingleObject(m_handle, UINT_MAX);
#else
		return !pthread_join(m_handle, NULL);
#endif
	}

#ifdef _WIN32
	DWORD WINAPI LogThread::work_thread(void * arg)
	{
		LogThread * t = (LogThread *) arg;
		t->m_threadID = thread_self();
		t->work();
		t->m_threadID = 0u;
		return 0;
	}
#else
	void * LogThread::work_thread(void * arg)
	{
		LogThread * t = (LogThread *) arg;
		t->m_threadID = thread_self();
		t->work();
		t->m_threadID = 0u;
		pthread_exit(NULL);
		return NULL;
	}
#endif

	size_t LogThread::thread_self()
	{
#ifdef _WIN32
		return GetCurrentThreadId();
#else
		return pthread_self();
#endif
	}
}