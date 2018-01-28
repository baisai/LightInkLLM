/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_C_CORE_H
#define TURF_C_CORE_H


#include "platform_detect.h"

// clang-format off

//---------------------------------------------
// Global system headers
//---------------------------------------------
#if TURF_TARGET_XBOX_360    // Xbox 360
    #include <xtl.h>
#elif TURF_TARGET_WIN32 || TURF_TARGET_MINGW     // Win32 API
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <windows.h>
    #undef WIN32_LEAN_AND_MEAN
    #undef NOMINMAX
#endif


//---------------------------------------------
// Macros
//---------------------------------------------
#if TURF_COMPILER_MSVC
    
	#include <intrin.h>

	//-------------------------------------
	//  Alignment
	//-------------------------------------
	#define TURF_DECL_ALIGNED(declaration, amt) __declspec(align(amt)) declaration

	//-------------------------------------
	//  Inlining
	//-------------------------------------
	#ifdef __cplusplus
	#define TURF_C_INLINE inline
	#else
	#define TURF_C_INLINE __inline
	#endif

	#define TURF_FORCE_INLINE __forceinline
	#define TURF_NO_INLINE __declspec(noinline)

	//-------------------------------------
	//  Thread local
	//-------------------------------------
	#define TURF_THREAD_LOCAL __declspec(thread)

	//-------------------------------------
	//  Debug break
	//-------------------------------------
	#define TURF_DEBUG_BREAK() __debugbreak()

	TURF_C_INLINE void turf_yieldHWThread() {
		YieldProcessor();
	}
	
	TURF_C_INLINE void turf_sleepMillis(int millis)
	{
		Sleep(millis);
	}
	
	TURF_C_INLINE DWORD turf_getCurrentThreadID() 
	{
#if TURF_CPU_X64                                 // Windows x64
        return ((DWORD*) __readgsqword(48))[18]; // Read directly from the TIB
#elif TURF_CPU_X86 // Windows x86
        return ((DWORD*) __readfsdword(24))[9]; // Read directly from the TIB
#else
        return GetCurrentThreadID();
#endif
    }
	
	TURF_C_INLINE uint32_t turf_getCurrentProcessID()
	{
#if TURF_TARGET_XBOX_360  // Xbox 360
		return 0;
#elif TURF_CPU_X64        // Windows x64
		return ((uint32_t*) __readgsqword(48))[16]; // Read directly from the TIB
#elif TURF_CPU_X86        // Windows x86
		return ((uint32_t*) __readfsdword(24))[8];  // Read directly from the TIB
#endif
	}

#elif TURF_COMPILER_GCC
	#include<pthread.h>
	#include <unistd.h>
	//-------------------------------------
	//  Alignment
	//-------------------------------------
	// Note: May not work on local variables.
	// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=24691
	#define TURF_DECL_ALIGNED(declaration, amt) declaration __attribute__((aligned(amt)))

	//-------------------------------------
	//  Inlining
	//-------------------------------------
	#define TURF_C_INLINE static inline
	#define TURF_FORCE_INLINE inline __attribute__((always_inline))
	#define TURF_NO_INLINE __attribute__((noinline))

	//-------------------------------------
	//  Thread local
	//-------------------------------------
	#define TURF_THREAD_LOCAL __thread
	
	#define TURF_DEBUG_BREAK() __builtin_trap()

	//-------------------------------------
	//  CPU intrinsics
	//-------------------------------------
	TURF_C_INLINE void turf_yieldHWThread() {
	#if TURF_CPU_X86 || TURF_CPU_X64
		// Only implemented on x86/64
		asm volatile("pause");
	#endif
	}

	TURF_C_INLINE void turf_sleepMillis(int millis)
	{
		struct timespec ts;
		ts.tv_sec = millis / 1000;
		ts.tv_nsec = (millis % 1000) * 1000 * 1000;
		while (nanosleep(&ts, &ts) < 0) continue;
	}
	
	TURF_C_INLINE size_t turf_getCurrentThreadID() 
	{
        // FIXME: On Linux, would the kernel task ID be more useful for debugging?
        // If so, detect NPTL at compile time and create TID_NPTL.h which uses gettid() instead.
#ifdef TURF_KERNEL_FREEBSD
		return pthread_getthreadid_np();
#elif TURF_TARGET_MINGW
        return pthread_self().p;
#else
        return pthread_self();
#endif
    }
	
	TURF_C_INLINE pid_t turf_getCurrentProcessID()
	{
		return getpid();
	}

#else
    #error "Unsupported platform!"
#endif

//---------------------------------------------
// Integer types
//---------------------------------------------
#include <stddef.h>
#if defined(_MSC_VER) && _MSC_VER < 1600
# include "Common/stdint_msvc2008.h"
#else
# include <stdint.h>
#endif

// FIXME: Check PowerPC, where registers might be larger than pointers.
typedef intptr_t intreg_t;
typedef uintptr_t uintreg_t;
#define INTREG_MIN INTPTR_MIN
#define INTREG_MAX INTPTR_MAX
#define UINTREG_MAX UINTPTR_MAX


#endif // TURF_C_CORE_H
