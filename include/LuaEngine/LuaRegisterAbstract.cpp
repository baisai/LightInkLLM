


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

#ifndef LIGHTINK_LUAENGINE_LUAREGISTERABSTRACT_CPP_
#define LIGHTINK_LUAENGINE_LUAREGISTERABSTRACT_CPP_

#include "LuaEngine/LuaRegisterAbstract.h"
#include "LuaEngine/LuaClassInfo.h"
#include "LuaEngine/LuaDefAutoTool.h"
#include "LuaEngine/LuaDefTool.h"
#include "LuaEngine/LuaDefTraits.h"
#include "LuaEngine/LuaClassCtorTraits.h"
#include "LuaEngine/LuaMetatableTraits.h"
#include "LuaEngine/LuaMetatable.h"
#include "LuaEngine/LuaCFunctionTraits.h"

namespace LightInk
{

	template <typename ClassType>
	LuaRegisterAbstract<ClassType>::LuaRegisterAbstract(lua_State * L, const string & name) : m_lua(L), LuaRegisterNode(L)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType>::LuaRegisterAbstract(lua_State * L, const string & name)");
		LuaStateProtect lsp(L, true);
		init_class(name);
		LuaClassInfo<ClassType>::template register_base_class<ClassType>(L);
		//pop metatable, class table, parent class table
		LogTraceStepReturnVoid;
	}

	template <typename ClassType>
	template <typename BaseType>
	LuaRegisterAbstract<ClassType>::LuaRegisterAbstract(lua_State * L, const string & name, BaseClassStrategy<BaseType>) : m_lua(L), LuaRegisterNode(L)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType>::LuaRegisterAbstract(lua_State * L, const string & name, BaseClassStrategy<BaseType>)");
		LuaStateProtect lsp(L, true);
		init_class(name);
		LuaClassInfo<ClassType>::template register_base_class<BaseType>(L);
		int top = lua_gettop(L);
		if (!LuaClassInfo<BaseType>::get_class_table(L))
		{
			LogError("Error!!!Get Base Class Table Failed!!!");
			LogTraceStepReturnVoid;
		}

		lua_pushnil(L);
		while (lua_next(L, -2))
		{
			lua_pushvalue(L, -2);
			lua_rawget(L, top);
			if(lua_isnoneornil(L, -1) && !LuaClassMgr::check_keep_field_stack(L, -3)) //no this field
			{
				lua_pushvalue(L, -3);
				lua_pushvalue(L, -3);
				lua_rawset(L, top);
			}
			lua_pop(L, 2);
		}

		//class table, parent class table
		LogTraceStepReturnVoid;
	}

	template <typename ClassType>
	LuaRegisterAbstract<ClassType>::LuaRegisterAbstract(const LuaRegisterAbstract<ClassType> & cp) : LuaRegisterNode(cp), m_lua(cp.m_lua)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType>::LuaRegisterAbstract(const LuaRegisterAbstract<ClassType> & cp)");
		LogTraceStepReturnVoid;
	}

	template <typename ClassType>
	template <typename T>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::operator = (const LuaRegisterAbstract<T> & right)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::operator = (const LuaRegisterAbstract<T> & right)");
		LuaRegisterNode::operator=(static_cast<const LuaRegisterNode &>(right));
		m_lua = right.m_lua;
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	template <typename T>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def(T obj, const string & name)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def(T obj, const string & name)");
		LuaDefTraits<T>::call(this, m_lua, obj, name);
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	LuaModuleByClass<ClassType> LuaRegisterAbstract<ClassType>::def_module(const string & name)
	{
		LogTraceStepCall("LuaModuleByClass<ClassType> LuaRegisterAbstract<ClassType>::def_module(const string & name)");
		LogTraceStepReturn((LuaModuleByClass<ClassType>(m_lua, name, m_value, *this)));
	}

	template <typename ClassType>
	template <typename ET, typename KT>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_enum(ET obj, const KT & name)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_enum(ET obj, const KT & name)");
		LuaStateProtect lsp(m_lua, true);
		if (get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
			LogTraceStepReturn(*this);
		}
		LuaStack<const KT>::push(m_lua, name);
		lua_pushinteger(m_lua, static_cast<lua_Integer>(obj));
		lua_rawset(m_lua, -3);
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	template <typename T, typename KT>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_const_copy(const T & obj, const KT & name)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_const_copy(const T & obj, const KT & name)");
		LuaStateProtect lsp(m_lua, true);
		if (get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
			LogTraceStepReturn(*this);
		}
		LuaStack<const KT>::push(m_lua, name);
		LuaStack<const T>::push(m_lua, obj);
		lua_rawset(m_lua, -3);
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	template <typename T, typename KT>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_const_ptr(const T * obj, const KT & name)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_const_ptr(const T * obj, const KT & name)");
		LuaStateProtect lsp(m_lua, true);
		if (get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
			LogTraceStepReturn(*this);
		}
		LuaStack<const KT>::push(m_lua, name);
		LuaStack<const T *>::push(m_lua, obj);
		lua_rawset(m_lua, -3);
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	template <typename T>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_class_func(T obj, const string & name)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_class_func(T obj, const string & name)");
		LuaStateProtect lsp(m_lua, true);
		if (get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
			LogTraceStepReturn(*this);
		}
		new (lua_newuserdata(m_lua, sizeof(T))) T(obj);
		lua_pushcclosure(m_lua, &LuaCFunctionTraits<T>::call, 1);
		rawsetfieldlen(m_lua, -2, name.c_str(), name.size());
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	template <typename T>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_class_property(T ClassType::* obj, const string & name)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_class_property(T ClassType::* obj, const string & name)");
		typedef LuaClassPropertyInfoImp<T ClassType::*> LCPI;
		LuaStateProtect lsp(m_lua, true);
		if (get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
			LogTraceStepReturn(*this);
		}
		LCPI  * p = (LCPI *)lua_newuserdata(m_lua, sizeof(LCPI));
		new (p) LCPI(name, LuaClassPropertyTraits<ClassType, T>::pt_index_function, LuaClassPropertyTraits<ClassType, T>::pt_newindex_function, p);
		p->set_imp(obj);
		LuaMetatable::push_property_function(m_lua, -1);
		rawsetfieldlen(m_lua, -3, name.c_str(), name.size());
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	template <typename T>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_property(T * obj, const string & name)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_property(T ClassType::* obj, const string & name)");
		LuaStateProtect lsp(m_lua, true);
		if (get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
			LogTraceStepReturn(*this);
		}
		new (lua_newuserdata(m_lua, sizeof(LuaClassPropertyInfo))) LuaClassPropertyInfo(name, LuaClassPropertyTraits<T, void>::pt_index_function, 
																						LuaClassPropertyTraits<T, void>::pt_newindex_function, obj);
		LuaMetatable::push_property_function(m_lua, -1);
		rawsetfieldlen(m_lua, -3, name.c_str(), name.size());
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_cclosure(lua_CFunction obj, const string & name)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_cclosure(lua_CFunction obj, const string & name)");
		LuaStateProtect lsp(m_lua, true);
		if (get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
			LogTraceStepReturn(*this);
		}
		LuaDefAutoTool::def(m_lua, obj, name);
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	RuntimeError LuaRegisterAbstract<ClassType>::get_class_table()
	{
		LogTraceStepCall("RuntimeError LuaRegisterAbstract<ClassType>::get_class_table()");
		push_value();
		if (!lua_istable(m_lua, -1))
		{
			LogTraceStepReturn(RE_Lua_ClassNotRegister);
		}
		LogTraceStepReturn(RE_Success);
	}

	template <typename ClassType>
	void LuaRegisterAbstract<ClassType>::init_class(const string & name)
	{
		LogTraceStepCall("void LuaRegisterAbstract<ClassType>::init_class(const string & name)");
		set_key(name);
		if(LuaClassInfo<ClassType>::is_registered(m_lua)) //have this name
		{
			LuaClassInfo<ClassType>::get_class_table(m_lua);
			set_value(-1);
		}
		else
		{
			LuaClassInfo<ClassType>::set_class_name(name.c_str());
			lua_createtable(m_lua, 0, 12); //class table
			set_value(-1);
			lua_pushlstring (m_lua, name.c_str(), name.size());
			rawsetfield(m_lua, -2, "type__");

			lua_createtable(m_lua, 0, 3); //class metatable
			lua_setmetatable(m_lua, -2);

			lua_pushcclosure(m_lua, &LuaMetatableTraits<ClassType>::mt_is_destroy_function, 0);
			rawsetfield(m_lua, -2, "is_destroy__");

			lua_pushcclosure(m_lua, &LuaMetatableTraits<ClassType>::mt_destroy_function, 0);
			rawsetfield(m_lua, -2, "destroy__");

			lua_pushcclosure(m_lua, &LuaMetatableTraits<ClassType>::mt_force_destroy_function, 0);
			rawsetfield(m_lua, -2, "force_destroy__");

			lua_pushcclosure(m_lua, &LuaMetatableTraits<ClassType>::mt_is_auto_gc_funtcion, 0);
			rawsetfield(m_lua, -2, "is_auto_gc__");

			//lua_pushcclosure(m_lua, &LuaMetatableTraits<ClassType>::mt_set_auto_gc_funtcion, 0);
			//rawsetfield(m_lua, -2, "set_auto_gc__");

			lua_pushboolean(m_lua, 1);
			rawsetfield(m_lua, -2, "is_cppobj__");

			lua_pushvalue(m_lua, -1);
			rawsetfield(m_lua, -2, "__index");

			lua_pushcclosure(m_lua, LuaMetatable::mt_new___function, 0);
			rawsetfield(m_lua, -2, "__call");

			lua_pushcclosure(m_lua, LuaMetatable::mt___gc_function, 0);
			rawsetfield(m_lua, -2, "__gc");

			LuaClassInfo<ClassType>::set_class_table(m_lua, -1);
		}
		LogTraceStepReturnVoid;
	}

	template <typename ClassType>
	bool LuaRegisterAbstract<ClassType>::is_disable_new()
	{
		LogTraceStepCall("bool LuaRegisterAbstract<ClassType>::is_disable_new()");
		LuaStateProtect lsp(m_lua, true);
		if (get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
			LogTraceStepReturn(false);
		}
		rawgetfield(m_lua, -1, "disable_new__");
		LogTraceStepReturn(lua_toboolean(m_lua, -1) != 0);
	}

	template <typename ClassType>
	bool LuaRegisterAbstract<ClassType>::is_disable_create()
	{
		LogTraceStepCall("bool LuaRegisterAbstract<ClassType>::is_disable_create()");
		LuaStateProtect lsp(m_lua, true);
		if (get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
			LogTraceStepReturn(false);
		}
		rawgetfield(m_lua, -1, "disable_create__");
		LogTraceStepReturn(lua_toboolean(m_lua, -1) != 0);
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//LuaModuleByClass
	//////////////////////////////////////////////////////////////////////////////////////
	template <typename ClassType>
	LuaModuleByClass<ClassType>::LuaModuleByClass(lua_State * L, const string & moduleName, const LuaRef & parent, LuaRegisterAbstract<ClassType> & c) : LuaModule(L, moduleName, parent), m_class(c)
	{
		LogTraceStepCall("LuaModuleByClass<ClassType>::LuaModuleByClass(lua_State * L, const string & moduleName, const LuaRef & parent, LuaRegisterAbstract<ClassType> & c)");
		LogTraceStepReturnVoid;
	}

	template <typename ClassType>
	LuaModuleByClass<ClassType>::~LuaModuleByClass()
	{
		LogTraceStepCall("LuaModuleByClass<ClassType>::~LuaModuleByClass()");
		LogTraceStepReturnVoid;
	}

	template <typename ClassType>
	inline LuaRegisterAbstract<ClassType> & LuaModuleByClass<ClassType>::module_end()
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaModuleByClass<ClassType>::module_end()");
		if (m_parent.is_table())
		{
			m_parent.rawset(m_key, m_value);
		}
		LogTraceStepReturn(m_class);
	}

	template <typename ClassType>
	template <typename T>
	LuaModuleByClass<ClassType> & LuaModuleByClass<ClassType>::def(T obj, const string & name)
	{
		LogTraceStepCall("LuaModuleByClass<ClassType> & LuaModuleByClass<ClassType>::def(T obj, const string & name)");
		LuaStateProtect lsp(state(), true);
		push_value();
		LuaDefAutoTool::def(state(), obj, name);
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	template <typename ET, typename KT>
	LuaModuleByClass<ClassType> & LuaModuleByClass<ClassType>::def_enum(ET obj, const KT & key)
	{
		LogTraceStepCall("LuaModuleByClass<ClassType> & LuaModuleByClass<ClassType>::def_enum(ET obj, const KT & key)");
		LuaStateProtect lsp(state(), true);
		push_value();
		LuaStack<const KT>::push(state(), key);
		lua_pushinteger(state(), static_cast<lua_Integer>(obj));
		lua_rawset(state(), -3);
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	template <typename T, typename KT>
	LuaModuleByClass<ClassType> & LuaModuleByClass<ClassType>::def_const_copy(const T & obj, const KT & key)
	{
		LogTraceStepCall("LuaModuleByClass<ClassType> & LuaModuleByClass<ClassType>::def_const_copy(const T & obj, const KT & key)");
		LuaStateProtect lsp(state(), true);
		push_value();
		LuaStack<const KT>::push(state(), key);
		LuaStack<const T>::push(state(), obj);
		lua_rawset(state(), -3);
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	template <typename T, typename KT>
	LuaModuleByClass<ClassType> & LuaModuleByClass<ClassType>::def_const_ptr(const T * obj, const KT & key)
	{
		LogTraceStepCall("LuaModuleByClass<ClassType> & LuaModuleByClass<ClassType>::def_const_ptr(const T * obj, const KT & key)");
		LuaStateProtect lsp(state(), true);
		push_value();
		LuaStack<const KT>::push(state(), key);
		LuaStack<const T *>::push(state(), obj);
		lua_rawset(state(), -3);
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	template <typename T>
	inline LuaRegisterAbstract<ClassType> & LuaModuleByClass<ClassType>::operator[](const T & idx)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaModuleByClass<ClassType>::operator[](T & idx)");
		LuaModule::operator[](idx);
		LogTraceStepReturn(m_class);
	}

}

#endif