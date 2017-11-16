


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

#ifndef LIGHTINK_COMMON_TYPELIST_H_
#define LIGHTINK_COMMON_TYPELIST_H_

namespace LightInk
{
	struct TypeEmpty{  };

	struct TypeNil {  };

	////////////////////////////////////////////////////////////
	//TypeList
	template <typename H, typename T>
	struct TypeList
	{
		typedef H Head;
		typedef T Tail;
	};

	////////////////////////////////////////////////////////////
	//TypeListLength
	template <typename TL> 
	struct TypeListLength;

	template <> 
	struct TypeListLength<TypeNil>
	{
		enum { Result = 0 };
	};

	template <typename H, typename T>
	struct TypeListLength< TypeList<H, T> >
	{
		enum { Result = 1 + TypeListLength<T>::Result };
	};


	////////////////////////////////////////////////////////////////
	//TypeListAt
	template <typename TL, unsigned int i> 
	struct TypeListAt;

	template <typename H, typename T>
	struct TypeListAt<TypeList<H, T>, 1>
	{
		typedef H Result;
	};

	template <typename H, typename T, unsigned int i>
	struct TypeListAt<TypeList<H, T>, i>
	{
		typedef typename TypeListAt<T, i - 1>::Result Result;
	};

	template <typename TL, unsigned int i,	typename D = TypeNil>
	struct TypeListAtDefault
	{
		typedef D Result;
	};
	//找不到返回特定类型
	template <typename H, typename T, typename D>
	struct TypeListAtDefault<TypeList<H, T>, 1, D>
	{
		typedef H Result;
	};

	template <typename H, typename T, unsigned int i, typename D>
	struct TypeListAtDefault<TypeList<H, T>, i, D>
	{
		typedef typename TypeListAtDefault<T, i - 1, D>::Result Result;
	};

	/////////////////////////////////////////////////////////////
	//TypeListIndex
	template <typename TL, typename V> 
	struct TypeListIndex;

	template <typename V>
	struct TypeListIndex<TypeNil, V>
	{
		enum { Result = 0 };
	};

	template <typename T, typename V>
	struct TypeListIndex<TypeList<V, T>, V>
	{
		enum { Result = 1 };
	};

	template <typename H, typename T, typename V>
	struct TypeListIndex<TypeList<H, T>, V>
	{
	private:
		enum { temp = TypeListIndex<T, V>::Result };
	public:
		enum { Result = (temp == 0 ? 0 : 1 + temp) };
	};

	/////////////////////////////////////////////////////////
	//TypeListAppend
	template <typename TL, typename V> 
	struct TypeListAppend;

	template <> 
	struct TypeListAppend<TypeNil, TypeNil>
	{
		typedef TypeNil Result;
	};

	template <typename V> 
	struct TypeListAppend<TypeNil, V>
	{
		typedef TypeList<V,TypeNil> Result;
	};

	template <typename H, typename T>
	struct TypeListAppend<TypeNil, TypeList<H, T> >
	{
		typedef TypeList<H, T> Result;
	};

	template <typename H, typename T, typename V>
	struct TypeListAppend<TypeList<H, T>, V>
	{
		typedef TypeList<H, typename TypeListAppend<T, V>::Result> Result;
	};

	////////////////////////////////////////////////////////////////////////
	//TypeListErase
	template <typename TL, typename V> 
	struct TypeListErase;

	template <typename V>
	struct TypeListErase<TypeNil, V>
	{
		typedef TypeNil Result;
	};

	template <typename V, typename T>
	struct TypeListErase<TypeList<V, T>, V>
	{
		typedef T Result;
	};

	template <typename H, typename T, typename V>
	struct TypeListErase<TypeList<H, T>, V>
	{
		typedef TypeList<H, typename TypeListErase<T, V>::Result> Result;
	};

	//////////////////////////////////////////////////////////////////////
	//TypeListEraseAll
	template <typename TL, typename V> struct TypeListEraseAll;
	template <typename V>
	struct TypeListEraseAll<TypeNil, V>
	{
		typedef TypeNil Result;
	};
	template <typename V, typename T>
	struct TypeListEraseAll<TypeList<V, T>, V>
	{
		typedef typename TypeListEraseAll<T, V>::Result Result;
	};
	template <typename H, typename T, typename V>
	struct TypeListEraseAll<TypeList<H, T>, V>
	{
		typedef TypeList<H, typename TypeListEraseAll<T, V>::Result> Result;
	};

	////////////////////////////////////////////////////////////////////////
	//TypeListNoRepeat
	template <typename TL> 
	struct TypeListNoRepeat;

	template <> 
	struct TypeListNoRepeat<TypeNil>
	{
		typedef TypeNil Result;
	};

	template <typename H, typename T>
	struct TypeListNoRepeat< TypeList<H, T> >
	{
	private:
		typedef typename TypeListNoRepeat<T>::Result L1;
		typedef typename TypeListErase<L1, H>::Result L2;
	public:
		typedef TypeList<H, L2> Result;
	};

	//////////////////////////////////////////////////////////////////////////
	//TypeListReplace
	template <typename TL, typename V, typename U>
	struct TypeListReplace;

	template <typename V, typename U>
	struct TypeListReplace<TypeNil, V, U>
	{
		typedef TypeNil Result;
	};

	template <typename V, typename T, typename U>
	struct TypeListReplace<TypeList<V, T>, V, U>
	{
		typedef TypeList<U, T> Result;
	};

	template <typename H, typename T, typename V, typename U>
	struct TypeListReplace<TypeList<H, T>, V, U>
	{
		typedef TypeList<H, typename TypeListReplace<T, V, U>::Result> Result;
	};

	/////////////////////////////////////////////////////////////////////////////////
	//TypeListReplaceAll
	template <typename TL, typename V, typename U>
	struct TypeListReplaceAll;

	template <typename V, typename U>
	struct TypeListReplaceAll<TypeNil, V, U>
	{
		typedef TypeNil Result;
	};

	template <typename V, typename T, typename U>
	struct TypeListReplaceAll<TypeList<V, T>, V, U>
	{
		typedef TypeList<U, typename TypeListReplaceAll<T, V, U>::Result> Result;
	};

	template <class H, class T, class V, class U>
	struct TypeListReplaceAll<TypeList<H, T>, V, U>
	{
		typedef TypeList<H, typename TypeListReplaceAll<T, V, U>::Result> Result;
	};

}




#endif