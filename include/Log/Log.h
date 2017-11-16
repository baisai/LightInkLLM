


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

#ifndef LIGHTINK_COMMON_LOG_H_
#define LIGHTINK_COMMON_LOG_H_

#include "Common/Config.h"
#include "Log/LoggerMgr.h"
#include "Log/helper/LogTraceHelper.h"

/////////////////////////////////
//系统日志
////////////////////////////////

////////////////////////////
//Trace
///////////////////////////
#ifdef LightInkNoTrace
#define LogTrace(ft, ...)
#define LogTraceEnd
#ifndef LightInkNoTraceStepCall
#define LightInkNoTraceStepCall
#endif
#else
#define LogTrace(name, ft, ...) \
		fmt::MemoryWriter __##name_mess; \
		__##name_mess.write("["#name"]"); \
		__##name_mess.write(ft, ##__VA_ARGS__); \
		LightInk::LogTraceHelper __##name_trace(__FILE__, __LINE__, &__##name_mess)
#define LogTraceEnd \
		do {__##name_trace.set_line(__LINE__); __##name_trace.log_out(); } while (0)
#endif

#ifdef LightInkNoTraceStepCall
#define LogTraceStepCall(ft, ...)
#define LogTraceStepReturn(v) return v
#define LogTraceStepReturnVoid return
#else
#define LogTraceStepCall(ft, ...) \
	fmt::MemoryWriter ___mess; \
	___mess.write(ft, ##__VA_ARGS__); \
	LightInk::LogTraceHelper ___trace(__FILE__, __LINE__, &___mess)
#define LogTraceStepReturn(v) \
	do {___trace.set_line(__LINE__); return v; } while (0)
#define LogTraceStepReturnVoid \
	do {___trace.set_line(__LINE__); return; } while (0)
#endif

////////////////////////
//Debug
///////////////////////
#ifdef LightInkNoDebug
#define LogDebug(ft, ...)
#else
#define LogDebug(ft, ...) \
	do  \
	{ \
		LightInkLogCpp->debug_fl(LightInkLogFileLine, ft, ##__VA_ARGS__); \
	} while (0) 

#endif

////////////////////////
//Message
///////////////////////
#ifdef LightInkNoMessage
#define LogMessage(ft, ...)
#else
#define LogMessage(ft, ...) \
	do  \
	{ \
		LightInkLogCpp->message_fl(LightInkLogFileLine, ft, ##__VA_ARGS__); \
	} while (0) 

#endif

////////////////////////
//Warning
///////////////////////
#ifdef LightInkNoWarning
#define LogWarning(ft, ...)
#else
#define LogWarning(ft, ...) \
	do  \
	{ \
		LightInkLogCpp->warning_fl(LightInkLogFileLine, ft, ##__VA_ARGS__); \
	} while (0) 

#endif


////////////////////////
//Error
///////////////////////
#ifdef LightInkNoError
#define LogError(ft, ...)
#else
#define LogError(ft, ...) \
	do  \
	{ \
		LightInkLogCpp->error_fl(LightInkLogFileLine, ft, ##__VA_ARGS__); \
	} while (0)

#endif

////////////////////////
//Fatal
///////////////////////
#ifdef LightInkNoFatal
#define LogFatal(ft, ...)
#else
#define LogFatal(ft, ...) \
	do  \
	{ \
		LightInkLogCpp->fatal_fl(LightInkLogFileLine, ft, ##__VA_ARGS__); \
	} while (0)

#endif



////////////////////////
//ScriptDebug
///////////////////////
#ifdef LightInkNoScriptDebug
#define LogScriptDebug(ft, ...)
#else
#define LogScriptDebug(ft, ...) \
	do  \
	{ \
		LightInkLogLua->debug_fl(LightInkLogFileLine, ft, ##__VA_ARGS__); \
	} while (0)
#endif



////////////////////////
//ScriptMessage
///////////////////////
#ifdef LightInkNoScriptMessage
#define LogScriptMessage(ft, ...)
#else
#define LogScriptMessage(ft, ...) \
	do  \
	{ \
		LightInkLogLua->message_fl(LightInkLogFileLine, ft, ##__VA_ARGS__); \
	} while (0)
#endif

////////////////////////
//ScriptWarning
///////////////////////
#ifdef LightInkNoScriptWarning
#define LogScriptWarning(ft, ...)
#else
#define LogScriptWarning(ft, ...) \
	do  \
	{ \
		LightInkLogLua->warning_fl(LightInkLogFileLine, ft, ##__VA_ARGS__); \
	} while (0)
#endif


////////////////////////
//ScriptError
///////////////////////
#ifdef LightInkNoScriptError
#define LogScriptError(ft, ...)
#else
#define LogScriptError(ft, ...) \
	do  \
	{ \
		LightInkLogLua->error_fl(LightInkLogFileLine, ft, ##__VA_ARGS__); \
	} while (0)
#endif


////////////////////////
//Fatal
///////////////////////
#ifdef LightInkNoScriptFatal
#define LogScriptFatal(ft, ...)
#else
#define LogScriptFatal(ft, ...) \
	do  \
	{ \
		LightInkLogLua->fatal_fl(LightInkLogFileLine, ft, ##__VA_ARGS__); \
	} while (0)

#endif


/////////////////////////
//ScriptError and Jump
////////////////////////
#ifdef LightInkNoScriptErrorJump
#define LogScriptErrorJump(L, ft, ...) LogScriptError(ft, ##__VA_ARGS__)
#else
#define LogScriptErrorJump(L, ft, ...)  \
		do \
		{ \
			fmt::MemoryWriter ___mess; \
			___mess.write(ft, ##__VA_ARGS__); \
			luaL_error(L, ___mess.c_str()); \
		} while (0)

#endif

#endif