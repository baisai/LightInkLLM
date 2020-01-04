


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

#include "Common/Type.h"
#include "Log/Log.h"
#include "LuaEngine/LuaClassPropertyTraits.h"
#include "LuaEngine/LuaMetatable.h"
#include "LuaEngine/LuaEngine.h"
#include "LuaEngine/LuaUserdataForClass.h"

namespace LightInk
{
	LuaUserdataForClass * LuaMetatable::get_userdata(lua_State * L, int idx)
	{
		LogTraceStepCall("LuaUserdataForClass * LuaMetatable::get_userdata(lua_State * L, int idx)");
		LuaStateProtect lsp(L, true);
		idx = lua_absindex(L, idx);
		if (lua_istable(L, idx))
		{
			lua_pushlightuserdata(L, LuaClassMgr::get_object_key());
			lua_rawget(L, idx);
		}
		else if (lua_isuserdata(L, idx))
		{
			lua_pushvalue(L, idx);
		}
		else
		{
			if (!lua_isnoneornil(L, idx))
				LogScriptError(L, "Error!!!The arg {} is not table or not userdata!!!", idx);
			LogTraceStepReturn(NULL);
		}
		LuaUserdataForClass * p = (LuaUserdataForClass *)lua_touserdata(L, -1);
		LogTraceStepReturn(p);
	}

	int LuaMetatable::mt_property_getset_function(lua_State * L)
	{
		LogTraceStepCall("int LuaMetatable::mt_property_getset_function(lua_State * L)");
		LuaClassPropertyInfo * p = (LuaClassPropertyInfo *) lua_touserdata(L, lua_upvalueindex(1));
		if (p != NULL)
		{
			if (lua_isnoneornil(L, 2))
				LogTraceStepReturn(p->call_index(L));
			else
				LogTraceStepReturn(p->call_newindex(L, 2));
		}
		LogScriptError(L, "Error!!!Not this field!!!");
		LogTraceStepReturn(0);
	}

	int LuaMetatable::mt_property___gc_function(lua_State * L)
	{
		LogTraceStepCall("int LuaMetatable::mt_property___gc_function(lua_State * L)");
		if (lua_isuserdata(L, 1))
		{
			LuaClassPropertyInfo * p = (LuaClassPropertyInfo *) lua_touserdata(L, 1);
			if (p != NULL) { p->~LuaClassPropertyInfo(); }
		}
		LogTraceStepReturn(0);
	}

	bool LuaMetatable::push_property_function(lua_State * L, int propertyIdx)
	{
		LogTraceStepCall("bool LuaMetatable::push_property_function(lua_State * L, int propertyIdx)");
		if (lua_isuserdata(L, propertyIdx))
		{
			propertyIdx = lua_absindex(L, propertyIdx);
			lua_pushlightuserdata(L, (void *)push_property_function);
			lua_rawget(L, LUA_REGISTRYINDEX);
			if (!lua_istable(L, -1))
			{
				lua_pop(L, 1);
				lua_createtable(L, 0, 1);
				lua_pushstring(L, "__gc");
				lua_pushcclosure(L, mt_property___gc_function, 0);
				lua_rawset(L, -3);

				lua_pushlightuserdata(L, (void *)push_property_function);
				lua_pushvalue(L, -2);
				lua_rawset(L, LUA_REGISTRYINDEX);
			}
			lua_setmetatable(L, propertyIdx);
			lua_pushvalue(L, propertyIdx);
			lua_pushcclosure(L, mt_property_getset_function, 1);
			LogTraceStepReturn(true);
		}
		LogTraceStepReturn(false);
	}

	int LuaMetatable::mt___gc_function(lua_State * L)
	{
		LogTraceStepCall("int LuaMetatable::mt___gc_function(lua_State * L)");
		if (lua_isuserdata(L, 1))
		{
			LuaUserdataForClass * p = (LuaUserdataForClass *)lua_touserdata(L, 1);
			if (p != NULL) { p->~LuaUserdataForClass(); }
		}
		LogTraceStepReturn(0);
	}

	int LuaMetatable::mt_new___function(lua_State * L)
	{
		LogTraceStepCall("int LuaMetatable::mt_new___function(lua_State * L)");
		if (lua_istable(L, 1))
		{
			int top = lua_gettop(L);
			lua_createtable(L, 0, 0);
			lua_pushvalue(L, 1);
			lua_setmetatable(L, -2);

			lua_getfield(L, 1, "ctor");
			if (lua_isfunction(L, -1))
			{
				lua_pushvalue(L, -2);
				for (int i = 2; i <= top; ++i)
				{
					lua_pushvalue(L, i);
				}
				lua_call(L, top, 0);
			}
			else
			{
				lua_pop(L, 1);
			}
			LogTraceStepReturn(1);
		}
		LogTraceStepReturn(0);
	}

}