


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


#include "Timer/TimerWheel.h"

namespace LightInk
{

	//////////////////////////////////////////////////
	//TimerNode
	/////////////////////////////////////////////////
	TimerNode::TimerNode(): m_timer(), m_pre(NULL), m_next(NULL)
	{
		LogTraceStepCall("TimerNode::TimerNode()");
		LogTraceStepReturnVoid;
	}

	TimerNode::~TimerNode()
	{
		LogTraceStepCall("TimerNode::~TimerNode()");
		release();
		LogTraceStepReturnVoid;
	}


	bool TimerNode::init(uint32 id, uint32 nextTime , uint32 interval, uint32 times, TimerCall * tc)
	{
		LogTraceStepCall("TimerNode::init(uint32 id, uint32 nextTime , uint32 interval, uint32 times, TimerCall * tc)");
		m_pre = NULL;
		m_next = NULL;
		LogTraceStepReturn(m_timer.init(id, nextTime, interval, times, tc));
	}


	bool TimerNode::release()
	{
		LogTraceStepCall("bool TimerNode::release()");
		if (m_pre)
		{
			m_pre->next(m_next);
		}
		if (m_next)
		{
			m_next->pre(m_pre);
		}
		m_pre = NULL;
		m_next = NULL;
		LogTraceStepReturn(m_timer.release());
	}


	//////////////////////////////////////////////////
	//TimerWheel
	/////////////////////////////////////////////////
	TimerWheel::TimerWheel(uint32 step): m_step(step)
	{
		LogTraceStepCall("TimerWheel::TimerWheel(uint32 step)");
		m_len = 0;
		m_cur  = 0;
		m_wheel  = NULL;
		m_next = NULL;
		LogTraceStepReturnVoid;
	}


	TimerWheel * TimerWheel::init(uint32 len, bool next)
	{
		LogTraceStepCall("TimerWheel::init(uint32 len, bool next)");
		if (m_wheel)
		{
			LogTraceStepReturn(NULL);
		}
		m_wheel = new TimerNode[len];
		for (uint32 i = 0; i < len ; ++i)
		{
			(m_wheel+i)->pre(m_wheel+i);
			(m_wheel+i)->next(m_wheel+i);
		}
		m_len = len;
		if (next)
		{
			m_next = new TimerWheel(m_step*len);
		}
		LogTraceStepReturn(m_next);
	}

	void TimerWheel::release()
	{
		LogTraceStepCall("TimerWheel::release()");
		TimerNode * node = NULL;
		TimerNode * next = NULL;
		for (size_t i = 0; i < m_len; ++i)
		{
			next = (m_wheel + i)->next();
			if (next != m_wheel + i)
			{
				next->pre(NULL);
				( m_wheel+i )->next(m_wheel+i);
				( m_wheel+i )->pre()->next(NULL);
				( m_wheel+i )->pre(m_wheel+i);
				while(next)
				{
					node = next;
					next = node ->next();
					delete node;
				}
			}
		}
		delete [] m_wheel;
		m_wheel = NULL;
		if (m_next)
		{
			delete m_next;
			m_next = NULL;
		}
		LogTraceStepReturnVoid;
	}

	TimerWheel::~ TimerWheel()
	{
		LogTraceStepCall("TimerWheel::~ TimerWheel()");
		release();
		LogTraceStepReturnVoid;
	}


	bool TimerWheel::add_timer(TimerNode * timer)
	{
		LogTraceStepCall("TimerWheel::add_timer (TimerNode * timer)");
		uint32 offset = static_cast<uint32>((timer->timer().get_next_time())/m_step);
		if(offset >= m_len)
		{
			if (m_next)
			{
				timer->timer().set_next_time(timer->timer().get_next_time()-(m_len-m_cur)*m_step);
				LogTraceStepReturn(m_next ->add_timer(timer));
			}
			LogError("Timer Is Too Long!!! Next Time Is {}", timer->timer().get_next_time());
			delete timer;
			LogTraceStepReturn(false);
		}

		timer->timer().set_next_time(timer->timer().get_next_time()-offset*m_step);
		offset = (m_cur + (offset==0?0:offset-1))% m_len;
		timer->pre(( m_wheel+offset )->pre());
		timer->pre()->next(timer);
		( m_wheel+offset )->pre( timer);
		timer->next(m_wheel+offset);
		LogTraceStepReturn(true);
	}


	TimerNode * TimerWheel::submit_timer()
	{
		LogTraceStepCall("TimerWheel::submit_timer()");
		TimerNode * next = NULL;
		TimerNode * node = NULL;
		if (m_cur == m_len)
		{
			m_cur = 0;
			if(m_next)
			{
				next = m_next ->submit_timer();
				while(next)
				{
					node = next;
					next = next ->next();
					add_timer(node);
				}
			}
		}
		node = m_wheel + m_cur;
		next = node ->next();
		if (next == node)
		{
			next = NULL;
		}
		else
		{
			node->pre()->next(NULL);
			next->pre(NULL);
			node->next(node);
			node->pre(node);
		}
		++m_cur;
		LogTraceStepReturn(next);
	}

}

