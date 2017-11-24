


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
#include "LuaEngine/LuaNewClassTraits.h"
#include "LuaEngine/LuaMetatableTraits.h"
#include "LuaEngine/LuaIndexMetatable.h"
#include "LuaEngine/LuaCFunctionTraits.h"

namespace LightInk
{

	template <typename ClassType>
	LuaRegisterAbstract<ClassType>::LuaRegisterAbstract(lua_State * L, const string & name) : m_lua(L), LuaRegisterNode(L)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType>::LuaRegisterAbstract(lua_State * L, const string & name)");
		LuaStateProtect lsp(L, true);
		init_class(name);
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
		int top = lua_gettop(L);
		if (!LuaClassInfo<BaseType>::get_class_table(L))
		{
			LogError("Error!!!Get Base Class Table Failed!!!");
			LogTraceStepReturnVoid;
		}
		rawgetfield(L, -1, "metatable__");
		lua_pushnil(L);
		while (lua_next(L, -2))
		{
			lua_pushvalue(L, -2);
			lua_rawget(L, top);
			if(lua_isnil(L, -1)) //no this field
			{
				lua_pushvalue(L, -3);
				lua_pushvalue(L, -3);
				lua_rawset(L, top);
			}
			lua_pop(L, 2);
		}

		lua_pushnil(L);
		while (lua_next(L, -3))
		{
			lua_pushvalue(L, -2);
			lua_rawget(L, top - 1);
			if (lua_isnil(L, -1))
			{
				lua_pushvalue(L, -3);
				lua_pushvalue(L, -3);
				lua_rawset(L, top - 1);
			}
			lua_pop(L, 2);
		}

		if (get_class_metatable() != RE_Success)
		{
			LogError("Error!!!Get Class Metatable Failed!!!");
			LogTraceStepReturnVoid;
		}

		lua_pushlightuserdata(L, LuaClassInfo<BaseType>::get_class_key());
		lua_pushboolean(L, 1);
		lua_rawset(L, -3);

		
		//pop metatable, class table, parent class table
		LogTraceStepReturnVoid;
	}

	template <typename ClassType>
	LuaRegisterAbstract<ClassType>::LuaRegisterAbstract(const LuaRegisterAbstract<ClassType> & cp) : LuaRegisterNode(cp), m_lua(cp.m_lua)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType>::LuaRegisterAbstract(const LuaRegisterAbstract<ClassType> & cp)");
		LogTraceStepReturnVoid;
	}

	template <typename ClassType>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::operator = (const LuaRegisterAbstract<ClassType> & right)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::operator = (const LuaRegisterAbstract<ClassType> & right)");
		LuaRegisterNode::operator=(right);
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
		if (get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
		}
		LuaRef lr = LuaRef::new_table(m_lua);
		lr.push();
		rawsetfieldlen(m_lua, -2, name.c_str(), name.size());
		LogTraceStepReturn((LuaModuleByClass<ClassType>(m_lua, name, lr, *this)));
	}

	template <typename ClassType>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::disable_new()
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::disable_new()");
		LuaStateProtect lsp(m_lua, true);
		if (get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
		}
		lua_pushnil(m_lua);
		rawsetfield(m_lua, -2, "new__");
		lua_getmetatable(m_lua, -1);
		lua_pushnil(m_lua);
		rawsetfield(m_lua, -2, "__call");
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_cclosure(lua_CFunction obj, const string & name)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_cclosure(lua_CFunction obj, const string & name)");
		LuaStateProtect lsp(m_lua, true);
		if (get_class_metatable() != RE_Success)
		{
			LogError("Error!!!Get Class Metatable Failed!!!");
		}
		LuaDefAutoTool::def(m_lua, obj, name);
		lua_pop(m_lua, 1);
		LuaDefAutoTool::def(m_lua, obj, name);
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	template <typename ET>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_enum(ET obj, const string & name)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_enum(ET obj, const string & name)");
		LuaStateProtect lsp(m_lua, true);
		if (get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
		}
		lua_pushinteger(m_lua, static_cast<lua_Integer>(obj));
		rawsetfieldlen(m_lua, -2, name.c_str(), name.size());
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	template <typename T>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_const_copy(const T & obj, const string & name)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_const_copy(const T & obj, const string & name)");
		LuaStateProtect lsp(m_lua, true);
		if (get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
		}
		LuaStack<const T>::push(m_lua, obj);
		rawsetfieldlen(m_lua, -2, name.c_str(), name.size());
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	template <typename T>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_const_ptr(const T * obj, const string & name)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_const_ptr(const T * obj, const string & name)");
		LuaStateProtect lsp(m_lua, true);
		if (get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
		}
		LuaStack<const T *>::push(m_lua, obj);
		rawsetfieldlen(m_lua, -2, name.c_str(), name.size());
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	template <typename T>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_class_func(T obj, const string & name)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_class_func(T obj, const string & name)");
		LuaStateProtect lsp(m_lua, true);
		if (get_class_metatable() != RE_Success)
		{
			LogError("Error!!!Get Class Metatable Failed!!!");
		}
		new (lua_newuserdata(m_lua, sizeof(LuaClassFunctionInfo<T>))) LuaClassFunctionInfo<T>(obj);
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
		if (get_class_metatable() != RE_Success)
		{
			LogError("Error!!!Get Class Metatable Failed!!!");
		}
		LCPI  * p = (LCPI *)lua_newuserdata(m_lua, sizeof(LCPI));
		new (p) LCPI(LuaClassPropertyTraits<ClassType, T>::pt_index_function, LuaClassPropertyTraits<ClassType, T>::pt_newindex_function, NULL);
		p->m_imp = obj;
		p->m_property = p;
		rawsetfieldlen(m_lua, -2, name.c_str(), name.size());
		LogTraceStepReturn(*this);
	}

	template <typename ClassType>
	template <typename T>
	LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_func(T obj, const string & name)
	{
		LogTraceStepCall("LuaRegisterAbstract<ClassType> & LuaRegisterAbstract<ClassType>::def_func(T obj, const string & name)");
		LuaStateProtect lsp(m_lua, true);
		if (get_class_metatable() != RE_Success)
		{
			LogError("Error!!!Get Class Metatable Failed!!!");
		}
		new (lua_newuserdata(m_lua, sizeof(T))) T(obj);
		lua_pushcclosure(m_lua, &LuaCFunctionTraits<T>::call, 1);
		lua_pushvalue(m_lua, -1);
		rawsetfieldlen(m_lua, -3, name.c_str(), name.size());
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
		}
		if (!lua_getmetatable(m_lua, -1))
		{
			LogError("Error!!!The Class Table Have Not Metatable!!!");
		}
		new (lua_newuserdata(m_lua, sizeof(LuaClassPropertyInfo))) LuaClassPropertyInfo(LuaClassPropertyTraits<T, void>::pt_index_function, 
																						LuaClassPropertyTraits<T, void>::pt_newindex_function, obj);
		rawsetfieldlen(m_lua, -2, name.c_str(), name.size());
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
	RuntimeError LuaRegisterAbstract<ClassType>::get_class_metatable()
	{
		LogTraceStepCall("RuntimeError LuaRegisterAbstract<ClassType>::get_class_metatable()");
		if (get_class_table()!= RE_Success)
		{
			LogTraceStepReturn(RE_Lua_ClassNotRegister);
		}
		rawgetfield(m_lua, -1, "metatable__");
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
		LuaClassInfo<ClassType>::set_class_name(name.c_str());
		if(LuaClassInfo<ClassType>::is_registered(m_lua)) //have this name
		{
			LuaClassInfo<ClassType>::get_class_metatable(m_lua);
			set_value(-2);
			//lua_rawgetp(m_lua, -1, LuaClassInfo<ClassType>::get_class_key());
		}
		else
		{
			lua_createtable(m_lua, 0, 3); //class table
			set_value(-1);
			lua_pushlstring (m_lua, name.c_str(), name.size());
			rawsetfield(m_lua, -2, "type__");

			lua_createtable(m_lua, 0, 3); //class table
			lua_pushcclosure(m_lua, &LuaIndexMetatable::ct_index_function, 0);
			rawsetfield(m_lua, -2, "__index");
			lua_pushcclosure(m_lua, &LuaIndexMetatable::ct_newindex_function, 0);
			rawsetfield(m_lua, -2, "__newindex");

			lua_setmetatable(m_lua, -2);


			lua_createtable(m_lua, 0, 8); // userdata metatable
			lua_pushvalue(m_lua, -1);
			rawsetfield(m_lua, -3, "metatable__");
			//lua_rawsetp(m_lua, -3, LuaClassInfo<ClassType>::get_class_key());

			lua_pushlstring (m_lua, name.c_str(), name.size());
			rawsetfield(m_lua, -2, "type__");

			lua_pushlightuserdata(m_lua, LuaClassInfo<ClassType>::get_class_key());
			rawsetfield(m_lua, -2, "key__");

			lua_pushboolean(m_lua, 1);
			lua_rawsetp(m_lua, -2, LuaClassInfo<ClassType>::get_class_key());

			lua_pushcclosure(m_lua, &LuaMetatableTraits<ClassType>::mt_isdelete_function, 0);
			rawsetfield(m_lua, -2, "isdelete__");

			lua_pushcclosure(m_lua, &LuaMetatableTraits<ClassType>::mt_delete_function, 0);
			rawsetfield(m_lua, -2, "delete__");

			lua_pushcclosure(m_lua, &LuaMetatableTraits<ClassType>::mt_force_delete_function, 0);
			rawsetfield(m_lua, -2, "force_delete__");

			lua_pushcclosure(m_lua, &LuaIndexMetatable::mt_index_function, 0);
			rawsetfield(m_lua, -2, "__index");

			lua_pushcclosure(m_lua, &LuaIndexMetatable::mt_newindex_function, 0);
			rawsetfield(m_lua, -2, "__newindex");

			lua_pushcclosure(m_lua, &LuaMetatableTraits<ClassType>::mt_gc_function, 0);
			rawsetfield(m_lua, -2, "__gc");

			LuaClassInfo<ClassType>::set_class_table(m_lua, -2);
		}
		LogTraceStepReturnVoid;
	}


	//////////////////////////////////////////////////////////////////////////////////////
	//LuaModuleByClass
	//////////////////////////////////////////////////////////////////////////////////////
	template <typename ClassType>
	LuaModuleByClass<ClassType>::LuaModuleByClass(lua_State * L, const string & moduleName, const LuaRef & table, LuaRegisterAbstract<ClassType> & c) : LuaModule(L, moduleName, table), m_class(c)
	{
		LogTraceStepCall("LuaModuleByClass<ClassType>::LuaModuleByClass(lua_State * L, const string & moduleName, const LuaRef & table, LuaRegisterAbstract<ClassType> & c)");
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
		LogTraceStepReturn(m_class);
	}

	template <typename ClassType>
	template <typename T>
	LuaModuleByClass<ClassType> & LuaModuleByClass<ClassType>::def(T obj, const string & name)
	{
		LogTraceStepCall("LuaModuleByClass<ClassType> & LuaModuleByClass<ClassType>::def(T obj, const string & name)");
		push_value();
		LuaDefAutoTool::def(state(), obj, name);
		lua_pop(state(), 1);
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