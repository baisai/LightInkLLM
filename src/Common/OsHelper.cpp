

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

#include "Common/OsHelper.h"

namespace LightInk
{
	void OsHelper::gettimeofday(struct timeval * tv)
	{
#ifdef _WIN32
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define delta 116444736000000000Ui64
#   else
#define delta 116444736000000000ULL
#   endif
#define secOffSet 10000000u
		if (tv != nullptr) 
		{
			FILETIME fileTime;
			GetSystemTimeAsFileTime(&fileTime);
			uint64 present = fileTime.dwHighDateTime;
			present <<= 32;
			present |= fileTime.dwLowDateTime;
			// Subtract the difference
			present -= delta;
			tv->tv_sec = static_cast<long>(present / secOffSet);
			tv->tv_usec = static_cast<long>((present % secOffSet) / 10u);
		}
#undef delta
#undef secOffSet
#else
		::gettimeofday(tv, NULL);
#endif
	}

	bool OsHelper::is_absolute_path(const FileCharType * path)
	{
		if (!path || path[0] == LIGHTINK_FILENAME_T('\0'))
		{
			return false;
		}
		return path[0] == LIGHTINK_FILENAME_T('/') || ::LIGHTINK_STRFUNC_T(chr)(path, LIGHTINK_FILENAME_T(':'));
	}

	size_t OsHelper::file_size(FILE * fp)
	{
		if (fp == NULL) return 0;
#ifdef _WIN32
		int fd = _fileno(fp);
#if _WIN64 //64 bits
		struct _stat64 st;
		if (_fstat64(fd, &st) == 0)
			return st.st_size;

#else //windows 32 bits
		struct _stat st;
		if (_fstat(fd, &st) == 0)
			return st.st_size;
#endif

#else // unix
		int fd = fileno(fp);
		//64 bits(but not in osx, where fstat64 is deprecated)
#if !defined(__FreeBSD__) && !defined(__APPLE__) && (defined(__x86_64__) || defined(__ppc64__))
		struct stat64 st;
		if (fstat64(fd, &st) == 0)
			return st.st_size;
#else // unix 32 bits or osx
		struct stat st;
		if (fstat(fd, &st) == 0)
			return st.st_size;
#endif
#endif
		return 0;
	}

	bool OsHelper::enum_dir_files(const FileCharType * path, vector<FileNameType>::type & outFiles, bool dir, bool recursion)
	{
		if (!path || path[0] == '\0')
		{
			return false;
		}
		FileNameType filePath = path;
		char lastChar = filePath[filePath.size() - 1];
		if (lastChar != '/' && lastChar != '\\')
		{
			filePath.push_back('/');
		}
#ifdef _WIN32
		filePath.append("*.*");
		WIN32_FIND_DATA findFileData;  
		HANDLE hFind = FindFirstFile(filePath.c_str(), &findFileData);  
		if (hFind != INVALID_HANDLE_VALUE)  
		{  
			do   
			{  
				if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)  
				{  
					continue;  
				}
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  
				{
					if (dir)
					{
						outFiles.push_back(findFileData.cFileName);
					}
					if (recursion)
					{
						FileNameType fullPath = filePath;
						fullPath.resize(fullPath.size() - 3);
						fullPath.append(findFileData.cFileName);
						vector<FileNameType>::type subFiles;
						bool result = enum_dir_files(fullPath.c_str(), subFiles, dir, recursion);
						if (result)
						{
							FileNameType subFile(findFileData.cFileName);
							subFile.push_back('/');
							size_t len = subFile.size();
							for (size_t i = 0; i < subFiles.size(); ++i)
							{
								subFile.resize(len);
								subFile.append(subFiles[i]);
								outFiles.push_back(subFile);
							}
						}
						else
						{
							return result;
						}
					}
				}  
				else  
				{
					outFiles.push_back(findFileData.cFileName);
				}  
			} while (FindNextFile(hFind, &findFileData));
		}
		return true;
#else
		DIR * pDir = opendir(filePath.c_str());  
		if (pDir == NULL)  
		{  
			return false;  
		}
		struct dirent * pDirent = NULL;
		while ((pDirent = readdir(pDir)) != NULL)
		{
			if (strcmp(pDirent->d_name, ".") == 0 || strcmp(pDirent->d_name, "..") == 0)  
			{  
				continue;  
			}
			if (pDirent->d_type == DT_DIR)  
			{
				if (dir)
				{
					outFiles.push_back(pDirent->d_name);
				}
				if (recursion)
				{
					FileNameType fullPath = filePath;
					fullPath.append(pDirent->d_name);
					vector<FileNameType>::type subFiles;
					bool result = enum_dir_files(fullPath.c_str(), subFiles, dir, recursion);
					if (result)
					{
						FileNameType subFile(pDirent->d_name);
						subFile.push_back('/');
						size_t len = subFile.size();
						for (size_t i = 0; i < subFiles.size(); ++i)
						{
							subFile.resize(len);
							subFile.append(subFiles[i]);
							outFiles.push_back(subFile);
						}
					}
					else
					{
						return result;
					}
				}
			}  
			else  
			{
				outFiles.push_back(pDirent->d_name);
			}
		}
		closedir(pDir);
		return true;
#endif
	}

	int OsHelper::utc_minutes_offset(const tm & t)
	{

#ifdef _WIN32
#if _WIN32_WINNT < _WIN32_WINNT_WS08
		TIME_ZONE_INFORMATION tzinfo;
		uint32 rv = GetTimeZoneInformation(&tzinfo);
#else
		DYNAMIC_TIME_ZONE_INFORMATION tzinfo;
		uint32 rv = GetDynamicTimeZoneInformation(&tzinfo);
#endif
		if (rv == TIME_ZONE_ID_INVALID) return 0;

		int offset = -tzinfo.Bias;
		if (t.tm_isdst)
			offset -= tzinfo.DaylightBias;
		else
			offset -= tzinfo.StandardBias;
		return offset;
#else

#if defined(sun) || defined(__sun)
		// 'tm_gmtoff' field is BSD extension and it's missing on SunOS/Solaris
		struct sun_helper
		{
			static long int calculate_gmt_offset(const tm & localtm, const tm & gmtm))
			{
				int local_year = localtm.tm_year + (1900 - 1);
				int gmt_year = gmtm.tm_year + (1900 - 1);

				long int days = (
									// difference in day of year
									localtm.tm_yday - gmtm.tm_yday

									// + intervening leap days
									+ ((local_year >> 2) - (gmt_year >> 2))
									- (local_year / 100 - gmt_year / 100)
									+ ((local_year / 100 >> 2) - (gmt_year / 100 >> 2))

									// + difference in years * 365 */
									+ (long int)(local_year - gmt_year) * 365
								);

				long int hours = (24 * days) + (localtm.tm_hour - gmtm.tm_hour);
				long int mins = (60 * hours) + (localtm.tm_min - gmtm.tm_min);
				long int secs = (60 * mins) + (localtm.tm_sec - gmtm.tm_sec);

				return secs;
			}
		};

		long int offset_seconds = sun_helper::calculate_gmt_offset(t, gmtime(mktime(t)));
#else
		long int offset_seconds = t.tm_gmtoff;
#endif

		return static_cast<int>(offset_seconds / 60);
#endif
	}

	string OsHelper::filename_to_string(const FileNameType & filename)
	{
#if defined(_WIN32) && defined(LIGHTINK_WCHAR_FILENAMES)
		int destLen = WideCharToMultiByte( CP_OEMCP, NULL , filename.c_str(), filename.length(), NULL , NULL, NULL, NULL );
		char * pDest = new char[destLen +1];
		WideCharToMultiByte(CP_OEMCP , NULL, filename.c_str(), filename.length() , pDest, destLen, NULL , NULL);
		pDest[destLen] = '\0';
		string dest( pDest);
		delete [] pDest;
		return dest;
#else
		return filename;
#endif
	}

	OsHelper::FileNameType OsHelper::string_to_filename(const string & str)
	{
#if defined(_WIN32) && defined(LIGHTINK_WCHAR_FILENAMES)
		int destLen = MultiByteToWideChar( CP_ACP, NULL , str.c_str(), -1, NULL, NULL);
		wchar_t * pDest = new wchar_t[destLen+1];
		MultiByteToWideChar(CP_ACP , NULL, str.c_str(), -1, pDest, destLen);
		pDest[destLen] = L'\0';
		wstring dest( pDest);
		delete [] pDest;
		return dest;
#else
		return str;
#endif
	}

	string OsHelper::errno_str(int err_num)
	{
		char buf[256];
		size_t buf_size = sizeof(buf);

#ifdef _WIN32
		if(::strerror_s(buf, buf_size, err_num) == 0)
			return string(buf);
		else
			return string("Unkown error");

#elif defined(__FreeBSD__) || defined(__APPLE__) || defined(__ANDROID__) || \
			((_POSIX_C_SOURCE >= 200112L) && ! _GNU_SOURCE) // posix version

		if (::strerror_r(err_num, buf, buf_size) == 0)
			return string(buf);
		else
			return string("Unkown error");

#else  // gnu version (might not use the given buf, so its retval pointer must be used)
		return string(::strerror_r(err_num, buf, buf_size));
#endif
	}


#ifdef _WIN32
	static long long get_frequency()
	{
		LARGE_INTEGER frequency;
		if (QueryPerformanceFrequency(&frequency))
			return frequency.QuadPart;
		return 0;
	}
	static long long frequency = get_frequency();
#endif

	uint64 OsHelper::get_up_time()
	{
#ifdef _WIN32
		if (frequency > 0)
		{
			LARGE_INTEGER counter;
			QueryPerformanceCounter(&counter);
			return counter.QuadPart * 1000 / frequency;
		}
		else
		{
			return GetTickCount();
		}
#else
		struct timespec ts = { 0, 0 };
		clock_gettime(CLOCK_MONOTONIC, &ts);
		return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
	}

	uint64 OsHelper::get_used_virtual()
	{
#ifdef _WIN32
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);
		return memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
#else
		struct sysinfo memInfo;
		sysinfo(&memInfo);
		uint64 virtualMemUsed = memInfo.totalram - memInfo.freeram;
		//Add other values in next statement to avoid int overflow on right hand side...
		virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
		virtualMemUsed *= memInfo.mem_unit;

		return virtualMemUsed;
#endif
	}
		
	uint64 OsHelper::get_total_virtual()
	{
#ifdef _WIN32
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);

		return memInfo.ullTotalPageFile;
#else
		struct sysinfo memInfo;
		sysinfo(&memInfo);
		uint64 totalVirtualMem = memInfo.totalram;
		//Add other values in next statement to avoid int overflow on right hand side...
		totalVirtualMem += memInfo.totalswap;
		totalVirtualMem *= memInfo.mem_unit;

		return totalVirtualMem;
#endif
	}

	uint64 OsHelper::get_used_physical()
	{
#ifdef _WIN32
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);

		return memInfo.ullTotalPhys - memInfo.ullAvailPhys;
#else
		struct sysinfo memInfo;
		sysinfo(&memInfo);
		uint64 physMemUsed = memInfo.totalram - memInfo.freeram;
		//Multiply in next statement to avoid int overflow on right hand side...
		physMemUsed *= memInfo.mem_unit;

		return physMemUsed;
#endif
	}

	uint64 OsHelper::get_total_physical()
	{
#ifdef _WIN32
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);

		return memInfo.ullTotalPhys;
#else
		struct sysinfo memInfo;
		sysinfo(&memInfo);
		long long totalPhysMem = memInfo.totalram;
		//Multiply in next statement to avoid int overflow on right hand side...
		totalPhysMem *= memInfo.mem_unit;

		return totalPhysMem;
#endif
	}


}
