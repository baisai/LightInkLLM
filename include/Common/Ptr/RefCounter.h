


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

#ifndef LIGHTINK_COMMON_REFCOUNTER_H_
#define LIGHTINK_COMMON_REFCOUNTER_H_

#include "Atomic/Atomic.h"

namespace LightInk
{
	class LIGHTINK_TEMPLATE_DECL RefCounter
	{
	public:
		RefCounter() : m_sharedRefs(1), m_weakRefs(0) {  }
		int64 inc_shared();
		int64 dec_shared();
		int64 inc_weak();
		int64 dec_weak();
		int64 get_shared_refs() const;
		int64 get_weak_refs() const;
		bool has_refs() const;

	private:
		int64 m_sharedRefs;
		int64 m_weakRefs;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline int64 RefCounter::inc_shared() { ++m_sharedRefs; return m_sharedRefs; }

	inline int64 RefCounter::dec_shared() { if (m_sharedRefs > 0) { --m_sharedRefs; } return m_sharedRefs; }

	inline int64 RefCounter::inc_weak() { ++m_weakRefs; return m_weakRefs; }

	inline int64 RefCounter::dec_weak() { if (m_weakRefs > 0) { --m_weakRefs; } return m_weakRefs; }

	inline int64 RefCounter::get_shared_refs() const { return m_sharedRefs; }

	inline int64 RefCounter::get_weak_refs() const { return m_weakRefs; }

	inline bool RefCounter::has_refs() const { return (m_sharedRefs + m_weakRefs > 0); }


	class LIGHTINK_TEMPLATE_DECL RefCounterTS
	{
	public:
		RefCounterTS() { m_sharedRefs.store(1); m_weakRefs.store(0); }
		int64 inc_shared();
		int64 dec_shared();
		int64 inc_weak();
		int64 dec_weak();
		int64 get_shared_refs() const;
		int64 get_weak_refs() const;
		bool has_refs() const;

	private:
		Atomic<uint64> m_sharedRefs;
		Atomic<uint64> m_weakRefs;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline int64 RefCounterTS::inc_shared() 
	{ return m_sharedRefs.fetch_add(1) + 1; }

	inline int64 RefCounterTS::dec_shared() 
	{ 
		int64 r = m_sharedRefs.fetch_sub(1);
		if (r == 0) { m_sharedRefs.store(0); return r; }
		return (r - 1); 
	}

	inline int64 RefCounterTS::inc_weak() 
	{ return m_weakRefs.fetch_add(1) + 1; }

	inline int64 RefCounterTS::dec_weak()
	{
		int64 r = m_weakRefs.fetch_sub(1);
		if (r == 0) { m_weakRefs.store(0); return r; }
		return (r - 1);
	}

	inline int64 RefCounterTS::get_shared_refs() const 
	{ return m_sharedRefs.load(); }

	inline int64 RefCounterTS::get_weak_refs() const 
	{ return m_weakRefs.load(); }

	inline bool RefCounterTS::has_refs() const 
	{ return m_sharedRefs.load() + m_weakRefs.load() > 0; }
}


#endif