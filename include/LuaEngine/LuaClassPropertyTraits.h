


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
	class LuaClassPropertyInfo
	{
	public:
		typedef int (*IndexFunction)(lua_State *, void *, const string &);
		typedef int (*NewindexFunction)(lua_State *, void *, const string &, int);
		LuaClassPropertyInfo(const string & name, IndexFunction i, NewindexFunction ni, void * p) : m_name(name), m_indexFunction(i), m_newindexFunction(ni), m_property(p)
		{  }

		virtual ~LuaClassPropertyInfo()
		{  }

		inline int call_index(lua_State * L) { return m_indexFunction(L, m_property, m_name); }
		inline int call_newindex(lua_State * L, int idx) { return m_newindexFunction(L, m_property, m_name, idx); }

	private:
		string m_name;
		IndexFunction m_indexFunction;
		NewindexFunction m_newindexFunction;
		void * m_property;
	};
	template <typename T>
	class LuaClassPropertyInfoImp : public LuaClassPropertyInfo
	{
	public:
		LuaClassPropertyInfoImp(const string & name, IndexFunction i, NewindexFunction ni, void * p) : LuaClassPropertyInfo(name, i, ni ,p), m_imp(NULL)
		{  }
		
		virtual ~LuaClassPropertyInfoImp()
		{  }

		inline const T & get_imp() const { return m_imp; }
		inline void set_imp(const T & imp) { m_imp = imp; }

	private:
		T m_imp;
	};

	template <typename ClassType, typename T>
	struct LuaClassPropertyTraits;

	template <typename ClassType, typename T>
	struct LuaClassPropertyTraits
	{
		typedef T PropertyType;
		typedef T ClassType::* PropertyTypePtr;
		static int pt_index_function(lua_State * L, void * field, const string & key)
		{
			LogTraceStepCall("int LuaClassPropertyTraits<ClassType, T>::pt_index_function(lua_State * L, void * field, const string & key)");
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			if (objPtr == NULL)
			{
				LogScriptError(L, "Error!!!this field \"{}\" first arg is not a object!!!", key);
				LogTraceStepReturn(0);
			}
			LuaClassPropertyInfoImp<PropertyTypePtr> * lcpii = (LuaClassPropertyInfoImp<PropertyTypePtr> * )field;
			PropertyTypePtr ptr = lcpii->get_imp();
			if (ptr)
			{
				LuaStack<const PropertyType>::push(L, (objPtr->*ptr));
				LogTraceStepReturn(1);
			}
			else
			{
				LogScriptError(L, "Error!!!Not this field!!!\"{}\"", key);
			}
			LogTraceStepReturn(0);
		}

		static int pt_newindex_function(lua_State * L, void * field, const string & key, int idx)
		{
			LogTraceStepCall("int LuaClassPropertyTraits<ClassType, T>::pt_newindex_function(lua_State * L, void * field, const string & key, int idx)");
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			if (objPtr == NULL)
			{
				LogScriptError(L, "Error!!!this field \"{}\" first arg is not a object!!!", key);
				LogTraceStepReturn(0);
			}
			LuaClassPropertyInfoImp<PropertyTypePtr> * lcpii = (LuaClassPropertyInfoImp<PropertyTypePtr> * )field;
			PropertyTypePtr ptr = lcpii->get_imp();
			if (ptr)
			{
				(objPtr->*ptr) = LuaStack<const PropertyType>::get(L, idx);
			}
			else
			{
				LogScriptErrorJump(L, "Error!!!Not this field!!!\"{}\"", key);
			}
			LogTraceStepReturn(0);
		}
	};



	template <typename T>
	struct LuaClassPropertyTraits<T, void>
	{
		typedef T PropertyType;
		static int pt_index_function(lua_State * L, void * field, const string & key)
		{
			LogTraceStepCall("int LuaClassPropertyTraits<T>::pt_index_function(lua_State * L, void * field, const string & key)");
			PropertyType * ptr = (PropertyType *)field;
			if (ptr)
			{
				LuaStack<const PropertyType>::push(L, *ptr);
				LogTraceStepReturn(1);
			}
			else
			{
				LogScriptError(L, "Error!!!Not this field!!!\"{}\"", key);
			}
			LogTraceStepReturn(0);
		}

		static int pt_newindex_function(lua_State * L, void * field, const string & key, int idx)
		{
			LogTraceStepCall("int LuaClassPropertyTraits<T>::pt_newindex_function(lua_State * L, void * field, const string & key, int idx)");
			PropertyType * ptr = (PropertyType *)field;
			if (ptr)
			{
				*ptr = LuaStack<const PropertyType>::get(L, idx);
			}
			else
			{
				LogScriptErrorJump(L, "Error!!!Not this field!!!\"{}\"", key);
			}
			LogTraceStepReturn(0);
		}
	};

}

#endif



