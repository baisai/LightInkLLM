



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
#include <assert.h>
#include <string.h>
#include "Common/Memory/MemoryAllocator.h"
#include "Common/Memory/SmallAllocator.h"

namespace LightInk
{
	///////////////////////////////////////////////////////////////////
	//Chunk
	//////////////////////////////////////////////////////////////////
	_Chunk::_Chunk(): m_pData(NULL), m_firstAvBlock(0), m_countAvBlocks(0), m_pre(NULL), m_next(NULL)
	{
		
	}

	bool _Chunk::init(size_t blockSize, uint8 countBlocks)
	{
		uint8 * p = static_cast<uint8 *>(li_malloc((blockSize+SmallAllocator::PointSize) * countBlocks));
		if (!p)
		{
			return false;
		}
		m_pData = p + SmallAllocator::PointSize;
		m_firstAvBlock = 0;
		m_countAvBlocks = countBlocks;
		_Chunk * t = this;
		for (uint8 i = 0; i != countBlocks; p += (blockSize+SmallAllocator::PointSize))
		{
			memcpy(p, &t, SmallAllocator::PointSize);
			*(p+SmallAllocator::PointSize) = ++i;
		}
		return true;
		
	}
	void _Chunk::release(size_t blockSize, uint8 countBlocks)
	{
		if (m_pData)
		{
			li_free(m_pData - SmallAllocator::PointSize);
			m_pData = NULL;
			m_firstAvBlock = 0;
			m_countAvBlocks = 0;
		}
	}
	void * _Chunk::allocate(size_t blockSize)
	{
		if (!m_countAvBlocks) //not free block
		{
			return NULL;
		}
		uint8 * p = m_pData + (m_firstAvBlock * (blockSize + SmallAllocator::PointSize)); //calc allocate address
		m_firstAvBlock = *p;
		--m_countAvBlocks;
		return p;
	}
	void _Chunk::deallocate(void * p, size_t blockSize)
	{
		assert(p >= m_pData);//error point
		uint8 * rele = static_cast<uint8 *>(p);
		assert((rele-m_pData) % (blockSize+SmallAllocator::PointSize) == 0); //error block size
		*rele = m_firstAvBlock; //record next free block
		m_firstAvBlock = static_cast<uint8>((rele-m_pData) / (blockSize+SmallAllocator::PointSize)); //calc the first free block
		assert (m_firstAvBlock == (rele - m_pData)/(blockSize+SmallAllocator::PointSize)); //overflow
		++m_countAvBlocks;
	}


	////////////////////////////////////////////////////////////////
	//FixedAllocator
	///////////////////////////////////////////////////////////////
	FixedAllocator::FixedAllocator(size_t blockSize, uint8 countBlocks): 
		m_blockSize(blockSize), m_countBlocks(countBlocks), 
		m_chunkCount(0u), m_emptyChunkCount(0u),
		m_chunkList(NULL), m_freeList(NULL), m_emptyList(NULL)
	{
		create_chunk();
	}
	
	FixedAllocator::~FixedAllocator()
	{
		_Chunk * temp = NULL;
		for (_Chunk * iter = m_chunkList; iter; iter = temp)
		{
			iter->release(m_blockSize, m_countBlocks);
			temp = iter->m_next;
			delete iter;
		}
		for (_Chunk * iter = m_freeList; iter; iter = temp)
		{
			iter->release(m_blockSize, m_countBlocks);
			temp = iter->m_next;
			delete iter;
		}
		for (_Chunk * iter = m_emptyList; iter; iter = temp)
		{
			iter->release(m_blockSize, m_countBlocks);
			temp = iter->m_next;
			delete iter;
		}
		m_chunkList = NULL;
		m_freeList = NULL;
		m_emptyList = NULL;
		m_chunkCount = 0u;
		m_emptyChunkCount = 0u;
	}

	void * FixedAllocator::allocate()
	{
		if (!m_freeList) //not free block
			create_chunk();
		if (!m_freeList)
			return NULL;
		void * p = m_freeList->allocate(m_blockSize);
		if (m_freeList->m_countAvBlocks == 0)
		{
			_Chunk * c = m_freeList;
			m_freeList = c->m_next;
			if (m_freeList)
				m_freeList->m_pre = NULL;
			c->m_pre = NULL;
			c->m_next = m_chunkList;
			if (m_chunkList)
				m_chunkList->m_pre = c;
			m_chunkList = c;
		}
		return p;
	}
	
	bool FixedAllocator::create_chunk()
	{
		if (!m_emptyList)
		{
			m_emptyList = new _Chunk();
			if (!m_emptyList)
				return false;
			if (!m_emptyList->init(m_blockSize, m_countBlocks))
			{
				delete m_emptyList;
				m_emptyList = NULL;
				return false;
			}
			++m_chunkCount;
			++m_emptyChunkCount;
		}
		_Chunk * nextEmpty = m_emptyList->m_next;
		if (nextEmpty)
			nextEmpty->m_pre = NULL;
		if (m_freeList)
			m_freeList->m_pre = m_emptyList;
		m_emptyList->m_pre = NULL;
		m_emptyList->m_next = m_freeList;
		m_freeList = m_emptyList;
		m_emptyList = nextEmpty;
		--m_emptyChunkCount;
		return true;
	}

	void FixedAllocator::deallocate(void * p)
	{
		_Chunk * pDeall = NULL;
		memcpy(&pDeall, static_cast<uint8 *>(p) - SmallAllocator::PointSize, SmallAllocator::PointSize);
		if (pDeall->m_countAvBlocks == 0) //full before
		{
			if (pDeall->m_pre)
				pDeall->m_pre->m_next = pDeall->m_next;
			else
				m_chunkList = pDeall->m_next;
			if (pDeall->m_next)
				pDeall->m_next->m_pre = pDeall->m_pre;
						
			pDeall->m_pre = NULL;
			pDeall->m_next = m_freeList;
			if (m_freeList)
				m_freeList->m_pre = pDeall;
			m_freeList = pDeall;
		}
		pDeall->deallocate(p, m_blockSize);
		if (pDeall->m_countAvBlocks == m_countBlocks) //empty
		{
			if(pDeall->m_pre)
				pDeall->m_pre->m_next = pDeall->m_next;
			else
				m_freeList = pDeall->m_next;
			if (pDeall->m_next)
				pDeall->m_next->m_pre = pDeall->m_pre;

			if (m_emptyList)
			{
				pDeall->m_next = m_emptyList;
				pDeall->m_pre = NULL;
				m_emptyList->m_pre = pDeall;
				m_emptyList = pDeall;
			}
			else
			{
				pDeall->m_next = NULL;
				pDeall->m_pre = NULL;
				m_emptyList = pDeall;
			}
			++m_emptyChunkCount;
		}
		if (m_emptyChunkCount > ((m_chunkCount >> 2) + 1u))
		{
			gc(false);
		}
	}

	void FixedAllocator::gc(bool all)
	{
		if (all)
		{
			_Chunk * temp = NULL;
			for (_Chunk * iter = m_emptyList; iter; iter = temp)
			{
				iter->release(m_blockSize, m_countBlocks);
				temp = iter->m_next;
				delete iter;
				--m_chunkCount;
				--m_emptyChunkCount;
			}
			m_emptyList = NULL;
		}
		else if (m_emptyList)
		{
			_Chunk * nextEmpty = m_emptyList->m_next;
			if (nextEmpty)
				nextEmpty->m_pre = NULL;
			m_emptyList->release(m_blockSize, m_countBlocks);
			delete m_emptyList;
			--m_chunkCount;
			--m_emptyChunkCount;
			m_emptyList = nextEmpty;
		}
	}



	//////////////////////////////////////////////////////////////
	//SmallAllocator
	/////////////////////////////////////////////////////////////
	SmallAllocator::SmallAllocator()
	{
		memset(m_pool, 0, sizeof(m_pool));
	}

	SmallAllocator::~SmallAllocator()
	{
		for ( size_t i = 0u; i < ARRAY_SIZE(m_pool); ++i)
		{
			if (m_pool[i])
				delete m_pool[i];
		}
	}

	void * SmallAllocator::allocate(size_t size)
	{
		if (size > MaxObjSize)
		{
			return li_malloc(size);
		}
		else
		{
			if (size == 0)
			{
				size = 1;
			}
			size_t index = (size-1) >> OffSet_Bit;
			FixedAllocator * f = *(m_pool + index);
			if (!f) //NULL
			{
				if (!*(m_pool + index))
				{
					f = new FixedAllocator((index + 1) << OffSet_Bit, 255);
					if (!f)
					{
						return NULL;
					}
					*(m_pool + index) = f;
				}
				else
				{
					f = *(m_pool + index);
				}
			}
			return f->allocate();
		}
		return NULL;
	}

	void SmallAllocator::deallocate(void * p, size_t size)
	{
		if (p)
		{
			if (size > MaxObjSize)
			{
				li_free(p);
			}
			else
			{
				if (size == 0)
				{
					size = 1;
				}
				FixedAllocator *pdell = *(m_pool+((size-1) >> OffSet_Bit));
				if(pdell)
				{
					pdell->deallocate(p);
				}
			}
		}
		
	}

	void * SmallAllocator::reallocate(void * p, size_t osize, size_t nsize)
	{
		if (!p || osize == 0)
			return allocate(nsize);
		if(nsize == 0)
		{
			deallocate(p, osize);
			return NULL;
		}
		if (((osize-1) >> OffSet_Bit) == ((nsize-1) >> OffSet_Bit))
			return p;
		if (osize > MaxObjSize && nsize > MaxObjSize)
		{
			return li_realloc(p, nsize);
		}

		void * np = allocate(nsize);
		if(np)
		{
			if (osize < nsize)
				memcpy(np, p, osize);
			else
				memcpy(np, p, nsize);
		}
		else
		{
			return NULL;
		}
		deallocate(p, osize);
		return np;
	}

	void SmallAllocator::gc(bool all)
	{
		for ( size_t i = 0u; i < ARRAY_SIZE(m_pool); ++i)
		{
			if (m_pool[i])
				m_pool[i]->gc(all);
		}
	}
}