

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

#ifndef LIGHTINK_LOG_DATENAMESTRATEGY_H_
#define LIGHTINK_LOG_DATENAMESTRATEGY_H_

#include "Common/Type.h"

namespace LightInk
{
	//////////////////////////////////////////////////////////////////////////////
	//DateNameHour
	/////////////////////////////////////////////////////////////////////////////
	struct LIGHTINK_DECL DateNameHour
	{
		static OsHelper::FileNameType calc_filename(const OsHelper::FileNameType & name, time_t t, const OsHelper::FileNameType & extName);
		static time_t get_next_time(time_t t);
	};


	//////////////////////////////////////////////////////////////////////////////
	//DateNameDay
	/////////////////////////////////////////////////////////////////////////////
	struct LIGHTINK_DECL DateNameDay
	{
		static OsHelper::FileNameType calc_filename(const OsHelper::FileNameType & name, time_t t, const OsHelper::FileNameType & extName);
		static time_t get_next_time(time_t t);

	};


	//////////////////////////////////////////////////////////////////////////////
	//DateNameWeek
	/////////////////////////////////////////////////////////////////////////////
	struct LIGHTINK_DECL DateNameWeek
	{
		static OsHelper::FileNameType calc_filename(const OsHelper::FileNameType & name, time_t t, const OsHelper::FileNameType & extName);
		static time_t get_next_time(time_t t);

	};


	//////////////////////////////////////////////////////////////////////////////
	//DateNameMonth
	/////////////////////////////////////////////////////////////////////////////
	struct LIGHTINK_DECL DateNameMonth
	{
		static OsHelper::FileNameType calc_filename(const OsHelper::FileNameType & name, time_t t, const OsHelper::FileNameType & extName);
		static time_t get_next_time(time_t t);

	};
	
}




#endif