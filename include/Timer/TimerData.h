


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

#ifndef LIGHTINK_RULEENGINE_TIMERDATA_H_
#define LIGHTINK_RULEENGINE_TIMERDATA_H_
#include "Common/Type.h"
#include "Common/SmallObject.h"
namespace LightInk
{
	struct LIGHTINK_DECL TimerCall : public SmallObject
	{
		TimerCall() : m_id(0) {  }
		virtual ~TimerCall(){  }
		virtual void destroy() = 0;
		virtual void operator ()() = 0;
		uint32 m_id;
	};

	class LIGHTINK_DECL TimerData : public SmallObject
	{
	public:
		typedef uint64 KeyType;
		const KeyType & get_key() const;
		bool operator == (const TimerData & right) const;
	public:
		TimerData();
		TimerData(uint32 id , uint64 nextTime, uint32 interval, uint32 times, TimerCall * tc);
		~TimerData();
		bool init(uint32 id, uint64 nextTime, uint32 interval , uint32 times, TimerCall * tc);
		bool tick_out();

		bool release();
		uint32 get_id () const;
		uint64 get_next_time () const;
		void set_next_time(uint64 time);
		uint32 get_interval () const;
		uint32 get_times () const;

	protected:
		uint32 m_id;
		uint64 m_nextTime;
		uint32 m_interval;
		uint32 m_times;
		TimerCall * m_call;

	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline const TimerData::KeyType & TimerData::get_key() const { return m_nextTime; }
	inline bool TimerData::operator == (const TimerData & right) const { return m_id == right.m_id; }
	inline uint32 TimerData::get_id () const { return m_id; }
	inline uint64 TimerData::get_next_time () const { return m_nextTime; }
	inline void TimerData::set_next_time(uint64 time) { m_nextTime = time; }
	inline uint32 TimerData::get_interval () const { return m_interval; }
	inline uint32 TimerData::get_times () const { return m_times; }

}
#endif