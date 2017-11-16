


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
#ifndef LIGHTINK_LUAENGINE_LUANEWCLASSTRAITS_H_
#define LIGHTINK_LUAENGINE_LUANEWCLASSTRAITS_H_

#include "Common/Type.h"
#include "Common/SmallObject.h"
#include "LuaEngine/LuaClassInfo.h"
#include "LuaEngine/LuaFixFunction.h"

namespace LightInk
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//LuaNewClassTraits
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename ClassType, bool luaGC, typename FunctionType>
	struct LuaNewClassTraits;

	template <typename ClassType, bool luaGC>
	struct LuaNewClassTraits<ClassType, luaGC, int()>
	{
		static  int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, int()>::call(lua_State * L)");
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, bool luaGC>
	struct LuaNewClassTraits<ClassType, luaGC, void>
	{
		static  int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void()>::call(lua_State * L)");
			lua_pushnil(L);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC>
	struct LuaNewClassTraits<ClassType, luaGC, void()>
	{
		static  int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void()>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
				LogTraceStepReturn(0);
			}
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);
			

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(), luaGC);
			LogTraceStepReturn(1);
		}
	};

/*
--lua生成
function create_traits(count)

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
		table.insert(temp, ">\n\tstruct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(")
		table.insert(temp, arg2)
		table.insert(temp, ")>\n\t{\n\t\tstatic int call(lua_State * L)\n\t\t{\n\t\t\tLogTraceStepCall(\"LuaNewClassTraits<ClassType, void(")
		table.insert(temp, arg2)
		table.insert(temp, ")>::call(lua_State * L)\");\n")
		table.insert(temp, "\t\t\tif (!lua_istable(L, 1))\n\t\t\t{\n\t\t\t\tLogScriptErrorJump(L, \"Error!!!The NewClass Traits First arg is not table!!!\");\n\t\t\t\tLogTraceStepReturn(0);\n\t\t\t}\n")
		table.insert(temp, arg3)
		table.insert(temp, "\t\t\trawgetfield(L, 1, \"metatable__\");\n\t\t\t//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());\n")
		table.insert(temp, "\t\t\tif (!lua_istable(L, -1))\n\t\t\t{\n\t\t\t\tLogScriptErrorJump(L, \"Error!!!The NewClass Traits First arg have not userdata metatable!!!\");\n\t\t\t\tLogTraceStepReturn(0);\n\t\t\t}\n")
		table.insert(temp, "\t\t\tvoid * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));\n\t\t\t")
		table.insert(temp, "lua_pushvalue(L, -2);\n\t\t\tlua_setmetatable(L, -2);\n\n\t\t\t")
		table.insert(temp, "new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(")
		table.insert(temp, arg4)
		table.insert(temp, "), luaGC);\n\t\t\tLogTraceStepReturn(1);\n\t\t}\n\t};\n\n")

		str = str .. table.concat(temp)
	end

	print(str)

end

create_traits(20)
*/

	template <typename ClassType, bool luaGC, typename Arg1>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5, arg6), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
				LogTraceStepReturn(0);
			}
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
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
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
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
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
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
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
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
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
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
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
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
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
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
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
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
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
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
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
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
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
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
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
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
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19), luaGC);
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, bool luaGC, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
	struct LIGHTINK_TEMPLATE_DECL LuaNewClassTraits<ClassType, luaGC, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20)>
	{
		static int call(lua_State * L)
		{
			LogTraceStepCall("LuaNewClassTraits<ClassType, void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20)>::call(lua_State * L)");
			if (!lua_istable(L, 1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg is not table!!!");
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
			rawgetfield(L, 1, "metatable__");
			//lua_rawgetp(L, 1, LuaClassInfo<ClassType>::get_class_key());
			if (!lua_istable(L, -1))
			{
				LogScriptErrorJump(L, "Error!!!The NewClass Traits First arg have not userdata metatable!!!");
				LogTraceStepReturn(0);
			}
			void * userdataPtr = lua_newuserdata(L, sizeof(LuaUserdataForClass<ClassType>));
			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);

			new(userdataPtr) LuaUserdataForClass<ClassType>(new ClassType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20), luaGC);
			LogTraceStepReturn(1);
		}
	};

}

#endif