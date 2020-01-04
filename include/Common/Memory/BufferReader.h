


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
#ifndef LIGHTINK_COMMON_BUFFERREADER_H_
#define LIGHTINK_COMMON_BUFFERREADER_H_

#include "Common/Type.h"
#include <string.h>

namespace LightInk
{
	class LIGHTINK_DECL BufferReader
	{
	public:
		BufferReader() : m_buffer(NULL), m_readPos(0), m_size(0) {  }
		BufferReader(const char * buffer, size_t size) : m_buffer(buffer), m_readPos(0), m_size(size) {  }
		BufferReader(const BufferReader & cp) : m_buffer(cp.m_buffer), m_readPos(0), m_size(cp.m_size) {  }
		virtual ~BufferReader() { ; }

		void reset(const char * data, size_t size);
		const char * get_buffer();
		const char * get_data();
		uint32 get_data_block(char * dest, uint32 start, uint32 size);
		size_t get_buffer_size() const;
		size_t get_data_size() const;
		size_t read_pos() const;
		void read_pos(size_t pos);
		void read_off(size_t size);
		bool is_empty();

	protected:
		const char * m_buffer;
		size_t m_readPos;
		size_t m_size;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline void BufferReader::reset(const char * data, size_t size) { m_buffer = data; m_size = size; m_readPos = 0; }
	inline const char * BufferReader::get_buffer() { return m_buffer; }
	inline const char * BufferReader::get_data() { return m_buffer + m_readPos; }
	inline size_t BufferReader::get_buffer_size() const { return m_size; }
	inline size_t BufferReader::get_data_size() const { return m_size - m_readPos; }
	inline size_t BufferReader::read_pos() const { return m_readPos; }
	inline void BufferReader::read_pos(size_t pos) { m_readPos = pos; }
	inline void BufferReader::read_off(size_t size) { m_readPos += size; if(m_readPos > m_size) { m_readPos = m_size; } }
	inline bool BufferReader::is_empty() { return m_readPos == m_size; }

	inline uint32 BufferReader::get_data_block(char * dest, uint32 start, uint32 size)
	{
		if (size == 0 || is_empty()) { return 0; }
		uint32 index = m_readPos + start;
		if (index >= m_size) { return 0; }
		uint32 dataLen = m_size - index;
		if (size < dataLen) { dataLen = size; }
		memcpy(dest, m_buffer + index, dataLen);
		return dataLen;
	}
}


#endif