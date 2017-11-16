


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
#ifndef LIGHTINK_LUAENGINE_LUACLASSPROPERTYTRAITS_H_
#define LIGHTINK_LUAENGINE_LUACLASSPROPERTYTRAITS_H_

#include "LuaEngine/LuaLib.h"
#include "LuaEngine/LuaMetatableTraits.h"
#include "LuaEngine/LuaStack.h"


namespace LightInk
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//LuaClassPropertyTraits
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct LuaClassPropertyInfo
	{
		typedef int (*IndexFunction)(lua_State *, void *, const char *);
		typedef int (*NewindexFunction)(lua_State *, void *, const char *, int);
		LuaClassPropertyInfo(IndexFunction i, NewindexFunction ni, void * p) : m_indexFunction(i), m_newindexFunction(ni), m_property(p)
		{}
		IndexFunction m_indexFunction;
		NewindexFunction m_newindexFunction;
		void * m_property;
	};
	template <typename T>
	struct LuaClassPropertyInfoImp : public LuaClassPropertyInfo
	{
		LuaClassPropertyInfoImp(IndexFunction i, NewindexFunction ni, void * p) : LuaClassPropertyInfo(i, ni ,p), m_imp(NULL){ ; }
		T m_imp;
	};

	template <typename ClassType, typename T>
	struct LuaClassPropertyTraits;

	template <typename ClassType, typename T>
	struct LuaClassPropertyTraits
	{
		typedef T PropertyType;
		typedef T ClassType::* PropertyTypePtr;
		static int pt_index_function(lua_State * L, void * field, const char * key)
		{
			LogTraceStepCall("int LuaClassPropertyTraits<ClassType, T>::pt_index_function(lua_State * L, void * field, const char * key)");
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			LuaClassPropertyInfoImp<PropertyTypePtr> * lcpii = (LuaClassPropertyInfoImp<PropertyTypePtr> * )field;
			PropertyTypePtr ptr = lcpii->m_imp;
			if (ptr)
			{
				LuaStack<const PropertyType>::push(L, (objPtr->*ptr));
				LogTraceStepReturn(1);
			}
			else
			{
				LogScriptErrorJump(L, "none this field<{}>\n", key);
				LogTraceStepReturn(0);
			}
			LogTraceStepReturn(0);
		}

		static int pt_newindex_function(lua_State * L, void * field, const char * key, int idx)
		{
			LogTraceStepCall("int LuaClassPropertyTraits<ClassType, T>::pt_newindex_function(lua_State * L, void * field, const char * key, int idx)");
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			LuaClassPropertyInfoImp<PropertyTypePtr> * lcpii = (LuaClassPropertyInfoImp<PropertyTypePtr> * )field;
			PropertyTypePtr ptr = lcpii->m_imp;
			if (ptr)
			{
				(objPtr->*ptr) = LuaStack<const PropertyType>::get(L, idx);
			}
			else
			{
				LogError("none this field<{}>\n", key);
			}
			LogTraceStepReturn(0);
		}
	};



	template <typename T>
	struct LuaClassPropertyTraits<T, void>
	{
		typedef T PropertyType;
		static int pt_index_function(lua_State * L, void * field, const char * key)
		{
			LogTraceStepCall("int LuaClassPropertyTraits<T>::pt_index_function(lua_State * L, void * field, const char * key)");
			PropertyType * ptr = (PropertyType *)field;
			if (ptr)
			{
				LuaStack<const PropertyType>::push(L, *ptr);
				LogTraceStepReturn(1);
			}
			else
			{
				LogScriptErrorJump(L, "none this field<{}>\n", key);
				LogTraceStepReturn(0);
			}
			LogTraceStepReturn(0);
		}

		static int pt_newindex_function(lua_State * L, void * field, const char * key, int idx)
		{
			LogTraceStepCall("int LuaClassPropertyTraits<T>::pt_newindex_function(lua_State * L, void * field, const char * key, int idx)");
			PropertyType * ptr = (PropertyType *)field;
			if (ptr)
			{
				*ptr = LuaStack<const PropertyType>::get(L, idx);
			}
			else
			{
				LogScriptErrorJump(L, "none this field<{}>\n", key);
			}
			LogTraceStepReturn(0);
		}
	};

}

#endif



