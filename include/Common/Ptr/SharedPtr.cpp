


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
#ifndef LIGHTINK_COMMON_SHAREDPTR_CPP_
#define LIGHTINK_COMMON_SHAREDPTR_CPP_

#include "SharedPtr.h"

namespace LightInk
{

	///////////////////////////////////////////////////////////////////////
	//SharedPtr
	//////////////////////////////////////////////////////////////////////
	template <typename T, typename Counter, typename DelStrategy>
	SharedPtrWrapper<T, Counter, DelStrategy>::SharedPtrWrapper() : m_objPtr(NULL), m_refControl(NULL)
	{
		
	}

	template <typename T, typename Counter, typename DelStrategy>
	template<typename __T>
	SharedPtrWrapper<T, Counter, DelStrategy>::SharedPtrWrapper(__T * ptr)
	{
		set_ptr(ptr);
	}

	template <typename T, typename Counter, typename DelStrategy>
	SharedPtrWrapper<T, Counter, DelStrategy>::SharedPtrWrapper(const SharedPtrWrapper<T, Counter, DelStrategy> & cp)
	{
		m_objPtr = cp.m_objPtr;
		m_refControl = cp.m_refControl;
		if (m_refControl) { m_refControl->inc_shared(); }
	}


	template <typename T, typename Counter, typename DelStrategy>
	SharedPtrWrapper<T, Counter, DelStrategy> & SharedPtrWrapper<T, Counter, DelStrategy>::operator = (const SharedPtrWrapper<T, Counter, DelStrategy> & right)
	{
		if (this == &right) { return (*this); }
		reset();
		m_objPtr = right.m_objPtr;
		m_refControl = right.m_refControl;
		if (m_refControl) { m_refControl->inc_shared(); }
		return (*this);
	}

	template <typename T, typename Counter, typename DelStrategy>
	SharedPtrWrapper<T, Counter, DelStrategy>::SharedPtrWrapper(const WeakPtrWrapper<T, Counter, DelStrategy> & wpw)
	{
		m_objPtr = wpw.m_objPtr;
		m_refControl = wpw.m_refControl;
		if (m_refControl) { m_refControl->inc_shared(); }
	}


	template <typename T, typename Counter, typename DelStrategy>
	template <typename __T, typename __Counter, typename __DelStrategy>
	SharedPtrWrapper<T, Counter, DelStrategy>::SharedPtrWrapper(const SharedPtrWrapper<__T, __Counter, __DelStrategy> & cp)
	{
		m_objPtr = cp.m_objPtr;
		m_refControl = cp.m_refControl;
		if (m_refControl) { m_refControl->inc_shared(); }
	}


	template <typename T, typename Counter, typename DelStrategy>
	template <typename __T, typename __Counter, typename __DelStrategy>
	SharedPtrWrapper<T, Counter, DelStrategy> & SharedPtrWrapper<T, Counter, DelStrategy>::operator = (const SharedPtrWrapper<__T, __Counter, __DelStrategy> & right)
	{
		if (this == &right) { return (*this); }
		reset();
		m_objPtr = right.m_objPtr;
		m_refControl = right.m_refControl;
		if (m_refControl) { m_refControl->inc_shared(); }
		return (*this);
	}


	template <typename T, typename Counter, typename DelStrategy>
	template <typename __T, typename __Counter, typename __DelStrategy>
	SharedPtrWrapper<T, Counter, DelStrategy>::SharedPtrWrapper(const WeakPtrWrapper<__T, __Counter, __DelStrategy> & wpw)
	{
		m_objPtr = wpw.m_objPtr;
		m_refControl = wpw.m_refControl;
		if (m_refControl) { m_refControl->inc_shared(); }
	}


	template <typename T, typename Counter, typename DelStrategy>
	SharedPtrWrapper<T, Counter, DelStrategy>::~SharedPtrWrapper()
	{
		check_delete_ptr();
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline void SharedPtrWrapper<T, Counter, DelStrategy>::reset()
	{
		check_delete_ptr();
	}


	template <typename T, typename Counter, typename DelStrategy>
	template <typename __T>
	inline void SharedPtrWrapper<T, Counter, DelStrategy>::reset(__T * ptr)
	{
		check_delete_ptr();
		set_ptr(ptr);
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline void SharedPtrWrapper<T, Counter, DelStrategy>::swap(SharedPtrWrapper<T, Counter, DelStrategy> & right)
	{
		if (this == &right) { return; }
		T * tempPtr = m_objPtr;
		Counter * tempControl = m_refControl;
		m_objPtr = right.m_objPtr;
		m_refControl = right.m_refControl;
		right.m_objPtr = tempPtr;
		right.m_refControl = tempControl;
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline T * SharedPtrWrapper<T, Counter, DelStrategy>::get() const
	{
		return m_objPtr;
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline T & SharedPtrWrapper<T, Counter, DelStrategy>::operator * () const
	{
		return (*m_objPtr);
	}


	template <typename T, typename Counter, typename DelStrategy>
	inline T * SharedPtrWrapper<T, Counter, DelStrategy>::operator -> () const
	{
		return m_objPtr;
	}


	template <typename T, typename Counter, typename DelStrategy>
	inline int64 SharedPtrWrapper<T, Counter, DelStrategy>::use_count() const
	{
		if (m_objPtr == NULL || m_refControl == NULL)
		{
			return 0;
		}
		return m_refControl->get_shared_refs();
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline bool SharedPtrWrapper<T, Counter, DelStrategy>::unique() const
	{
		return use_count() == 1;
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline SharedPtrWrapper<T, Counter, DelStrategy>::operator bool() const
	{
		return m_objPtr != NULL;
	}

	template <typename T, typename Counter, typename DelStrategy>
	template <typename __T, typename __Counter, typename __DelStrategy>
	inline bool SharedPtrWrapper<T, Counter, DelStrategy>::operator == (const SharedPtrWrapper<__T, __Counter, __DelStrategy> & right) const
	{
		return m_objPtr == right.m_objPtr;
	}


	template <typename T, typename Counter, typename DelStrategy>
	template <typename __T, typename __Counter, typename __DelStrategy>
	inline bool SharedPtrWrapper<T, Counter, DelStrategy>::operator != (const SharedPtrWrapper<__T, __Counter, __DelStrategy> & right) const
	{
		return m_objPtr != right.m_objPtr;
	}


	template <typename T, typename Counter, typename DelStrategy>
	template <typename __T, typename __Counter, typename __DelStrategy>
	inline bool SharedPtrWrapper<T, Counter, DelStrategy>::operator < (const SharedPtrWrapper<__T, __Counter, __DelStrategy> & right) const
	{
		return m_objPtr < right.m_objPtr;
	}

	template <typename T, typename Counter, typename DelStrategy>
	template <typename __T, typename __Counter, typename __DelStrategy>
	inline bool SharedPtrWrapper<T, Counter, DelStrategy>::operator > (const SharedPtrWrapper<__T, __Counter, __DelStrategy> & right) const
	{
		return m_objPtr > right.m_objPtr;
	}

	template <typename T, typename Counter, typename DelStrategy>
	template <typename __T, typename __Counter, typename __DelStrategy>
	inline bool SharedPtrWrapper<T, Counter, DelStrategy>::operator <= (const SharedPtrWrapper<__T, __Counter, __DelStrategy> & right) const
	{
		return m_objPtr <= right.m_objPtr;
	}


	template <typename T, typename Counter, typename DelStrategy>
	template <typename __T, typename __Counter, typename __DelStrategy>
	inline bool SharedPtrWrapper<T, Counter, DelStrategy>::operator >= (const SharedPtrWrapper<__T, __Counter, __DelStrategy> & right) const
	{
		return m_objPtr >= right.m_objPtr;
	}



	template <typename T, typename Counter, typename DelStrategy>
	inline void SharedPtrWrapper<T, Counter, DelStrategy>::check_delete_ptr()
	{
		if (m_refControl == NULL)
		{
			return;
		}
		m_refControl->dec_shared();
		T * ptr = NULL;
		if (m_refControl->get_shared_refs() == 0)
		{
			ptr = m_objPtr;
		}
		if (!m_refControl->has_refs())
		{
			delete m_refControl;
		}
		reset_no_delete();
		DelStrategy::release(ptr);
	}


	template <typename T, typename Counter, typename DelStrategy>
	inline void SharedPtrWrapper<T, Counter, DelStrategy>::reset_no_delete()
	{
		m_objPtr = NULL;
		m_refControl = NULL;
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline void SharedPtrWrapper<T, Counter, DelStrategy>::reset_ref_counter(T * ptr, Counter * cntr)
	{
		m_objPtr = ptr;
		m_refControl = cntr;
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline void SharedPtrWrapper<T, Counter, DelStrategy>::set_ptr(T * ptr)
	{
		m_refControl = new Counter();
		m_objPtr = ptr;
	}


	/////////////////////////////////////////////////////////////////////////
	//Ptr operator
	////////////////////////////////////////////////////////////////////////
	template <typename T, typename Counter, typename DelStrategy>
	inline bool operator == (const SharedPtrWrapper<T, Counter, DelStrategy> & left, const T * right)
	{
		return left.get() == right;
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline bool operator == (const T * left, const SharedPtrWrapper<T, Counter, DelStrategy> & right)
	{
		return left == right.get();
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline bool operator != (const SharedPtrWrapper<T, Counter, DelStrategy> & left, const T * right)
	{
		return left.get() != right;
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline bool operator != (const T * left,const SharedPtrWrapper<T, Counter, DelStrategy> & right)
	{
		return left == right.get();
	}


	///////////////////////////////////////////////////////////////////////
	//WeakPtr
	//////////////////////////////////////////////////////////////////////
	template <typename T, typename Counter, typename DelStrategy>
	WeakPtrWrapper<T, Counter, DelStrategy>::WeakPtrWrapper() : m_objPtr(NULL), m_refControl(NULL)
	{
		
	}

	template <typename T, typename Counter, typename DelStrategy>
	template<typename __T>
	WeakPtrWrapper<T, Counter, DelStrategy>::WeakPtrWrapper(__T * ptr)
	{
		set_ptr(ptr);
	}

	template <typename T, typename Counter, typename DelStrategy>
	WeakPtrWrapper<T, Counter, DelStrategy>::WeakPtrWrapper(const SharedPtrWrapper<T, Counter, DelStrategy> & spw) : 
		m_objPtr(spw.m_objPtr), m_refControl(spw.m_refControl)
	{
		if (m_refControl) { m_refControl->inc_weak(); }
	}

	template <typename T, typename Counter, typename DelStrategy>
	WeakPtrWrapper<T, Counter, DelStrategy>::WeakPtrWrapper(const WeakPtrWrapper<T, Counter, DelStrategy> & cp) : 
		m_objPtr(cp.m_objPtr), m_refControl(cp.m_refControl)
	{
		if (m_refControl) { m_refControl->inc_weak(); }
	}

	template <typename T, typename Counter, typename DelStrategy>
	WeakPtrWrapper<T, Counter, DelStrategy> & WeakPtrWrapper<T, Counter, DelStrategy>::operator = (const WeakPtrWrapper<T, Counter, DelStrategy> & right)
	{
		if (this == &right) { return (*this); }
		m_objPtr = right.m_objPtr;
		m_refControl = right.m_refControl;
		if (m_refControl) { m_refControl->inc_weak(); }
		return (*this);
	}


	template <typename T, typename Counter, typename DelStrategy>
	template <typename __T, typename __Counter, typename __DelStrategy>
	WeakPtrWrapper<T, Counter, DelStrategy>::WeakPtrWrapper(const SharedPtrWrapper<__T, __Counter, __DelStrategy> & spw) : 
		m_objPtr(spw.m_objPtr), m_refControl(spw.m_refControl)
	{
		if (m_refControl) { m_refControl->inc_weak(); }
	}


	template <typename T, typename Counter, typename DelStrategy>
	template <typename __T, typename __Counter, typename __DelStrategy>
	WeakPtrWrapper<T, Counter, DelStrategy> & WeakPtrWrapper<T, Counter, DelStrategy>::operator = (const WeakPtrWrapper<__T, __Counter, __DelStrategy> & right)
	{
		if (this == &right) { return(*this); }
		m_objPtr = right.m_objPtr;
		m_refControl = right.m_refControl;
		if (m_refControl) { m_refControl->inc_weak(); }
		return (*this);
	}


	template <typename T, typename Counter, typename DelStrategy>
	template <typename __T, typename __Counter, typename __DelStrategy>
	WeakPtrWrapper<T, Counter, DelStrategy>::WeakPtrWrapper(const WeakPtrWrapper<__T, __Counter, __DelStrategy> & cp) : 
		m_objPtr(cp.m_objPtr), m_refControl(cp.m_refControl)
	{
		if (m_refControl) { m_refControl->inc_weak(); }
	}


	template <typename T, typename Counter, typename DelStrategy>
	WeakPtrWrapper<T, Counter, DelStrategy>::~WeakPtrWrapper()
	{
		check_delete_ptr();
	}


	template <typename T, typename Counter, typename DelStrategy>
	inline void WeakPtrWrapper<T, Counter, DelStrategy>::reset()
	{
		check_delete_ptr();
	}

	template <typename T, typename Counter, typename DelStrategy>
	template <typename __T, typename __Counter, typename __DelStrategy>
	inline void WeakPtrWrapper<T, Counter, DelStrategy>::reset(const SharedPtrWrapper<__T, __Counter, __DelStrategy> & spw)
	{
		check_delete_ptr();
		m_objPtr = spw.m_objPtr;
		m_refControl = spw.m_refControl;
		if (m_refControl) { m_refControl->inc_weak(); }
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline void WeakPtrWrapper<T, Counter, DelStrategy>::swap(WeakPtrWrapper<T, Counter, DelStrategy> & right)
	{
		if (this == &right) { return; }
		T * tempPtr = m_objPtr;
		Counter * tempControl = m_refControl;
		m_objPtr = right.m_objPtr;
		m_refControl = right.m_refControl;
		right.m_objPtr = tempPtr;
		right.m_refControl = tempControl;
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline int64 WeakPtrWrapper<T, Counter, DelStrategy>::use_count() const
	{
		if (m_refControl == NULL)
		{
			return 0;
		}
		return m_refControl->get_shared_refs();
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline bool WeakPtrWrapper<T, Counter, DelStrategy>::expired() const
	{
		if (m_refControl == NULL)
		{
			return true;
		}
		return m_refControl->get_shared_refs() == 0;
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline WeakPtrWrapper<T, Counter, DelStrategy>::operator bool() const
	{
		return !expired();
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline SharedPtrWrapper<T, Counter, DelStrategy> WeakPtrWrapper<T, Counter, DelStrategy>::lock() const
	{
		if (expired())
		{
			return SharedPtrWrapper<T, Counter, DelStrategy>();
		}
		return SharedPtrWrapper<T, Counter, DelStrategy>(*this);
	}

	
	template <typename T, typename Counter, typename DelStrategy>
	inline void WeakPtrWrapper<T, Counter, DelStrategy>::check_delete_ptr()
	{
		m_refControl->dec_weak();
		if (!m_refControl->has_refs())
		{
			delete m_refControl;
		}
		reset_no_delete();
	}
		
	template <typename T, typename Counter, typename DelStrategy>
	inline void WeakPtrWrapper<T, Counter, DelStrategy>::reset_no_delete()
	{
		m_objPtr = NULL;
		m_refControl = NULL;
	}

	template <typename T, typename Counter, typename DelStrategy>
	inline void WeakPtrWrapper<T, Counter, DelStrategy>::set_ptr(T * ptr)
	{
		m_refControl = new Counter();
		m_objPtr = ptr;
	}


}

#endif