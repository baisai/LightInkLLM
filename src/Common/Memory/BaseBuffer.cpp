


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
#include <string.h>
#include "Common/Memory/BaseBuffer.h"
#include "Common/Memory/MemoryAllocator.h"

namespace LightInk
{
	BaseBuffer::BaseBuffer() : m_buffer(NULL), m_readPos(0), m_writePos(0), m_size(0)
	{

	}

	BaseBuffer::BaseBuffer(uint32 size) : m_buffer(NULL), m_readPos(0), m_writePos(0), m_size(0)
	{
		if (size > 0)
		{
			m_buffer = (char *)li_malloc(size);
		}
	}

	BaseBuffer::~BaseBuffer()
	{
		if (m_buffer)
		{
			li_free(m_buffer);
		}
	}

	bool BaseBuffer::init(uint32 size)
	{
		if (m_buffer)
		{
			if (m_size == size) { return true; }
			else { return false; }
		}
		m_buffer = (char *)li_malloc(size);
		m_size = size;
		return true;
	}

	uint32 BaseBuffer::set_data(const char * src, uint32 size)
	{
		uint32 dataLen = get_free_size();
		if (dataLen > size)
		{
			dataLen = size;
		}
		memcpy(m_buffer + m_writePos, src, dataLen);
		m_writePos += dataLen;
		return dataLen;
	}

	uint32 BaseBuffer::get_data_block(char * dest, uint32 start, uint32 size)
	{
		if (size == 0 || is_empty())
		{
			return 0;
		}
		uint32 index = m_readPos + start;
		if (index >= m_writePos)
		{
			return 0;
		}
		uint32 dataLen = m_writePos - index;
		if (size < dataLen)
		{
			dataLen = size;
		}
		memcpy(dest, m_buffer + index, dataLen);
		return dataLen;
	}

	uint32 BaseBuffer::add_read_pos(uint32 size)
	{
		if (m_readPos + size > m_writePos)
		{
			size = m_writePos - m_readPos;
		}
		m_readPos += size;
		return size;
	}

	uint32 BaseBuffer::add_write_pos(uint32 size)
	{
		if (m_writePos + size > m_size)
		{
			size = m_size - m_writePos;
		}
		m_writePos += size;
		return size;
	}
}