


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

#ifndef LIGHTINK_RULEENGINE_TIMERWHEEL_H_
#define LIGHTINK_RULEENGINE_TIMERWHEEL_H_

#include "Log/Log.h"
#include "Timer/TimerData.h"

namespace LightInk
{
	class LIGHTINK_DECL TimerNode : public SmallObject
	{
	public:
		TimerNode();
		~ TimerNode();
		bool init (uint32 id, uint32 nextTime , uint32 interval, uint32 times, TimerCall * tc);

		bool release();

		TimerNode * pre();

		void pre( TimerNode * pre );

		TimerNode * next();

		void next( TimerNode * next );

		TimerData & timer();

	protected:
		TimerData m_timer ;
		TimerNode * m_pre ;
		TimerNode * m_next ;

	LIGHTINK_DISABLE_COPY(TimerNode)
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline TimerNode * TimerNode::pre() { return m_pre; }
	inline void TimerNode::pre( TimerNode * pre ) { m_pre = pre; }
	inline TimerNode * TimerNode::next() { return m_next; }
	inline void TimerNode::next( TimerNode * next ) { m_next = next; }
	inline TimerData & TimerNode::timer() { return m_timer; }

	class TimerWheel: public SmallObject
		{
		public:
			TimerWheel(uint32 step);
			TimerWheel * init(uint32 len, bool next);
			void release();
			~ TimerWheel();

			bool add_timer(TimerNode * timer);

			//指向下一个tick并提交
			TimerNode* submit_timer();

			uint32 get_len();
			uint32 get_cur();
			uint32 get_step() ;

		protected:
			uint32 m_len ; //本轮长度
			uint32 m_cur ; //轮转到第几个刻度了
			uint32 m_step ; //一个刻度的时间

			TimerNode * m_wheel ; //钟盘

			TimerWheel * m_next ; //下一个钟盘

		LIGHTINK_DISABLE_COPY(TimerWheel)
		};
	
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline uint32 TimerWheel::get_len() { return m_len; }
	inline uint32 TimerWheel::get_cur() { return m_cur; }
	inline uint32 TimerWheel::get_step() { return m_step; }

}


#endif
