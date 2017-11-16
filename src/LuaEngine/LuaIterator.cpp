


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

#include "LuaEngine/LuaIterator.h"

namespace LightInk
{
	//////////////////////////////////////////////////////////
	//LuaIterator
	/////////////////////////////////////////////////////////
	LuaIterator::LuaIterator(lua_State * L, int idx) : m_L(L), m_top(lua_gettop(L)), m_over(false), m_keyIdx(0), m_copyCount(0)
	{
		LogTraceStepCall("LuaIterator::LuaIterator(lua_State * L, int idx)");
		lua_pushvalue(L, idx);
		lua_pushnil(L);
		lua_pushnil(L);
		next();
		m_keyIdx = lua_absindex(m_L, -2);
		LogTraceStepReturnVoid;
	}

	void LuaIterator::set_over()
	{
		LogTraceStepCall("LuaIterator::set_over()");
		if (!m_over)
		{
			m_over = true;
			m_keyIdx = 0;
			m_copyCount = 0;
			lua_settop(m_L, m_top);
		}
		LogTraceStepReturnVoid;
	}
	lua_State * LuaIterator::state()
	{
		LogTraceStepCall("lua_State * LuaIterator::state()");
		LogTraceStepReturn(m_L);
	}
	bool LuaIterator::is_over()
	{
		LogTraceStepCall("bool LuaIterator::is_over()");
		LogTraceStepReturn(m_over);
	}
		

	LuaIterator & LuaIterator::operator ++ ()
	{
		LogTraceStepCall("LuaIterator & LuaIterator::operator ++ ()");
		if (!m_over)
		{
			next();
		}
		LogTraceStepReturn(*this);
	}

	LuaIterator & LuaIterator::operator ++ (int)
	{
		LogTraceStepCall("LuaIterator & LuaIterator::operator ++ (int)");
		if (!m_over)
		{
			next();
		}
		LogTraceStepReturn(*this);
	}

	void LuaIterator::next()
	{
		LogTraceStepCall("void LuaIterator::next()");
		lua_pop(m_L, 1);
		if (!lua_next(m_L, -2))
		{
			set_over();
		}
		LogTraceStepReturnVoid;
	}


	//////////////////////////////////////////////////////////
	//LuaRefIterator
	/////////////////////////////////////////////////////////
	LuaRefIterator::LuaRefIterator(const LuaRef & table) : m_L(table.state()), m_table(table), m_key(table.state()), m_value(table.state())
	{
		LogTraceStepCall("LuaRefIterator::LuaRefIterator(const LuaRef & table)");
		LogTraceStepReturnVoid;
	}

	lua_State * LuaRefIterator::state()
	{
		LogTraceStepCall("lua_State * LuaRefIterator::state()");
		LogTraceStepReturn(m_L);
	}

	bool LuaRefIterator::is_over()
	{
		LogTraceStepCall("bool LuaRefIterator::is_over()");
		LogTraceStepReturn(m_key.is_nil());
	}

	LuaRef & LuaRefIterator::get_key()
	{
		LogTraceStepCall("LuaRef & LuaRefIterator::get_key()");
		LogTraceStepReturn(m_key);
	}

	LuaRef & LuaRefIterator::get_value()
	{
		LogTraceStepCall("LuaRef & LuaRefIterator::get_value()");
		LogTraceStepReturn(m_value);
	}

	LuaRef & LuaRefIterator::operator * ()
	{
		LogTraceStepCall("LuaRef & LuaRefIterator::operator * ()");
		LogTraceStepReturn(m_value);
	}

	LuaRef & LuaRefIterator::operator -> ()
	{
		LogTraceStepCall("LuaRef & LuaRefIterator::operator -> ()");
		LogTraceStepReturn(m_value);
	}

	LuaRefIterator & LuaRefIterator::operator ++ ()
	{
		LogTraceStepCall("LuaRefIterator & LuaRefIterator::operator ++ ()");
		if (!m_key.is_nil())
		{
			next();
		}
		LogTraceStepReturn(*this);
	}

	LuaRefIterator & LuaRefIterator::operator ++ (int)
	{
		LogTraceStepCall("LuaRefIterator & LuaRefIterator::operator ++ (int)");
		if (!m_key.is_nil())
		{
			next();
		}
		LogTraceStepReturn(*this);
	}

	void LuaRefIterator::next()
	{
		LogTraceStepCall("void LuaRefIterator::next()");
		m_table.push();
		m_key.push ();
		if (lua_next (m_L, -2))
		{
			m_value.pop ();
			m_key.pop ();
		}
		else
		{
			m_key.set_nil();
			m_value.set_nil();
		}
		lua_pop(m_L, 1);
		LogTraceStepReturnVoid;
	}

}