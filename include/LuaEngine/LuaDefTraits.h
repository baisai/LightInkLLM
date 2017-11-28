


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

#ifndef LIGHTINK_LUAENGINE_LUADEFTRAITS_H_
#define LIGHTINK_LUAENGINE_LUADEFTRAITS_H_

#include "Common/Type.h"
#include "LuaEngine/LuaDefTool.h"

namespace LightInk
{
	/////////////////////////////////////////////////////////////////////////////////
	//LuaDefTraits
	/////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	struct LuaDefTraits;

	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<lua_CFunction>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, lua_CFunction arg, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T>::call(RegisterType* reg, lua_State * L, lua_CFunction arg, const string & name)");
			reg->def_cclosure(arg, name);
			LogTraceStepReturnVoid;
		}

		static void call(lua_State * L, lua_CFunction arg, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T>::call(lua_State * L, lua_CFunction arg, const string & name)");
			LuaDefTool::def_cclosure(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};
	
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T arg, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T>::call(RegisterType* reg, lua_State * L, T arg, const string & name)");
			reg->def_class_property(arg, name);
			LogTraceStepReturnVoid;
		}

		static void call(lua_State * L, T arg, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T>::call(lua_State * L, T arg, const string & name)");
			LuaDefTool::def_property(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)()>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (*arg)(), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)()>::call(RegisterType* reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}

		static void call(lua_State * L, T (*arg)(), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)()>::call(lua_State * L, T (*arg)(), const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

/*
--lua生成
function create_traits(count)

	local temp = {}

	for k = 1, count do
		local arg1, arg2 = "", ""
		for ki = 1, k do
			arg1 = string.format("%s, typename Arg%d", arg1, ki)
			arg2 = string.format("%sArg%d, ", arg2, ki)
		end
		arg2 = string.sub(arg2, 1, #arg2-2)

		table.insert(temp, "\ttemplate <typename T")
		table.insert(temp, arg1)
		table.insert(temp, ">\n\tstruct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(")
		table.insert(temp, arg2)
		table.insert(temp, ")>\n\t{\n\t\ttemplate <typename RegisterType>\n\t\tstatic void call(RegisterType * reg, lua_State * L, T (*arg)(")
		table.insert(temp, arg2)
		table.insert(temp, "), const string & name)\n\t\t{\n\t\t\tLogTraceStepCall(\"LuaDefTraits<T (*)(")
		table.insert(temp, arg2)
		table.insert(temp, ")>::call(RegisterType * reg, lua_State * L, arg, const string & name)\");\n\t\t\t")
		table.insert(temp, "reg->def_func(arg, name);\n\t\t\tLogTraceStepReturnVoid;\n\t\t}\n")

		table.insert(temp, "\t\tstatic void call(lua_State * L, T (*arg)(")
		table.insert(temp, arg2)
		table.insert(temp, "), const string & name)\n\t\t{\n\t\t\tLogTraceStepCall(\"LuaDefTraits<T (*)(")
		table.insert(temp, arg2)
		table.insert(temp, ")>::call(lua_State * L, arg, const string & name)\");\n\t\t\t")
		table.insert(temp, "LuaDefTool::def_func(L, arg, name);\n\t\t\tLogTraceStepReturnVoid;\n\t\t}\n\t};\n\n")

	end

	print(table.concat(temp))

end

create_traits(20)
*/
	template <typename T, typename Arg1>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20)>
	{
		template <typename RegisterType>
		static void call(RegisterType * reg, lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_func(arg, name);
			LogTraceStepReturnVoid;
		}
		static void call(lua_State * L, T (*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20)>::call(lua_State * L, arg, const string & name)");
			LuaDefTool::def_func(L, arg, name);
			LogTraceStepReturnVoid;
		}
	};





	template <typename T, typename ClassType>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)()>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)()>::call(RegisterType* reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};
	template <typename T, typename ClassType>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)() const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)() const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)() const>::call(RegisterType* reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};
/*
--lua生成
function create_traits(count, isConst)

	local str = ""

	for k = 1, count do
		local arg1, arg2 = "", ""
		for ki = 1, k do
			arg1 = string.format("%s, typename Arg%d", arg1, ki)
			arg2 = string.format("%sArg%d, ", arg2, ki)
		end
		arg2 = string.sub(arg2, 1, #arg2-2)

		local temp = {}
		table.insert(temp, "\ttemplate <typename T, typename ClassType")
		table.insert(temp, arg1)
		table.insert(temp, ">\n\tstruct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(")
		table.insert(temp, arg2)
		if isConst then
			table.insert(temp, ") const>")
		else
			table.insert(temp, ")>")
		end

		table.insert(temp, "\n\t{\n\t\ttemplate <typename RegisterType>\n\t\tstatic void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(")
		table.insert(temp, arg2)
		if isConst then
			table.insert(temp, ") const, ")
		else
			table.insert(temp, "), ")
		end
		table.insert(temp, "const string & name)\n\t\t{\n\t\t\tLogTraceStepCall(\"LuaDefTraits<T (ClassType::*)(")
		table.insert(temp, arg2)
		if isConst then
			table.insert(temp, ") const>")
		else
			table.insert(temp, ")>")
		end
		table.insert(temp, "::call(RegisterType * reg, lua_State * L, arg, const string & name)\");\n\t\t\t")
		table.insert(temp, "reg->def_class_func(arg, name);\n\t\t\tLogTraceStepReturnVoid;\n\t\t}\n\t};\n\n")

		str = str .. table.concat(temp)
	end

	print(str)

end

create_traits(20)
create_traits(20, true)
*/


	template <typename T, typename ClassType, typename Arg1>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20)>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20), const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20)>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};


	template <typename T, typename ClassType, typename Arg1>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};

	template <typename T, typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
	struct LIGHTINK_TEMPLATE_DECL LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) const>
	{
		template <typename RegisterType>
		static void call(RegisterType* reg, lua_State * L, T (ClassType::*arg)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) const, const string & name)
		{
			LogTraceStepCall("LuaDefTraits<T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) const>::call(RegisterType * reg, lua_State * L, arg, const string & name)");
			reg->def_class_func(arg, name);
			LogTraceStepReturnVoid;
		}
	};


}

#endif