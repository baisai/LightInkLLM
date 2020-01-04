


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

#ifndef LIGHTINK_COMMON_CONFIG_H_
#define LIGHTINK_COMMON_CONFIG_H_

#if defined WIN32 && !defined WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#if defined(DEBUG) || defined(_DEBUG) || !defined(NDEBUG)
#define LIGHTINK_DEBUG
#endif // DEBUG

//#define LightInkLogTraceStepCall //开启每次Call的trace日志

#define LightInkLogTrace //开启trace日志

#ifdef LIGHTINK_DEBUG
#define LightInkLogDebug //开启debug日志
#endif

#define LightInkLogMessage
#define LightInkLogWarning
#define LightInkLogError
#define LightInkLogFatal

#ifdef LIGHTINK_DEBUG
#define LightInkLogScriptDebug
#endif

#define LightInkLogScriptMessage
#define LightInkLogScriptWarning
#define LightInkLogScriptError
#define LightInkLogScriptErrorJump
#define LightInkLogScriptFatal


#define LIGHTINK_SHOWVERSION


#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif


#if defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64) || defined(__aarch64__) || defined(__ppc64__)
#define LIGHTINK_X64
#endif

//#ifndef LIGHTINK_LUAJIT
//#define LIGHTINK_LUAJIT
//#endif


/////////////////////////////////////////////////////////
//导出,导入变量
////////////////////////////////////////////////////////
#ifdef LIGHTINK_AS_DLL

#ifdef LIGHTINK_EXPORTS

#ifdef WIN32
#define LIGHTINK_DECL __declspec(dllexport)
#define LIGHTINK_TEMPLATE_DECL //__declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define LIGHTINK_DECL __attribute__((visibility ("default")))
#define LIGHTINK_TEMPLATE_DECL
#else
#define LIGHTINK_DECL
#define LIGHTINK_TEMPLATE_DECL
#endif

#else
#ifdef WIN32
#define LIGHTINK_DECL __declspec(dllimport)
#define LIGHTINK_TEMPLATE_DECL //__declspec(dllexport)
#else
#define LIGHTINK_DECL
#define LIGHTINK_TEMPLATE_DECL
#endif

#endif

#else

#ifdef LIGHTINK_EXPORTS

#define LIGHTINK_DECL
#define LIGHTINK_TEMPLATE_DECL

#else

#define LIGHTINK_DECL
#define LIGHTINK_TEMPLATE_DECL

#endif
	
	
#endif // LIGHTINK_BUILD_DLL



#endif