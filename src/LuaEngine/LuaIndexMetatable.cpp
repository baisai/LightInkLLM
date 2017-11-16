


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
#include "LuaEngine/LuaIndexMetatable.h"
#include "LuaEngine/LuaEngine.h"
namespace LightInk
{
	int LuaIndexMetatable::ct_index_function(lua_State * L)
	{
		LogTraceStepCall("int LuaIndexMetatable::ct_index_function(lua_State * L)");
		if (!lua_istable(L, 1))
		{
			LogScriptErrorJump(L, "Error!!!The First arg is not class table!!!!");
			LogTraceStepReturn(0);
		}
		if (!lua_getmetatable(L, 1))
		{
			LogScriptErrorJump(L, "Error!!!The class table have not metatable!!!!");
			LogTraceStepReturn(0);
		}
		const char * key = luaL_checkstring(L, 2);
		lua_pushvalue(L, 2);
		lua_rawget(L, -2);

		if (lua_isnil(L, -1))
		{
			LogScriptErrorJump(L, "Error!!!Not this field!!!\"{}\"", key);
			LogTraceStepReturn(0);
		}
		else if (lua_isuserdata(L, -1))
		{
			LuaClassPropertyInfo * p = (LuaClassPropertyInfo *) lua_touserdata(L, -1);
			LogTraceStepReturn((p->m_indexFunction)(L, p->m_property, key));
		}
		LogTraceStepReturn(1);
	}

	int LuaIndexMetatable::ct_newindex_function(lua_State * L)
	{
		LogTraceStepCall("int LuaIndexMetatable::ct_newindex_function(lua_State * L)");
		if (!lua_istable(L, 1))
		{
			LogScriptErrorJump(L, "Error!!!The First arg is not class table!!!!");
			LogTraceStepReturn(0);
		}
		if (!lua_getmetatable(L, 1))
		{
			LogScriptErrorJump(L, "Error!!!The class table have not metatable!!!!");
			LogTraceStepReturn(0);
		}

		const char * key = luaL_checkstring(L, 2);
		lua_pushvalue(L, 2);
		lua_rawget(L, -2);

		if (lua_isnil(L, -1))
		{
			LogScriptErrorJump(L, "Error!!!Not this field!!!\"{}\"", key);
				
		}
		else if (lua_isuserdata(L, -1))
		{
			LuaClassPropertyInfo * p = (LuaClassPropertyInfo *) lua_touserdata(L, -1);
			lua_pop(L, 1); //pop userdata
			LogTraceStepReturn((p->m_newindexFunction)(L, p->m_property, key, 3));
		}
		else
		{
			LogScriptErrorJump(L, "Error!!!This field is not a property!!!\"{}\"", key);
		}
		LogTraceStepReturn(0);
	}

	int LuaIndexMetatable::gt_index_function(lua_State * L)
	{
		LogTraceStepCall("int LuaIndexMetatable::gt_index_function(lua_State * L)");
		if (lua_isuserdata(L, 1))
		{
			LuaClassPropertyInfo * p = (LuaClassPropertyInfo *) lua_touserdata(L, 1);
			LogTraceStepReturn((p->m_indexFunction)(L, p->m_property, "gt_index_function"));
		}
		else
		{
			LogScriptErrorJump(L, "Error!!!This field is not a property userdata!!!");
		}
		LogTraceStepReturn(0);
	}

	int LuaIndexMetatable::gt_newindex_function(lua_State * L)
	{
		LogTraceStepCall("int LuaIndexMetatable::gt_newindex_function(lua_State * L)");
		if (lua_isuserdata(L, 1))
		{
			LuaClassPropertyInfo * p = (LuaClassPropertyInfo *) lua_touserdata(L, 1);
			LogTraceStepReturn((p->m_newindexFunction)(L, p->m_property, "gt_index_function", 2));
		}
		else
		{
			LogScriptErrorJump(L, "Error!!!This field is not a property userdata!!!");
		}
		LogTraceStepReturn(0);
	}


	int LuaIndexMetatable::mt_index_function(lua_State * L)
	{
		LogTraceStepCall("int LuaIndexMetatable::mt_index_function(lua_State * L)");
		if (!lua_isuserdata(L, 1))
		{
			LogScriptErrorJump(L, "Error!!!The First arg is not userdata!!!!");
			LogTraceStepReturn(0);
		}
		if (!lua_getmetatable(L, 1))
		{
			LogScriptErrorJump(L, "Error!!!The userdata have not metatable!!!!");
			LogTraceStepReturn(0);
		}
		const char * key = luaL_checkstring(L, 2);
		lua_pushvalue(L, 2);
		lua_rawget(L, -2);

		if (lua_isnil(L, -1))
		{
			LogScriptErrorJump(L, "Error!!!Not this field!!!\"{}\"", key);
			LogTraceStepReturn(0);
		}
		else if (lua_isuserdata(L, -1))
		{
			LuaClassPropertyInfo * p = (LuaClassPropertyInfo *) lua_touserdata(L, -1);
			LogTraceStepReturn((p->m_indexFunction)(L, p->m_property, key));
		}
		LogTraceStepReturn(1);
	}

	int LuaIndexMetatable::mt_newindex_function(lua_State * L)
	{
		LogTraceStepCall("int LuaIndexMetatable::mt_newindex_function(lua_State * L)");
		if (!lua_isuserdata(L, 1))
		{
			LogScriptErrorJump(L, "Error!!!The First arg is not userdata!!!!");
			LogTraceStepReturn(0);
		}
		if (!lua_getmetatable(L, 1))
		{
			LogScriptErrorJump(L, "Error!!!The userdata have not metatable!!!!");
			LogTraceStepReturn(0);
		}

		const char * key = luaL_checkstring(L, 2);
		lua_pushvalue(L, 2);
		lua_rawget(L, -2);
		if (lua_isnil(L, -1))
		{
			LogScriptErrorJump(L, "Error!!!Not this field!!!\"{}\"", key);
				
		}
		else if (lua_isuserdata(L, -1))
		{
			LuaClassPropertyInfo * p = (LuaClassPropertyInfo *) lua_touserdata(L, -1);
			lua_pop(L, 1); //pop userdata
			LogTraceStepReturn((p->m_newindexFunction)(L, p->m_property, key, 3));
		}
		else
		{
			LogScriptErrorJump(L, "Error!!!This field is not a property!!!\"{}\"", key);
		}
		LogTraceStepReturn(0);
	}

}