


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
	template<typename ElemType>
	SelfDQueue<ElemType>::SelfDQueue() : m_head(), m_tail(), m_size(0)
	{
		m_head.next(&m_tail);
		m_tail.pre(&m_head);
	}

	template<typename ElemType>
	SelfDQueue<ElemType>::~SelfDQueue()
	{
		clear();
	}

	template<typename ElemType>
	void SelfDQueue<ElemType>::clear()
	{
		if (m_head.next())
		{			m_head.next()->pre(NULL);
		}
		if (m_tail.pre())
		{
			m_tail.pre()->next(NULL);
		}
		m_head.next(&m_tail);
		m_head.pre(NULL);
		m_tail.next(NULL);
		m_tail.pre(&m_head);
		m_size = 0;
	}

	template<typename ElemType>
	SelfDQueue<ElemType>::SelfDQueue(const SelfDQueue & cp) : m_head(), m_tail(), m_size(0)
	{
		const_cast<SelfDQueue &>(cp).swap_queue(*this);
	}

	template<typename ElemType>
	SelfDQueue<ElemType> & SelfDQueue<ElemType>::operator = (const SelfDQueue & right)
	{
		const_cast<SelfDQueue &>(right).swap_queue(*this);
		return *this;
	}

	template<typename ElemType>
	uint32 SelfDQueue<ElemType>::push_front(ElemType & node)
	{
		NodeType & nt = static_cast<NodeType &>(node);
		nt.next(m_head.next());
		nt.pre(&m_head);
		m_head.next()->pre(&nt);
		m_head.next(&nt);
		++m_size;
		return m_size;
	}


	template<typename ElemType>
	uint32 SelfDQueue<ElemType>::push_back(ElemType & node)
	{
		NodeType & nt = static_cast<NodeType &>(node);
		nt.next(&m_tail);
		nt.pre(m_tail.pre());
		m_tail.pre()->next(&nt);
		m_tail.pre(&nt);
		++m_size;
		return m_size;
	}

	template<typename ElemType>
	uint32 SelfDQueue<ElemType>::remove_node(ElemType & node)
	{
		if (static_cast<NodeType &>(node).remove()) { --m_size; }
		return m_size;
	}

	template<typename ElemType>
	ElemType * SelfDQueue<ElemType>::pop_front()
	{
		ElemType * p = get_head();
		if (p && static_cast<NodeType *>(p)->remove()) { --m_size; }
		return p;
	}

	template<typename ElemType>
	ElemType * SelfDQueue<ElemType>::pop_back()
	{
		ElemType * p = get_tail();
		if (p && static_cast<NodeType *>(p)->remove()) { --m_size; }
		return p;
	}

	template<typename ElemType>
	bool SelfDQueue<ElemType>::pop(ElemType & node)
	{
		if (static_cast<NodeType &>(node).remove())
		{ 
			--m_size; 
			return true;
		}
		return false;
	}

	template<typename ElemType>
	void SelfDQueue<ElemType>::swap_queue(SelfDQueue & queue)
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

	template<typename ElemType>
	inline uint32 SelfDQueue<ElemType>::size() const
	{
		return m_size;
	}

	template<typename ElemType>
	inline bool SelfDQueue<ElemType>::empty() const
	{
		return m_head.next() == &m_tail;
	}

	template<typename ElemType>
	inline ElemType * SelfDQueue<ElemType>::get_head()
	{
		NodeType * p = m_head.next();
		return p == &m_tail ? NULL : static_cast<ElemType *>(p);
	}
	template<typename ElemType>
	inline const ElemType * SelfDQueue<ElemType>::get_head() const
	{
		const NodeType * p = m_head.next();
		return p == &m_tail ? NULL : static_cast<const ElemType *>(p);
	}

	template<typename ElemType>
	inline ElemType * SelfDQueue<ElemType>::get_tail()
	{
		NodeType * p = m_tail.pre();
		return p == &m_head ? NULL : static_cast<ElemType *>(p);
	}
	template<typename ElemType>
	inline const ElemType * SelfDQueue<ElemType>::get_tail() const
	{
		const NodeType * p = m_tail.pre();
		return p == &m_head ? NULL : static_cast<const ElemType *>(p);
	}
	template<typename ElemType>
	inline ElemType * SelfDQueue<ElemType>::next(ElemType & node) 
	{ 
		NodeType * pNext = static_cast<NodeType &>(node).next(); 
		if (pNext == &m_tail) 
		{ return NULL; } 
		return static_cast<ElemType *>(pNext); 
	}
	template<typename ElemType>
	inline const ElemType * SelfDQueue<ElemType>::next(const ElemType & node) const 
	{ 
		const NodeType * pNext = static_cast<const NodeType &>(node).next();
		if (pNext == &m_tail)
		{ return NULL; }
		return static_cast<const ElemType *>(pNext); 
	}
	template<typename ElemType>
	inline ElemType * SelfDQueue<ElemType>::pre(ElemType & node) 
	{ 
		NodeType * pPre = static_cast<NodeType &>(node).pre();
		if (pPre == &m_head)
		{ return NULL; }
		return static_cast<ElemType *>(pPre); 
	}
	template<typename ElemType>
	inline const ElemType * SelfDQueue<ElemType>::pre(const ElemType & node) const 
	{
		const NodeType * pPre = static_cast<const NodeType &>(node).pre();
		if (pPre == &m_head)
		{ return NULL; }
		return static_cast<const ElemType *>(pPre); 
	}
}

#endif