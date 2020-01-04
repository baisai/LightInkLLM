


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
#ifndef LIGHTINK_LUAENGINE_LUAOPTIONTOOL_H_
#define LIGHTINK_LUAENGINE_LUAOPTIONTOOL_H_

#include "Common/Type.h"
#include "LuaEngine/LuaLib.h"
#include "Common/STLType.h"
#include "Log/Log.h"
#include "LuaEngine/LuaStateProtect.h"
#include "LuaEngine/LuaCFunctionTraits.h"
#include "LuaEngine/LuaClassPropertyTraits.h"
#include "LuaEngine/LuaMetatable.h"

namespace LightInk
{
	struct LIGHTINK_DECL LuaDefTool
	{
		template <typename T>
		static void def_func(lua_State * L, T obj, const string & name)
		{
			LogTraceStepCall("void LuaDefTool::def_func<T>(lua_State * L, T & obj, const string & name)");
			LuaStateProtect lsp(L, true);
			if (!lua_istable(L, -1))
			{
				//lua_getglobal(L, "_G");
				lua_pushvalue(L, LUA_GLOBALSINDEX);
			}
			else
			{
				lua_pushvalue(L, -1);
			}
			new (lua_newuserdata(L, sizeof(T))) T(obj);
			lua_pushcclosure(L, &LuaCFunctionTraits<T>::call, 1);
			rawsetfieldlen(L, -2, name.c_str(), name.size());
			LogTraceStepReturnVoid;
		}

		template <typename T>
		static void def_property(lua_State * L, T * obj, const string & name)
		{
			LogTraceStepCall("void LuaDefTool::def_property<T>(lua_State * L, T * obj, const string & name)");
			LuaStateProtect lsp(L, true);
			if (!lua_istable(L, -1))
			{
				//lua_getglobal(L, "_G");
				lua_pushvalue(L, LUA_GLOBALSINDEX);
			}
			else
			{
				lua_pushvalue(L, -1);
			}
			new (lua_newuserdata(L, sizeof(LuaClassPropertyInfo))) LuaClassPropertyInfo(name, LuaClassPropertyTraits<T, void>::pt_index_function, 
																						LuaClassPropertyTraits<T, void>::pt_newindex_function, obj);
			LuaMetatable::push_property_function(L, -1);
			rawsetfieldlen(L, -3, name.c_str(), name.size());
			LogTraceStepReturnVoid;
		}

		static void def_cclosure(lua_State * L, lua_CFunction obj, const string & name)
		{
			LogTraceStepCall("void LuaDefTool::def_cclosure(lua_State * L, lua_CFunction ObjectCloseAuditAlarm, const string & name)");
			LuaStateProtect lsp(L, true);
			if (!lua_istable(L, -1))
			{
				//lua_getglobal(L, "_G");
				lua_pushvalue(L, LUA_GLOBALSINDEX);
			}
			else
			{
				lua_pushvalue(L, -1);
			}
			lua_pushlstring(L, name.c_str(), name.size());
			lua_pushcclosure(L, obj, 0);
			lua_rawset(L, -3);
			LogTraceStepReturnVoid;
		}
	};


}

#endif