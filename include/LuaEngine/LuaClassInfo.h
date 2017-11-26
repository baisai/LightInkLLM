


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

#ifndef LIGHTINK_LUAENGINE_LUACLASSINFO_H_
#define LIGHTINK_LUAENGINE_LUACLASSINFO_H_

#include <string.h>
#include "LuaEngine/LuaLib.h"
#include "LuaEngine/LuaStateProtect.h"
#include "Log/Log.h"
#include "LuaEngine/LuaFixFunction.h"

namespace LightInk
{
	template <typename T>
	class LIGHTINK_TEMPLATE_DECL LuaClassInfo
	{
	public:
		inline static const char * get_class_name()
		{
			LogTraceStepCall("const char * LuaClassInfo<T>::get_class_name()");
			LogTraceStepReturn(m_className);
		}
		static void set_class_name(const char * name)
		{
			LogTraceStepCall("void LuaClassInfo<T>::set_class_name(const char * name)");
			if (have_name())
			{
				LogTraceStepReturnVoid;
			}
			strncpy(m_className, name, 128);
			LogTraceStepReturnVoid;
		}
		inline static void * get_class_key()
		{
			LogTraceStepCall("void * LuaClassInfo<T>::get_class_key()");
			LogTraceStepReturn(&m_key);
		}

		inline static bool have_name()
		{
			LogTraceStepCall("bool LuaClassInfo<T>::have_name()");
			LogTraceStepReturn((m_className[0] != '\0'));
		}

		inline static bool is_registered(lua_State * L)
		{
			LogTraceStepCall("bool is_registered(lua_State * L)");
			LuaStateProtect lsp(L, true);
			lua_pushlightuserdata(L, &m_key);
			lua_rawget(L, LUA_REGISTRYINDEX);
			
			if (lua_istable(L, -1))
			{
				LogTraceStepReturn(true);
			}
			LogTraceStepReturn(false);
		}

		inline static bool check_registered(lua_State * L, int idx)
		{
			LogTraceStepCall("bool check_registered(lua_State * L, int idx)");
			LuaStateProtect lsp(L, true);
			idx = lua_absindex(L, idx);
			lua_pushlightuserdata(L, &m_key);
			lua_rawget(L, idx);
			LogTraceStepReturn((lua_toboolean(L, -1) != 0));
		}

		static bool get_class_table(lua_State * L)
		{
			LogTraceStepCall("bool LuaClassInfo<T>::get_class_table(lua_State * L)");
			lua_pushlightuserdata(L, &m_key);
			lua_rawget(L, LUA_REGISTRYINDEX);
			if (lua_istable(L, -1))
			{
				LogTraceStepReturn(true);
			}
			LogScriptError("Error!!!This  class {} is not register!!!", m_className);
			LogTraceStepReturn(false);
		}

		static bool get_class_metatable(lua_State * L)
		{
			LogTraceStepCall("bool LuaClassInfo<T>::get_class_metatable(lua_State * L)");
			if (!get_class_table(L))
			{
				LogTraceStepReturn(false);
			}
			lua_pushstring(L, "metatable__");
			lua_rawget(L, -2);
			LogTraceStepReturn(true);
		}

		static bool set_class_table(lua_State * L, int idx)
		{
			LogTraceStepCall("LuaClassInfo<T>::set_class_table(lua_State * L, int idx)");
			{
				LuaStateProtect lsp(L, true);
				lua_pushvalue(L, idx);
				lua_pushstring(L, "metatable__");
				lua_rawget(L, -2);
				if (!lua_istable(L, -1))
				{
					LogScriptError("Error!!!This  class {} do not have metatable__!!!", m_className);
					LogTraceStepReturn(false);
				}
				lua_pushstring(L, "key__");
				lua_rawget(L, -2);
				lua_pushlightuserdata(L, &m_key);
				if (!lua_rawequal(L, -1, -2))
				{
					LogScriptError("Error!!!This  class {} do not have metatable__!!!", m_className);
					LogTraceStepReturn(false);
				}
			}
			lua_pushvalue(L, idx);
			lua_rawsetp(L, LUA_REGISTRYINDEX, &m_key);
			LogTraceStepReturn(true);
		}
	private:
		static char m_key;
		static char m_className[128];
	};

	template<typename T>
	char LuaClassInfo<T>::m_key = 0;
	template<typename T>
	char LuaClassInfo<T>::m_className[128] = { 0 };
}

#endif