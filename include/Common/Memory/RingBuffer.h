


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
#ifndef LIGHTINK_COMMON_RINGBUFFER_H_
#define LIGHTINK_COMMON_RINGBUFFER_H_

#include "Common/STLType.h"

namespace LightInk
{
	class LIGHTINK_DECL RingBuffer
	{
	public:
		RingBuffer();
		RingBuffer(uint32 size);
		~RingBuffer();

		bool init(uint32 size);

		char * get_free_block(uint32 & size);
		uint32 get_data(char * dest);
		uint32 get_data_block(char * dest, uint32 start, uint32 size);
		uint32 get_data_block(string & dest, uint32 start, uint32 size);
		uint32 get_ab_data(char ** a, uint32 & sizeA, char ** b, uint32 & sizeB);
		uint32 set_data(const char * src, uint32 size);
		uint32 get_free_size() const;
		uint32 get_data_size() const;
		uint32 get_buffer_size() const;
		bool is_full() const;
		bool is_empty() const;
		void clear();
		uint32 insert(uint32 size);
		uint32 remove(uint32 size);

		char & operator [](int32 index);

	private:
		char * m_buffer;
		uint32 m_size;
		uint32 m_readIndexA;
		uint32 m_writeIndexA;
		uint32 m_writeIndexB;

	LIGHTINK_DISABLE_COPY(RingBuffer)
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline uint32 RingBuffer::get_free_size() const { return m_size - m_writeIndexA + m_readIndexA - m_writeIndexB; }
	inline uint32 RingBuffer::get_data_size() const { return (m_writeIndexA - m_readIndexA + m_writeIndexB); }
	inline uint32 RingBuffer::get_buffer_size() const { return m_size; }
	inline bool RingBuffer::is_full() const { return (m_writeIndexA >= m_size && m_writeIndexB == m_readIndexA); }
	inline bool RingBuffer::is_empty() const { return (m_readIndexA == m_writeIndexA); }
	inline void RingBuffer::clear() { m_readIndexA = 0; m_writeIndexA = 0; m_writeIndexB = 0; }

}



#endif