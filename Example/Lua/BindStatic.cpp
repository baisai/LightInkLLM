


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

#include "LuaEngine/LuaEngine.h"
#include "LuaEngine/LuaIterator.h"

static void test_static()
{
	LogMessage("call --> static void test_static()");
}

static int test_cclosure(lua_State * lua)
{
	LogMessage("call --> static int test_cclosure()");

	for (LightInk::LuaIterator iter(lua, 1); !iter.is_over(); ++iter)
	{
		std::string key = iter.get_key<std::string>();
		LightInk::CharPtrBridge value = iter.get_value<LightInk::CharPtrBridge>();
		LogMessage("key = \"{}\"; value = \"{}\"", key.c_str(), value.m_charPtr);
	}

	lua_pushboolean(lua, 1);
	return 1;
}

static int int_static = 0;

static void bind_cppclass(lua_State * lua)
{
	LightInk::LuaDefAutoTool::def(lua, test_static, "test_static");
	LightInk::LuaDefAutoTool::def(lua, test_cclosure, "test_cclosure");
	LightInk::LuaDefAutoTool::def(lua, &int_static, "int_static");
}

void test_bind_static()
{
	LogDebug("start...call...test_bind_static()");

	LightInk::LuaEngine le;
	le.init();
	le.register_module(bind_cppclass);

	le.add_package_path("../../Example/Lua/?.lua");

	le.require_file("BindStatic");

	LogDebug("over...call...test_bind_static()");
}