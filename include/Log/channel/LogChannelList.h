

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

#ifndef LIGHTINK_LOG_LOGCHANNELLIST_H_
#define LIGHTINK_LOG_LOGCHANNELLIST_H_

#include "Log/channel/LogChannelMT.h"

namespace LightInk
{
	template <typename M>
	class LIGHTINK_TEMPLATE_DECL LogChannelList : public LogChannelMT<M>
	{
	public:
		typedef SelectSharedPtr(M, LogChannel) LogChannelPtr;
		typedef typename vector<LogChannelPtr>::type::iterator ListIterator;

		LogChannelList();
		virtual ~LogChannelList();

		void clear_channel();
		void add_channel(LogChannelPtr channel);
		void remove_channel(LogChannelPtr channel);

	protected:
		virtual RuntimeError do_log(const LogItem & item);
		virtual RuntimeError do_flush();

	protected:
		typename vector<LogChannelPtr>::type m_list;

	LIGHTINK_DISABLE_COPY(LogChannelList<M>)
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <typename M>
	LogChannelList<M>::LogChannelList() : 
		LogChannelMT<M>(string(), ~0u, ~0u) 
	{  }

	template <typename M>
	LogChannelList<M>::~LogChannelList() { clear_channel(); }

	template <typename M>
	void LogChannelList<M>::clear_channel() { Guard<M> l(this->m_lock); m_list.clear(); }

	template <typename M>
	void LogChannelList<M>::add_channel(LogChannelPtr channel)
	{
		if (channel)
		{
			Guard<M> l(this->m_lock);
			m_list.push_back(channel);
		}
	}

	template <typename M>
	void LogChannelList<M>::remove_channel(LogChannelPtr channel)
	{
		Guard<M> l(this->m_lock);
		m_list.erase(std::remove(m_list.begin(), m_list.end(), channel), m_list.end());
	}

	template <typename M>
	RuntimeError LogChannelList<M>::do_log(const LogItem & item)
	{
		RuntimeError err = RE_Success;
		for (ListIterator iter = m_list.begin(); iter != m_list.end(); ++iter)
		{
			if (this->should_log(item.m_level))
			{
				if (err == RE_Success) err = (*iter)->log(item);
				else (*iter)->log(item);
			}
		}
		return err;
	}

	template <typename M>
	RuntimeError LogChannelList<M>::do_flush()
	{
		RuntimeError err = RE_Success;
		for (ListIterator iter = m_list.begin(); iter != m_list.end(); ++iter)
		{
			if (err == RE_Success) err = (*iter)->flush(LogLevel::LogMsg_Fatal);
			else (*iter)->flush(LogLevel::LogMsg_Fatal);
		}
		return err;
	}

	typedef LogChannelList<LogLockAuto> ChannelList;
	typedef LogSharedPtrAuto<ChannelList>::type ChannelListPtr;
}

#endif