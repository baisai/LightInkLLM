/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#ifndef TURF_C_IMPL_ATOMIC_GCC_ARM_H
#define TURF_C_IMPL_ATOMIC_GCC_ARM_H

//-------------------------------------
//  Fences
//-------------------------------------
// clang-format off
#define turf_signalFenceConsume() (0)
#define turf_signalFenceAcquire() asm volatile("" ::: "memory")
#define turf_signalFenceRelease() asm volatile("" ::: "memory")
#define turf_signalFenceSeqCst() asm volatile("" ::: "memory")

#define turf_threadFenceConsume() (0)
#if TURF_CPU_ARM_VERSION == 7
    #define turf_threadFenceAcquire() asm volatile("dmb ish" ::: "memory")
    #define turf_threadFenceRelease() asm volatile("dmb ish" ::: "memory")
    #define turf_threadFenceSeqCst() asm volatile("dmb ish" ::: "memory")
#elif TURF_CPU_ARM_VERSION == 6
    // dmb instruction does not exist in ARMv6 and lower.
    // mcr instruction can accomplish the same thing.
    #if TURF_CPU_ARM_THUMB
        // When compiling for ARMv6 in Thumb mode, the mcr instruction is not available.
        // We need to switch to ARM mode, by calling standalone functions, to use this instruction.
        // As non-inlined function calls, they function as compiler barriers as well.
		#define turf_threadFenceAcquire() std::atomic_thread_fence(std::memory_order_acquire)
        #define turf_threadFenceRelease() std::atomic_thread_fence(std::memory_order_release)
        #define turf_threadFenceSeqCst() std::atomic_thread_fence(std::memory_order_seq_cst)
		
    #else
        // Do we really need to set the source register to 0?
        #define turf_threadFenceAcquire() asm volatile("mcr p15, 0, %0, c7, c10, 5" :: "r"(0) : "memory")
        #define turf_threadFenceRelease() asm volatile("mcr p15, 0, %0, c7, c10, 5" :: "r"(0) : "memory")
        #define turf_threadFenceSeqCst() asm volatile("mcr p15, 0, %0, c7, c10, 5" :: "r"(0) : "memory")
    #endif
#endif
// clang-format on

#define TURF_ATOMIC_NOSUPPORT


#endif // TURF_C_IMPL_ATOMIC_GCC_ARM_H
