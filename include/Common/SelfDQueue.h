


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
#ifndef LIGHTINK_COMMON_SELFDQUEUE_H_
#define LIGHTINK_COMMON_SELFDQUEUE_H_

#include "Common/Type.h"

namespace LightInk
{
	template<typename ElemType>
	class LIGHTINK_TEMPLATE_DECL SelfDQueue
	{
	public:
		class NodeType
		{
		public:
			NodeType() : m_next(NULL), m_pre(NULL) {}
			virtual ~NodeType() { remove(); }
			bool is_in_queue() const { return m_next && m_pre; }

			void next(NodeType * next) { m_next = next; }
			NodeType * next() { return m_next; }
			const NodeType * next() const { return m_next; }

			bool remove()
			{
				bool isIn = is_in_queue();
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
				return isIn;
			}

			void pre(NodeType * pre) { m_pre = pre; }
			NodeType * pre() { return m_pre; }
			const NodeType * pre() const { return m_pre; }

			void insert_list(NodeType * head, NodeType * tail)
			{
				if(m_next)
				{
					m_next->m_pre = tail;
				}
				tail->m_next = m_next;

				head->m_pre = this;
				m_next = head;
			}
			void clear_point() { m_next = NULL; m_pre = NULL; }

		private:
			NodeType * m_next;
			NodeType * m_pre;
		};
	public:
		SelfDQueue();
		~SelfDQueue();
		SelfDQueue(const SelfDQueue & cp);
		SelfDQueue & operator = (const SelfDQueue & right);

		uint32 push_front(ElemType & node);
		uint32 push_back(ElemType & node);
		uint32 remove_node(ElemType & node);
		ElemType * pop_front();
		ElemType * pop_back();
		bool pop(ElemType & node);

		ElemType * get_head();
		const ElemType * get_head() const;
		ElemType * get_tail();
		const ElemType * get_tail() const;

		ElemType * next(ElemType & node);
		const ElemType * next(const ElemType & node) const;

		ElemType * pre(ElemType & node);
		const ElemType * pre(const ElemType & node) const;
		void swap_queue(SelfDQueue & queue);

		uint32 size() const;
		bool empty() const;
		void clear();
	private:
		NodeType m_head;
		NodeType m_tail;
		uint32 m_size;
	};
}

#include "SelfDQueue.cpp"

#endif