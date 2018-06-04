


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

#ifndef LIGHTINK_RULEENGINE_STTIMERMGR_H_
#define LIGHTINK_RULEENGINE_STTIMERMGR_H_

#include <unordered_map>
#include "Log/Log.h"
#include "Timer/TimerWheel.h"

namespace LightInk
{
	class LIGHTINK_DECL STTimerMgr: public SmallObject
	{
	public:
		typedef std::unordered_map<uint32, TimerNode *> TimersMap;
	public:
		STTimerMgr();

		bool init(uint32 step = 10, uint32 wheel_1 = 10, uint32 wheel_2 = 8, uint32 wheel_3 = 8);//10,8,8,6
		void release();

		virtual void tick();
		virtual ~STTimerMgr();

		uint32 add_timer(uint32 nextTime, uint32 interval, uint32 times, TimerCall * tc);
		uint32 add_abs_timer(double nextTime, uint32 interval, uint32 times, TimerCall * tc);
		uint32 add_from_run_timer(uint32 nextTime, uint32 interval, uint32 times, TimerCall * tc);
		bool remove_timer(uint32 id);

		uint64 get_server_milli();
		double get_server_second();
		uint32 get_server_run_milli();

	protected:
		bool init_wheel(uint32 wheel_1 = 10, uint32 wheel_2 = 8, uint32 wheel_3 = 8);

	protected:
		uint32 m_step ;
		uint32 m_startTime;
		uint32 m_time;
		uint32 m_fixTime;
		uint64 m_preUpTime;

		bool m_run;
		
		TimerWheel * m_wheels ;

		TimersMap m_timers;
		uint32 m_ids;
	
	LIGHTINK_DISABLE_COPY(STTimerMgr)
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline uint64 STTimerMgr::get_server_milli() { uint64 t = m_startTime; t = t * 1000 + m_time; return t; }
	inline double STTimerMgr::get_server_second() { double t = m_time; t = t / 1000.0 + m_startTime; return t; }
	inline uint32 STTimerMgr::get_server_run_milli() { return m_time; }
}


#endif
