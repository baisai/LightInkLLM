



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
#include "Common/SmallObject.h"
#include "Common/UserMemory.h"

namespace LightInk
{

	//////////////////////////////////////////////////////////////
	//SmallObject实现
	/////////////////////////////////////////////////////////////
	void * SmallObject::operator new(size_t size)
	{
		return LightInkUerMemory->malloc_user(size);
	}

	void SmallObject::operator delete(void * p, size_t size)
	{
		LightInkUerMemory->free_user(p);
	}

	void * SmallObject::operator new[](size_t size)
	{
		return LightInkUerMemory->malloc_user(size);
	}

	void SmallObject::operator delete[](void *p)
	{
		if (!p)
		{
			return;
		}
		LightInkUerMemory->free_user(p);
	}

	void * SmallObject::malloc_user(size_t size)
	{
		return LightInkUerMemory->malloc_user(size);
	}

	void SmallObject::free_user(void * p, size_t size)
	{
		LightInkUerMemory->free_user(p);
	}

	void * SmallObject::realloc_user(void * p, size_t osize, size_t nsize)
	{
		return LightInkUerMemory->realloc_user(p, nsize);
	}

	void * calloc_user(size_t count, size_t sizeObj)
	{
		void * p = SmallObject::malloc_user(count * sizeObj);
		memset(p, 0, count * sizeObj);
		return p;
	}


	void * SmallObject::malloc(size_t size)
	{
		return LightInkUerMemory->malloc_user(size);
	}

	void SmallObject::free(void * p)
	{
		if (!p)
		{
			return;
		}
		LightInkUerMemory->free_user(p);
	}

	void * SmallObject::realloc(void * p, size_t nsize)
	{
		return LightInkUerMemory->realloc_user(p, nsize);
	}

	void * SmallObject::calloc(size_t count, size_t sizeObj)
	{
		void * p = SmallObject::malloc(count * sizeObj);
		memset(p, 0, count * sizeObj);
		return p;
	}

	SmallObject::~SmallObject()
	{
		
	}
}