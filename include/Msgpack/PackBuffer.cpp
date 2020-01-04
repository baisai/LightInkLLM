


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

#ifndef LIGHTINK_MSGPACK_PACKBUFFER_CPP_
#define LIGHTINK_MSGPACK_PACKBUFFER_CPP_

#include "Msgpack/PackBuffer.h"
#include "Msgpack/PackerAdaptor.h"
#include "Msgpack/UnpackerAdaptor.h"


namespace LightInk
{
	template <typename DBuffer>
	inline PackBuffer<DBuffer>::PackBuffer() : m_readPos(0)
	{
		LogTraceStepCall("PackBuffer<DBuffer>::PackBuffer()");
		LogTraceStepReturnVoid;
	}

	template <typename DBuffer>
	inline PackBuffer<DBuffer>::PackBuffer(uint32 initSize) : m_buffer(initSize), m_readPos(0)
	{
		LogTraceStepCall("PackBuffer<DBuffer>::PackBuffer(uint32 initSize)");
		LogTraceStepReturnVoid;
	}

	template <typename DBuffer>
	inline PackBuffer<DBuffer>::PackBuffer(const PackBuffer & cp) : m_buffer(cp.m_buffer), m_readPos(cp.m_readPos)
	{
		LogTraceStepCall("PackBuffer<DBuffer>::PackBuffer(const PackBuffer & cp)");
		LogTraceStepReturnVoid;
	}

	template <typename DBuffer>
	inline PackBuffer<DBuffer> & PackBuffer<DBuffer>::operator = (const PackBuffer & right)
	{
		LogTraceStepCall("PackBuffer<DBuffer> & PackBuffer<DBuffer>::operator = (const PackBuffer & right)");
		m_buffer = right.m_buffer;
		m_readPos = right.m_readPos;
		LogTraceStepReturn((*this));
	}

	template <typename DBuffer>
	inline PackBuffer<DBuffer>::~PackBuffer()
	{
		LogTraceStepCall("PackBuffer<DBuffer>::~PackBuffer()");
		m_readPos = 0;
		LogTraceStepReturnVoid;
	}

	template <typename DBuffer>
	inline PackBuffer<DBuffer> * PackBuffer<DBuffer>::clone()
	{
		LogTraceStepCall("PackBuffer<DBuffer> * PackBuffer<DBuffer>::clone()");
		LogTraceStepReturn((new PackBuffer<DBuffer>(*this)));
	}

	template <typename DBuffer>
	inline PackBuffer<DBuffer> * PackBuffer<DBuffer>::deep_clone()
	{
		LogTraceStepCall("PackBuffer<DBuffer> * PackBuffer<DBuffer>::deep_clone()");
		PackBuffer<DBuffer> * p = new PackBuffer<DBuffer>(m_buffer.buffer_size());
		p->write(*this);
		LogTraceStepReturn(p);
	}

	template <typename DBuffer>
	inline void PackBuffer<DBuffer>::swap_buffer(DBuffer & buffer)
	{
		LogTraceStepCall("void PackBuffer<DBuffer>::swap_buffer(DBuffer & buffer)");
		m_buffer.swap(buffer);
		LogTraceStepReturnVoid;
	}

	template <typename DBuffer>
	inline bool PackBuffer<DBuffer>::release()
	{
		LogTraceStepCall("bool PackBuffer<DBuffer>::release()");
		clear();
		if (m_buffer.buffer_size() > 0xffu)
		{
			LogTraceStepReturn(false);
		}
		LogTraceStepReturn(true);
	}

	template <typename DBuffer>
	inline void PackBuffer<DBuffer>::clear()
	{
		LogTraceStepCall("void PackBuffer<DBuffer>::clear()");
		m_buffer.clear();
		m_readPos = 0;
		LogTraceStepReturnVoid;
	}

	template <typename DBuffer>
	inline char * PackBuffer<DBuffer>::data() const
	{
		LogTraceStepCall("char * PackBuffer<DBuffer>::data()");
		LogTraceStepReturn((m_buffer.data() + m_readPos));
	}

	template <typename DBuffer>
	inline DBuffer * PackBuffer<DBuffer>::get_buffer()
	{
		LogTraceStepCall("DBuffer * PackBuffer<DBuffer>::get_buffer()");
		LogTraceStepReturn(&m_buffer);
	}

	template <typename DBuffer>
	inline uint32 PackBuffer<DBuffer>::size() const
	{
		LogTraceStepCall("uint32 PackBuffer<DBuffer>::size()");
		LogTraceStepReturn((m_buffer.write_pos() - m_readPos));
	}

	template <typename DBuffer>
	inline uint32 PackBuffer<DBuffer>::buffer_size() const
	{
		LogTraceStepCall("uint32 PackBuffer<DBuffer>::buffer_size()");
		LogTraceStepReturn(m_buffer.buffer_size());
	}

	template <typename DBuffer>
	inline uint32 PackBuffer<DBuffer>::free_size() const
	{
		LogTraceStepCall("uint32 PackBuffer<DBuffer>::free_size() const");
		LogTraceStepReturn(m_buffer.buffer_size() - m_buffer.write_pos());
	}

	template <typename DBuffer>
	inline void PackBuffer<DBuffer>::write_pos(uint32 pos)
	{
		LogTraceStepCall("void PackBuffer<DBuffer>::write_pos(uint32 pos)");
		m_buffer.write_pos(pos);
		LogTraceStepReturnVoid;
	}

	template <typename DBuffer>
	inline uint32 PackBuffer<DBuffer>::write_pos() const
	{
		LogTraceStepCall("uint32 PackBuffer<DBuffer>::write_pos()");
		LogTraceStepReturn(m_buffer.write_pos());
	}

	template <typename DBuffer>
	inline void PackBuffer<DBuffer>::read_pos(uint32 pos)
	{
		LogTraceStepCall("void PackBuffer<DBuffer>::read_pos(uint32 pos)");
		if (pos <= m_buffer.write_pos())
		{
			m_readPos = pos;
		} 
		else
		{
			m_readPos = m_buffer.write_pos();
		}
		LogTraceStepReturnVoid;
	}

	template <typename DBuffer>
	inline uint32 PackBuffer<DBuffer>::read_pos() const
	{
		LogTraceStepCall("uint32 PackBuffer<DBuffer>::read_pos()");
		LogTraceStepReturn(m_readPos);
	}

	template <typename DBuffer>
	inline RuntimeError PackBuffer<DBuffer>::write(const DataBuffer & data)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::write(const DataBuffer & data)");
		LogTraceStepReturn(m_buffer.write(data.data(), data.write_pos()));
	}

	template <typename DBuffer>
	inline RuntimeError PackBuffer<DBuffer>::write(const PackBuffer<DBuffer> & data)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::write(const PackBuffer<DBuffer> & data)");
		LogTraceStepReturn(m_buffer.write(data.data(), data.write_pos()));
	}

	template <typename DBuffer>
	inline RuntimeError PackBuffer<DBuffer>::write(const void * data, uint32 size)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::write(const void * data, uint32 size)");
		LogTraceStepReturn(m_buffer.write(data, size));
	}

	template <typename DBuffer>
	inline RuntimeError PackBuffer<DBuffer>::write(const string & data)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::write(const string & data)");
		LogTraceStepReturn(m_buffer.write(data));
	}

	template <typename DBuffer>
	template <typename T>
	inline RuntimeError PackBuffer<DBuffer>::pack(const T & data)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::pack(const T & data)");
		LogTraceStepReturn(LightInk::pack(*this, data));
	}

	template <typename DBuffer>
	inline RuntimeError PackBuffer<DBuffer>::pack(const char * data)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::pack(const char * data)");
		LogTraceStepReturn(LightInk::pack(*this, data));
	}

	template <typename DBuffer>
	inline RuntimeError PackBuffer<DBuffer>::read(PackBuffer & data)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::read(PackBuffer & data)");
		LogTraceStepReturn(data.write(*this));
	}

	template <typename DBuffer>
	inline RuntimeError PackBuffer<DBuffer>::read(void * data, uint32 size)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::read(void * data, uint32 size)");
		RuntimeError e = m_buffer.read(data, size, m_readPos);
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		m_readPos += size;
		LogTraceStepReturn(RE_Success);
	}

	template <typename DBuffer>
	inline RuntimeError PackBuffer<DBuffer>::read(string & data, uint32 size)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::read(string & data, uint32 size)");
		RuntimeError e = m_buffer.read(data, size, m_readPos);
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		m_readPos += size;
		LogTraceStepReturn(RE_Success);
	}

	template <typename DBuffer>
	inline RuntimeError PackBuffer<DBuffer>::read(const char ** data, uint32 size)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::read(const char ** data, uint32 size)");
		RuntimeError e = m_buffer.read(data, size, m_readPos);
		if (e != RE_Success)
		{
			LogTraceStepReturn(e);
		}
		m_readPos += size;
		LogTraceStepReturn(RE_Success);
	}

	template <typename DBuffer>
	template <typename T>
	inline RuntimeError PackBuffer<DBuffer>::unpack(T & data)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::unpack(T & data)");
		LogTraceStepReturn(LightInk::unpack(*this, data));
	}

	template <typename DBuffer>
	inline RuntimeError PackBuffer<DBuffer>::unpack(char * data)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::unpack(char * data)");
		LogTraceStepReturn(LightInk::unpack(*this, data));
	}

	template <typename DBuffer>
	template <typename T>
	inline PackBuffer<DBuffer> & PackBuffer<DBuffer>::operator << (const T & data)
	{
		LogTraceStepCall("PackBuffer<DBuffer> & PackBuffer<DBuffer>::operator << (const T & data)");
		RuntimeError e = LightInk::pack(*this, data);
		if(e != RE_Success)
		{
			throw e;
		}
		LogTraceStepReturn((*this));
	}

	template <typename DBuffer>
	inline PackBuffer<DBuffer> & PackBuffer<DBuffer>::operator << (const char * data)
	{
		LogTraceStepCall("PackBuffer<DBuffer> & PackBuffer<DBuffer>::operator << (const char * data)");
		RuntimeError e = LightInk::pack(*this, data);
		if(e != RE_Success)
		{
			throw e;
		}
		LogTraceStepReturn((*this));
	}

	template <typename DBuffer>
	template <typename T>
	inline PackBuffer<DBuffer> & PackBuffer<DBuffer>::operator >> (T & data)
	{
		LogTraceStepCall("PackBuffer<DBuffer> & PackBuffer<DBuffer>::operator >> (T & data)");
		RuntimeError e = LightInk::unpack(*this, data);
		if(e != RE_Success)
		{
			throw e;
		}
		LogTraceStepReturn((*this));
	}

	template <typename DBuffer>
	inline PackBuffer<DBuffer> & PackBuffer<DBuffer>::operator >> (char * data)
	{
		LogTraceStepCall("PackBuffer<DBuffer> & PackBuffer<DBuffer>::operator >> (char * data)");
		RuntimeError e = LightInk::unpack(*this, data);
		if(e != RE_Success)
		{
			throw e;
		}
		LogTraceStepReturn((*this));
	}

	template <typename DBuffer>
	inline RuntimeError PackBuffer<DBuffer>::resize_buffer(uint32 size)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::resize_buffer(uint32 size)");
		LogTraceStepReturn(m_buffer.resize_buffer(size));
	}

	template <typename DBuffer>
	template <typename Compresser>
	inline RuntimeError PackBuffer<DBuffer>::compress(DataBuffer * dest)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::compress(DataBuffer * dest)");
		LogTraceStepReturn(Compresser::compress(*this, dest));
	}

	template <typename DBuffer>
	template <typename Compresser>
	inline RuntimeError PackBuffer<DBuffer>::uncompress(DataBuffer * dest)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::uncompress(DataBuffer * dest)");
		LogTraceStepReturn(Compresser::uncompress(*this, dest));
	}

	template <typename DBuffer>
	template <typename Encrypter>
	inline RuntimeError PackBuffer<DBuffer>::encrypt(DataBuffer * dest, const char * key, uint32 keyLen)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::encrypt(DataBuffer * dest, const char * key, uint32 keyLen)");
		LogTraceStepReturn(Encrypter::encrypt(*this, dest, key, keyLen));
	}

	template <typename DBuffer>
	template <typename Encrypter>
	inline RuntimeError PackBuffer<DBuffer>::decrypt(DataBuffer * dest, const char * key, uint32 keyLen)
	{
		LogTraceStepCall("RuntimeError PackBuffer<DBuffer>::decrypt(DataBuffer * dest, const char * key, uint32 keyLen)");
		LogTraceStepReturn(Encrypter::decrypt(*this, dest, key, keyLen));
	}

	
}

#endif