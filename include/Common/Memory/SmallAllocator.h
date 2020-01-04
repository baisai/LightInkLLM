


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
#ifndef LIGHTINK_COMMON_SMALLALLOCATOR_H_
#define LIGHTINK_COMMON_SMALLALLOCATOR_H_

#include <stdlib.h>
#include "Common/Type.h"

namespace LightInk
{
	struct _Chunk
	{
		_Chunk();
		bool init(size_t blockSize, uint8 countBlocks);
		void release(size_t blockSize, uint8 countBlocks);
		void * allocate(size_t blockSize);
		void deallocate(void * p, size_t blockSize);
		uint8 * m_pData; /*被管理的内存*/
		uint8 m_firstAvBlock; /*第一个可用的块index*/
		uint8 m_countAvBlocks; /*可用的块总数*/

		_Chunk * m_pre;
		_Chunk * m_next;
	};

	class  FixedAllocator
	{
	public:
		FixedAllocator(size_t blockSize, uint8 countBlocks);
		~FixedAllocator();

		void * allocate();
		void deallocate(void * p);

		void gc(bool all);

	protected:
		bool create_chunk();

	protected:
		size_t m_blockSize; /*块的大小*/
		uint8 m_countBlocks; /*一个Chunk包含块的数量*/
		size_t m_chunkCount; /*总Chunk数量*/
		size_t m_emptyChunkCount; /*空Chunk数量*/
		_Chunk* m_chunkList; /*Chunk数组*/
		_Chunk* m_freeList; /*有空闲的Chunk数组*/
		_Chunk* m_emptyList; /*没有被使用的Chunk*/

	};


	class LIGHTINK_DECL SmallAllocator
	{
	public:
		enum SizeConst
		{
			MaxObjSize = LIGHTINK_MP_MAXSIZE,
			OffSet = 1 << LIGHTINK_MP_OFFSETBIT,
			OffSet_Bit = LIGHTINK_MP_OFFSETBIT,
			PointSize = sizeof(void *),
		};
	public:
		SmallAllocator();
		~SmallAllocator();

		void * allocate(size_t size);
		void deallocate(void * p, size_t size);
		void * reallocate(void * p, size_t osize, size_t nsize);

		void gc(bool all);

	protected:
		FixedAllocator * m_pool[(MaxObjSize+OffSet-1)/OffSet];

	LIGHTINK_DISABLE_COPY(SmallAllocator)

	};

}

#endif