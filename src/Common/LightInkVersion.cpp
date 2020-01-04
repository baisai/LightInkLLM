


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
#include "Common/LightInkVersion.h"
#include "LuaEngine/LuaLib.h"
#include "fmt/fmt.h"

namespace LightInk
{
	int get_major_version ()
	{
		return (LIGHTINK_MAJOR_VERSION_);
	}

	int get_minor_version ()
	{
		return (LIGHTINK_MINOR_VERSION_);
	}

	int get_micro_version ()
	{
		return (LIGHTINK_MICRO_VERSION_);
	}

	uint32 get_version_hex()
	{
		return (LIGHTINK_VERSION_HEX_);
	}

	const char * get_version_string ()
	{
		return (LIGHTINK_VERSION_S_(LIGHTINK_VERSION_));
	}

	bool check_version (int major, int minor, int micro)
	{
		return (LIGHTINK_CHECK_VERSION_(major, minor, micro));
	}

	const char * get_authors()
	{
		return (LIGHTINK_AUTHORS);
	}

	struct LightInkVersion
	{ 
		LightInkVersion()
		{
			snprintf(dependLib, sizeof(dependLib), 
				"\nTurf version(201608)\nfmt version(%d)\nLua version(%s)\n",
				FMT_VERSION, LUA_RELEASE);
		}

		static char dependLib[288];
	};
	char LightInkVersion::dependLib[288] = { 0 };
	LightInkVersion liv;
	
	const char * get_depend_lib_version()
	{
		return LightInkVersion::dependLib;
	}
}