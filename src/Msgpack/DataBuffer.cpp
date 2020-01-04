


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

#include "Msgpack/DataBuffer.h"
#include <stdlib.h>
#include <memory.h>
#include "Common/STLType.h"

namespace LightInk
{
	DataBuffer::DataBuffer() : m_buffer(NULL), m_size(0), m_writePos(0)
	{
		LogTraceStepCall("DataBuffer::DataBuffer()");
		//m_buffer = (char *)li_realloc(NULL, m_size);
		LogTraceStepReturnVoid;
	}

	DataBuffer::DataBuffer(const DataBuffer & cp) : m_buffer(NULL), m_size(cp.m_size), m_writePos(cp.m_writePos)
	{
		LogTraceStepCall("DataBuffer::DataBuffer(const DataBuffer & cp)");
		if (m_size > 0)
		{
			m_buffer = (char *)li_realloc(m_buffer, m_size);
			memcpy(m_buffer, cp.m_buffer, m_writePos); 
		}
		LogTraceStepReturnVoid;
	}

	DataBuffer::DataBuffer(uint32 size) : m_buffer(NULL), m_size(size), m_writePos(0)
	{
		LogTraceStepCall("DataBuffer::DataBuffer(uint32 size)");
		if (m_size > 0)
		{
			m_buffer = (char *)li_realloc(m_buffer, m_size);
		}
		LogTraceStepReturnVoid;
	}

	DataBuffer::~DataBuffer()
	{
		LogTraceStepCall("DataBuffer::~DataBuffer()");
		li_realloc(m_buffer, 0);
		m_size = 0;
		m_writePos = 0;
		m_buffer = NULL;
		LogTraceStepReturnVoid;
	}

	DataBuffer & DataBuffer::operator = (const DataBuffer & right)
	{
		LogTraceStepCall("DataBuffer & DataBuffer::operator = (const DataBuffer & right)");
		m_buffer = (char *)li_realloc(m_buffer, right.m_size);
		if (right.m_size > 0)
		{
			memcpy(m_buffer, right.m_buffer, right.m_writePos); 
		}
		m_size = right.m_size;
		m_writePos = right.m_writePos;
		LogTraceStepReturn(*this);
	}

	RuntimeError DataBuffer::write(const void * data, uint32 size)
	{
		LogTraceStepCall("RuntimeError DataBuffer::write(const void * data, uint32 size)");
		if (m_writePos + size > m_size)
		{
			RuntimeError e = grow_buffer(m_writePos + size);
			if (e != RE_Success)
			{
				LogTraceStepReturn(e);
			}
		}
		memcpy(m_buffer + m_writePos, data, size);
		m_writePos += size;
		LogTraceStepReturn(RE_Success);
	}

	RuntimeError DataBuffer::write(const string & data)
	{
		LogTraceStepCall("RuntimeError DataBuffer::write(const string & data)");
		LogTraceStepReturn(write(data.c_str(), data.size()));
	}

	RuntimeError DataBuffer::read(void * data, uint32 size, uint32 offset)
	{
		LogTraceStepCall("RuntimeError DataBuffer::read(void * data, uint32 size, uint32 offset)");
		if (m_writePos - offset < size) //超过位置
		{
			LogTraceStepReturn(RE_Msgpack_DataOutOfRangeError);
		}
		memcpy(data, m_buffer + offset, size);
		LogTraceStepReturn(RE_Success);
	}
	RuntimeError DataBuffer::read(string & data, uint32 size, uint32 offset)
	{
		LogTraceStepCall("RuntimeError DataBuffer::read(string & data, uint32 size, uint32 offset)");
		if (m_writePos - offset < size) //超过位置
		{
			LogTraceStepReturn(RE_Msgpack_DataOutOfRangeError);
		}
		data.append(m_buffer + offset, size);
		LogTraceStepReturn(RE_Success);
	}

	RuntimeError DataBuffer::read(const char ** data, uint32 size, uint32 offset)
	{
		LogTraceStepCall("RuntimeError DataBuffer::read(const char ** data, uint32 size, uint32 offset)");
		if (m_writePos - offset < size) //超过位置
		{
			LogTraceStepReturn(RE_Msgpack_DataOutOfRangeError);
		}
		*data = m_buffer + offset;
		LogTraceStepReturn(RE_Success);
	}

	RuntimeError DataBuffer::resize_buffer(uint32 size)
	{
		LogTraceStepCall("RuntimeError DataBuffer::resize_buffer(uint32 size)");
		void * tmp = li_realloc(m_buffer, size);
		if (!tmp)
		{
			LogTraceStepReturn(RE_Msgpack_MemoryNotEnoughError);
		}
		m_buffer = static_cast<char *>(tmp);
		if (m_writePos > size)
		{
			m_writePos = size;
		}
		m_size = size;
		LogTraceStepReturn(RE_Success);
	}

	void DataBuffer::swap(DataBuffer & right)
	{
		LogTraceStepCall("void DataBuffer::swap(DataBuffer & right)");
		char * buffer = right.m_buffer;
		uint32 size = right.m_size;
		uint32 writePos = right.m_writePos;
		right.m_buffer = m_buffer;
		right.m_size = m_size;
		right.m_writePos = m_writePos;
		m_buffer = buffer;
		m_size = size;
		m_writePos = writePos;
		LogTraceStepReturnVoid;
	}

	RuntimeError DataBuffer::grow_buffer(uint32 size)
	{
		LogTraceStepCall("RuntimeError DataBuffer::grow_buffer(uint32 size)");
		if (size <= m_size)
		{
			LogTraceStepReturn(RE_Success);
		}
		if (m_size > 0)
		{
			size += ((m_size >> 3) + 16);
		}
		void* tmp = li_realloc(m_buffer, size);
		if(!tmp) {
			LogTraceStepReturn(RE_Msgpack_MemoryNotEnoughError);
		}
		m_buffer = static_cast<char*>(tmp);
		m_size = size;
		LogTraceStepReturn(RE_Success);
	}

}

