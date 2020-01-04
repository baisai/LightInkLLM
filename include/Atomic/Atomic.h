

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

#ifndef LIGHTINK_THREAD_ATOMIC_H_
#define LIGHTINK_THREAD_ATOMIC_H_

#include "Common/Type.h"
#include "Atomic/Turf/turf.h"


#ifndef TURF_ATOMIC_NOSUPPORT

namespace LightInk
{
	template <typename T>
	class LIGHTINK_TEMPLATE_DECL Atomic;

	template <>
	class LIGHTINK_TEMPLATE_DECL Atomic<uint8>
	{
	public:
		Atomic() { store(0); }
		inline uint8 load() const { return turf_load8Relaxed(&m_handle); }
		inline void store(uint8 desired) { turf_store8Relaxed(&m_handle, desired); }
		inline uint8 exchange(uint8 desired) { return turf_exchange8Relaxed(&m_handle, desired); }
		inline bool compare_exchange(uint8 expected, uint8 desired) { return turf_compareExchange8Relaxed(&m_handle, expected, desired) == expected; }
		inline uint8 fetch_add(int8 operand) { return turf_fetchAdd8Relaxed(&m_handle, operand); }
		inline uint8 fetch_sub(int8 operand) { return turf_fetchAdd8Relaxed(&m_handle, -operand); }
		inline uint8 fetch_or(uint8 operand) { return turf_fetchOr8Relaxed(&m_handle, operand); }
		inline uint8 fetch_and(uint8 operand) { return turf_fetchAnd8Relaxed(&m_handle, operand); }

	private:
		turf_atomic8_t m_handle;
		LIGHTINK_DISABLE_COPY(Atomic)
	};

	template <>
	class LIGHTINK_TEMPLATE_DECL Atomic<uint16>
	{
	public:
		Atomic() { store(0); }
		inline uint16 load() const { return turf_load16Relaxed(&m_handle); }
		inline void store(uint16 desired) { turf_store16Relaxed(&m_handle, desired); }
		inline uint16 exchange(uint16 desired) { return turf_exchange16Relaxed(&m_handle, desired); }
		inline bool compare_exchange(uint16 expected, uint16 desired) { return turf_compareExchange16Relaxed(&m_handle, expected, desired) == expected; }
		inline uint16 fetch_add(int16 operand) { return turf_fetchAdd16Relaxed(&m_handle, operand); }
		inline uint16 fetch_sub(int16 operand) { return turf_fetchAdd16Relaxed(&m_handle, -operand); }
		inline uint16 fetch_or(uint16 operand) { return turf_fetchOr16Relaxed(&m_handle, operand); }
		inline uint16 fetch_and(uint16 operand) { return turf_fetchAnd16Relaxed(&m_handle, operand); }

	private:
		turf_atomic16_t m_handle;
		LIGHTINK_DISABLE_COPY(Atomic)
	};

	template <>
	class LIGHTINK_TEMPLATE_DECL Atomic<uint32>
	{
	public:
		Atomic() { store(0); }
		inline uint32 load() const { return turf_load32Relaxed(&m_handle); }
		inline void store(uint32 desired) { turf_store32Relaxed(&m_handle, desired); }
		inline uint32 exchange(uint32 desired) { return turf_exchange32Relaxed(&m_handle, desired); }
		inline bool compare_exchange(uint32 expected, uint32 desired) { return turf_compareExchange32Relaxed(&m_handle, expected, desired) == expected; }
		inline uint32 fetch_add(int32 operand) { return turf_fetchAdd32Relaxed(&m_handle, operand); }
		inline uint32 fetch_sub(int32 operand) { return turf_fetchAdd32Relaxed(&m_handle, -operand); }
		inline uint32 fetch_or(uint32 operand) { return turf_fetchOr32Relaxed(&m_handle, operand); }
		inline uint32 fetch_and(uint32 operand) { return turf_fetchAnd32Relaxed(&m_handle, operand); }

	private:
		turf_atomic32_t m_handle;
	LIGHTINK_DISABLE_COPY(Atomic)
	};

	template <>
	class LIGHTINK_TEMPLATE_DECL Atomic<uint64>
	{
	public:
		Atomic() { store(0); }
		inline uint64 load() const { return turf_load64Relaxed(&m_handle); }
		inline void store(uint64 desired) { turf_store64Relaxed(&m_handle, desired); }
		inline uint64 exchange(uint64 desired) { return turf_exchange64Relaxed(&m_handle, desired); }
		inline bool compare_exchange(uint64 expected, uint64 desired) { return turf_compareExchange64Relaxed(&m_handle, expected, desired) == expected; }
		inline uint64 fetch_add(int64 operand) { return turf_fetchAdd64Relaxed(&m_handle, operand); }
		inline uint64 fetch_sub(int64 operand) { return turf_fetchAdd64Relaxed(&m_handle, -operand); }
		inline uint64 fetch_or(uint64 operand) { return turf_fetchOr64Relaxed(&m_handle, operand); }
		inline uint64 fetch_and(uint64 operand) { return turf_fetchAnd64Relaxed(&m_handle, operand); }
	private:
		turf_atomic64_t m_handle;
	LIGHTINK_DISABLE_COPY(Atomic)
	};

	template <typename T>
	class LIGHTINK_TEMPLATE_DECL Atomic<T *>
	{
	public:
		Atomic() { store(NULL); }
		inline T * load() const { return (T *)turf_loadPtrRelaxed(&m_handle); }
		inline void store(T * desired) { turf_storePtrRelaxed(&m_handle, desired); }
		inline T * exchange(T * desired) { return (T *)turf_exchangePtrRelaxed(&m_handle, desired); }
		inline bool compare_exchange(void * expected, void * desired) { return turf_compareExchangePtrRelaxed(&m_handle, expected, desired) == expected; }
		inline T * fetch_add(intptr_t operand) { return (T *)turf_fetchAddPtrRelaxed(&m_handle, operand); }
		inline T * fetch_sub(intptr_t operand) { return (T *)turf_fetchAddPtrRelaxed(&m_handle, -operand); }
		inline T * fetch_or(size_t operand) { return (T *)turf_fetchOrPtrRelaxed(&m_handle, operand); }
		inline T * fetch_and(size_t operand) { return (T *)turf_fetchAndPtrRelaxed(&m_handle, operand); }
	private:
		turf_atomicPtr_t m_handle;
	LIGHTINK_DISABLE_COPY(Atomic)
	};
}
#else

#include <atomic>
namespace LightInk
{
	template <typename T>
	class LIGHTINK_TEMPLATE_DECL Atomic
	{
	public:
		Atomic() : m_handle(0) {  }
		inline T load() const { return m_handle.load(std::memory_order_relaxed); }
		inline void store(T desired) { m_handle.store(desired, std::memory_order_relaxed); }
		inline T exchange(T desired) { return m_handle.exchange(desired, std::memory_order_relaxed); }
		inline bool compare_exchange(T expected, T desired) { return m_handle.compare_exchange_strong(expected, desired, std::memory_order_relaxed); }
		inline T fetch_add(T operand) { return m_handle.fetch_add(operand, std::memory_order_relaxed); }
		inline T fetch_sub(T operand) { return m_handle.fetch_sub(operand, std::memory_order_relaxed); }
		inline T fetch_or(T operand) { return m_handle.fetch_or(operand, std::memory_order_relaxed); }
		inline T fetch_and(T operand) { return m_handle.fetch_and(operand, std::memory_order_relaxed); }

	private:
		std::atomic<T> m_handle;
		LIGHTINK_DISABLE_COPY(Atomic)
	};
}
#endif

#endif