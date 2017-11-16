

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

#ifndef LIGHTINK_LOG_LOGCONFIG_H_
#define LIGHTINK_LOG_LOGCONFIG_H_


#define LIGHTINK_LOG_MULTITHREAD

#define LIGHTINK_LOG_WCHAR_FILENAMES

namespace LightInk
{
	namespace LogLevel
	{
		enum LEVEL
		{
			LogMsg_Trace = 0x1,
			LogMsg_Debug = 0x2,
			LogMsg_Message = 0x4,
			LogMsg_Warning = 0x8,
			LogMsg_Error = 0x10,
			LogMsg_Fatal = 0x20,
		};

		enum LEVELCOUNT
		{
			LogMsg_Count = 6
		};
	}

}


#endif