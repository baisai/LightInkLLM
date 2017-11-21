


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

#include "LuaEngine/LuaRegisterAbstract.h"

namespace LightInk
{
	//namespace use ::table::table::classname
	template <typename ClassType, typename CtorType>
	class LIGHTINK_TEMPLATE_DECL LuaRegister : public LuaRegisterAbstract<ClassType>
	{
	public:
		template <typename T>
		friend struct LuaDefTraits;

		LuaRegister(lua_State * L, const string & name);

		template <typename BaseType>
		LuaRegister(lua_State * L, const string & name, BaseClassStrategy<BaseType> bcs); //call once again base every class

	
	protected:
		RuntimeError add_ctor();

	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	template <typename ClassType, typename CtorType>
	LuaRegister<ClassType, CtorType>::LuaRegister(lua_State * L, const string & name) : LuaRegisterAbstract<ClassType>(L, name)
	{
		LogTraceStepCall("LuaRegister<ClassType, CtorType>::LuaRegister(lua_State * L, const string & name)");
		LuaStateProtect lsp(L, true);
		RuntimeError err = add_ctor();
		if (err != RE_Success)
		{
			LogError(get_runtime_error(err));
		}
		//pop metatable, class table, parent class table
		LogTraceStepReturnVoid;
	}

	template <typename ClassType, typename CtorType>
	template <typename BaseType>
	LuaRegister<ClassType, CtorType>::LuaRegister(lua_State * L, const string & name, BaseClassStrategy<BaseType> bcs) : LuaRegisterAbstract<ClassType>(L, name, bcs)
	{
		LogTraceStepCall("LuaRegister<ClassType, CtorType>::LuaRegister(lua_State * L, const string & name, BaseClassStrategy<BaseType> bcs)");
		LuaStateProtect lsp(L, true);
		RuntimeError err = add_ctor();
		if (err != RE_Success)
		{
			LogError(get_runtime_error(err));
		}
		//pop metatable, class table, parent class table
		LogTraceStepReturnVoid;
	}

	template <typename ClassType, typename CtorType>
	RuntimeError LuaRegister<ClassType, CtorType>::add_ctor()
	{
		LogTraceStepCall("RuntimeError LuaRegister<ClassType, CtorType>::add_ctor()");
		LuaStateProtect lsp(m_lua, true);
		RuntimeError err = get_class_table();
		if (err != RE_Success)
		{
			LogTraceStepReturn(err);
		}
		lua_pushcclosure(m_lua, &LuaNewClassTraits<ClassType, false, CtorType>::call, 0);
		rawsetfield(m_lua, -2, "new__"); //class table
		lua_getmetatable(m_lua, -1);
		lua_pushcclosure(m_lua, &LuaNewClassTraits<ClassType, true, CtorType>::call, 0);
		rawsetfield(m_lua, -2, "__call"); //class table
		//pop metatable, class table, parent class table
		LogTraceStepReturn(RE_Success);
	}
}

#endif