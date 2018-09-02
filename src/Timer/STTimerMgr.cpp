


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

#include "Timer/STTimerMgr.h"
#include "Common/OsHelper.h"

namespace LightInk
{
	//////////////////////////////////////////////////
	//STTimerMgr
	/////////////////////////////////////////////////              
	STTimerMgr::STTimerMgr(): m_step (0), m_startTime(0), m_time(), m_fixTime(0), m_preUpTime(0), m_run(false), m_wheels(NULL), m_ids(0)
	{
		LogTraceStepCall("STTimerMgr::STTimerMgr()");
		LogTraceStepReturnVoid;
	}

	bool STTimerMgr::init(uint32 step, uint32 wheel_1, uint32 wheel_2, uint32 wheel_3)
	{
		LogTraceStepCall("bool STTimerMgr::init(uint32 step, uint32 wheel_1, uint32 wheel_2, uint32 wheel_3)");
		if (m_run || m_time > 0 || m_startTime > 0)
		{
			LogTraceStepReturn(false);
		}
		m_step = step;
		bool result = init_wheel(wheel_1, wheel_2, wheel_3);
		if (!result)
		{
			LogTraceStepReturn(false);
		}
		
		timeval tv;
		OsHelper::gettimeofday(&tv);
		m_startTime = tv.tv_sec;
		m_time = tv.tv_usec / 1000;
		m_preUpTime = OsHelper::get_up_time();
		m_run = true;
		LogTraceStepReturn(true);
	}

	void STTimerMgr::release()
	{
		LogTraceStepCall("void STTimerMgr::release()");
		if (m_run)
		{
			m_run = false;
			delete m_wheels ;
			m_wheels = NULL;
			m_timers.clear();
		}
		LogTraceStepReturnVoid;
	}

	bool STTimerMgr::init_wheel(uint32 wheel_1, uint32 wheel_2 , uint32 wheel_3)//10,8,8,6
	{
		LogTraceStepCall("STTimerMgr::init_wheel(uint32 wheel_1, uint32 wheel_2 , uint32 wheel_3)");
		if (m_wheels)
		{
			LogTraceStepReturn(false);
		}
		int32 wheel_4 =  32 - (wheel_1+ wheel_2+wheel_3 );
		if (wheel_4 < 0) //最高支持位的时间
		{
			LogTraceStepReturn(false);
		}
		wheel_1 = 1 << wheel_1;
		wheel_2 = 1 << wheel_2;
		wheel_3 = 1 << wheel_3;
		wheel_4 = 1 << wheel_4;
		TimerWheel * w = new TimerWheel(m_step);
		m_wheels = w ;
		w = w ->init( wheel_1, true );
		w = w ->init( wheel_2, true );
		if (wheel_4 > 0)
		{
			w = w ->init( wheel_3, true );
			w = w ->init( wheel_4, false );
		}
		else
		{
			w = w ->init( wheel_3, false );
		}
		LogTraceStepReturn(true);

	}

	STTimerMgr::~STTimerMgr ()
	{
		LogTraceStepCall("STTimerMgr::~STTimerMgr ()");
		release();
		LogTraceStepReturnVoid;
	}

	void STTimerMgr::tick()
	{
		LogTraceStepCall("STTimerMgr::tick()");
		if(m_run)
		{
			uint64 start = OsHelper::get_up_time();
			uint32 dur = static_cast<uint32>(start - m_preUpTime) + m_fixTime;
			if (dur >= m_step) { m_fixTime = dur - m_step; }
			else { LogTraceStepReturnVoid; }

			m_preUpTime = start;
			m_time += m_step;

			TimerNode * node = NULL;
			TimerNode * next = m_wheels->submit_timer();
			while(next)
			{
				node = next ;
				next = next ->next();
				if (node->timer().tick_out())
				{
					m_wheels->add_timer(node);
				}
				else
				{
					delete node;
				}
			}
		}
		LogTraceStepReturnVoid;
	}

	uint32 STTimerMgr::add_timer(uint32 nextTime, uint32 interval, uint32 times, TimerCall * tc)
	{
		LogTraceStepCall("uint32 STTimerMgr::add_timer(uint32 nextTime, uint32 interval, uint32 times, TimerCall * tc)");
		if (!m_run || !tc) { LogTraceStepReturn(0); }
		TimerNode * t =  new TimerNode;
		uint32 id = 0;

		while (!++m_ids) { ; }
		id = m_ids;
		t->init(id, nextTime + (m_step >> 1), interval + (m_step >> 1), times, tc);
		if (m_wheels->add_timer(t))
		{
			m_timers[id] = t;
			LogTraceStepReturn(id);
		}
		delete t;
		LogTraceStepReturn(0);
	}

	uint32 STTimerMgr::add_abs_timer(double nextTime, uint32 interval, uint32 times, TimerCall * tc)
	{
		LogTraceStepCall("uint32 STTimerMgr::add_abs_timer(double nextTime, uint32 interval, uint32 times, TimerCall * tc)");
		uint64 now = get_server_milli();
		uint64 nt = static_cast<uint64>(nextTime * 1000);
		while (nt < now && times > 0)
		{
			nt += interval;
			--times;
		}
		if (nt < now)
		{
			LogTraceStepReturn(0);
		}
		LogTraceStepReturn(add_timer(static_cast<uint32>(nt - now), interval, times, tc));
	}

	uint32 STTimerMgr::add_from_run_timer(uint32 nextTime, uint32 interval, uint32 times, TimerCall * tc)
	{
		LogTraceStepCall("uint32 STTimerMgr::add_from_run_timer(uint32 nextTime, uint32 interval, uint32 times, TimerCall * tc)");
		uint32 now = m_time;
		while (nextTime < now && times > 0)
		{
			nextTime += interval;
			--times;
		}
		if (nextTime < now)
		{
			LogTraceStepReturn(0);
		}
		LogTraceStepReturn(add_timer(nextTime, interval, times, tc));
	}

	bool STTimerMgr::remove_timer(uint32 id)
	{
		LogTraceStepCall("bool STTimerMgr::remove_timer(uint32 id)");
		if (!m_run) { LogTraceStepReturn(false); }
		TimerNode * timer = NULL;
		TimersMap::iterator iter = m_timers.find(id);
		if (iter != m_timers.end())
		{
			timer = iter->second;
			m_timers.erase(iter);
		}
		if (timer)
		{
			delete timer;
			LogTraceStepReturn(true);
		}
		LogTraceStepReturn(false);
	}


}

