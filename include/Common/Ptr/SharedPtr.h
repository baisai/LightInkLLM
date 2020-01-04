


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

#ifndef LIGHTINK_COMMON_SHAREDPTR_H_
#define LIGHTINK_COMMON_SHAREDPTR_H_

#include "Common/Ptr/RefCounter.h"
#include "Common/Ptr/AutoPtr.h"

namespace LightInk
{

	template <typename T, typename Counter, typename DelStrategy>
	class WeakPtrWrapper;

	template <typename T, typename Counter, typename DelStrategy>
	class LIGHTINK_TEMPLATE_DECL SharedPtrWrapper
	{
	public:
		SharedPtrWrapper();

		template<typename __T>
		explicit SharedPtrWrapper(__T * ptr);

		SharedPtrWrapper(const SharedPtrWrapper<T, Counter, DelStrategy> & cp);
		SharedPtrWrapper<T, Counter, DelStrategy> & operator = (const SharedPtrWrapper<T, Counter, DelStrategy> & right);
		explicit SharedPtrWrapper(const WeakPtrWrapper<T, Counter, DelStrategy> & wpw);
		
		template <typename __T, typename __Counter, typename __DelStrategy>
		SharedPtrWrapper(const SharedPtrWrapper<__T, __Counter, __DelStrategy> & cp);

		template <typename __T, typename __Counter, typename __DelStrategy>
		SharedPtrWrapper<T, Counter, DelStrategy> & operator = (const SharedPtrWrapper<__T, __Counter, __DelStrategy> & right);

		template <typename __T, typename __Counter, typename __DelStrategy>
		explicit SharedPtrWrapper(const WeakPtrWrapper<__T, __Counter, __DelStrategy> & wpw);

		~SharedPtrWrapper();

		void reset();

		template <typename __T>
		void reset(__T * ptr);

		void swap(SharedPtrWrapper<T, Counter, DelStrategy> & right);

		T * get() const;

		T & operator * () const;

		T * operator -> () const;

		int64 use_count() const;

		bool unique() const;

		operator bool() const;


		template <typename __T, typename __Counter, typename __DelStrategy>
		bool operator == (const SharedPtrWrapper<__T, __Counter, __DelStrategy> & right) const;

		template <typename __T, typename __Counter, typename __DelStrategy>
		bool operator != (const SharedPtrWrapper<__T, __Counter, __DelStrategy> & right) const;

		template <typename __T, typename __Counter, typename __DelStrategy>
		bool operator < (const SharedPtrWrapper<__T, __Counter, __DelStrategy> & right) const;

		template <typename __T, typename __Counter, typename __DelStrategy>
		bool operator > (const SharedPtrWrapper<__T, __Counter, __DelStrategy> & right) const;

		template <typename __T, typename __Counter, typename __DelStrategy>
		bool operator <= (const SharedPtrWrapper<__T, __Counter, __DelStrategy> & right) const;

		template <typename __T, typename __Counter, typename __DelStrategy>
		bool operator >= (const SharedPtrWrapper<__T, __Counter, __DelStrategy> & right) const;


	private:
		void check_delete_ptr();
		void reset_no_delete();
		void reset_ref_counter(T * ptr, Counter * cntr);
		void set_ptr(T * ptr);

		template <typename __T, typename __Counter, typename __DelStrategy>
		friend class WeakPtrWrapper;
		template <typename __T, typename __Counter, typename __DelStrategy>
		friend class SharedPtrWrapper;

	private:
		T * m_objPtr;
		Counter * m_refControl;
	};




	template <typename T, typename Counter, typename DelStrategy>
	bool operator == (const SharedPtrWrapper<T, Counter, DelStrategy> & left, const T * right);

	template <typename T, typename Counter, typename DelStrategy>
	bool operator == (const T * left, const SharedPtrWrapper<T, Counter, DelStrategy> & right);

	template <typename T, typename Counter, typename DelStrategy>
	bool operator != (const SharedPtrWrapper<T, Counter, DelStrategy> & left, const T * right);

	template <typename T, typename Counter, typename DelStrategy>
	bool operator != (const T * left,const SharedPtrWrapper<T, Counter, DelStrategy> & right);





	template <typename T, typename Counter, typename DelStrategy>
	class LIGHTINK_TEMPLATE_DECL WeakPtrWrapper
	{
	public:
		WeakPtrWrapper();

		template<typename __T>
		explicit WeakPtrWrapper(__T * ptr);

		explicit WeakPtrWrapper(const SharedPtrWrapper<T, Counter, DelStrategy> & spw);
		WeakPtrWrapper(const WeakPtrWrapper<T, Counter, DelStrategy> & cp);
		WeakPtrWrapper<T, Counter, DelStrategy> & operator = (const WeakPtrWrapper<T, Counter, DelStrategy> & right);

		template <typename __T, typename __Counter, typename __DelStrategy>
		explicit WeakPtrWrapper(const SharedPtrWrapper<__T, __Counter, __DelStrategy> & spw);

		template <typename __T, typename __Counter, typename __DelStrategy>
		WeakPtrWrapper<T, Counter, DelStrategy> & operator = (const WeakPtrWrapper<__T, __Counter, __DelStrategy> & right);

		template <typename __T, typename __Counter, typename __DelStrategy>
		explicit WeakPtrWrapper(const WeakPtrWrapper<__T, __Counter, __DelStrategy> & cp);

		~WeakPtrWrapper();

		void reset();

		template <typename __T, typename __Counter, typename __DelStrategy>
		void reset(const SharedPtrWrapper<__T, __Counter, __DelStrategy> & spw);

		void swap(WeakPtrWrapper<T, Counter, DelStrategy> & right);

		int64 use_count() const;

		bool expired() const;

		operator bool() const;

		SharedPtrWrapper<T, Counter, DelStrategy> lock() const;

	private:
		void check_delete_ptr();
		void reset_no_delete();
		void set_ptr(T * ptr);

		friend class SharedPtrWrapper<T, Counter, DelStrategy>;

	private:
		T * m_objPtr;
		Counter * m_refControl;

	};

	template <typename T>
	struct SharedPtr
	{
		typedef SharedPtrWrapper<T, RefCounter, PtrDelStrategy> type;
	};

	template <typename T, typename TDelStrategy>
	struct SharedPtrUser
	{
		typedef SharedPtrWrapper<T, RefCounter, TDelStrategy> type;
	};

	template <typename T>
	struct SharedArrayPtr
	{
		typedef SharedPtrWrapper<T, RefCounter, ArrayDelStrategy> type;
	};

	template <typename T>
	struct SharedPtrTS
	{
		typedef SharedPtrWrapper<T, RefCounterTS, PtrDelStrategy> type;
	};

	template <typename T, typename TDelStrategy>
	struct SharedPtrTSUser
	{
		typedef SharedPtrWrapper<T, RefCounterTS, TDelStrategy> type;
	};

	template <typename T>
	struct SharedArrayPtrTS
	{
		typedef SharedPtrWrapper<T, RefCounterTS, ArrayDelStrategy> type;
	};

	template <typename T>
	struct WeakPtr
	{
		typedef WeakPtrWrapper<T, RefCounter, PtrDelStrategy> type;
	};

	template <typename T, typename TDelStrategy>
	struct WeakPtrUser
	{
		typedef WeakPtrWrapper<T, RefCounter, TDelStrategy> type;
	};

	template <typename T>
	struct WeakArrayPtr
	{
		typedef WeakPtrWrapper<T, RefCounter, ArrayDelStrategy> type;
	};


	template <typename T>
	struct WeakPtrTS
	{
		typedef WeakPtrWrapper<T, RefCounterTS, PtrDelStrategy> type;
	};

	template <typename T, typename TDelStrategy>
	struct WeakPtrTSUser
	{
		typedef WeakPtrWrapper<T, RefCounterTS, TDelStrategy> type;
	};

	template <typename T>
	struct WeakArrayPtrTS
	{
		typedef WeakPtrWrapper<T, RefCounterTS, ArrayDelStrategy> type;
	};


}

#include "SharedPtr.cpp"

#endif