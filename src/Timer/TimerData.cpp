


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

#include "Timer/TimerData.h"
#include "Log/Log.h"


namespace LightInk
{
	//////////////////////////////////////////////////
	//定时器
	/////////////////////////////////////////////////
	TimerData::TimerData() : m_id(0), m_nextTime(0), m_interval(0), m_times(0), m_call(NULL)
	{
		LogTraceStepCall("TimerData::TimerData()");
		LogTraceStepReturnVoid;
	}

	TimerData::TimerData(uint32 id , uint64 nextTime , uint32 interval, uint32 times, TimerCall * tc) : m_id(id ), m_nextTime( nextTime), m_interval (interval), m_times(times), m_call(tc)
	{
		LogTraceStepCall("TimerData::TimerData(uint32 id , uint64 nextTime , uint32 interval, uint32 times)");
		tc->m_id = id;
		LogTraceStepReturnVoid;
	}


	TimerData::~TimerData()
	{
		LogTraceStepCall("TimerData::~TimerData()");
		release();
		LogTraceStepReturnVoid;
	}

	bool TimerData::init( uint32 id, uint64 nextTime, uint32 interval , uint32 times, TimerCall * tc)
	{
		LogTraceStepCall("TimerData::init( uint32 parent , uint32 id, uint64 nextTime, uint32 interval , uint32 times )");
		if(m_times > 0)
		{
			LogTraceStepReturn(false);
		}
		m_id = id ;
		m_nextTime = nextTime ;
		m_interval = interval ;
		m_times = times ;
		if (m_call)
		{
			m_call->destroy();
		}
		m_call = tc;
		tc->m_id = id;
		LogTraceStepReturn(true);
	}

	bool TimerData::tick_out()
	{
		LogTraceStepCall("TimerData::tick_out()");
		if (!m_call) { LogTraceStepReturn(false); }
		(*m_call)();
		--m_times;
		if (m_times <= 0) { LogTraceStepReturn(false); }
		m_nextTime += m_interval;
		LogTraceStepReturn(true);
	}

	bool TimerData::release()
	{
		LogTraceStepCall("bool TimerData::release()");
		m_id = 0;
		m_nextTime = 0;
		m_interval = 0;
		m_times = 0;
		if (m_call)
		{
			m_call->destroy();
			m_call = NULL;
		}
		LogTraceStepReturn(true);
	}

}