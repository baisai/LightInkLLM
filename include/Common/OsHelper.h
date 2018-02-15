

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

#include <stdio.h>
#include <string>
#include <time.h>
#include <sys/stat.h>

#ifndef _WIN32
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#else
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "windows.h"
#include "winsock2.h"
#endif

//#define LIGHTINK_WCHAR_FILENAMES

namespace LightInk
{
	struct LIGHTINK_DECL OsHelper
	{
		typedef void (* log_err_handle)(const string & err);

#if defined(_WIN32) && defined(LIGHTINK_WCHAR_FILENAMES)
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


		static inline FILE* fopen(const FileCharType * filename, const FileCharType * mode)
		{
#ifdef _WIN32
#ifdef LIGHTINK_WCHAR_FILENAMES
			FILE* fp = ::_wfsopen(filename, mode, _SH_DENYWR);
#else
			FILE* fp = ::_fsopen(filename, mode, _SH_DENYWR);
#endif
#else
			FILE* fp = ::fopen(filename, mode);
#endif
			return fp;
		}
		static inline int remove(const FileCharType * filename)
		{
#if defined(_WIN32) && defined(LIGHTINK_WCHAR_FILENAMES)
			return ::_wremove(filename);
#else
			return ::remove(filename);
#endif
		}
		static inline int rename(const FileCharType * oldName, const FileCharType * newName)
		{
#if defined(_WIN32) && defined(LIGHTINK_WCHAR_FILENAMES)
			return ::_wrename(oldName, newName);
#else
			return ::rename(oldName, newName);
#endif
		}
		static inline bool file_exists(const FileCharType * filename)
		{
#ifdef _WIN32
#ifdef LIGHTINK_WCHAR_FILENAMES
			uint32 attribs = ::GetFileAttributesW(filename);
#else
			uint32 attribs = ::GetFileAttributesA(filename);
#endif
			return (attribs != INVALID_FILE_ATTRIBUTES && !(attribs & FILE_ATTRIBUTE_DIRECTORY));
#else //common linux/unix all have the stat system call
			struct stat buffer;
			return (::stat (filename, &buffer) == 0);
#endif
		}

		static bool is_absolute_path(const FileCharType * path);

		static size_t file_size(FILE * fp);

		static bool enum_dir_files(const FileCharType * path, vector<FileNameType>::type & outFiles, bool dir, bool recursion);
		
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

#if defined(_WIN32) && defined(LIGHTINK_WCHAR_FILENAMES)
#define LIGHTINK_FILENAME_T(s) L ## s
#else
#define LIGHTINK_FILENAME_T(s) s
#endif

#if defined(_WIN32) && defined(LIGHTINK_WCHAR_FILENAMES)
#define LIGHTINK_STRFUNC_T(s) wcs ## s
#else
#define LIGHTINK_STRFUNC_T(s) str ## s
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