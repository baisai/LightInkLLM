


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
#ifndef LIGHTINK_LUAENGINE_LUAOBJECTCACHE_H_
#define LIGHTINK_LUAENGINE_LUAOBJECTCACHE_H_

#include "Common/Type.h"
#include "Log/Log.h"
#include "Log/LoggerMgr.h"
#include "LuaEngine/LuaLib.h"

namespace LightInk
{
	template<typename T>
	void lua_set_cache_object(lua_State * L, const T * obj, int objectIdx)
	{
		LogTraceStepCall("void lua_set_cache_object<T>(lua_State * L, const T * obj, int objectIdx)");
		LuaStateProtect lsp(L, true);
		objectIdx = lua_absindex(L, objectIdx);
		lua_pushlightuserdata(L, LuaClassMgr::get_weak_key());
		lua_rawget(L, LUA_REGISTRYINDEX);
		if (!lua_istable(L, -1))
		{
			lua_pop(L, 1);
			lua_createtable(L, 0, 128);
			lua_pushstring(L, "__mode");
			lua_pushstring(L, "v");
			lua_rawset(L, -3);
			lua_pushvalue(L, -1);
			lua_setmetatable(L, -2);
			lua_pushlightuserdata(L, LuaClassMgr::get_weak_key());
			lua_pushvalue(L, -2);
			lua_rawset(L, LUA_REGISTRYINDEX);
		}
		lua_pushlightuserdata(L, (void *)obj);
		lua_pushvalue(L, objectIdx);
		lua_rawset(L, -3);
		LogTraceStepReturnVoid;
	}

	template <typename T>
	bool lua_get_cache_object(lua_State * L, const T * obj)
	{
		LogTraceStepCall("bool lua_get_cache_object<T>(lua_State * L, const T * obj)");
		lua_pushlightuserdata(L, LuaClassMgr::get_weak_key());
		lua_rawget(L, LUA_REGISTRYINDEX);
		if (lua_istable(L, -1))
		{
			lua_pushlightuserdata(L, (void *)obj);
			lua_rawget(L, -2);
			lua_remove(L, -2);
			if (!lua_isnoneornil(L, -1))
			{
				LogTraceStepReturn(true);
			}
		}
		lua_pop(L, 1);
		LogTraceStepReturn(false);
	}

	template<typename T>
	void lua_push_object(lua_State * L, int classIdx, const T * obj, int offset, bool luaGC)
	{
		LogTraceStepCall("void lua_push_object<T>(lua_State * L, int classIdx, const T * obj, int offset, bool luaGC)");
		if (obj == NULL)
		{
			lua_pushnil(L);
			LogTraceStepReturnVoid;
		}
		if (lua_get_cache_object(L, obj))
		{
			LogTraceStepReturnVoid;
		}
		classIdx = lua_absindex(L, classIdx);
		void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass));
		lua_pushvalue(L, classIdx);
		lua_setmetatable(L, -2);
		new(userdataPtr) LuaUserdataForClass(obj, offset, luaGC);
		LogTraceStepReturnVoid;
	}

	template<typename T>
	void lua_push_object_user(lua_State * L, int classIdx, const T * obj, int offset, bool autoGC, typename GCCallbackT<T>::CallbackType luaGC, void * context)
	{
		LogTraceStepCall("void lua_push_object_user<T>(lua_State * L, int classIdx, const T * obj, int offset, bool autoGC, typename GCCallbackT<T>::CallbackType luaGC, void * context)");
		if (obj == NULL)
		{
			lua_pushnil(L);
			LogTraceStepReturnVoid;
		}
		if (lua_get_cache_object(L, obj))
		{
			LogTraceStepReturnVoid;
		}
		classIdx = lua_absindex(L, classIdx);
		void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass));
		lua_pushvalue(L, classIdx);
		lua_setmetatable(L, -2);
		new(userdataPtr) LuaUserdataForClass(obj, offset, autoGC, luaGC, context);
		LogTraceStepReturnVoid;
	}

}

#endif