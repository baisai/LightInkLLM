


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
#ifndef LIGHTINK_COMMON_USERMEMORY_H_
#define LIGHTINK_COMMON_USERMEMORY_H_

#include <stdlib.h>
#include "Config.h"

namespace LightInk
{
	struct LIGHTINK_DECL MemoryCall
	{
		MemoryCall() : malloc(::malloc), calloc(::calloc), realloc(::realloc), free(::free) { ; }
		void * (*malloc) (size_t numBytes);
		void * (*calloc) (size_t count,  size_t numBytes);
		void * (*realloc) (void * mem, size_t numBytes);
		void (*free) (void * mem);
	};

	class LIGHTINK_DECL UserMemory
	{
	public:
		inline void reset_memory(const MemoryCall & mc)
		{ 
			m_memory.malloc = mc.malloc;
			m_memory.calloc = mc.calloc;
			m_memory.realloc = mc.realloc;
			m_memory.free = mc.free;
		}

		static UserMemory * get_instance() { static UserMemory s_um; return &s_um; }

		inline void * malloc_user(size_t numBytes) { return m_memory.malloc(numBytes); }
		inline void * calloc_user(size_t count, size_t numBytes) { return m_memory.calloc(count, numBytes); }
		inline void * realloc_user(void * mem, size_t numBytes) { return m_memory.realloc(mem, numBytes); }
		inline void free_user(void * mem) { m_memory.free(mem); }
	private:
		MemoryCall m_memory;
	};

}

#define LightInkUerMemory LightInk::UserMemory::get_instance()

#endif