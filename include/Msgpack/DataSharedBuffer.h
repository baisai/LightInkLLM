



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
#ifndef LIGHTINK_MSGPACK_DATASHAREDBUFFER_H_
#define LIGHTINK_MSGPACK_DATASHAREDBUFFER_H_

#include "Common/Memory/ObjectPool.h"
#include "Common/Ptr/SharedPtr.h"
#include "Msgpack/DataBuffer.h"

namespace LightInk
{
	class LIGHTINK_DECL DataSharedBuffer : public SmallObject
	{
	public:
		DataSharedBuffer() : m_buffer(BufferStrategy::create()) {  }
		DataSharedBuffer(const DataSharedBuffer & cp) : m_buffer(cp.m_buffer) {  }
		DataSharedBuffer(uint32 size) : m_buffer(BufferStrategy::create()) { m_buffer->resize_buffer(size); }
		~DataSharedBuffer() {  }

		DataSharedBuffer & operator = (const DataSharedBuffer & right);

		DataBuffer * get_buffer();

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

	public:
		struct BufferStrategy
		{
			static DataBuffer * create();
			static void release(DataBuffer * ptr);
		};
	protected:
		SharedPtrTSUser<DataBuffer, BufferStrategy>::type m_buffer;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline DataSharedBuffer & DataSharedBuffer::operator = (const DataSharedBuffer & right)
	{ m_buffer = right.m_buffer; return *this; }

	inline DataBuffer * DataSharedBuffer::get_buffer() { return m_buffer.get(); }

	inline void DataSharedBuffer::clear() { m_buffer->clear(); }
	inline char * DataSharedBuffer::data() const { return m_buffer->data(); }

	inline uint32 DataSharedBuffer::buffer_size() const { return m_buffer->buffer_size(); }

	inline void DataSharedBuffer::write_pos(uint32 pos) { m_buffer->write_pos(pos); }
	inline uint32 DataSharedBuffer::write_pos() const { return m_buffer->write_pos(); }

	inline RuntimeError DataSharedBuffer::write(const void * data, uint32 size) { return m_buffer->write(data, size); }
	inline RuntimeError DataSharedBuffer::write(const string & data) { return m_buffer->write(data); }

	inline RuntimeError DataSharedBuffer::read(void * data, uint32 size, uint32 offset) 
	{ return m_buffer->read(data, size, offset); }
	inline RuntimeError DataSharedBuffer::read(string & data, uint32 size, uint32 offset) 
	{ return m_buffer->read(data, size, offset); }
	inline RuntimeError DataSharedBuffer::read(const char ** data, uint32 size, uint32 offset) 
	{ return m_buffer->read(data, size, offset); }

	inline RuntimeError DataSharedBuffer::resize_buffer(uint32 size) 
	{ return m_buffer->resize_buffer(size); }

	inline DataBuffer * DataSharedBuffer::BufferStrategy::create()
	{ return LightInkDataBufferPool->allocate_object(); }
	inline void DataSharedBuffer::BufferStrategy::release(DataBuffer * ptr)
	{ LightInkDataBufferPool->deallocate_object(ptr); }


}

#endif