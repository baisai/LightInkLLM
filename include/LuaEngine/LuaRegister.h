


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

#ifndef LIGHTINK_LUAENGINE_LUAREGISTER_H_
#define LIGHTINK_LUAENGINE_LUAREGISTER_H_

#include "Common/RuntimeError.h"
#include "LuaEngine/LuaModule.h"

namespace LightInk
{
	template <typename ClassType, typename CtorType>
	class LuaModuleByClass;

	template <typename T>
	struct BaseClassStrategy{};

	//namespace use ::table::table::classname
	template <typename ClassType, typename CtorType>
	class LIGHTINK_TEMPLATE_DECL LuaRegister : public LuaRegisterNode
	{
	public:
		template <typename T>
		friend struct LuaDefTraits;

		LuaRegister(lua_State * L, const string & name);

		template <typename BaseType>
		LuaRegister(lua_State * L, const string & name, BaseClassStrategy<BaseType>); //call once again base every class

		LuaRegister(const LuaRegister<ClassType, CtorType> & cp);

		LuaRegister<ClassType, CtorType> & operator = (const LuaRegister<ClassType, CtorType> & right);

		template <typename T>
		LuaRegister & def(T obj, const string & name);

		LuaModuleByClass<ClassType, CtorType> def_module(const string & name);

		template <typename T>
		LuaRegister & def_property(T * obj, const string & name);

		LuaRegister & disable_new();

		LuaRegister & def_cclosure(lua_CFunction obj, const string & name);

	private:

		template <typename T>
		LuaRegister & def_class_func(T obj, const string & name);

		template <typename T>
		LuaRegister & def_class_property(T ClassType::* obj, const string & name);

		template <typename T>
		LuaRegister & def_func(T obj, const string & name);

		RuntimeError get_class_table();

		RuntimeError get_class_metatable();

		void init_class(const string & name);


	private:
		lua_State * m_lua;

	};

	template <typename ClassType, typename CtorType>
	class LIGHTINK_TEMPLATE_DECL LuaModuleByClass : public LuaModule
	{
	public:
		LuaModuleByClass(lua_State * L, const string & moduleName, const LuaRef & table, LuaRegister<ClassType, CtorType> & c);

		virtual ~LuaModuleByClass();

		LuaRegister<ClassType, CtorType> & module_end();

		template <typename T>
		LuaModuleByClass & def(T obj, const string & name);

		template <typename T>
		LuaRegister<ClassType, CtorType> & operator[](const T & idx);

	protected:
		LuaRegister<ClassType, CtorType> & m_class;
	};

}

#include "LuaEngine/LuaRegister.cpp"

#endif