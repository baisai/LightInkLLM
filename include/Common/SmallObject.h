


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
#ifndef LIGHTINK_COMMON_SMALLOBJECT_H_
#define LIGHTINK_COMMON_SMALLOBJECT_H_
#include "Common/Type.h"
#include <cstdlib>

namespace LightInk
{
	class LIGHTINK_DECL SmallObject
	{
	public:
		static void * operator new(size_t size);
		static void operator delete(void *p, size_t size);
		static void * operator new[](size_t size);
		static void operator delete[](void *p);
		static void * malloc(size_t size);
		static void free(void * p);
		static void * realloc(void * p, size_t nsize);
		static void * calloc(size_t count, size_t sizeObj);

		static void * malloc_user(size_t size);
		static void free_user(void * p, size_t size);
		static void * realloc_user(void * p, size_t osize, size_t nsize);
		static void * calloc_user(size_t count, size_t sizeObj);
		virtual ~SmallObject();
	};
	

	class LIGHTINK_DECL EmptyObject
	{
	public:
		inline static void * malloc(size_t size) { return ::malloc(size); }
		inline static void free(void * p) { ::free(p); }
		inline static void * realloc(void * p, size_t nsize) { return ::realloc(p, nsize); }
		inline static void * calloc(size_t count, size_t sizeObj) { return ::calloc(count, sizeObj); }

		inline static void * malloc_user(size_t size) { return ::malloc(size);}
		inline static void free_user(void * p, size_t size){::free(p);}
		inline static void * realloc_user(void * p, size_t osize, size_t nsize) { return ::realloc(p, nsize);}
		inline static void * calloc_user(size_t count, size_t sizeObj) { return ::calloc(count, sizeObj); }
		inline virtual ~EmptyObject(){;}
	};

}

#endif