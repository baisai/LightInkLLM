


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
#ifndef LIGHTINK_COMMON_STLTYPE_H_

#define LIGHTINK_COMMON_STLTYPE_H_

#include <wchar.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <list>
#include "Common/Type.h"
#include "Common/Memory/MemoryAllocator.h"

namespace LightInk
{
	typedef std::basic_string<char, std::char_traits<char> > string;
	typedef std::basic_string<wchar_t, std::char_traits<wchar_t> > wstring;


	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL vector { typedef std::vector<T> type; };


	template <typename K, typename V>
	struct LIGHTINK_TEMPLATE_DECL map { typedef std::map<K, V> type; };


	template <typename T>
	struct set { typedef std::set<T> type; };


	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL deque { typedef std::deque<T> type; };


	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL list { typedef std::list<T> type; };
	
}



#endif