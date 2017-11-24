


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

#ifndef LIGHTINK_LUAENGINE_LUAREGISTERABSTRACT_H_
#define LIGHTINK_LUAENGINE_LUAREGISTERABSTRACT_H_

#include "Common/RuntimeError.h"
#include "LuaEngine/LuaModule.h"

namespace LightInk
{
	template <typename ClassType>
	class LuaModuleByClass;

	template <typename T>
	struct BaseClassStrategy{};

	//namespace use ::table::table::classname
	template <typename ClassType>
	class LIGHTINK_TEMPLATE_DECL LuaRegisterAbstract : public LuaRegisterNode
	{
	public:
		template <typename T>
		friend struct LuaDefTraits;

		LuaRegisterAbstract(lua_State * L, const string & name);

		template <typename BaseType>
		LuaRegisterAbstract(lua_State * L, const string & name, BaseClassStrategy<BaseType>); //call once again base every class

		LuaRegisterAbstract(const LuaRegisterAbstract<ClassType> & cp);

		virtual ~LuaRegisterAbstract() {  }

		LuaRegisterAbstract & operator = (const LuaRegisterAbstract<ClassType> & right);

		template <typename T>
		LuaRegisterAbstract & def(T obj, const string & name);

		LuaModuleByClass<ClassType> def_module(const string & name);

		template <typename T>
		LuaRegisterAbstract & def_property(T * obj, const string & name);

		LuaRegisterAbstract & disable_new();

		LuaRegisterAbstract & def_cclosure(lua_CFunction obj, const string & name);

		template <typename ET>
		LuaRegisterAbstract & def_enum(ET obj, const string & name);

		template <typename T>
		LuaRegisterAbstract & def_const_copy(const T & obj, const string & name);

		template <typename T>
		LuaRegisterAbstract & def_const_ptr(const T * obj, const string & name);

	protected:
		template <typename T>
		LuaRegisterAbstract & def_class_func(T obj, const string & name);

		template <typename T>
		LuaRegisterAbstract & def_class_property(T ClassType::* obj, const string & name);

		template <typename T>
		LuaRegisterAbstract & def_func(T obj, const string & name);

		RuntimeError get_class_table();

		RuntimeError get_class_metatable();

		void init_class(const string & name);


	protected:
		lua_State * m_lua;

	};

	template <typename ClassType>
	class LIGHTINK_TEMPLATE_DECL LuaModuleByClass : public LuaModule
	{
	public:
		LuaModuleByClass(lua_State * L, const string & moduleName, const LuaRef & table, LuaRegisterAbstract<ClassType> & c);

		virtual ~LuaModuleByClass();

		LuaRegisterAbstract<ClassType> & module_end();

		template <typename T>
		LuaModuleByClass & def(T obj, const string & name);

		template <typename T>
		LuaRegisterAbstract<ClassType> & operator[](const T & idx);

	protected:
		LuaRegisterAbstract<ClassType> & m_class;
	};

}

#include "LuaEngine/LuaRegisterAbstract.cpp"

#endif