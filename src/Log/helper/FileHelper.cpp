

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


#include "Log/helper/FileHelper.h"


namespace LightInk
{
	RuntimeError FileHelper::open(const OsHelper::FileNameType & name, bool cover)
	{
		RuntimeError err = close();
		if (err != RE_Success) return err;
		const OsHelper::FileCharType * mode = cover ? LIGHTINK_FILENAME_T("wb") : LIGHTINK_FILENAME_T("ab");
		m_filename = name;
		m_fp = OsHelper::fopen(m_filename.c_str(), mode);
		if (!m_fp) err = RE_Log_FileFailed;
		return err;
	}
	RuntimeError FileHelper::flush()
	{
		if (fflush(m_fp) == EOF) { return RE_Log_FileFailed; }
		return RE_Success;
	}
	RuntimeError FileHelper::close()
	{
		if (m_fp)
		{
			if (fclose(m_fp) == EOF) { return RE_Log_FileFailed; }
			m_fp = NULL;
		}
		return RE_Success;
	}
	RuntimeError FileHelper::write(const LogItem & item)
	{
		size_t count = fwrite(item.m_format.data(), sizeof(char), item.m_format.size(), m_fp);
		if (count == sizeof(char) * item.m_format.size()) { return RE_Success; }
		return RE_Log_FileFailed;
	}
	size_t FileHelper::size()
	{
		return OsHelper::file_size(m_fp);
	}
	const OsHelper::FileNameType & FileHelper::file_name() const
	{
		return m_filename;
	}
	bool FileHelper::is_open()
	{
		return m_fp != NULL;
	}

	RuntimeError FileHelper::reopen(bool cover)
	{
		return open(m_filename, cover);
	}
}