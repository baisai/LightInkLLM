

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

#include "Log/helper/FormatHelper.h"

namespace LightInk
{
	const char * const FormatHelper::wdays[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	const char * const FormatHelper::full_wdays[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
	const char * const FormatHelper::months[] = { "Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sept", "Oct", "Nov", "Dec" };
	const char * const FormatHelper::full_months[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
	const char * const FormatHelper::level[] = { "T", "D", "M", "W", "E", "F"};
	const char * const FormatHelper::full_level[] = { "Trace", "Debug", "Message",  "Warning", "Error", "Fatal" };
#ifdef _WIN32
	const char * const FormatHelper::endLine = "\r\n";
	const size_t FormatHelper::endLineSize = sizeof(FormatHelper::endLine) - 1;
#else
	const char * const FormatHelper::endLine = "\n";
	const size_t FormatHelper::endLineSize = sizeof(FormatHelper::endLine) - 1;
#endif
	const char * const FormatHelper::file_line = "";
}
