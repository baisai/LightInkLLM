


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
#include "Log/Log.h"
#include "LuaEngine/LuaDebugger.h"
#include "LuaEngine/LuaStateProtect.h"
#include "LuaEngine/LuaFixFunction.h"
#include "LuaEngine/LuaClassMgr.h"
#include "LuaEngine/LuaUserdataForClass.h"

namespace LightInk
{
	template <typename T>
	class LIGHTINK_TEMPLATE_DECL LuaClassInfo
	{
	public:
		template<class BaseType>
		inline static int32 get_class_offset()
		{
			LogTraceStepCall("int32 LuaClassInfo<T>::get_class_offset<BaseType>()");
			LogTraceStepReturn((((char*)static_cast<BaseType*>((T*)1)) - (char*)1));
		}

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

		template<class BaseType>
		inline static bool register_base_class(lua_State * L)
		{
			LogTraceStepCall("bool add_base_class(lua_State * L)");
			int32 classOffset = get_class_offset<BaseType>();
			bool success = LuaClassMgr::get_class_mgr(L)->add_base_class(get_class_key(), LuaClassInfo<BaseType>::get_class_key(), classOffset);
			LogTraceStepReturn(success);
		}

		inline static bool is_registered(lua_State * L)
		{
			LogTraceStepCall("bool is_registered(lua_State * L)");
			LuaStateProtect lsp(L, true);
			lua_pushlightuserdata(L, get_class_key());
			lua_rawget(L, LUA_REGISTRYINDEX);
			if (lua_istable(L, -1))
			{
				LogTraceStepReturn(true);
			}
			LogTraceStepReturn(false);
		}

		inline static int32 calc_from_offset(lua_State * L, int fromIdx)
		{
			LogTraceStepCall("int32 LuaClassInfo<T>::calc_from_offset(lua_State * L, int fromIdx)");
			LuaStateProtect lsp(L, true);
			int32 offset = LuaClassMgr::InvalidOffset;
			fromIdx = lua_absindex(L, fromIdx);
			//lua_pushstring (L, "key__");
			lua_pushlightuserdata(L, LuaClassMgr::get_class_key());
			lua_gettable(L, fromIdx);
			void * fromKey = lua_touserdata(L, -1);
			offset = LuaClassMgr::get_class_mgr(L)->get_class_offset(fromKey, get_class_key());
			LogTraceStepReturn(offset);
		}

		inline static int32 calc_to_offset(lua_State * L, int toIdx)
		{
			LogTraceStepCall("int32 LuaClassInfo<T>::calc_to_offset(lua_State * L, int toIdx)");
			LuaStateProtect lsp(L, true);
			int32 offset = LuaClassMgr::InvalidOffset;
			toIdx = lua_absindex(L, toIdx);
			//lua_pushstring(L, "key__");
			lua_pushlightuserdata(L, LuaClassMgr::get_class_key());
			lua_gettable(L, toIdx);
			void * toKey = lua_touserdata(L, -1);
			offset = LuaClassMgr::get_class_mgr(L)->get_class_offset(get_class_key(), toKey);
			LogTraceStepReturn(offset);
		}

		static bool get_class_table(lua_State * L)
		{
			LogTraceStepCall("bool LuaClassInfo<T>::get_class_table(lua_State * L)");
			lua_pushlightuserdata(L, get_class_key());
			lua_rawget(L, LUA_REGISTRYINDEX);
			if (lua_istable(L, -1))
			{
				LogTraceStepReturn(true);
			}
			LogScriptError(L, "Error!!!This  class {} is not register!!!", m_className);
			LogTraceStepReturn(false);
		}

		static bool set_class_table(lua_State * L, int idx)
		{
			LogTraceStepCall("LuaClassInfo<T>::set_class_table(lua_State * L, int idx)");
			idx = lua_absindex(L, idx);
			LuaStateProtect lsp(L, true);
			lua_pushvalue(L, idx);
			lua_rawsetp(L, LUA_REGISTRYINDEX, get_class_key());
			lua_pushlightuserdata(L, get_class_key());
			//rawsetfield(L, idx, "key__");
			lua_rawsetp(L, idx, LuaClassMgr::get_class_key());
			LuaClassMgr::get_class_mgr(L)->register_class(get_class_key());
			LogTraceStepReturn(true);
		}

	private:
		static char m_key;
		static char m_metaKey;
		static char m_className[128];
	};

	template<typename T>
	char LuaClassInfo<T>::m_key = 0;
	template<typename T>
	char LuaClassInfo<T>::m_className[128] = { 0 };
}

#endif