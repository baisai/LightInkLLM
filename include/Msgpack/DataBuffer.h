



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
#ifndef LIGHTINK_MSGPACK_DATABUFFER_H_
#define LIGHTINK_MSGPACK_DATABUFFER_H_

#include <string>
#include "Common/RuntimeError.h"
#include "Common/Type.h"
#include "Log/Log.h"
#include "Common/SmallObject.h"
#include "Common/STLType.h"

namespace LightInk
{
	class LIGHTINK_DECL DataBuffer : public SmallObject
	{
	public:
		DataBuffer();
		DataBuffer(const DataBuffer & cp);
		DataBuffer(uint32 size);
		~DataBuffer();

		DataBuffer & operator = (const DataBuffer & right);

		bool release();
		void clear();
		char * data() const;

		uint32 buffer_size() const;

		void write_pos(uint32 pos);
		uint32 write_pos() const;

		RuntimeError write(const void * data, uint32 size);
		RuntimeError write(const string & data);


		RuntimeError read(void * data, uint32 size, uint32 offset = 0);
		RuntimeError read(string & data, uint32 size, uint32 offset = 0);
		RuntimeError read(const char ** data, uint32 size, uint32 offset = 0);

		RuntimeError resize_buffer(uint32 size);

	protected:
		RuntimeError reset_buffer(uint32 size);

	protected:
		char * m_buffer;
		uint32 m_size;
		uint32 m_writePos;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline bool DataBuffer::release()
	{
		LogTraceStepCall("bool DataBuffer::release()");
		clear();
		LogTraceStepReturn(true);
	}

	inline void DataBuffer::clear()
	{
		LogTraceStepCall("DataBuffer::clear()");
		m_writePos = 0;
		LogTraceStepReturnVoid;
	}

	inline char * DataBuffer::data() const
	{
		LogTraceStepCall("char * DataBuffer::data() const");
		LogTraceStepReturn(m_buffer);
	}

	inline uint32 DataBuffer::buffer_size() const
	{
		LogTraceStepCall("uint32 DataBuffer::buffer_size() const");
		LogTraceStepReturn(m_size);
	}

	inline void DataBuffer::write_pos(uint32 pos)
	{
		LogTraceStepCall("void DataBuffer::write_pos(uint32 pos)");
		if (pos <= m_size)
		{
			m_writePos = pos;
		}
		else
		{
			m_writePos = m_size;
		}
		LogTraceStepReturnVoid;
	}

	inline uint32 DataBuffer::write_pos() const
	{
		LogTraceStepCall("uint32 DataBuffer::write_pos() const");
		LogTraceStepReturn(m_writePos);
	}

}

#endif