


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
#ifndef LIGHTINK_LUAENGINE_LUASTATEPROTECT_H_
#define LIGHTINK_LUAENGINE_LUASTATEPROTECT_H_

#include "Common/Type.h"
#include "LuaEngine/LuaLib.h"

namespace LightInk
{
	class LIGHTINK_DECL LuaStateProtect
	{
	public:
		LuaStateProtect(lua_State * L, bool autoPop = false) : m_lua(L), m_top(lua_gettop(L)), m_autoPop(autoPop) { ; }
		~LuaStateProtect() {if (m_autoPop) lua_settop(m_lua, m_top);}
		void reset() { lua_settop(m_lua, m_top); }
		int get_top() { return m_top; }
	private:
		lua_State * m_lua;
		int m_top;
		bool m_autoPop;
	LIGHTINK_DISABLE_COPY(LuaStateProtect)
	};
}

#endif