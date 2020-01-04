


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

#ifndef LIGHTINK_LUAENGINE_LUAREGISTER_CPP_
#define LIGHTINK_LUAENGINE_LUAREGISTER_CPP_

#include "LuaEngine/LuaRegister.h"

namespace LightInk
{
	template <typename ClassType, typename CtorType>
	LuaRegister<ClassType, CtorType>::LuaRegister(lua_State * L, const string & name) : LuaRegisterAbstract<ClassType>(L, name)
	{
		LogTraceStepCall("LuaRegister<ClassType, CtorType>::LuaRegister(lua_State * L, const string & name)");
		check_new();
		LogTraceStepReturnVoid;
	}

	template <typename ClassType, typename CtorType>
	template <typename BaseType>
	LuaRegister<ClassType, CtorType>::LuaRegister(lua_State * L, const string & name, BaseClassStrategy<BaseType> bcs) : LuaRegisterAbstract<ClassType>(L, name, bcs)
	{
		LogTraceStepCall("LuaRegister<ClassType, CtorType>::LuaRegister(lua_State * L, const string & name, BaseClassStrategy<BaseType> bcs)");
		check_new();
		LogTraceStepReturnVoid;
	}

	template <typename ClassType, typename CtorType>
	LuaRegister<ClassType, CtorType> & LuaRegister<ClassType, CtorType>::disable_new_create__()
	{
		LogTraceStepCall("LuaRegister<ClassType, CtorType> & LuaRegister<ClassType, CtorType>::disable_new_create__()");
		disable_new__();
		disable_create__();
		LogTraceStepReturn(*this);
	}

	template <typename ClassType, typename CtorType>
	LuaRegister<ClassType, CtorType> & LuaRegister<ClassType, CtorType>::disable_new__()
	{
		LogTraceStepCall("LuaRegister<ClassType, CtorType> & LuaRegister<ClassType, CtorType>::disable_new__()");
		LuaStateProtect lsp(this->m_lua, true);
		if (this->get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
			LogTraceStepReturn(*this);
		}

		lua_pushboolean(this->m_lua, 1);
		rawsetfield(this->m_lua, -2, "disable_new__");
		lua_pushnil(this->m_lua);
		rawsetfield(this->m_lua, -2, "new__");
		lua_pushnil(this->m_lua);
		rawsetfield(this->m_lua, -2, "newcpp__");

		lua_getmetatable(this->m_lua, -1);
		lua_pushnil(this->m_lua);
		rawsetfield(this->m_lua, -2, "__call");
		LogTraceStepReturn(*this);
	}

	template <typename ClassType, typename CtorType>
	LuaRegister<ClassType, CtorType> & LuaRegister<ClassType, CtorType>::disable_create__()
	{
		LogTraceStepCall("LuaRegister<ClassType, CtorType> & LuaRegister<ClassType, CtorType>::disable_create__()");
		LuaStateProtect lsp(this->m_lua, true);
		if (this->get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
			LogTraceStepReturn(*this);
		}

		lua_pushboolean(this->m_lua, 1);
		rawsetfield(this->m_lua, -2, "disable_create__");
		lua_pushnil(this->m_lua);
		rawsetfield(this->m_lua, -2, "create__");
		lua_pushnil(this->m_lua);
		rawsetfield(this->m_lua, -2, "createcpp__");
		lua_pushnil(this->m_lua);
		rawsetfield(this->m_lua, -2, "destroy__");
		LogTraceStepReturn(*this);
	}

	template <typename ClassType, typename CtorType>
	LuaRegister<ClassType, CtorType> & LuaRegister<ClassType, CtorType>::reset_ctor(lua_CFunction ctorFunc)
	{
		LogTraceStepCall("LuaRegister<ClassType, CtorType> & LuaRegister<ClassType, CtorType>::reset_ctor(lua_CFunction ctorFunc)");
		LuaStateProtect lsp(this->m_lua, true);
		if (this->get_class_table() != RE_Success)
		{
			LogError("Error!!!Get Class Table Failed!!!");
			LogTraceStepReturn(*this);
		}

		rawgetfield(this->m_lua, -1, "disable_new__");
		if (lua_toboolean(this->m_lua, -1) == 0)
		{
			lua_pushcclosure(this->m_lua, ctorFunc, 0);
			lua_pushcclosure(this->m_lua, &lua_class_create_user<ClassType, true>, 1);
			rawsetfield(this->m_lua, -3, "newcpp__"); //class table
		}
		lua_pop(this->m_lua, 1);

		rawgetfield(this->m_lua, -1, "disable_create__");
		if (lua_toboolean(this->m_lua, -1) == 0)
		{
			lua_pushcclosure(this->m_lua, ctorFunc, 0);
			lua_pushcclosure(this->m_lua, &lua_class_create_user<ClassType, false>, 1);
			rawsetfield(this->m_lua, -3, "createcpp__"); //class table
		}
		lua_pop(this->m_lua, 1);

		lua_pushcclosure(this->m_lua, ctorFunc, 0);
		lua_pushcclosure(this->m_lua, &lua_class_ctor_user<ClassType, true>, 1);
		rawsetfield(this->m_lua, -2, "ctor");

		LogTraceStepReturn(*this);
	}

	template <typename ClassType, typename CtorType>
	void LuaRegister<ClassType, CtorType>::check_new()
	{
		LogTraceStepCall("void LuaRegister<ClassType, CtorType>::check_new()");
		RuntimeError err = add_ctor();
		if (err != RE_Success)
		{
			LogError(get_runtime_error(err));
			LogTraceStepReturnVoid;
		}

		if (!this->is_disable_new())
		{
			err = add_new();
			if (err != RE_Success)
			{
				LogError(get_runtime_error(err));
			}
		}

		if (!this->is_disable_create())
		{
			err = add_create();
			if (err != RE_Success)
			{
				LogError(get_runtime_error(err));
			}
		}
		LogTraceStepReturnVoid;
	}

	template <typename ClassType, typename CtorType>
	RuntimeError LuaRegister<ClassType, CtorType>::add_new()
	{
		LogTraceStepCall("RuntimeError LuaRegister<ClassType, CtorType>::add_new()");
		LuaStateProtect lsp(this->m_lua, true);
		RuntimeError err = this->get_class_table();
		if (err != RE_Success)
		{
			LogTraceStepReturn(err);
		}
		lua_pushcclosure(this->m_lua, LuaMetatable::mt_new___function, 0);
		rawsetfield(this->m_lua, -2, "new__"); //class table
		lua_pushvalue(this->m_lua, -1);
		lua_pushcclosure(this->m_lua, &LuaClassCreateTraits<ClassType, true, CtorType>::call, 1);
		rawsetfield(this->m_lua, -2, "newcpp__"); //class table

		lua_getmetatable(this->m_lua, -1);
		lua_pushcclosure(this->m_lua, LuaMetatable::mt_new___function, 0);
		rawsetfield(this->m_lua, -2, "__call"); //class table
		//pop metatable, class table, parent class table
		LogTraceStepReturn(RE_Success);
	}

	template <typename ClassType, typename CtorType>
	RuntimeError LuaRegister<ClassType, CtorType>::add_create()
	{
		LogTraceStepCall("RuntimeError LuaRegister<ClassType, CtorType>::add_create()");
		LuaStateProtect lsp(this->m_lua, true);
		RuntimeError err = this->get_class_table();
		if (err != RE_Success)
		{
			LogTraceStepReturn(err);
		}

		lua_pushcclosure(this->m_lua, &LuaMetatableTraits<ClassType>::mt_create___function, 0);
		rawsetfield(this->m_lua, -2, "create__"); //class table
		lua_pushvalue(this->m_lua, -1);
		lua_pushcclosure(this->m_lua, &LuaClassCreateTraits<ClassType, false, CtorType>::call, 1);
		rawsetfield(this->m_lua, -2, "createcpp__"); //class table

		//class table, parent class table
		LogTraceStepReturn(RE_Success);
	}

	template <typename ClassType, typename CtorType>
	RuntimeError LuaRegister<ClassType, CtorType>::add_ctor()
	{
		LogTraceStepCall("RuntimeError LuaRegister<ClassType, CtorType>::add_ctor()");
		LuaStateProtect lsp(this->m_lua, true);
		RuntimeError err = this->get_class_table();
		if (err != RE_Success)
		{
			LogTraceStepReturn(err);
		}
		rawgetfield(this->m_lua, -1, "ctor");
		if (lua_isnoneornil(this->m_lua, -1))
		{
			lua_pop(this->m_lua, 1);
			lua_pushvalue(this->m_lua, -1);
			lua_pushcclosure(this->m_lua, &LuaClassCtorTraits<ClassType, true, CtorType>::call, 1);
			rawsetfield(this->m_lua, -2, "ctor");
		}
		LogTraceStepReturn(RE_Success);
	}
}

#endif