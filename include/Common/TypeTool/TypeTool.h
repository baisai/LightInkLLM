


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
#ifndef LIGHTINK_COMMON_TYPETOOL_H_
#define LIGHTINK_COMMON_TYPETOOL_H_

namespace LightInk
{
	template <bool P, typename T = void>
	struct enable_if {
		typedef T type;
	};

	template <typename T>
	struct enable_if<false, T> {
	};

	template <typename T>
	struct IsPointer
	{
		enum { Result = false };
		typedef T type;
		static T & get_ref(T & data);
		static const T & get_ref(const T & data);
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <typename T>
	inline T & IsPointer<T>::get_ref(T & data) { return data; }
	template <typename T>
	inline const T & IsPointer<T>::get_ref(const T & data) { return data; }

	template <typename T>
	struct IsPointer <T *>
	{
		enum { Result = true };
		typedef T type;
		static T & get_ref(T * data);
		static const T & get_ref(const T * data);
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <typename T>
	inline T & IsPointer<T *>::get_ref(T * data) { return *data; }
	template <typename T>
	inline const T & IsPointer<T *>::get_ref(const T * data) { return *data; }


	template <typename T>
	struct GetType
	{
		typedef T type;
	};

	template <typename T>
	struct GetType<T *>
	{
		typedef T type;
	};

	template <typename T>
	struct GetType<T &>
	{
		typedef T type;
	};

	template <int v>
	struct Int2Type
	{
		enum { Result = v };
	};

	template <typename T>
	struct Type2Type
	{
		typedef T Result;
	};

	template <bool flag, typename T, typename U>
	struct TypeSelect
	{
		typedef T Result;
	};
	template <typename T, typename U>
	struct TypeSelect<false, T, U>
	{
		typedef U Result;
	};

	template <typename T, typename U>
	struct IsSameType
	{
		enum { Result = false };
	};

	template <typename T>
	struct IsSameType<T,T>
	{
		enum { Result = true };
	};

	template <typename T>
	struct GuardDelete
	{
		GuardDelete(T * p) : m_p(p){;}
		~GuardDelete() {delete m_p;}
		T * m_p;
	};
}




#endif