


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
#ifndef LIGHTINK_LUAENGINE_LUAFIXFUNCTION_H_
#define LIGHTINK_LUAENGINE_LUAFIXFUNCTION_H_

#include "Common/Type.h"
#include "LuaEngine/LuaLib.h"


#if LUA_VERSION_NUM < 502
	inline LIGHTINK_DECL int lua_absindex (lua_State* L, int idx)
	{
		if (idx > LUA_REGISTRYINDEX && idx < 0)
		return lua_gettop (L) + idx + 1;
		else
		return idx;
	}

	inline LIGHTINK_DECL void lua_rawgetp (lua_State* L, int idx, void const* p)
	{
		idx = lua_absindex (L, idx);
		lua_pushlightuserdata (L, const_cast <void*> (p));
		lua_rawget (L,idx);
	}

	inline LIGHTINK_DECL void lua_rawsetp (lua_State* L, int idx, void const* p)
	{
		idx = lua_absindex (L, idx);
		lua_pushlightuserdata (L, const_cast <void*> (p));
		// put key behind value
		lua_insert (L, -2);
		lua_rawset (L, idx);
	}


#else
	inline LIGHTINK_DECL int lua_objlen (lua_State* L, int idx)
	{
		return lua_rawlen(L, idx);
	}

#endif


	
	//Get a table value, bypassing metamethods.
	inline LIGHTINK_DECL void rawgetfield (lua_State* L, int index, const char * key)
	{
	  index = lua_absindex (L, index);
	  lua_pushstring (L, key);
	  lua_rawget (L, index);
	}

	//Set a table value, bypassing metamethods.
	inline LIGHTINK_DECL void rawsetfield (lua_State* L, int index, const char * key)
	{
	  index = lua_absindex (L, index);
	  lua_pushstring (L, key);
	  lua_insert (L, -2);
	  lua_rawset (L, index);
	}

	//Get a table value, bypassing metamethods.
	inline LIGHTINK_DECL void rawgetfieldlen (lua_State* L, int index, const char * key, size_t l)
	{
	  index = lua_absindex (L, index);
	  lua_pushlstring (L, key, l);
	  lua_rawget (L, index);
	}

	//Set a table value, bypassing metamethods.
	inline LIGHTINK_DECL void rawsetfieldlen (lua_State* L, int index, const char * key, size_t l)
	{
	  index = lua_absindex (L, index);
	  lua_pushlstring (L, key, l);
	  lua_insert (L, -2);
	  lua_rawset (L, index);
	}

	//Returns true if the value is a full userdata (not light).
	inline LIGHTINK_DECL bool isfulluserdata (lua_State* L, int index)
	{
	  return lua_isuserdata (L, index) && !lua_islightuserdata (L, index);
	}

	inline LIGHTINK_DECL void lock_metatable(lua_State * L, int index)
	{
		index = lua_absindex(L, index);
		lua_pushstring(L, "__metatable");
		lua_pushstring(L, "error!!!metatable is locked!!!");
		lua_rawset(L, index);
	}

	/*
	inline LIGHTINK_DECL int pcall_callback(lua_State * L)
	{
		LogTraceStepCall("int pcall_callback(lua_State * L)");
		lua_Debug debug= {};  
		int ret = lua_getstack(L, 2, &debug); // 0是pcall_callback_err_fun自己, 1是error函数, 2是真正出错的函数  
		lua_getinfo(L, "Sln", &debug);  
  
		string err = lua_tostring(L, -1);  
		lua_pop(L, 1);  
		std::stringstream msg;  
		msg << debug.short_src << ":line " << debug.currentline;  
		if (debug.name != 0) {  
		msg << "(" << debug.namewhat << " " << debug.name << ")";  
		}  
  
		msg << " [" << err << "]";  
		lua_pushstring(L, msg.str().c_str());  
		LogTraceStepReturn(1);
	}*/
	

#endif