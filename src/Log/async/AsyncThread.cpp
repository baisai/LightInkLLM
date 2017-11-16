

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

#include "Log/async/AsyncThread.h"
#include "Log/LogItem.h"
#ifdef LIGHTINK_LOG_MULTITHREAD
namespace LightInk
{
	AsyncThread::AsyncThread(AsyncMsg::AsyncOverflow aof, uint32 size) : m_queue(size), m_release(false), m_overflow(aof), m_closing(false)
	{

	}

	AsyncThread::~AsyncThread() {  }
	void AsyncThread::work()
	{
		int sleep_millis = 0;
		while (true)
		{
			AsyncMsg * am = m_queue.pop();
			if (m_release && !am) { break; }
			if (am)
			{
				if (sleep_millis > 0) { sleep_millis = 0; }
				switch (am->get_type())
				{
				case AsyncMsg::Flush :
					am->get_channel()->flush();
					break;
				case AsyncMsg::Log :
					{
						LogItem item(NULL, LogLevel::LogMsg_Trace);
						am->fill_log_item(item);
						am->get_format()->format(item);
						am->get_channel()->log(item);
					}
					break;
				default:
					break;
				}
				delete am;
			}
			else
			{
				if (sleep_millis < 10) { ++sleep_millis; }
				LogSleepMillis(sleep_millis);
			}
		}
		m_closing = false;
	}
	RuntimeError AsyncThread::async_flush(const ChannelListPtr & channel)
	{
		AsyncMsg * am = new AsyncMsg(channel);
		push(am);
		return RE_Success;
	}
	RuntimeError AsyncThread::async_channel(const ChannelListPtr & channel, const LogFormatPtr & format, LogItem & item)
	{
		AsyncMsg * am = new AsyncMsg(channel, format, item);
		push(am);
		return RE_Success;
	}
	void AsyncThread::release()
	{
		m_closing = true;
		m_release = true;
		while (m_closing)
		{
			LogSleepMillis(10);
		}
	}


	void AsyncThread::push(AsyncMsg * am)
	{
		switch (m_overflow)
		{
		case AsyncMsg::Block:
			while (!m_queue.push(am))
			{
				LogSleepMillis(1);
			}
			break;
		case AsyncMsg::Discard:
			if (!m_queue.push(am))
			{
				delete am;
			}
			break;
		default:
			break;
		}
	}

}

#endif