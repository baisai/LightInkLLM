


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

#include <stdlib.h>
#include <memory.h>
#include "Msgpack/DataRefBuffer.h"


namespace LightInk
{
	DataRefBuffer::DataRefBuffer() : m_buffer(NULL), m_size(0)
	{
		LogTraceStepCall("DataRefBuffer::DataRefBuffer()");
		LogTraceStepReturnVoid;
	}

	DataRefBuffer::DataRefBuffer(const DataRefBuffer & cp) : m_buffer(cp.m_buffer), m_size(cp.m_size)
	{
		LogTraceStepCall("DataRefBuffer::DataRefBuffer(const DataRefBuffer & cp)");
		LogTraceStepReturnVoid;
	}

	DataRefBuffer::DataRefBuffer(uint32 size) : m_buffer(NULL), m_size(0)
	{
		LogTraceStepCall("DataRefBuffer::DataRefBuffer(uint32 size)");
		LogTraceStepReturnVoid;
	}

	DataRefBuffer::~DataRefBuffer()
	{
		LogTraceStepCall("DataRefBuffer::~DataRefBuffer()");
		m_size = 0;
		m_buffer = NULL;
		LogTraceStepReturnVoid;
	}

	DataRefBuffer & DataRefBuffer::operator = (const DataRefBuffer & right)
	{
		LogTraceStepCall("DataRefBuffer & DataRefBuffer::operator = (const DataRefBuffer & right)");
		m_buffer = right.m_buffer;
		m_size = right.m_size;
		LogTraceStepReturn(*this);
	}

	RuntimeError DataRefBuffer::write(const void * data, uint32 size)
	{
		LogTraceStepCall("RuntimeError DataRefBuffer::write(const void * data, uint32 size)");
		m_buffer = (const char *)data;
		m_size = size;
		LogTraceStepReturn(RE_Success);
	}

	RuntimeError DataRefBuffer::write(const string & data)
	{
		LogTraceStepCall("RuntimeError DataRefBuffer::write(const string & data)");
		LogTraceStepReturn(write(data.c_str(), data.size()));
	}

	RuntimeError DataRefBuffer::read(void * data, uint32 size, uint32 offset)
	{
		LogTraceStepCall("RuntimeError DataRefBuffer::read(void * data, uint32 size, uint32 offset)");
		if (m_size - offset < size) //超过位置
		{
			LogTraceStepReturn(RE_Msgpack_DataOutOfRangeError);
		}
		memcpy(data, m_buffer + offset, size);
		LogTraceStepReturn(RE_Success);
	}

	RuntimeError DataRefBuffer::read(string & data, uint32 size, uint32 offset)
	{
		LogTraceStepCall("RuntimeError DataRefBuffer::read(string & data, uint32 size, uint32 offset)");
		if (m_size - offset < size) //超过位置
		{
			LogTraceStepReturn(RE_Msgpack_DataOutOfRangeError);
		}
		data.append(m_buffer + offset, size);
		LogTraceStepReturn(RE_Success);
	}

	RuntimeError DataRefBuffer::read(const char ** data, uint32 size, uint32 offset)
	{
		LogTraceStepCall("RuntimeError DataRefBuffer::read(const char ** data, uint32 size, uint32 offset)");
		if (m_size - offset < size) //超过位置
		{
			LogTraceStepReturn(RE_Msgpack_DataOutOfRangeError);
		}
		*data = m_buffer + offset;
		LogTraceStepReturn(RE_Success);
	}


}

