


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


class CppClassBase
{
public:
	CppClassBase() { LogMessage("new CppClassBase"); }
	~CppClassBase() { LogMessage("delete CppClassBase"); }

	int get_base() { return m_base; }
	void set_base(int num) { m_base = num; }

	void test() { LogMessage("call --> void CppClassBase.test()"); }

	static void test_static() { LogMessage("call --> void CppClassBase::test_static()"); }
	static void test_static_base() { LogMessage("call --> void CppClassBase::test_static_base()"); }

	int m_test;

private:
	int m_base;
};

class CppClassInherit : public CppClassBase
{
public:
	CppClassInherit() { LogMessage("new CppClassInherit"); }
	~CppClassInherit() { LogMessage("delete CppClassInherit"); }

	int get_inherit() { return m_inherit; }
	void set_inherit(int num) { m_inherit = num; }

	void test() { LogMessage("call --> void CppClassInherit.test()"); }

	static void test_static() { LogMessage("call --> void CppClassInherit::test_static()"); }
	static void test_static_inherit() { LogMessage("call --> void CppClassInherit::test_static_inherit()"); }

	int m_test;

private:
	int m_inherit;
};

static LightInk::LuaRegisterNode bind_base(lua_State * lua)
{
	return(
		LightInk::LuaRegister<CppClassBase, void()>(lua, "CppClassBase")
			.def(&CppClassBase::get_base, "get_base")
			.def(&CppClassBase::set_base, "set_base")
			.def(&CppClassBase::test, "test")
			.def(CppClassBase::test_static, "test_static")
			.def(CppClassBase::test_static_base, "test_static_base")
			.def(&CppClassBase::m_test, "m_test")
		);
}

static LightInk::LuaRegisterNode bind_inherit(lua_State * lua)
{
	return (
		LightInk::LuaRegister<CppClassInherit, void()>(lua, "CppClassInherit", LightInk::BaseClassStrategy<CppClassBase>())
			.def(&CppClassInherit::get_inherit, "get_inherit")
			.def(&CppClassInherit::set_inherit, "set_inherit")
			.def(&CppClassInherit::test, "test")
			.def(CppClassInherit::test_static, "test_static")
			.def(CppClassInherit::test_static_inherit, "test_static_inherit")
			.def(&CppClassInherit::m_test, "m_test")
		);
}

static void bind_cppclass2(lua_State * lua)
{
	LightInk::LuaModule(lua, "CppClassList")
	[
		bind_inherit(lua).self()
		<=
		bind_base(lua).self()
	];
}

static void bind_cppclass(lua_State * lua)
{
	LightInk::LuaModule(lua, "CppClassList")
	[
		LightInk::LuaRegister<CppClassBase, void()>(lua, "CppClassBase")
			.def(&CppClassBase::get_base, "get_base")
			.def(&CppClassBase::set_base, "set_base")
			.def(&CppClassBase::test, "test")
			.def(CppClassBase::test_static, "test_static")
			.def(CppClassBase::test_static_base, "test_static_base")
			.def(&CppClassBase::m_test, "m_test")
			
	];

	LightInk::LuaModule(lua, "CppClassList")
	[
		LightInk::LuaRegister<CppClassInherit, void()>(lua, "CppClassInherit", LightInk::BaseClassStrategy<CppClassBase>())
			.def(&CppClassInherit::get_inherit, "get_inherit")
			.def(&CppClassInherit::set_inherit, "set_inherit")
			.def(&CppClassInherit::test, "test")
			.def(CppClassInherit::test_static, "test_static")
			.def(CppClassInherit::test_static_inherit, "test_static_inherit")
			.def(&CppClassInherit::m_test, "m_test")
			
	];
}

void test_bind_inherit()
{
	LogDebug("start...call...test_bind_inherit()");

	LightInk::LuaEngine le;
	le.init();
	le.register_module(bind_cppclass);
	//le.register_module(bind_cppclass2);

	le.add_package_path("../../Example/Lua/?.lua");

	le.require_file("BindInherit");

	LogDebug("over...call...test_bind_inherit()");
}