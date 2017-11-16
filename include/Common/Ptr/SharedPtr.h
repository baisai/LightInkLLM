


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

#include "Common/SmallObject.h"
#include "Common/Ptr/RefCounter.h"
#include "Common/Ptr/AutoPtr.h"

namespace LightInk
{

	template <typename T, typename Counter, typename DelStrategy, typename Allocator>
	class WeakPtrWrapper;

	template <typename T, typename Counter, typename DelStrategy, typename Allocator = SmallObject>
	class LIGHTINK_TEMPLATE_DECL SharedPtrWrapper : public Allocator
	{
	public:
		SharedPtrWrapper();

		template<typename __T>
		explicit SharedPtrWrapper(__T * ptr);

		SharedPtrWrapper(const SharedPtrWrapper<T, Counter, DelStrategy, Allocator> & cp);
		SharedPtrWrapper<T, Counter, DelStrategy, Allocator> & operator = (const SharedPtrWrapper<T, Counter, DelStrategy, Allocator> & right);
		explicit SharedPtrWrapper(const WeakPtrWrapper<T, Counter, DelStrategy, Allocator> & wpw);
		
		template <typename __T, typename __Counter, typename __DelStrategy, typename __Allocator>
		SharedPtrWrapper(const SharedPtrWrapper<__T, __Counter, __DelStrategy, __Allocator> & cp);

		template <typename __T, typename __Counter, typename __DelStrategy, typename __Allocator>
		SharedPtrWrapper<T, Counter, DelStrategy, Allocator> & operator = (const SharedPtrWrapper<__T, __Counter, __DelStrategy, __Allocator> & right);

		template <typename __T, typename __Counter, typename __DelStrategy, typename __Allocator>
		explicit SharedPtrWrapper(const WeakPtrWrapper<__T, __Counter, __DelStrategy, __Allocator> & wpw);

		~SharedPtrWrapper();

		void reset();

		template <typename __T>
		void reset(__T * ptr);

		void swap(SharedPtrWrapper<T, Counter, DelStrategy, Allocator> & right);

		T * get() const;

		T & operator * () const;

		T * operator -> () const;

		int64 use_count() const;

		bool unique() const;

		operator bool() const;


		template <typename __T, typename __Counter, typename __DelStrategy, typename __Allocator>
		bool operator == (const SharedPtrWrapper<__T, __Counter, __DelStrategy, __Allocator> & right) const;

		template <typename __T, typename __Counter, typename __DelStrategy, typename __Allocator>
		bool operator != (const SharedPtrWrapper<__T, __Counter, __DelStrategy, __Allocator> & right) const;

		template <typename __T, typename __Counter, typename __DelStrategy, typename __Allocator>
		bool operator < (const SharedPtrWrapper<__T, __Counter, __DelStrategy, __Allocator> & right) const;

		template <typename __T, typename __Counter, typename __DelStrategy, typename __Allocator>
		bool operator > (const SharedPtrWrapper<__T, __Counter, __DelStrategy, __Allocator> & right) const;

		template <typename __T, typename __Counter, typename __DelStrategy, typename __Allocator>
		bool operator <= (const SharedPtrWrapper<__T, __Counter, __DelStrategy, __Allocator> & right) const;

		template <typename __T, typename __Counter, typename __DelStrategy, typename __Allocator>
		bool operator >= (const SharedPtrWrapper<__T, __Counter, __DelStrategy, __Allocator> & right) const;


	private:
		void check_delete_ptr();
		void reset_no_delete();
		void reset_ref_counter(T * ptr, Counter * cntr);
		void set_ptr(T * ptr);

		template <typename __T, typename __Counter, typename __DelStrategy, typename __Allocator>
		friend class WeakPtrWrapper;
		template <typename __T, typename __Counter, typename __DelStrategy, typename __Allocator>
		friend class SharedPtrWrapper;

	private:
		T * m_objPtr;
		Counter * m_refControl;
	};




	template <typename T, typename Counter, typename DelStrategy, typename Allocator>
	bool operator == (const SharedPtrWrapper<T, Counter, DelStrategy, Allocator> & left, const T * right);

	template <typename T, typename Counter, typename DelStrategy, typename Allocator>
	bool operator == (const T * left, const SharedPtrWrapper<T, Counter, DelStrategy, Allocator> & right);

	template <typename T, typename Counter, typename DelStrategy, typename Allocator>
	bool operator != (const SharedPtrWrapper<T, Counter, DelStrategy, Allocator> & left, const T * right);

	template <typename T, typename Counter, typename DelStrategy, typename Allocator>
	bool operator != (const T * left,const SharedPtrWrapper<T, Counter, DelStrategy, Allocator> & right);





	template <typename T, typename Counter, typename DelStrategy, typename Allocator = SmallObject>
	class LIGHTINK_TEMPLATE_DECL WeakPtrWrapper : public Allocator
	{
	public:
		WeakPtrWrapper();

		template<typename __T>
		explicit WeakPtrWrapper(__T * ptr);

		explicit WeakPtrWrapper(const SharedPtrWrapper<T, Counter, DelStrategy, Allocator> & spw);
		WeakPtrWrapper(const WeakPtrWrapper<T, Counter, DelStrategy, Allocator> & cp);
		WeakPtrWrapper<T, Counter, DelStrategy, Allocator> & operator = (const WeakPtrWrapper<T, Counter, DelStrategy, Allocator> & right);

		template <typename __T, typename __Counter, typename __DelStrategy, typename __Allocator>
		explicit WeakPtrWrapper(const SharedPtrWrapper<__T, __Counter, __DelStrategy, __Allocator> & spw);

		template <typename __T, typename __Counter, typename __DelStrategy, typename __Allocator>
		WeakPtrWrapper<T, Counter, DelStrategy, Allocator> & operator = (const WeakPtrWrapper<__T, __Counter, __DelStrategy, __Allocator> & right);

		template <typename __T, typename __Counter, typename __DelStrategy, typename __Allocator>
		explicit WeakPtrWrapper(const WeakPtrWrapper<__T, __Counter, __DelStrategy, __Allocator> & cp);

		~WeakPtrWrapper();

		void reset();

		template <typename __T, typename __Counter, typename __DelStrategy, typename __Allocator>
		void reset(const SharedPtrWrapper<__T, __Counter, __DelStrategy, __Allocator> & spw);

		void swap(WeakPtrWrapper<T, Counter, DelStrategy, Allocator> & right);

		int64 use_count() const;

		bool expired() const;

		operator bool() const;

		SharedPtrWrapper<T, Counter, DelStrategy, Allocator> lock() const;

	private:
		void check_delete_ptr();
		void reset_no_delete();
		void set_ptr(T * ptr);

		friend class SharedPtrWrapper<T, Counter, DelStrategy, Allocator>;

	private:
		T * m_objPtr;
		Counter * m_refControl;

	};

	template <typename T>
	struct SharedPtr
	{
		typedef SharedPtrWrapper<T, RefCounter<SmallObject>, PtrDelStrategy, SmallObject> type;
	};

	template <typename T, typename TDelStrategy>
	struct SharedPtrUser
	{
		typedef SharedPtrWrapper<T, RefCounter<SmallObject>, TDelStrategy, SmallObject> type;
	};

	template <typename T>
	struct SharedArrayPtr
	{
		typedef SharedPtrWrapper<T, RefCounter<SmallObject>, ArrayDelStrategy, SmallObject> type;
	};

	template <typename T>
	struct SharedPtrTS
	{
		typedef SharedPtrWrapper<T, RefCounterTS<SmallObject>, PtrDelStrategy, SmallObject> type;
	};

	template <typename T, typename TDelStrategy>
	struct SharedPtrTSUser
	{
		typedef SharedPtrWrapper<T, RefCounterTS<SmallObject>, TDelStrategy, SmallObject> type;
	};

	template <typename T>
	struct SharedArrayPtrTS
	{
		typedef SharedPtrWrapper<T, RefCounterTS<SmallObject>, ArrayDelStrategy, SmallObject> type;
	};

	template <typename T>
	struct WeakPtr
	{
		typedef WeakPtrWrapper<T, RefCounter<SmallObject>, PtrDelStrategy, SmallObject> type;
	};

	template <typename T, typename TDelStrategy>
	struct WeakPtrUser
	{
		typedef WeakPtrWrapper<T, RefCounter<SmallObject>, TDelStrategy, SmallObject> type;
	};

	template <typename T>
	struct WeakArrayPtr
	{
		typedef WeakPtrWrapper<T, RefCounter<SmallObject>, ArrayDelStrategy, SmallObject> type;
	};


	template <typename T>
	struct WeakPtrTS
	{
		typedef WeakPtrWrapper<T, RefCounterTS<SmallObject>, PtrDelStrategy, SmallObject> type;
	};

	template <typename T, typename TDelStrategy>
	struct WeakPtrTSUser
	{
		typedef WeakPtrWrapper<T, RefCounterTS<SmallObject>, TDelStrategy, SmallObject> type;
	};

	template <typename T>
	struct WeakArrayPtrTS
	{
		typedef WeakPtrWrapper<T, RefCounterTS<SmallObject>, ArrayDelStrategy, SmallObject> type;
	};


}

#include "SharedPtr.cpp"

#endif