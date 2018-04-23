


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
#ifndef LIGHTINK_COMMON_SELFDQUEUE_CPP_
#define LIGHTINK_COMMON_SELFDQUEUE_CPP_

#include "Common/SelfDQueue.h"
#include "Common/TypeTool/TypeTool.h"

namespace LightInk
{
	template<typename ElemType, typename Allocator>
	SelfDQueue<ElemType, Allocator>::SelfDQueue() : m_head(), m_tail(), m_size(0)
	{
		m_head.next(&m_tail);
		m_tail.pre(&m_head);
	}

	template<typename ElemType, typename Allocator>
	SelfDQueue<ElemType, Allocator>::~SelfDQueue()
	{
		clear();
	}

	template<typename ElemType, typename Allocator>
	void SelfDQueue<ElemType, Allocator>::clear()
	{
		m_head.next(&m_tail);
		m_head.pre(NULL);
		m_tail.next(NULL);
		m_tail.pre(&m_head);
		m_size = 0;
	}

	template<typename ElemType, typename Allocator>
	SelfDQueue<ElemType, Allocator>::SelfDQueue(const SelfDQueue & cp) : m_head(), m_tail(), m_size(0)
	{
		static_cast<SelfDQueue &>(cp).swap_queue(*this);
	}

	template<typename ElemType, typename Allocator>
	SelfDQueue<ElemType, Allocator> & SelfDQueue<ElemType, Allocator>::operator = (const SelfDQueue & right)
	{
		static_cast<SelfDQueue &>(right).swap_queue(*this);
		return *this;
	}

	template<typename ElemType, typename Allocator>
	uint32 SelfDQueue<ElemType, Allocator>::push_front(NodeType * node)
	{
		node->next(m_head.next());
		node->pre(&m_head);
		m_head->next().pre(node);
		m_head->next(node);
		++m_size;
		return m_size;
	}


	template<typename ElemType, typename Allocator>
	uint32 SelfDQueue<ElemType, Allocator>::push_back(NodeType * node)
	{
		node->next(&m_tail);
		node->pre(m_tail.pre());
		m_tail.pre()->next(node);
		m_tail.pre(node);
		++m_size;
		return m_size;
	}

	template<typename ElemType, typename Allocator>
	uint32 SelfDQueue<ElemType, Allocator>::remove_node(NodeType * node)
	{
		if (node->remove()) { --m_size; }
		return m_size;
	}

	template<typename ElemType, typename Allocator>
	typename SelfDQueue<ElemType, Allocator>::NodeType * SelfDQueue<ElemType, Allocator>::pop_front()
	{
		NodeType * p = m_head.next();
		if (p->remove()) { --m_size; }
		return p;
	}

	template<typename ElemType, typename Allocator>
	typename SelfDQueue<ElemType, Allocator>::NodeType * SelfDQueue<ElemType, Allocator>::pop_back()
	{
		NodeType * p = m_tail.pre();
		if (p->remove()) { --m_size; }
		return p;
	}

	template<typename ElemType, typename Allocator>
	bool SelfDQueue<ElemType, Allocator>::pop(NodeType * node)
	{
		if (node->remove()) 
		{ 
			--m_size; 
			return true;
		}
		return false;
	}

	template<typename ElemType, typename Allocator>
	void SelfDQueue<ElemType, Allocator>::swap_queue(SelfDQueue & queue)
	{
		if (empty())
		{
			queue.clear();
		}
		else
		{
			queue.m_head.next(m_head.next());
			m_head.next()->pre(&queue.m_head);
			queue.m_tail.pre(m_tail.pre());
			m_tail.pre()->next(&queue.m_tail);
			queue.m_size = m_size;

			clear();
		}
	}

	template<typename ElemType, typename Allocator>
	inline uint32 SelfDQueue<ElemType, Allocator>::size()
	{
		return m_size;
	}

	template<typename ElemType, typename Allocator>
	inline bool SelfDQueue<ElemType, Allocator>::empty()
	{
		return m_head.next() == &m_tail;
	}

	template<typename ElemType, typename Allocator>
	inline bool SelfDQueue<ElemType, Allocator>::is_head(NodeType * node)
	{
		return &m_head == node;
	}

	template<typename ElemType, typename Allocator>
	inline typename SelfDQueue<ElemType, Allocator>::NodeType * SelfDQueue<ElemType, Allocator>::get_head()
	{
		return m_head.next();
	}

	template<typename ElemType, typename Allocator>
	inline bool SelfDQueue<ElemType, Allocator>::is_tail(NodeType * node)
	{
		return &m_tail == node;
	}

	template<typename ElemType, typename Allocator>
	inline typename SelfDQueue<ElemType, Allocator>::NodeType * SelfDQueue<ElemType, Allocator>::get_tail()
	{
		return m_tail.pre();
	}

}

#endif