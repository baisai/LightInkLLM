


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

#ifndef LIGHTINK_COMMON_WINSNPRINTF_H_
#define LIGHTINK_COMMON_WINSNPRINTF_H_

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include "Common/Config.h"

#if defined(_MSC_VER) && _MSC_VER < 1900

	#ifdef __cplusplus
	extern "C" {
	#endif

		LIGHTINK_DECL int WinSnpirintf(char* buf, size_t len, const char* fmt, ...);

	#ifdef __cplusplus
	}
	#endif

	#ifndef snprintf
	#define snprintf WinSnpirintf
	#endif

#endif

#ifndef WIN32
	#ifdef __cplusplus
	extern "C" {
	#endif

		LIGHTINK_DECL int _vscprintf (const char * format, va_list pargs);

	#ifdef __cplusplus
	}
	#endif

#endif


#endif

