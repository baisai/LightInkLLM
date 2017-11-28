


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


class CppClassAbs
{
public:
	CppClassAbs() { LogMessage("new CppClassAbs"); }
	virtual ~CppClassAbs() { LogMessage("delete CppClassAbs"); }

	virtual void test_abstract() = 0;
};

class CppClassAbsInherit : public CppClassAbs
{
public:
	CppClassAbsInherit() { LogMessage("new CppClassAbsInherit"); }
	virtual ~CppClassAbsInherit() { LogMessage("delete CppClassAbsInherit"); }

	virtual void test_abstract() { LogMessage("call --> virtual void CppClassAbsInherit.test_abstract()"); }

	static LightInk::SharedPtrTS<CppClassAbs>::type get_shared() { return LightInk::SharedPtrTS<CppClassAbs>::type(static_cast<CppClassAbs *>(new CppClassAbsInherit)); }

	static void check_shared(LightInk::SharedPtrTS<CppClassAbs>::type sp) { LogMessage("SharedPtrTS Use Count = {}", sp.use_count()); }

	static int show_class_name(lua_State * L) {  LogMessage("ClassName = {}", LightInk::LuaEngine::get_class_name(L, 1)); return 0; }
};

static void bind_cppclass(lua_State * lua)
{
	LightInk::LuaModule(lua, "CppClassList")
	[
		LightInk::LuaRegisterAbstract<CppClassAbs>(lua, "CppClassAbs")
			.def(&CppClassAbs::test_abstract, "test_abstract")
			
	];

	LightInk::LuaModule(lua, "CppClassList")
	[
		LightInk::LuaRegister<CppClassAbsInherit, void()>(lua, "CppClassAbsInherit", LightInk::BaseClassStrategy<CppClassAbs>())
			.def(CppClassAbsInherit::get_shared, "get_shared")
			.def(CppClassAbsInherit::check_shared, "check_shared")
			.def(CppClassAbsInherit::show_class_name, "show_class_name")
			
	];
}

void test_bind_abstract()
{
	LogDebug("start...call...test_bind_abstract()");

	LightInk::LuaEngine le;
	le.init();

	le.register_module(bind_cppclass);

	le.add_package_path("../../Example/Lua");

	le.require_file("BindAbstract");

	LogDebug("over...call...test_bind_abstract()");
}