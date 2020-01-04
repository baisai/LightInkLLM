


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

#ifndef LIGHTINK_LUAENGINE_LUATABLEREF_H_
#define LIGHTINK_LUAENGINE_LUATABLEREF_H_

#include "Common/Type.h"
#include "LuaEngine/LuaStateProtect.h"
#include "Log/Log.h"


namespace LightInk
{
	class LuaRef;
	class LIGHTINK_DECL LuaTableRef
	{
	public:
		LuaTableRef();
		LuaTableRef(const LuaRef & tab);
		LuaTableRef(const LuaRef & tab, const LuaRef & key);
		LuaTableRef(const LuaTableRef & cp);
		~LuaTableRef();

		void clear();

		int create_ref() const;

		template <typename T>
		LuaTableRef & operator = (const T & v)
		{
			LogTraceStepCall("LuaTableRef & LuaRef::LuaTableRef::operator =<T> (const T & v)");
			if (m_L != NULL)
			{
				LuaStateProtect lsp(m_L);
				lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_tableRef);
				lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_keyRef);
				LuaStack<const T>::push(m_L, v);
				lua_settable(m_L, -3);
				lsp.reset();
			}
			LogTraceStepReturn(*this);
		}

		template <typename T>
		LuaTableRef & rawset (const T & v)
		{
			LogTraceStepCall("LuaTableRef & LuaRef::LuaTableRef::rawset<T> (const T & v)");
			if (m_L != NULL)
			{
				LuaStateProtect lsp(m_L);
				lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_tableRef);
				lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_keyRef);
				LuaStack<const T>::push(m_L, v);
				lua_rawset(m_L, -3);
				lsp.reset();
			}
			LogTraceStepReturn(*this);
		}

		lua_State * state() const;

		bool push() const;

		int create_tab_ref() const;
		int create_key_ref() const;

		LuaRef get_ref() const;
		LuaRef get_tab_ref() const;
		LuaRef get_key_ref() const;

	private:
		lua_State * m_L;
		int m_tableRef;
		int m_keyRef;
	};

}

#endif