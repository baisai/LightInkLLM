


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

#include "LuaEngine/LuaRef.h"
#include "LuaEngine/LuaTableRef.h"

namespace LightInk
{
	LuaTableRef::LuaTableRef(const LuaRef & tab, const LuaRef & key) : m_L(tab.state()), m_tableRef(tab.create_ref()), m_keyRef(key.create_ref())
	{
		LogTraceStepCall("LuaTableRef::LuaTableRef(const LuaRef & tab, const LuaRef & key)");
		LogTraceStepReturnVoid;
	}
	LuaTableRef::LuaTableRef(const LuaRef & tab) : m_L(tab.state()), m_tableRef(tab.create_ref()), m_keyRef(luaL_ref(tab.state(), LUA_REGISTRYINDEX))
	{ 
		LogTraceStepCall("LuaRef::LuaTableRef::LuaTableRef(const LuaRef & tab)");
		LogTraceStepReturnVoid; 
	}
	LuaTableRef::LuaTableRef(const LuaTableRef & cp) : m_L(cp.m_L), m_tableRef(cp.create_tab_ref()), m_keyRef(cp.create_key_ref())
	{
		LogTraceStepCall("LuaRef::LuaTableRef::LuaTableRef(const LuaTableRef & cp)");
		LogTraceStepReturnVoid;
	}
	LuaTableRef::~LuaTableRef()
	{
		LogTraceStepCall("LuaRef::LuaTableRef::~LuaTableRef()");
		if (m_L) { luaL_unref(m_L, LUA_REGISTRYINDEX, m_keyRef); }
		LogTraceStepReturnVoid;
	}

	void LuaTableRef::clear_lua()
	{
		LogTraceStepCall("void LuaRef::LuaTableRef::clear_lua()");
		m_L = NULL;
		m_tableRef = LUA_REFNIL;
		m_keyRef = LUA_REFNIL;
		LogTraceStepReturnVoid;
	}

	int LuaTableRef::create_ref() const
	{
		LogTraceStepCall("int LuaRef::LuaTableRef::create_ref() const");
		if (m_tableRef == LUA_REFNIL || m_keyRef == LUA_REFNIL)
		{
			LogTraceStepReturn(LUA_REFNIL);
		}
		push();
		LogTraceStepReturn(luaL_ref(m_L, LUA_REGISTRYINDEX));
	}



	lua_State * LuaTableRef::state() const
	{ 
		LogTraceStepCall("lua_State * LuaRef::LuaTableRef::state()"); 
		LogTraceStepReturn(m_L); 
	}

	void LuaTableRef::push() const
	{
		LogTraceStepCall("LuaRef::LuaTableRef::push() const");
		lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_tableRef);
		lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_keyRef);
		lua_gettable(m_L, -2);
		lua_remove(m_L, -2); // remove the table
		LogTraceStepReturnVoid;
	}

	int LuaTableRef::create_tab_ref() const
	{
		LogTraceStepCall("int LuaTableRef::create_tab_ref() const");
		if (m_tableRef == LUA_REFNIL)
		{
			LogTraceStepReturn(m_tableRef);
		}
		lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_tableRef);
		LogTraceStepReturn(luaL_ref(m_L, LUA_REGISTRYINDEX));
	}

	int LuaTableRef::create_key_ref() const
	{
		LogTraceStepCall("int LuaTableRef::create_key_ref() const");
		if (m_keyRef == LUA_REFNIL)
		{
			LogTraceStepReturn(m_keyRef);
		}
		lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_keyRef);
		LogTraceStepReturn(luaL_ref(m_L, LUA_REGISTRYINDEX));
	}

	LuaRef LuaTableRef::get_ref() const
	{
		LogTraceStepCall("LuaRef LuaRef:;LuaTableRef::get_ref()");
		LogTraceStepReturn(LuaRef(*this));
	}
	LuaRef LuaTableRef::get_tab_ref() const
	{
		LogTraceStepCall("LuaRef LuaTableRef::get_tab_ref() const");
		lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_tableRef);
		LogTraceStepReturn(LuaRef(m_L, true));
	}
	LuaRef LuaTableRef::get_key_ref() const
	{
		LogTraceStepCall("LuaRef LuaTableRef::get_key_ref() const");
		lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_keyRef);
		LogTraceStepReturn(LuaRef(m_L, true));
	}

}
