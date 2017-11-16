

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

#ifndef LIGHTINK_LOG_OSHELPER_H_
#define LIGHTINK_LOG_OSHELPER_H_

#include "Common/Type.h"
#include "Common/RuntimeError.h"
#include "Common/STLType.h"
#include "Log/LogConfig.h"

#include <stdio.h>
#include <string>
#include <time.h>
#include <sys/stat.h>

#ifndef _WIN32
#include <sys/time.h>
#include "sys/sysinfo.h"
#include <string.h>
#include <errno.h>
#else
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "Windows.h"
#include "WinSock.h"
#endif

namespace LightInk
{
	struct LIGHTINK_DECL OsHelper
	{
		typedef void (* log_err_handle)(const string & err);

#if defined(_WIN32) && defined(LIGHTINK_LOG_WCHAR_FILENAMES)
		typedef wstring FileNameType;
		typedef wchar_t FileCharType;
#else
		typedef string FileNameType;
		typedef char FileCharType;
#endif



		static inline tm localtime(const time_t &time_tt)
		{

#ifdef _WIN32
			tm t;
			::localtime_s(&t, &time_tt);
#else
			tm t;
			::localtime_r(&time_tt, &t);
#endif
			return t;
		}
		static inline tm localtime()
		{
			return localtime(time(NULL));
		}

		static inline tm gmtime(const time_t &time_tt)
		{

#ifdef _WIN32
			tm t;
			::gmtime_s(&t, &time_tt);
#else
			tm t;
			::gmtime_r(&time_tt, &t);
#endif
			return t;
		}
		static inline tm gmtime()
		{
			return gmtime(time(NULL));
		}


		static void gettimeofday(struct timeval * tv);


		static inline FILE* fopen(const FileNameType& filename, const FileCharType * mode)
		{
#ifdef _WIN32
#ifdef LIGHTINK_LOG_WCHAR_FILENAMES
			FILE* fp = ::_wfsopen((filename.c_str()), mode, _SH_DENYWR);
#else
			FILE* fp = ::_fsopen((filename.c_str()), mode, _SH_DENYWR);
#endif
#else
			FILE* fp = ::fopen((filename.c_str()), mode);
#endif
			return fp;
		}
		static inline int remove(const FileNameType &filename)
		{
#if defined(_WIN32) && defined(LIGHTINK_LOG_WCHAR_FILENAMES)
			return ::_wremove(filename.c_str());
#else
			return ::remove(filename.c_str());
#endif
		}
		static inline int rename(const FileNameType & oldName, const FileNameType & newName)
		{
#if defined(_WIN32) && defined(LIGHTINK_LOG_WCHAR_FILENAMES)
			return ::_wrename(oldName.c_str(), newName.c_str());
#else
			return ::rename(oldName.c_str(), newName.c_str());
#endif
		}
		static inline bool file_exists(const FileNameType & filename)
		{
#ifdef _WIN32
#ifdef LIGHTINK_LOG_WCHAR_FILENAMES
			uint32 attribs = ::GetFileAttributesW(filename.c_str());
#else
			uint32 attribs = ::GetFileAttributesA(filename.c_str());
#endif
			return (attribs != INVALID_FILE_ATTRIBUTES && !(attribs & FILE_ATTRIBUTE_DIRECTORY));
#else //common linux/unix all have the stat system call
			struct stat buffer;
			return (::stat (filename.c_str(), &buffer) == 0);
#endif
		}

		static size_t file_size(FILE * fp);
		
		static int utc_minutes_offset(const tm & t = localtime());

		static string filename_to_string(const FileNameType & filename);

		static FileNameType string_to_filename(const string & str);

		static string errno_str(int err_num);

		static uint64 get_up_time();

		static uint64 get_used_virtual();
		
		static uint64 get_total_virtual();

		static uint64 get_used_physical();

		static uint64 get_total_physical();

		
	};

#if defined(_WIN32) && defined(LIGHTINK_LOG_WCHAR_FILENAMES)
#define LIGHTINK_LOG_FILENAME_T(s) L ## s
#else
#define LIGHTINK_LOG_FILENAME_T(s) s
#endif


	inline bool operator==(const tm& tm1, const tm& tm2)
	{
		return (tm1.tm_sec == tm2.tm_sec &&
				tm1.tm_min == tm2.tm_min &&
				tm1.tm_hour == tm2.tm_hour &&
				tm1.tm_mday == tm2.tm_mday &&
				tm1.tm_mon == tm2.tm_mon &&
				tm1.tm_year == tm2.tm_year &&
				tm1.tm_isdst == tm2.tm_isdst);
	}
	inline bool operator!=(const tm& tm1, const tm& tm2)
	{
		return !(tm1 == tm2);
	}

}

#endif