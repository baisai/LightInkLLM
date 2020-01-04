



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
#ifndef LIGHTINK_MSGPACK_PACKBUFFER_H_
#define LIGHTINK_MSGPACK_PACKBUFFER_H_

#include "Common/RuntimeError.h"
#include "Common/STLType.h"
#include "Msgpack/DataBuffer.h"

namespace LightInk
{
	template <typename DBuffer>
	class LIGHTINK_TEMPLATE_DECL PackBuffer
	{
	public:
		PackBuffer();
		PackBuffer(uint32 initSize);
		PackBuffer(const PackBuffer & cp);
		PackBuffer & operator = (const PackBuffer & right);
		virtual ~PackBuffer();
		virtual PackBuffer * clone();
		virtual PackBuffer * deep_clone();
		virtual void swap_buffer(DBuffer & buffer);

		virtual bool release();

		void clear();
		char * data() const;

		DBuffer * get_buffer();

		uint32 size() const;
		uint32 buffer_size() const;
		uint32 free_size() const;

		void write_pos(uint32 pos);
		uint32 write_pos() const;
		void read_pos(uint32 pos);
		uint32 read_pos() const;

		RuntimeError write(const DataBuffer & data);

		RuntimeError write(const PackBuffer & data);

		RuntimeError write(const void * data, uint32 size);

		RuntimeError write(const string & data);

		template <typename T>
		RuntimeError pack(const T & data);

		RuntimeError pack(const char * data);

		RuntimeError read(PackBuffer & data);

		RuntimeError read(void * data, uint32 size);

		RuntimeError read(string & data, uint32 size);

		RuntimeError read(const char ** data, uint32 size);

		template <typename T>
		RuntimeError unpack(T & data);

		RuntimeError unpack(char * data);

		template <typename T>
		PackBuffer & operator << (const T & data);

		PackBuffer & operator << (const char * data);

		template <typename T>
		PackBuffer & operator >> (T & data);

		PackBuffer & operator >> (char * data);

		RuntimeError resize_buffer(uint32 size);

		template <typename Compresser>
		RuntimeError compress(DataBuffer * dest);

		template <typename Compresser>
		RuntimeError uncompress(DataBuffer * dest);

		template <typename Encrypter>
		RuntimeError encrypt(DataBuffer * dest, const char * key, uint32 keyLen);

		template <typename Encrypter>
		RuntimeError decrypt(DataBuffer * dest, const char * key, uint32 keyLen);


	protected:
		DBuffer m_buffer;
		uint32 m_readPos;
	};

}


#include "Msgpack/PackBuffer.cpp"

#endif