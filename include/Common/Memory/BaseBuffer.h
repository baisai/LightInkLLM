


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
#ifndef LIGHTINK_COMMON_BASEBUFFER_H_
#define LIGHTINK_COMMON_BASEBUFFER_H_

#include "Common/Type.h"

namespace LightInk
{
	class LIGHTINK_DECL BaseBuffer
	{
	public:
		BaseBuffer();
		BaseBuffer(uint32 size);
		~BaseBuffer();
		bool init(uint32 size);
		void clear();
		uint32 set_data(const char * src, uint32 size);
		uint32 get_data_block(char * dest, uint32 start, uint32 size);
		char * get_buffer();
		char * get_data();
		char * get_free();
		uint32 get_buffer_size() const;
		uint32 get_data_size() const;
		uint32 get_free_size() const;
		uint32 read_pos() const;
		void read_pos(uint32 pos);
		uint32 add_read_pos(uint32 size);
		uint32 write_pos() const;
		void write_pos(uint32 pos);
		uint32 add_write_pos(uint32 size);
		void head_pos(uint32 pos);
		bool release();
		bool is_full();
		bool is_empty();
	private:
		char * m_buffer;
		uint32 m_readPos;
		uint32 m_writePos;
		uint32 m_size;
	LIGHTINK_DISABLE_COPY(BaseBuffer)
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline void BaseBuffer::clear() { m_readPos = 0; m_writePos = 0; }
	inline char * BaseBuffer::get_buffer() { return m_buffer; }
	inline char * BaseBuffer::get_data() { return m_buffer + m_readPos; }
	inline char * BaseBuffer::get_free() { return m_buffer + m_writePos; }
	inline uint32 BaseBuffer::get_buffer_size() const { return m_size; }
	inline uint32 BaseBuffer::get_data_size() const { return m_writePos - m_readPos; }
	inline uint32 BaseBuffer::get_free_size() const { return m_size - m_writePos; }
	inline uint32 BaseBuffer::read_pos() const { return m_readPos; }
	inline void BaseBuffer::read_pos(uint32 pos) { m_readPos = pos; }
	inline uint32 BaseBuffer::write_pos() const { return m_writePos; }
	inline void BaseBuffer::write_pos(uint32 pos) { m_writePos = pos; }
	inline void BaseBuffer::head_pos(uint32 pos) { m_readPos = pos, m_writePos = pos; }
	inline bool BaseBuffer::release() { clear(); return true; }
	inline bool BaseBuffer::is_full() { return m_writePos >= m_size; }
	inline bool BaseBuffer::is_empty() { return m_readPos == m_writePos; }

}



#endif