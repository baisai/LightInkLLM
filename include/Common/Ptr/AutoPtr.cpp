


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

#ifndef LIGHTINK_COMMON_AUTOPTR_CPP_
#define LIGHTINK_COMMON_AUTOPTR_CPP_

#include "AutoPtr.h"

namespace LightInk
{
	template <typename T, typename DelStrategy, typename Allocator>
	AutoPtrWrapper<T, DelStrategy, Allocator>::AutoPtrWrapper() : m_objPtr(NULL)
	{
		
	}

	template <typename T, typename DelStrategy, typename Allocator>
	template<typename __T>
	AutoPtrWrapper<T, DelStrategy, Allocator>::AutoPtrWrapper(__T * ptr) : m_objPtr(ptr)
	{
		
	}

	template <typename T, typename DelStrategy, typename Allocator>
	AutoPtrWrapper<T, DelStrategy, Allocator>::AutoPtrWrapper(const AutoPtrWrapper<T, DelStrategy, Allocator> & cp) : m_objPtr(const_cast<AutoPtrWrapper<T, DelStrategy, Allocator> &>(cp).release())
	{
		
	}

	template <typename T, typename DelStrategy, typename Allocator>
	inline AutoPtrWrapper<T, DelStrategy, Allocator> & AutoPtrWrapper<T, DelStrategy, Allocator>::operator = (const AutoPtrWrapper<T, DelStrategy, Allocator> & right)
	{
		reset(const_cast<AutoPtrWrapper<T, DelStrategy, Allocator> &>(right).release());
		return *this;
	}


	template <typename T, typename DelStrategy, typename Allocator>
	template <typename __T, typename __DelStrategy, typename __Allocator>
	AutoPtrWrapper<T, DelStrategy, Allocator>::AutoPtrWrapper(const AutoPtrWrapper<__T, __DelStrategy, __Allocator> & cp) : m_objPtr(const_cast<AutoPtrWrapper<__T, __DelStrategy, __Allocator> &>(cp).release())
	{
		
	}


	template <typename T, typename DelStrategy, typename Allocator>
	template <typename __T, typename __DelStrategy, typename __Allocator>
	inline AutoPtrWrapper<T, DelStrategy, Allocator> & AutoPtrWrapper<T, DelStrategy, Allocator>::operator = (const AutoPtrWrapper<__T, __DelStrategy, __Allocator> & right)
	{
		reset(const_cast<AutoPtrWrapper<__T, __DelStrategy, __Allocator> &>(right).release());
		return *this;
	}


	template <typename T, typename DelStrategy, typename Allocator>
	AutoPtrWrapper<T, DelStrategy, Allocator>::~AutoPtrWrapper()
	{
		if (m_objPtr)
		{
			DelStrategy::release(m_objPtr);
			m_objPtr = NULL;
		}
	}

	template <typename T, typename DelStrategy, typename Allocator>
	template<typename __T>
	inline void AutoPtrWrapper<T, DelStrategy, Allocator>::reset(__T * ptr)
	{
		if (ptr && ptr != m_objPtr)
		{
			DelStrategy::release(m_objPtr);
		}
		m_objPtr = ptr;
	}

	template <typename T, typename DelStrategy, typename Allocator>
	inline T * AutoPtrWrapper<T, DelStrategy, Allocator>::get() const
	{
		return m_objPtr;
	}

	template <typename T, typename DelStrategy, typename Allocator>
	inline T * AutoPtrWrapper<T, DelStrategy, Allocator>::release()
	{
		T * t = m_objPtr;
		m_objPtr = NULL;
		return t;
	}

	template <typename T, typename DelStrategy, typename Allocator>
	inline T & AutoPtrWrapper<T, DelStrategy, Allocator>::operator * () const
	{
		return *m_objPtr;
	}

	template <typename T, typename DelStrategy, typename Allocator>
	inline T * AutoPtrWrapper<T, DelStrategy, Allocator>::operator -> () const
	{
		return m_objPtr;
	}

	template <typename T, typename DelStrategy, typename Allocator>
	inline AutoPtrWrapper<T, DelStrategy, Allocator>::operator bool() const
	{
		return m_objPtr != NULL;
	}



	template <typename T, typename DelStrategy, typename Allocator>
	bool operator == (const AutoPtrWrapper<T, DelStrategy, Allocator> & left, const T * right)
	{
		return left.get() == right;
	}

	template <typename T, typename DelStrategy, typename Allocator>
	bool operator == (const T * left, const AutoPtrWrapper<T, DelStrategy, Allocator> & right)
	{
		return left == right.get();
	}

	template <typename T, typename DelStrategy, typename Allocator>
	bool operator != (const AutoPtrWrapper<T, DelStrategy, Allocator> & left, const T * right)
	{
		return left.get() != right;
	}

	template <typename T, typename DelStrategy, typename Allocator>
	bool operator != (const T * left,const AutoPtrWrapper<T, DelStrategy, Allocator> & right)
	{
		return left != right.get();
	}

	//////////////////////////////////////////////////////////////////////////////
	//DelStrategy
	/////////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline void PtrDelStrategy::release(T * ptr)
	{
		if (ptr) { delete ptr; }
	}

	template <typename T>
	inline void PtrDelStrategyUser::release(T * ptr)
	{
		if (ptr) { ptr->destroy(); }
	}

	template <typename T>
	inline void ArrayDelStrategy::release(T * ptr)
	{
		if (ptr) { delete [] ptr; }
	}

}

#endif