


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
#ifndef LIGHTINK_MSGPACK_DATAFIXBUFFER_CPP_
#define LIGHTINK_MSGPACK_DATAFIXBUFFER_CPP_

#include "DataFixBuffer.h"

namespace LightInk
{
	template <uint32 FixLen>
	DataFixBuffer<FixLen>::DataFixBuffer() : m_writePos(0)
	{
		LogTraceStepCall("DataFixBuffer<FixLen>::DataFixBuffer()");
		LogTraceStepReturnVoid;
	}

	template <uint32 FixLen>
	DataFixBuffer<FixLen>::DataFixBuffer(const DataFixBuffer & cp) : m_writePos(cp.m_writePos)
	{
		LogTraceStepCall("DataFixBuffer<FixLen>::DataFixBuffer(const DataFixBuffer & cp)");
		memcpy(m_buffer, cp.m_buffer, m_writePos); 
		LogTraceStepReturnVoid;
	}

	template <uint32 FixLen>
	DataFixBuffer<FixLen>::DataFixBuffer(uint32 size) : m_writePos(0)
	{
		LogTraceStepCall("DataFixBuffer<FixLen>::DataFixBuffer(uint32 size)");
		LogTraceStepReturnVoid;
	}

	template <uint32 FixLen>
	DataFixBuffer<FixLen>::~DataFixBuffer()
	{
		LogTraceStepCall("DataFixBuffer<FixLen>::~DataFixBuffer()");
		m_writePos = 0;
		LogTraceStepReturnVoid;
	}

	template <uint32 FixLen>
	DataFixBuffer<FixLen> & DataFixBuffer<FixLen>::operator = (const DataFixBuffer & right)
	{
		LogTraceStepCall("DataFixBuffer<FixLen> & DataFixBuffer<FixLen>::operator = (const DataFixBuffer & right)");
		m_writePos = right.m_writePos;
		memcpy(m_buffer, right.m_buffer, m_writePos);
		LogTraceStepReturn(*this);
	}


	template <uint32 FixLen>
	inline void DataFixBuffer<FixLen>::clear()
	{
		LogTraceStepCall("void DataFixBuffer<FixLen>::clear()");
		m_writePos = 0;
		LogTraceStepReturnVoid;
	}

	template <uint32 FixLen>
	inline char * DataFixBuffer<FixLen>::data() const
	{
		LogTraceStepCall("char * DataFixBuffer<FixLen>::data() const");
		LogTraceStepReturn((char *)m_buffer);
	}


	template <uint32 FixLen>
	inline uint32 DataFixBuffer<FixLen>::buffer_size() const
	{
		LogTraceStepCall("uint32 DataFixBuffer<FixLen>::buffer_size() const");
		LogTraceStepReturn(FixLen);
	}


	template <uint32 FixLen>
	inline void DataFixBuffer<FixLen>::write_pos(uint32 pos)
	{
		LogTraceStepCall("void DataFixBuffer<FixLen>::write_pos(uint32 pos)");
		m_writePos = pos;
		LogTraceStepReturnVoid;
	}

	template <uint32 FixLen>
	inline uint32 DataFixBuffer<FixLen>::write_pos() const
	{
		LogTraceStepCall("uint32 DataFixBuffer<FixLen>::write_pos() const");
		LogTraceStepReturn(m_writePos);
	}

	template <uint32 FixLen>
	RuntimeError DataFixBuffer<FixLen>::write(const void * data, uint32 size)
	{
		LogTraceStepCall("RuntimeError DataFixBuffer<FixLen>::write(const void * data, uint32 size)");
		if (m_writePos + size > FixLen)
		{
			LogTraceStepReturn(RE_Msgpack_DataOutOfRangeError);
		}
		memcpy(m_buffer + m_writePos, data, size);
		m_writePos += size;
		LogTraceStepReturn(RE_Success);
	}

	template <uint32 FixLen>
	inline RuntimeError DataFixBuffer<FixLen>::write(const string & data)
	{
		LogTraceStepCall("RuntimeError DataFixBuffer<FixLen>::write(const string & data)");
		LogTraceStepReturn(write(data.c_str(), data.size()));
	}



	template <uint32 FixLen>
	RuntimeError DataFixBuffer<FixLen>::read(void * data, uint32 size, uint32 offset)
	{
		LogTraceStepCall("RuntimeError DataFixBuffer<FixLen>::read(void * data, uint32 size, uint32 offset)");
		if (m_writePos - offset < size) //超过位置
		{
			LogTraceStepReturn(RE_Msgpack_DataOutOfRangeError);
		}
		memcpy(data, m_buffer + offset, size);
		LogTraceStepReturn(RE_Success);
	}

	template <uint32 FixLen>
	RuntimeError DataFixBuffer<FixLen>::read(string & data, uint32 size, uint32 offset)
	{
		LogTraceStepCall("RuntimeError DataFixBuffer<FixLen>::read(string & data, uint32 size, uint32 offset)");
		if (m_writePos - offset < size) //超过位置
		{
			LogTraceStepReturn(RE_Msgpack_DataOutOfRangeError);
		}
		data.append(m_buffer + offset, size);
		LogTraceStepReturn(RE_Success);
	}

	template <uint32 FixLen>
	RuntimeError DataFixBuffer<FixLen>::read(const char ** data, uint32 size, uint32 offset)
	{
		LogTraceStepCall("RuntimeError DataFixBuffer<FixLen>::read(const char ** data, uint32 size, uint32 offset)");
		if (m_writePos - offset < size) //超过位置
		{
			LogTraceStepReturn(RE_Msgpack_DataOutOfRangeError);
		}
		*data = m_buffer + offset;
		LogTraceStepReturn(RE_Success);
	}


	template <uint32 FixLen>
	inline RuntimeError DataFixBuffer<FixLen>::resize_buffer(uint32 size)
	{
		LogTraceStepCall("RuntimeError DataFixBuffer<FixLen>::resize_buffer(uint32 size)");
		LogTraceStepReturn(RE_Msgpack_DisableResize);
	}
}


#endif