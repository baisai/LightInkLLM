


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

class CppClass
{
public:
	enum CppEnum
	{
		enum1,
		enum2,
	};
public:
	CppClass(int pri) : m_private(pri) {  }
	~CppClass() {  }

	int get_private() { return m_private; }
	void set_private(int pri) { m_private = pri; }

	CppEnum get_enum() { return m_enum; }	void set_enum(CppEnum e) { m_enum = e; }

	static void test_static() { LogMessage("call --> void CppClass::test_static()"); }

public:
	char m_char;
	short m_short;
	int m_int;
	std::string m_string;
	std::vector<int> m_vector;
private:
	int m_private;
	CppEnum m_enum;
};

class CppClass2
{
public:
	CppClass2() { LogMessage("new CppClass2"); }
	~CppClass2() { LogMessage("delete CppClass2"); }
	void test() { LogMessage("call -->  void CppClass2.test()"); }
	static CppClass2 * create() { return new CppClass2; }
	static void destroy(CppClass2 * obj) { delete obj; }

public:
	static const LightInk::string s_str;
	static const CppClass s_cppclass;

};

const LightInk::string CppClass2::s_str("this is static const string");
const CppClass CppClass2::s_cppclass(10000);


namespace LightInk
{
	LightInkLuaEnumType(CppClass::CppEnum)}
static void bind_cppclass(lua_State * lua)
{
	LightInk::LuaModule lm(lua, "CppClassList");

	lm[
		LightInk::LuaRegister<CppClass, void(int)>(lua, "CppClass")
			.def(&CppClass::get_private, "get_private")
			.def(&CppClass::set_private, "set_private")
			.def(&CppClass::get_enum, "get_enum")
			.def(&CppClass::set_enum, "set_enum")
			.def(CppClass::test_static, "test_static")
			.def(&CppClass::m_char, "m_char")
			.def(&CppClass::m_short, "m_short")
			.def(&CppClass::m_int, "m_int")
			.def(&CppClass::m_string, "m_string")
			.def(&CppClass::m_vector, "m_vector")
			.def_enum(CppClass::enum1, "enum1")
			.def_enum(CppClass::enum2, "enum2")
	];
	lm[
		LightInk::LuaRegister<CppClass2, void()>(lua, "CppClass2")
			.disable_new() //disable CppClass2.new__
			.def(CppClass2::create, "create")
			.def(CppClass2::destroy, "destroy")
			.def(&CppClass2::test, "test")
			.def_const_copy(CppClass2::s_str, "s_str")
			.def_const_copy(CppClass2::s_cppclass, "s_cppclass")
	];
}


void test_bind_class()
{
	LogDebug("start...call...test_bind_class()");
	LightInk::LuaEngine le;
	le.init();
	le.register_module(bind_cppclass);

	le.add_package_path("../../Example/Lua/?.lua");

	le.require_file("BindClass");

	LogDebug("over...call...test_bind_class()");
}