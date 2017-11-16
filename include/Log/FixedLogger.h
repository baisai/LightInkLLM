

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

#ifndef LIGHTINK_LOG_FIXEDLOGGER_H_
#define LIGHTINK_LOG_FIXEDLOGGER_H_

#include "Common/SingletonBase.h"
#include "Log/Logger.h"

namespace LightInk
{
	template <int32 Idx>
	class LIGHTINK_TEMPLATE_DECL FixedLogger : public SingletonBase, public SmallObject
	{
	private:
		FixedLogger();
		~FixedLogger();
		Logger * get_logger();
		bool init(const LogOption & op);

		void destroy();

		void set_backup(FixedLogger<Idx> ** fl);
		
	private:
		Logger * m_logger;
		LoggerPtr m_loggerPtr;

		FixedLogger<Idx> ** m_backup;

		friend class LoggerMgr;

	LIGHTINK_DISABLE_COPY(FixedLogger)
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <int32 Idx>
	FixedLogger<Idx>::FixedLogger() : m_logger(NULL), m_backup(NULL) {  }

	template <int32 Idx>
	FixedLogger<Idx>::~FixedLogger() {  }

	template <int32 Idx>
	inline Logger * FixedLogger<Idx>::get_logger() { return m_logger; }

	template <int32 Idx>
	inline bool FixedLogger<Idx>::init(const LogOption & op) 
	{ 
		if (m_logger) { return false; }
		m_logger = new Logger(op);
		m_loggerPtr.reset(m_logger);
		return true;
	}

	template <int32 Idx>
	inline void FixedLogger<Idx>::destroy() 
	{ 
		if (m_logger)
		{ 
			m_logger = NULL;
			m_loggerPtr.reset();
		}
		if (m_backup)
		{
			*m_backup = NULL;
			m_backup = NULL;
		}
	}

	template <int32 Idx>
	inline void FixedLogger<Idx>::set_backup(FixedLogger<Idx> ** fl) { m_backup = fl; }

}

#endif