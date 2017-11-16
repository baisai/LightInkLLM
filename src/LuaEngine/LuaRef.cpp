


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
	LuaRef::LuaRef(lua_State * L) : m_L(L), m_ref(LUA_REFNIL)
	{ 
		LogTraceStepCall("LuaRef::LuaRef(lua_State * L)"); 
		LogTraceStepReturnVoid; 
	}

	LuaRef::LuaRef(lua_State * L, bool fromStack) : m_L(L), m_ref(luaL_ref(L, LUA_REGISTRYINDEX))
	{ 
		LogTraceStepCall("LuaRef::LuaRef(lua_State * L, bool fromStack)"); 
		LogTraceStepReturnVoid; 
	}

	LuaRef::LuaRef(lua_State * L, int idx) : m_L(L), m_ref(LUA_REFNIL)
	{
		LogTraceStepCall("LuaRef::LuaRef(lua_State * L, int idx)");
		lua_pushvalue(L, idx);
		m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
		LogTraceStepReturnVoid;
	}

	LuaRef::LuaRef(const LuaTableRef & v) : m_L(v.state()), m_ref(v.create_ref())
	{ 
		LogTraceStepCall("LuaRef::LuaRef(const LuaTableRef & v)");
		LogTraceStepReturnVoid;
	}

	LuaRef::LuaRef(const LuaRef & cp) : m_L(cp.m_L), m_ref(cp.create_ref())
	{
		LogTraceStepCall("LuaRef::LuaRef(const LuaRef & cp)"); 
		LogTraceStepReturnVoid;
	}

	LuaRef::~LuaRef()
	{
		LogTraceStepCall("LuaRef::~LuaRef()");
		if (m_L) { luaL_unref(m_L, LUA_REGISTRYINDEX, m_ref); }
		LogTraceStepReturnVoid;
	}

	void LuaRef::clear_lua()
	{
		LogTraceStepCall("void LuaRef::LuaTableRef::clear_lua()");
		m_L = NULL;
		m_ref = LUA_REFNIL;
		LogTraceStepReturnVoid;
	}

	int LuaRef::create_ref() const
	{
		LogTraceStepCall("int LuaRef::create_ref() const");
		if (m_ref == LUA_REFNIL)
		{
			LogTraceStepReturn(m_ref);
		}
		push();
		LogTraceStepReturn(luaL_ref(m_L, LUA_REGISTRYINDEX));
	}

	void LuaRef::set_nil()
	{
		LogTraceStepCall("LuaRef::set_nil()");
		if (m_ref != LUA_REFNIL)
		{
			luaL_unref(m_L, LUA_REGISTRYINDEX, m_ref);
			m_ref = LUA_REFNIL;
		}
		LogTraceStepReturnVoid;
	}

	LuaRef & LuaRef::operator = (const LuaRef & right)
	{
		LogTraceStepCall("LuaRef & LuaRef::operator = (const LuaRef & right)");
		luaL_unref(m_L, LUA_REGISTRYINDEX, m_ref);
		right.push();
		m_L = right.state();
		m_ref = luaL_ref(m_L, LUA_REGISTRYINDEX);
		LogTraceStepReturn(*this);
	}

	LuaRef & LuaRef::operator = (const LuaTableRef & right)
	{
		LogTraceStepCall("LuaRef & LuaRef::operator = (const LuaTableRef & right)");
		luaL_unref(m_L, LUA_REGISTRYINDEX, m_ref);
		right.push();
		m_L = right.state();
		m_ref = luaL_ref(m_L, LUA_REGISTRYINDEX);
		LogTraceStepReturn(*this);
	}


	LuaRef LuaRef::new_table(lua_State * L, int narr, int nrec)
	{
		LogTraceStepCall("LuaRef LuaRef::new_table(lua_State * L, int narr, int nrec)");
		lua_createtable(L, narr, nrec);
		LogTraceStepReturn(LuaRef(L, true));
	}
	LuaRef LuaRef::get_global_var(lua_State * L, const char * name)
	{
		LogTraceStepCall("LuaRef LuaRef::get_global_var(lua_State * L, const char * name)");
		lua_getglobal(L, name);
		LogTraceStepReturn(LuaRef(L, true));
	}


	string LuaRef::to_string() const
	{
		LogTraceStepCall("string LuaRef::to_string() const");
		LuaStateProtect lsp(m_L);
		lua_getglobal(m_L, "tostring");
		push();
		if (lua_pcall(m_L, 1, 1, 0))
		{
			const char * errStr = lua_tostring(m_L, -1);
			if (errStr) LogScriptError(errStr);
			LogTraceStepReturn(string());
		}
		const char * str = lua_tostring(m_L, -1);
		lsp.reset();
		LogTraceStepReturn(string(str));
	}
	void LuaRef::print() const
	{
		LogTraceStepCall("LuaRef::print(std::ostream & os) const");
		switch (type())
		{
		case LUA_TNIL:
			LogScriptMessage("nil");break;
		case LUA_TNUMBER:
			LogScriptMessage(to_string().c_str());break;
		case LUA_TBOOLEAN:
			LogScriptMessage(cast<bool>() ? "true" : "false");break;
		case LUA_TSTRING:
			LogScriptMessage("\"{}\"", cast<string>());break;
		case LUA_TTABLE:
			LogScriptMessage("table : {}", to_string());break;
		case LUA_TFUNCTION:
			LogScriptMessage("function : {}", to_string());break;
		case LUA_TUSERDATA:
			LogScriptMessage("userdata : {}", to_string());break;
		case LUA_TTHREAD:
			LogScriptMessage("thread : {}", to_string());break;
		case LUA_TLIGHTUSERDATA:
			LogScriptMessage("lightuserdata : {}", to_string());break;
		default:
			LogScriptMessage("unknown");break;
		}
		LogTraceStepReturnVoid;
	}
	lua_State * LuaRef::state() const
	{ LogTraceStepCall("lua_State * LuaRef::state()"); LogTraceStepReturn(m_L); }
	void LuaRef::push() const
	{
		LogTraceStepCall("LuaRef::push() const");
		lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_ref);
		LogTraceStepReturnVoid;
	}
	void LuaRef::pop()
	{
		LogTraceStepCall("LuaRef::pop() const");
		luaL_unref(m_L, LUA_REGISTRYINDEX, m_ref);
		m_ref = luaL_ref(m_L, LUA_REGISTRYINDEX);
		LogTraceStepReturnVoid;
	}
	int LuaRef::type() const
	{
		LogTraceStepCall("int LuaRef::type() const");
		if (m_ref == LUA_REFNIL)
		{
			LogTraceStepReturn(LUA_TNIL);
		}
		LuaStateProtect lsp(m_L, true);
		push();
		LogTraceStepReturn(lua_type(m_L, -1));
	}

	bool LuaRef::is_nil() const
	{
		LogTraceStepCall("bool LuaRef::is_nil() const"); 
		LogTraceStepReturn((type() == LUA_TNIL));
	}
	bool LuaRef::is_number() const
	{
		LogTraceStepCall("bool LuaRef::is_number() const");
		LogTraceStepReturn((type() == LUA_TNUMBER));
	}
	bool LuaRef::is_string() const
	{
		LogTraceStepCall("bool LuaRef::is_string() const"); 
		LogTraceStepReturn((type() == LUA_TSTRING));
	}
	bool LuaRef::is_table() const
	{
		LogTraceStepCall("bool LuaRef::is_table() const"); 
		LogTraceStepReturn((type() == LUA_TTABLE));
	}
	bool LuaRef::is_function() const
	{
		LogTraceStepCall("bool LuaRef::is_function() const"); 
		LogTraceStepReturn((type() == LUA_TFUNCTION));
	}
	bool LuaRef::is_userdata() const
	{
		LogTraceStepCall("bool LuaRef::is_userdata() const"); 
		LogTraceStepReturn((type() == LUA_TUSERDATA));
	}
	bool LuaRef::is_thread() const
	{
		LogTraceStepCall("bool LuaRef::is_thread() const"); 
		LogTraceStepReturn((type() == LUA_TTHREAD));
	}
	bool LuaRef::is_lightuserdata() const
	{
		LogTraceStepCall("bool LuaRef::is_lightuserdata() const"); 
		LogTraceStepReturn((type() == LUA_TLIGHTUSERDATA));
	}
	bool LuaRef::is_bool() const
	{
		LogTraceStepCall("bool LuaRef::is_bool() const"); 
		LogTraceStepReturn((type() == LUA_TBOOLEAN));
	}

	bool LuaRef::is_cfunction() const
	{
		LogTraceStepCall("bool LuaRef::is_cfunction() const");
		if (m_ref == LUA_REFNIL)
		{
			LogTraceStepReturn(false);
		}
		LuaStateProtect lsp(m_L, true);
		push();
		LogTraceStepReturn(lua_iscfunction(m_L, -1) == 1);
	}


	size_t LuaRef::length() const
	{
		LogTraceStepCall("LuaRef::length() const");
		LuaStateProtect lsp(m_L, true);
		push();
		size_t len = lua_objlen(m_L, -1);
		lsp.reset();
		LogTraceStepReturn(len);
	}

		
	const LuaRef LuaRef::operator () () const
	{
		LogTraceStepCall("const LuaRef LuaRef::operator () () const");
		push();
		if (lua_pcall(m_L, 0, 1, 0))
		{
			const char * errStr = lua_tostring(m_L, -1);
			if (errStr) LogScriptError(errStr);
			LogTraceStepReturn(LuaRef(m_L));
		}
		LogTraceStepReturn(LuaRef(m_L, true));
	}

}
