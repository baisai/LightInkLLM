

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

#ifndef LIGHTINK_LOG_LOGCHANNELFILE_H_
#define LIGHTINK_LOG_LOGCHANNELFILE_H_

#include "Log/channel/LogChannelMT.h"
#include "Log/helper/FileHelper.h"
#include "Common/TypeTool/TypeTool.h"

namespace LightInk
{
	//////////////////////////////////////////////////////////////////////////////
	// LogChannelFile
	/////////////////////////////////////////////////////////////////////////////
	template <typename M>
	class LIGHTINK_TEMPLATE_DECL LogChannelFile : public LogChannelMT<M>
	{
	public:
		LogChannelFile(const OsHelper::FileNameType & name, bool cover = false) { m_file.open(name, cover); }
		virtual ~LogChannelFile() { m_file.close(); }


	protected:
		virtual RuntimeError do_log(const LogItem & item)
		{
			return m_file.write(item);
		}
		virtual RuntimeError do_flush()
		{
			return m_file.flush();
		}

		FileHelper m_file;

	LIGHTINK_DISABLE_COPY(LogChannelFile)
	};


	//////////////////////////////////////////////////////////////////////////////
	// LogChannelBlockFile
	/////////////////////////////////////////////////////////////////////////////
	template <typename M>
	class LIGHTINK_TEMPLATE_DECL LogChannelBlockFile : public LogChannelMT<M>
	{
	public:
		LogChannelBlockFile(const OsHelper::FileNameType & filename, size_t maxSize, size_t maxFiles) : 
			m_filename(filename), m_maxSize(maxSize), m_maxFiles(maxFiles), m_currSize(0), m_currIndex(0)
		{
			size_t ext = m_filename.find_last_of('.');
			if (ext != OsHelper::FileNameType::npos)
			{
				m_extname = m_filename.substr(ext, OsHelper::FileNameType::npos);
			}
			m_file.open(calc_filename(0), false); 
			m_currSize = m_file.size();
		}
		virtual ~LogChannelBlockFile() { m_file.close(); }


	protected:
		virtual inline RuntimeError do_log(const LogItem & item)
		{
			m_currSize += item.m_format.size();
			if (m_currSize > m_maxSize)
			{
				RuntimeError err = confirm_file();
				if (err != RE_Success) return err;
				m_currSize = item.m_format.size();
			}
			return m_file.write(item);
		}
		virtual inline RuntimeError do_flush()
		{
			return m_file.flush();
		}

		OsHelper::FileNameType calc_filename(size_t index)
		{
			if (index) 
			{
				TypeSelect<IsSameType<OsHelper::FileCharType, char>::Result, fmt::MemoryWriter, fmt::WMemoryWriter>::Result w;
				w.write(LIGHTINK_FILENAME_T("{}.{}{}"), m_filename, index, m_extname);
				return w.str();
			}
			return m_filename;
		}

		RuntimeError confirm_file()
		{
			if (m_file.close() != RE_Success) return RE_Log_FileFailed;
			for (size_t i = m_currIndex + 1; i <= m_maxFiles; ++i)
			{
				OsHelper::FileNameType name = calc_filename(i);
				if (!OsHelper::file_exists(name.c_str()))
				{
					m_currIndex = i;
					break;
				}
			}

			for (size_t i = m_currIndex; i > 0; --i)
			{
				OsHelper::FileNameType src = calc_filename(i - 1);
				OsHelper::FileNameType target = calc_filename(i);
				if (OsHelper::file_exists(target.c_str()))
				{
					if (OsHelper::remove(target.c_str()) != 0)
					{
						return RE_Log_FileFailed;
					}
				}
				if (OsHelper::file_exists(src.c_str()) && OsHelper::rename(src.c_str(), target.c_str()))
				{
					return RE_Log_FileFailed;
				}
			}

			return m_file.reopen(true);
		}

		OsHelper::FileNameType m_filename;
		OsHelper::FileNameType m_extname;
		size_t m_maxSize;
		size_t m_maxFiles;
		size_t m_currSize;
		size_t m_currIndex;
		FileHelper m_file;

	LIGHTINK_DISABLE_COPY(LogChannelBlockFile)
	};

	//////////////////////////////////////////////////////////////////////////////
	// LogChannelDateFile
	/////////////////////////////////////////////////////////////////////////////
	template <typename M, typename DateNameStrategy>
	class LIGHTINK_TEMPLATE_DECL LogChannelDateFile : public LogChannelMT<M>
	{
	public:
		LogChannelDateFile(const OsHelper::FileNameType & filename) : m_filename(filename)
		{
			size_t ext = m_filename.find_last_of('.');
			if (ext != OsHelper::FileNameType::npos)
			{
				m_extname = m_filename.substr(ext, OsHelper::FileNameType::npos);
			}
			m_file.open(m_filename, false); 
			m_nextTime = DateNameStrategy::get_next_time(time(NULL));
		}
		virtual ~LogChannelDateFile() { m_file.close(); }


	protected:
		virtual inline RuntimeError do_log(const LogItem & item)
		{
			if (time(NULL) >= m_nextTime)
			{
				m_file.close();
				OsHelper::FileNameType target = DateNameStrategy::calc_filename(m_filename, m_nextTime, m_extname);
				if (OsHelper::file_exists(target.c_str()))
				{
					OsHelper::remove(target.c_str());
				}
				OsHelper::rename(m_filename.c_str(), target.c_str());
				m_nextTime = DateNameStrategy::get_next_time(m_nextTime);
				m_file.reopen(true);
			}
			return m_file.write(item);
		}
		virtual inline RuntimeError do_flush()
		{
			return m_file.flush();
		}


		OsHelper::FileNameType m_filename;
		OsHelper::FileNameType m_extname;
		time_t m_nextTime;
		FileHelper m_file;

	LIGHTINK_DISABLE_COPY(LogChannelDateFile)
	};

}

#endif