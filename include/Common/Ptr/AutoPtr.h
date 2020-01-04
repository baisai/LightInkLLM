


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


#ifndef LIGHTINK_COMMON_AUTOPTR_H_
#define LIGHTINK_COMMON_AUTOPTR_H_

#include "Common/Type.h"

namespace LightInk
{

	template <typename T, typename DelStrategy>
	class LIGHTINK_TEMPLATE_DECL AutoPtrWrapper
	{
	public:
		AutoPtrWrapper();

		template<typename __T>
		explicit AutoPtrWrapper(__T * ptr);

		AutoPtrWrapper(const AutoPtrWrapper<T, DelStrategy> & cp);
		AutoPtrWrapper<T, DelStrategy> & operator = (const AutoPtrWrapper<T, DelStrategy> & right);

		template <typename __T, typename __DelStrategy>
		AutoPtrWrapper(const AutoPtrWrapper<__T, __DelStrategy> & cp);

		template <typename __T, typename __DelStrategy>
		AutoPtrWrapper<T, DelStrategy> & operator = (const AutoPtrWrapper<__T, __DelStrategy> & cp);

		~AutoPtrWrapper();

		template<typename __T>
		void reset(__T * ptr = NULL);

		T * get() const;

		T * release();

		T & operator * () const;

		T * operator -> () const;

		operator bool() const;

	private:
		T * m_objPtr;

	};

	template <typename T, typename DelStrategy>
	bool operator == (const AutoPtrWrapper<T, DelStrategy> & left, const T * right);

	template <typename T, typename DelStrategy>
	bool operator == (const T * left, const AutoPtrWrapper<T, DelStrategy> & right);

	template <typename T, typename DelStrategy>
	bool operator != (const AutoPtrWrapper<T, DelStrategy> & left, const T * right);

	template <typename T, typename DelStrategy>
	bool operator != (const T * left,const AutoPtrWrapper<T, DelStrategy> & right);



	struct PtrDelStrategy
	{
		template <typename T>
		static void release(T * ptr);
	};

	struct PtrDelStrategyUser
	{
		template <typename T>
		static void release(T * ptr);
	};

	struct ArrayDelStrategy
	{
		template <typename T>
		static void release(T * ptr);
	};


	template <typename T>
	struct AutoPtr
	{
		typedef AutoPtrWrapper<T, PtrDelStrategy> type;
	};

	template <typename T, typename TDelStrategy>
	struct AutoPtrUser
	{
		typedef AutoPtrWrapper<T, TDelStrategy> type;
	};

	template <typename T>
	struct AutoArrayPtr
	{
		typedef AutoPtrWrapper<T, ArrayDelStrategy> type;
	};

}

#include "AutoPtr.cpp"

#endif