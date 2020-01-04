


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
	LuaRef::LuaRef() : m_L(NULL), m_ref(LUA_REFNIL)
	{
		LogTraceStepCall("LuaRef::LuaRef()");
		LogTraceStepReturnVoid;
	}

	LuaRef::LuaRef(lua_State * L) : m_L(L), m_ref(LUA_REFNIL)
	{ 
		LogTraceStepCall("LuaRef::LuaRef(lua_State * L)"); 
		LogTraceStepReturnVoid; 
	}

	LuaRef::LuaRef(lua_State * L, bool fromStack) : m_L(NULL), m_ref(LUA_REFNIL)
	{ 
		LogTraceStepCall("LuaRef::LuaRef(lua_State * L, bool fromStack)"); 
		reset(L, fromStack);
		LogTraceStepReturnVoid; 
	}

	LuaRef::LuaRef(lua_State * L, int idx) : m_L(NULL), m_ref(LUA_REFNIL)
	{
		LogTraceStepCall("LuaRef::LuaRef(lua_State * L, int idx)");
		reset(L, idx);
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

	void LuaRef::clear()
	{
		LogTraceStepCall("void LuaRef::LuaTableRef::clear()");
		m_L = NULL;
		m_ref = LUA_REFNIL;
		LogTraceStepReturnVoid;
	}

	void LuaRef::reset()
	{
		LogTraceStepCall("void LuaRef::reset()");
		set_nil();
		LogTraceStepReturnVoid;
	}

	void LuaRef::reset(lua_State * L)
	{
		LogTraceStepCall("void LuaRef::reset(lua_State * L)");
		set_nil();
		m_L = L;
		LogTraceStepReturnVoid;
	}

	void LuaRef::reset(lua_State * L, bool fromStack)
	{
		LogTraceStepCall("void LuaRef::reset(lua_State * L, bool fromStack)");
		set_nil();
		if (L != NULL)
		{
			m_L = L;
			m_ref = luaL_ref(m_L, LUA_REGISTRYINDEX);
		}
		else
		{
			lua_pop(L, -1); //failed pop stack
		}
		LogTraceStepReturnVoid;
	}

	void LuaRef::reset(lua_State * L, int idx)
	{
		LogTraceStepCall("void LuaRef::reset(lua_State * L, int idx)");
		set_nil();
		if (L != NULL)
		{
			m_L = L;
			lua_pushvalue(L, idx);
			m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
		}
		LogTraceStepReturnVoid;
	}

	void LuaRef::set_nil()
	{
		LogTraceStepCall("LuaRef::set_nil()");
		if (m_L != NULL && m_ref != LUA_REFNIL)
		{
			luaL_unref(m_L, LUA_REGISTRYINDEX, m_ref);
			m_ref = LUA_REFNIL;
		}
		LogTraceStepReturnVoid;
	}

	int LuaRef::create_ref() const
	{
		LogTraceStepCall("int LuaRef::create_ref() const");
		if (m_L == NULL || m_ref == LUA_REFNIL)
		{
			LogTraceStepReturn(LUA_REFNIL);
		}
		push();
		LogTraceStepReturn(luaL_ref(m_L, LUA_REGISTRYINDEX));
	}

	LuaRef & LuaRef::operator = (const LuaRef & right)
	{
		LogTraceStepCall("LuaRef & LuaRef::operator = (const LuaRef & right)");
		if (right.push())
		{
			set_nil();
			reset(right.state(), true);
		}
		LogTraceStepReturn(*this);
	}

	LuaRef & LuaRef::operator = (const LuaTableRef & right)
	{
		LogTraceStepCall("LuaRef & LuaRef::operator = (const LuaTableRef & right)");
		if (right.push())
		{
			set_nil();
			reset(right.state(), true);
		}
		LogTraceStepReturn(*this);
	}


	LuaRef LuaRef::new_table(lua_State * L, int narr, int nrec)
	{
		LogTraceStepCall("LuaRef LuaRef::new_table(lua_State * L, int narr, int nrec)");
		if (L != NULL)
		{
			lua_createtable(L, narr, nrec);
			LogTraceStepReturn(LuaRef(L, true));
		}
		LogTraceStepReturn(LuaRef());
	}
	LuaRef LuaRef::get_global_var(lua_State * L, const char * name)
	{
		LogTraceStepCall("LuaRef LuaRef::get_global_var(lua_State * L, const char * name)");
		if (L != NULL)
		{
			lua_getglobal(L, name);
			LogTraceStepReturn(LuaRef(L, true));
		}
		LogTraceStepReturn(LuaRef());
	}


	string LuaRef::to_string() const
	{
		LogTraceStepCall("string LuaRef::to_string() const");
		if (m_L == NULL || m_ref == LUA_REFNIL)
		{
			LogTraceStepReturn(string("nil"));
		}
		LuaStateProtect lsp(m_L);
		lua_getglobal(m_L, "tostring");
		push();
		if (lua_pcall(m_L, 1, 1, 0))
		{
			const char * errStr = lua_tostring(m_L, -1);
			if (errStr) LogScriptError(m_L, errStr);
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
	bool LuaRef::push() const
	{
		LogTraceStepCall("bool LuaRef::push() const");
		if (m_L == NULL)
		{
			LogScriptError(m_L, "Error!!! the lua state is NULL!!!");
			LogTraceStepReturn(false);
		}
		lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_ref);
		LogTraceStepReturn(true);
	}
	bool LuaRef::pop()
	{
		LogTraceStepCall("bool LuaRef::pop()");
		if (m_L != NULL)
		{
			set_nil();
			m_ref = luaL_ref(m_L, LUA_REGISTRYINDEX);
			LogTraceStepReturn(true);
		}
		LogTraceStepReturn(false);
	}
	int LuaRef::type() const
	{
		LogTraceStepCall("int LuaRef::type() const");
		if (m_L == NULL || m_ref == LUA_REFNIL)
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
		LuaStateProtect lsp(m_L, true);
		if (push())
			LogTraceStepReturn(lua_iscfunction(m_L, -1) == 1);
		LogTraceStepReturn(false);
	}


	size_t LuaRef::length() const
	{
		LogTraceStepCall("LuaRef::length() const");
		LuaStateProtect lsp(m_L, true);
		if (push() && lua_istable(m_L, -1))
		{
			size_t len = lua_objlen(m_L, -1);
			LogTraceStepReturn(len);
		}
		LogTraceStepReturn(0u);
	}

		
	const LuaRef LuaRef::operator () () const
	{
		LogTraceStepCall("const LuaRef LuaRef::operator () () const");
		if (!push())
		{
			LogError("Error!!!the lua state is NULL!!!");
			LogTraceStepReturn(LuaRef());
		}
		if (lua_pcall(m_L, 0, 1, 0))
		{
			const char * errStr = lua_tostring(m_L, -1);
			if (errStr) LogScriptError(m_L, errStr);
			LogTraceStepReturn(LuaRef(m_L));
		}
		LogTraceStepReturn(LuaRef(m_L, true));
	}

}
