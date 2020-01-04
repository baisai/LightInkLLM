


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
#ifndef LIGHTINK_LUAENGINE_LUACLASSCTORTRAITS_H_
#define LIGHTINK_LUAENGINE_LUACLASSCTORTRAITS_H_

#include "Common/Type.h"
#include "LuaEngine/LuaClassInfo.h"
#include "LuaEngine/LuaFixFunction.h"
#include "LuaEngine/LuaStack.h"
#include "LuaEngine/LuaObjectCache.h"

namespace LightInk
{
	template<typename T>
	void lua_set_object_userdata(lua_State * L, int objectIdx, const T * obj, int offset, bool luaGC)
	{
		LogTraceStepCall("void lua_set_object_userdata<T>(lua_State * L, int objectIdx, const T * obj, int offset, bool luaGC)");
		if (obj == NULL)
			LogTraceStepReturnVoid;

		objectIdx = lua_absindex(L, objectIdx);
		lua_pushlightuserdata(L, LuaClassMgr::get_object_key());
		void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass));
		lua_pushvalue(L, lua_upvalueindex(1));
		lua_setmetatable(L, -2);
		lua_rawset(L, objectIdx);
		new(userdataPtr) LuaUserdataForClass(obj, offset, luaGC);
		lua_set_cache_object(L, obj, objectIdx);
		LogTraceStepReturnVoid;
	}

	template<typename T>
	void lua_new_object_userdata(lua_State * L, const T * obj, int offset, bool luaGC)
	{
		LogTraceStepCall("void lua_new_object_userdata<T>(lua_State * L, const T * obj, int offset, bool luaGC)");
		if (obj == NULL)
		{
			lua_pushnil(L);
			LogTraceStepReturnVoid;
		}
		
		void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass));
		lua_pushvalue(L, lua_upvalueindex(1));
		lua_setmetatable(L, -2);
		new(userdataPtr) LuaUserdataForClass(obj, offset, luaGC);
		lua_set_cache_object(L, obj, -1);
		LogTraceStepReturnVoid;
	}

	template <typename T, bool luaGC>
	int lua_class_create_user(lua_State * L)
	{
		LogTraceStepCall("int lua_class_create_user<T, luaGC>(lua_State * L)");
		lua_CFunction f =  lua_tocfunction(L, lua_upvalueindex(1));
		if (f(L) == 1)
		{
			LuaUserdataForClass * p = LuaMetatable::get_userdata(L, -1);
			if (p != NULL)
			{
				p->set_atuto_gc(luaGC);
				lua_set_cache_object(L, p->get_object(), -1);
				LogTraceStepReturn(1);
			}
		}
		LogScriptError(L, "Error!!!The user ctor not return a new object!!!");
		LogTraceStepReturn(0);
	}

	template <typename T, bool luaGC>
	int lua_class_ctor_user(lua_State * L)
	{
		LogTraceStepCall("int lua_class_ctor_user<T, luaGC>(lua_State * L)");
		lua_CFunction f =  lua_tocfunction(L, lua_upvalueindex(1));
		if (f(L) == 1)
		{
			LuaUserdataForClass * p = LuaMetatable::get_userdata(L, -1);
			if (p != NULL)
			{
				p->set_atuto_gc(luaGC);
				lua_pushlightuserdata(L, LuaClassMgr::get_object_key());
				lua_pushvalue(L, -2);
				lua_rawset(L, 1);
				lua_set_cache_object(L, p->get_object(), 1);
				LogTraceStepReturn(0);
			}
		}
		LogScriptErrorJump(L, "Error!!!The user ctor not return a new object!!!");
		LogTraceStepReturn(0);
	}

	

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//LuaClassCtorTraits and LuaClassCreateTraits
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename ClassType, bool luaGC, typename FunctionType>
	struct LuaClassCtorTraits;

	template <typename ClassType, bool luaGC, typename FunctionType>
	struct LuaClassCreateTraits;


	template <typename ClassType, bool luaGC>
	struct LuaClassCtorTraits<ClassType, luaGC, void()>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void()>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			lua_set_object_userdata(L, 1, new ClassType(), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC>
	struct LuaClassCreateTraits<ClassType, luaGC, void()>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void()>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			lua_new_object_userdata(L, new ClassType(), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

/*
--lua生成
function create_ctor_traits(count)

	local str = ""

	for k = 1, count do
		local arg1, arg2, arg3, arg4 = "", "", "", ""
		for ki = 1, k do
			arg1 = string.format("%s, typename Arg%d", arg1, ki)
			arg2 = string.format("%sArg%d, ", arg2, ki)
			arg3 = string.format("%s\t\t\tArg%d arg%d = LuaStack<const Arg%d>::get(L, %d);\n", arg3, ki, ki, ki, ki+1)
			arg4 = string.format("%sarg%d, ", arg4, ki)
		end
		arg2 = string.sub(arg2, 1, #arg2-2)
		arg4 = string.sub(arg4, 1, #arg4-2)

		local temp = {}
		table.insert(temp, "\ttemplate <typename ClassType, bool luaGC")
		table.insert(temp, arg1)
		table.insert(temp, ">\n\tstruct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(")
		table.insert(temp, arg2)
		table.insert(temp, ")>\n\t{\n\t\tstatic int call(lua_State * L)\n\t\t{\n\t\t\tLogTraceStepCall(\"LuaClassCtorTraits<ClassType, void(")
		table.insert(temp, arg2)
		table.insert(temp, ")>::call(lua_State * L)\");\n")
		table.insert(temp, "\t\t\tif (!lua_istable(L, 1))\n\t\t\t{\n\t\t\t\tLogScriptErrorJump(L, \"Error!!!The NewClass Traits First arg is not object table!!!\");\n\t\t\t\tLogTraceStepReturn(0);\n\t\t\t}\n")
		table.insert(temp, arg3)
		table.insert(temp, "\t\t\tlua_set_object_userdata(L, 1, new ClassType(")
		table.insert(temp, arg4)
		table.insert(temp, "), 0, luaGC);\n\t\t\tLogTraceStepReturn(0);\n\t\t}\n\t};\n\n")

		str = str .. table.concat(temp)
	end

	print(str)

end

function create_create_traits(count)

	local str = ""

	for k = 1, count do
		local arg1, arg2, arg3, arg4 = "", "", "", ""
		for ki = 1, k do
			arg1 = string.format("%s, typename Arg%d", arg1, ki)
			arg2 = string.format("%sArg%d, ", arg2, ki)
			arg3 = string.format("%s\t\t\tArg%d arg%d = LuaStack<const Arg%d>::get(L, %d);\n", arg3, ki, ki, ki, ki+1)
			arg4 = string.format("%sarg%d, ", arg4, ki)
		end
		arg2 = string.sub(arg2, 1, #arg2-2)
		arg4 = string.sub(arg4, 1, #arg4-2)

		local temp = {}
		table.insert(temp, "\ttemplate <typename ClassType, bool luaGC")
		table.insert(temp, arg1)
		table.insert(temp, ">\n\tstruct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(")
		table.insert(temp, arg2)
		table.insert(temp, ")>\n\t{\n\t\tstatic int call(lua_State * L)\n\t\t{\n\t\t\tLogTraceStepCall(\"LuaClassCreateTraits<ClassType, void(")
		table.insert(temp, arg2)
		table.insert(temp, ")>::call(lua_State * L)\");\n")
		table.insert(temp, "\t\t\tif (!lua_istable(L, 1))\n\t\t\t{\n\t\t\t\tLogScriptError(L, \"Error!!!The NewClass Traits First arg is not class table!!!\");\n\t\t\t\tLogTraceStepReturn(0);\n\t\t\t}\n")
		table.insert(temp, arg3)
		table.insert(temp, "\t\t\tlua_new_object_userdata(L, new ClassType(")
		table.insert(temp, arg4)
		table.insert(temp, "), 0, luaGC);\n\t\t\tLogTraceStepReturn(1);\n\t\t}\n\t};\n\n")

		str = str .. table.concat(temp)
	end

	print(str)

end

create_ctor_traits(20)
create_create_traits(20)
*/

	template <typename ClassType, bool luaGC, typename Arg1>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			lua_set_object_userdata(L, 1, new ClassType(arg1), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 15);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 15);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 16);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 15);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 16);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 17);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 15);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 16);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 17);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 18);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 15);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 16);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 17);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 18);
			Arg18 arg18 = LuaStack<const Arg18>::get(L, 19);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 15);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 16);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 17);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 18);
			Arg18 arg18 = LuaStack<const Arg18>::get(L, 19);
			Arg19 arg19 = LuaStack<const Arg19>::get(L, 20);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCtorTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCtorTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not object table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 15);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 16);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 17);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 18);
			Arg18 arg18 = LuaStack<const Arg18>::get(L, 19);
			Arg19 arg19 = LuaStack<const Arg19>::get(L, 20);
			Arg20 arg20 = LuaStack<const Arg20>::get(L, 21);
			lua_set_object_userdata(L, 1, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20), 0, luaGC);
			LogTraceStepReturn(0);
		}
	};


	template <typename ClassType, bool luaGC, typename Arg1>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			lua_new_object_userdata(L, new ClassType(arg1), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			lua_new_object_userdata(L, new ClassType(arg1, arg2), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 15);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 15);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 16);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 15);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 16);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 17);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 15);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 16);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 17);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 18);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 15);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 16);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 17);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 18);
			Arg18 arg18 = LuaStack<const Arg18>::get(L, 19);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 15);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 16);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 17);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 18);
			Arg18 arg18 = LuaStack<const Arg18>::get(L, 19);
			Arg19 arg19 = LuaStack<const Arg19>::get(L, 20);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
	struct LIGHTINK_TEMPLATE_DECL LuaClassCreateTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaClassCreateTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptError(L, "Error!!!The NewClass Traits First arg is not class table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 11);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 12);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 13);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 14);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 15);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 16);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 17);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 18);
			Arg18 arg18 = LuaStack<const Arg18>::get(L, 19);
			Arg19 arg19 = LuaStack<const Arg19>::get(L, 20);
			Arg20 arg20 = LuaStack<const Arg20>::get(L, 21);
			lua_new_object_userdata(L, new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20), 0, luaGC);
			LogTraceStepReturn(1);
		}
	};

}

#endif