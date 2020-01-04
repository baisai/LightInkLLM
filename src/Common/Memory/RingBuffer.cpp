


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
#include "Common/Memory/RingBuffer.h"
#include "Common/Memory/MemoryAllocator.h"

namespace LightInk
{
	RingBuffer::RingBuffer() : m_buffer(NULL), m_size(0), m_readIndexA(0), m_writeIndexA(0), m_writeIndexB(0)
	{

	}
	RingBuffer::RingBuffer(uint32 size) : m_buffer(NULL), m_size(size), m_readIndexA(0), m_writeIndexA(0), m_writeIndexB(0)
	{
		if (size > 0)
		{
			m_buffer = (char *)li_malloc(size);
		}
	}

	RingBuffer::~RingBuffer()
	{
		li_free(m_buffer);
		m_buffer = NULL;
		m_size = 0;
		clear();
	}

	bool RingBuffer::init(uint32 size)
	{
		if (m_buffer || m_size)
		{
			return false;
		}
		m_size = size;
		m_buffer = (char *)li_malloc(size);
		return true;
	}

	char * RingBuffer::get_free_block(uint32 & size)
	{
		char * p = NULL;
		if (m_writeIndexA >= m_size) //A full
		{
			if (m_writeIndexB >= m_readIndexA) //B full
			{
				size = 0;
				return p;
			}
			p = m_buffer + m_writeIndexB;
			size = m_readIndexA - m_writeIndexB;
		}
		else
		{
			p = m_buffer + m_writeIndexA;
			size = m_size - m_writeIndexA;
		}
		return p;
	}

	uint32 RingBuffer::get_data(char * dest)
	{
		if (is_empty())
		{
			return 0;
		}
		uint32 dataLen = 0;
		if (m_writeIndexA < m_size)
		{
			dataLen = m_writeIndexA - m_readIndexA;
			memcpy(dest, m_buffer + m_readIndexA, dataLen);
		}
		else
		{
			dataLen = m_size - m_readIndexA;
			memcpy(dest, m_buffer + m_readIndexA, dataLen);
			if (m_writeIndexB > 0)
			{
				memcpy(dest + dataLen, m_buffer, m_writeIndexB);
				dataLen += m_writeIndexB;
			}
		}
		clear();
		return dataLen;
	}

	uint32 RingBuffer::get_data_block(string & dest, uint32 start, uint32 size)
	{
		if (size == 0 || is_empty())
		{
			return 0;
		}
		uint32 index = m_readIndexA + start;
		uint32 dataLen = 0;
		if (m_writeIndexA < m_size) //onle A
		{
			if (index >= m_writeIndexA)
			{
				return 0;
			}
			dataLen = m_writeIndexA - index;
			if (size < dataLen)
			{
				dataLen = size;
			}
			dest.append(m_buffer + index, dataLen);
		}
		else //have B
		{
			if (index >= m_size) //start from B
			{
				index -= m_size;
				if (index >= m_writeIndexB)
				{
					return 0;
				}
				dataLen = m_writeIndexB - index;
				if (size < dataLen)
				{
					dataLen = size;
				}
				dest.append(m_buffer + index, dataLen);
			}
			else
			{
				dataLen = m_size - index;
				if (size <= dataLen)
				{
					dataLen = size;
					dest.append(m_buffer + index, dataLen);
				}
				else
				{
					dest.append(m_buffer + index, dataLen);
					dest += dataLen;
					size -= dataLen;
					if (size > m_writeIndexB)
					{
						size = m_writeIndexB;
					}
					dataLen += size;
					dest.append(m_buffer, size);
				}
			}
		}
		return dataLen;
	}

	uint32 RingBuffer::get_data_block(char * dest, uint32 start, uint32 size)
	{
		if (size == 0 || is_empty())
		{
			return 0;
		}
		uint32 index = m_readIndexA + start;
		uint32 dataLen = 0;
		if (m_writeIndexA < m_size) //only A
		{
			if (index >= m_writeIndexA)
			{
				return 0;
			}
			dataLen = m_writeIndexA - index;
			if (size < dataLen)
			{
				dataLen = size;
			}
			memcpy(dest, m_buffer + index, dataLen);
		}
		else //have B
		{
			if (index >= m_size) //start from B
			{
				index -= m_size;
				if (index >= m_writeIndexB)
				{
					return 0;
				}
				dataLen = m_writeIndexB - index;
				if (size < dataLen)
				{
					dataLen = size;
				}
				memcpy(dest, m_buffer + index, dataLen);
			}
			else
			{
				dataLen = m_size - index;
				if (size <= dataLen)
				{
					dataLen = size;
					memcpy(dest, m_buffer + index, dataLen);
				}
				else
				{
					memcpy(dest, m_buffer + index, dataLen);
					dest += dataLen;
					size -= dataLen;
					if (size > m_writeIndexB)
					{
						size = m_writeIndexB;
					}
					dataLen += size;
					memcpy(dest, m_buffer, size);
				}
			}
		}
		return dataLen;
	}

	uint32 RingBuffer::get_ab_data(char ** a, uint32 & sizeA, char ** b, uint32 & sizeB)
	{
		*a = m_buffer + m_readIndexA;
		sizeA = m_writeIndexA - m_readIndexA;
		*b = m_buffer;
		sizeB = m_writeIndexB;
		return (sizeA + sizeB);
	}

	uint32 RingBuffer::set_data(const char * src, uint32 size)
	{
		if (size == 0 || is_full())
		{
			return 0;
		}
		uint32 dataLen = 0;
		if (m_writeIndexA >= m_size) //have B
		{
			dataLen = m_readIndexA - m_writeIndexB;
			if (size < dataLen)
			{
				dataLen = size;
			}
			memcpy(m_buffer + m_writeIndexB, src, dataLen);
			m_writeIndexB += dataLen;
		}
		else
		{
			dataLen = m_size - m_writeIndexA;
			if (size <= dataLen)
			{
				dataLen = size;
				memcpy(m_buffer + m_writeIndexA, src, dataLen);
				m_writeIndexA += dataLen;
			}
			else
			{
				memcpy(m_buffer + m_writeIndexA, src, dataLen);
				m_writeIndexA = m_size;
				if (m_readIndexA > 0)
				{
					size -= dataLen;
					if (size > m_readIndexA)
					{
						size = m_readIndexA;
					}
					memcpy(m_buffer, src + dataLen, size);
					dataLen += size;
					m_writeIndexB += size;
				}
			}
		}
		return dataLen;
	}

	uint32 RingBuffer::insert(uint32 size)
	{
		if (size == 0 || is_full())
		{
			return 0;
		}
		uint32 dataLen = 0;
		if (m_writeIndexA >= m_size) //have B
		{
			dataLen = m_readIndexA - m_writeIndexB;
			if (size < dataLen)
			{
				dataLen = size;
			}
			m_writeIndexB += dataLen;
		}
		else
		{
			dataLen = m_size - m_writeIndexA;
			if (size <= dataLen)
			{
				dataLen = size;
				m_writeIndexA += dataLen;
			}
			else
			{
				m_writeIndexA = m_size;
				if (m_readIndexA > 0)
				{
					size -= dataLen;
					if (size > m_readIndexA)
					{
						size = m_readIndexA;
					}
					dataLen += size;
					m_writeIndexB += size;
				}
			}
		}
		return dataLen;
	}

	uint32 RingBuffer::remove(uint32 size)
	{
		if (size == 0 || is_empty())
		{
			return 0;
		}
		uint32 dataLen = 0;
		if (m_writeIndexA < m_size) //only A
		{
			dataLen = m_writeIndexA - m_readIndexA;
			if (size < dataLen)
			{
				dataLen = size;
			}
			m_readIndexA += dataLen;
		}
		else //have B
		{
			dataLen = m_size - m_readIndexA;
			if (size < dataLen)
			{
				dataLen = size;
				m_readIndexA += dataLen;
			}
			else
			{
				size -= dataLen;
				if (size > m_writeIndexB)
				{
					size = m_writeIndexB;
				}
				dataLen += size;
				m_readIndexA = size;
				m_writeIndexA = m_writeIndexB;
				m_writeIndexB = 0;
			}
		}
		return dataLen;
	}

	char & RingBuffer::operator [](int32 index)
	{
		uint32 idx = m_readIndexA + index;
		if (idx >= m_size)
		{
			idx -= m_size;
		}
		return m_buffer[idx];
	}
}