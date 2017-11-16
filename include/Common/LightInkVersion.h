

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

#ifndef LIGHTINK_COMMON_LIGHTINKVERSION_H_
#define LIGHTINK_COMMON_LIGHTINKVERSION_H_

#include "Common/Type.h"

//major version component
#define LIGHTINK_MAJOR_VERSION_ 0

//minor version component
#define LIGHTINK_MINOR_VERSION_ 3

//micro version component
#define LIGHTINK_MICRO_VERSION_ 3

//prerelease version component
//#define LIGHTINK_PRERELEASE_VERSION_ rc0

#ifdef LIGHTINK_PRERELEASE_VERSION_
#define LIGHTINK_VERSION_ LIGHTINK_MAJOR_VERSION_.LIGHTINK_MINOR_VERSION_.LIGHTINK_MICRO_VERSION_-LIGHTINK_PRERELEASE_VERSION_
#else
#define LIGHTINK_VERSION_ LIGHTINK_MAJOR_VERSION_.LIGHTINK_MINOR_VERSION_.LIGHTINK_MICRO_VERSION_
#endif

#define _LIGHTINK_VERSION_S_(str) #str
#define LIGHTINK_VERSION_S_(str) _LIGHTINK_VERSION_S_(str)



#define LIGHTINK_VERSION_HEX_ (LIGHTINK_MAJOR_VERSION_ << 24 | \
                          LIGHTINK_MINOR_VERSION_ << 16 | \
                          LIGHTINK_MICRO_VERSION_ << 8)

#define LIGHTINK_CHECK_VERSION_(major,minor,micro)   \
        (LIGHTINK_MAJOR_VERSION_ > (major) || \
         (LIGHTINK_MAJOR_VERSION_ == (major) && LIGHTINK_MINOR_VERSION_ > (minor)) || \
         (LIGHTINK_MAJOR_VERSION_ == (major) && LIGHTINK_MINOR_VERSION_ == (minor) && \
          LIGHTINK_MICRO_VERSION_ >= (micro)))

#define LIGHTINK_AUTHORS "ChenDong(Wilbur)"

namespace LightInk
{
	LIGHTINK_DECL int get_major_version ();
	LIGHTINK_DECL int get_minor_version ();
	LIGHTINK_DECL int get_micro_version ();
	LIGHTINK_DECL uint32 get_version_hex();
	LIGHTINK_DECL const char * get_version_string ();
	LIGHTINK_DECL bool check_version (int major, int minor, int micro);
	LIGHTINK_DECL const char * get_authors();

	LIGHTINK_DECL const char * get_depend_lib_version();
}

#endif