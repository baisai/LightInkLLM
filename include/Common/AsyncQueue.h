

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

#ifndef LIGHTINK_COMMON_ASYNCQUEUE_H_
#define LIGHTINK_COMMON_ASYNCQUEUE_H_

#include "Atomic/Atomic.h"
#include "Common/SmallObject.h"

namespace LightInk
{
	template <typename T>
	class AsyncQueue : SmallObject
	{
	public:
		AsyncQueue(uint32 size);
		~AsyncQueue();

		bool empty();
		bool full();

		bool push(T * t);
		T * pop();

	private:
		uint32 real_index(uint32 idx);

	private:
		Atomic<T *> * m_queue;
		const uint32 m_size;
		Atomic<uint32> m_readIdx;
		Atomic<uint32> m_writeIdx;
	LIGHTINK_DISABLE_COPY(AsyncQueue)
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <typename T>
	AsyncQueue<T>::AsyncQueue(uint32 size) : m_size(size), m_queue(NULL)
	{
		m_queue = (Atomic<T *> *)malloc_user(size * sizeof(Atomic<T *>));
		for (uint32 i = 0; i < size; i++)
		{
			m_queue[i].store(NULL);
		}
		m_readIdx.store(0);
		m_writeIdx.store(0);
	}

	template <typename T>
	AsyncQueue<T>::~AsyncQueue() 
	{ free_user(m_queue, m_size * sizeof(Atomic<T *>)); m_queue = NULL; }

	template <typename T>
	inline bool AsyncQueue<T>::empty()
	{
		return (m_writeIdx.load() == m_readIdx.load());
	}

	template <typename T>
	inline bool AsyncQueue<T>::full()
	{
		return (real_index(m_writeIdx.load() + 1) == m_readIdx.load());
	}

	template <typename T>
	bool AsyncQueue<T>::push(T * t)
	{
		while (true)
		{
			uint32 writeIdx = m_writeIdx.load();
			uint32 nextWriteIdx = real_index(writeIdx + 1);
			if (nextWriteIdx == m_readIdx.load())
			{
				return false;
			}
			if (!m_writeIdx.compare_exchange(writeIdx, nextWriteIdx))
			{
				continue;
			}
			while (!m_queue[writeIdx].compare_exchange(NULL, t)) {  }
			return true;
		}
	}

	template <typename T>
	T * AsyncQueue<T>::pop()
	{
		while (true)
		{
			uint32 readIdx = m_readIdx.load();
			if (m_writeIdx.load() == readIdx)
			{
				return NULL;
			}
			uint32 nextReadIdx = real_index(readIdx + 1);
			if (!m_readIdx.compare_exchange(readIdx, nextReadIdx))
			{
				continue;
			}
			while (m_queue[readIdx].load() == NULL) {  }
			T * t = m_queue[readIdx].load();
			m_queue[readIdx].store(NULL);
			return t;
		}
	}

	template <typename T>
	inline uint32 AsyncQueue<T>::real_index(uint32 idx) 
	{ return (idx % m_size); }
}


#endif