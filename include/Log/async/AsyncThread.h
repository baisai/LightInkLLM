

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

#ifndef LIGHTINK_LOG_ASYNCTHREAD_H_
#define LIGHTINK_LOG_ASYNCTHREAD_H_

#include "Log/LogThreadType.h"
#include "Common/AsyncQueue.h"
#include "Log/channel/LogChannelList.h"
#include "Log/LogFormat.h"
#include "Log/async/AsyncMsg.h"

namespace LightInk
{
	struct LogItem;
#ifdef LIGHTINK_LOG_MULTITHREAD
	class AsyncThread : public LogThread
	{
	public:
		AsyncThread(AsyncMsg::AsyncOverflow aof, uint32 size);
		~AsyncThread();
		void work();
		RuntimeError async_flush(const ChannelListPtr & channel);
		RuntimeError async_channel(const ChannelListPtr & channel, const LogFormatPtr & format, LogItem & item);
		void release();

	private:
		void push(AsyncMsg * am);

	private:
		AsyncQueue<AsyncMsg> m_queue;
		bool m_release;
		const AsyncMsg::AsyncOverflow m_overflow;
		bool m_closing;

	LIGHTINK_DISABLE_COPY(AsyncThread)
	};
#else
	class AsyncThread
	{
	public:
		AsyncThread(AsyncMsg::AsyncOverflow aof, uint32 size) {  }
		~AsyncThread() {  }
		void work();
		int32 run();
		RuntimeError async_flush(const ChannelListPtr & channel);
		RuntimeError async_channel(const ChannelListPtr & channel, const LogFormatPtr & format, LogItem & item);
		void release();

	LIGHTINK_DISABLE_COPY(AsyncThread)
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline void AsyncThread::work() {  }

	inline int32 AsyncThread::run() { return 0; }

	inline RuntimeError AsyncThread::async_flush(const ChannelListPtr & channel) 
	{ return channel->flush(); }

	inline RuntimeError AsyncThread::async_channel(const ChannelListPtr & channel, const LogFormatPtr & format, LogItem & item)
	{ 
		format->format(item);
		return channel->log(item);
	}

	inline void AsyncThread::release() {  }
#endif
}


#endif