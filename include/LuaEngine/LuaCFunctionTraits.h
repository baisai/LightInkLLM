


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
#ifndef LIGHTINK_LUAENGINE_LUACFUNCTIONTRAITS_H_
#define LIGHTINK_LUAENGINE_LUACFUNCTIONTRAITS_H_

#include "Common/Type.h"
#include "Log/Log.h"
#include "LuaEngine/LuaLib.h"
#include "LuaEngine/LuaMetatableTraits.h"
#include "LuaEngine/LuaStack.h"

namespace LightInk
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//LuaCFunctionTraits
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename FunctionType>
	struct LuaClassFunctionInfo
	{
		LuaClassFunctionInfo(FunctionType func): m_func(func){ ; }
		FunctionType   m_func;
	};

	template <typename FunctionType>
	struct LuaCFunctionTraits;


	template <typename ClassType, typename T>
	struct LuaCFunctionTraits< T (ClassType::*)() >
	{
		typedef T (ClassType::*FunctionType)();
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)() >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			LuaStack<const T>::push(L, (objPtr->*func)());
			LogTraceStepReturn(1);
		}
	};


	template <typename ClassType, typename T>
	struct LuaCFunctionTraits< T (ClassType::*)() const >
	{
		typedef T (ClassType::*FunctionType)() const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)() const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			LuaStack<const T>::push(L, (objPtr->*func)());
			LogTraceStepReturn(1);
		}
	};


	template <typename ClassType>
	struct LuaCFunctionTraits< void (ClassType::*)() >
	{
		typedef void (ClassType::*FunctionType)();
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)() >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			(objPtr->*func)();
			LogTraceStepReturn(0);
		}
	};


	template <typename ClassType>
	struct LuaCFunctionTraits< void (ClassType::*)() const >
	{
		typedef void (ClassType::*FunctionType)() const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)() const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			(objPtr->*func)();
			LogTraceStepReturn(0);
		}
	};

/*
--lua生成
function create_traits(count, isReturn, isConst)

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
		table.insert(temp, "\ttemplate <typename ClassType")
		if isReturn then
			table.insert(temp, ", typename T")
		else

		end
		table.insert(temp, arg1)
		table.insert(temp, ">\n\tstruct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits<")
		if isReturn then
			table.insert(temp, " T (ClassType::*)(")
		else
			table.insert(temp, " void (ClassType::*)(")
		end
		table.insert(temp, arg2)
		if isConst then
			table.insert(temp, ") const >\n\t{\n\t\t")
		else
			table.insert(temp, ") >\n\t{\n\t\t")
		end
		if isReturn then
			table.insert(temp, "typedef T (ClassType::*FunctionType)(")
		else
			table.insert(temp, "typedef void (ClassType::*FunctionType)(")
		end
		table.insert(temp, arg2)
		if isConst then
			table.insert(temp, ") const;\n")
		else
			table.insert(temp, ");\n")
		end
		table.insert(temp, "\t\tstatic int call(lua_State * L)\n\t\t{\n\t\t\tLogTraceStepCall(\"int LuaCFunctionTraits<")
		if isReturn then
			table.insert(temp, " T (ClassType::*)(")
		else
			table.insert(temp, " void (ClassType::*)(")
		end
		table.insert(temp, arg2)
		if isConst then
			table.insert(temp, ") const >")
		else
			table.insert(temp, ") >")
		end
		table.insert(temp, "::call(lua_State * L)\");\n")
		table.insert(temp, "\t\t\tLuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));\n")
		table.insert(temp, "\t\t\tFunctionType func = funcInfo->m_func;\n")
		table.insert(temp, "\t\t\tClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);\n")
		table.insert(temp, arg3)
		if isReturn then
			table.insert(temp, "\t\t\tLuaStack<const T>::push(L, (objPtr->*func)(")
		else
			table.insert(temp, "\t\t\t(objPtr->*func)(")
		end
		table.insert(temp, arg4)
		if isReturn then
			table.insert(temp, "));\n\t\t\tLogTraceStepReturn(1);\n\t\t}\n\t};\n\n")
		else
			table.insert(temp, ");\n\t\t\tLogTraceStepReturn(0);\n\t\t}\n\t};\n\n")
		end

		str = str .. table.concat(temp)
	end

	print(str)

end

create_traits(20, true, false)
print("\n\n\n")
create_traits(20, true, true)
print("\n\n\n")
create_traits(20, false, false)
print("\n\n\n")
create_traits(20, false, true)
print("\n\n\n")
*/
	template <typename ClassType, typename T, typename Arg1>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1) >
	{
		typedef T (ClassType::*FunctionType)(Arg1);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20));
			LogTraceStepReturn(1);
		}
	};






	template <typename ClassType, typename T, typename Arg1>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19));
			LogTraceStepReturn(1);
		}
	};

	template <typename ClassType, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) const >
	{
		typedef T (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			LuaStack<const T>::push(L, (objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20));
			LogTraceStepReturn(1);
		}
	};






	template <typename ClassType, typename Arg1>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1) >
	{
		typedef void (ClassType::*FunctionType)(Arg1);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			(objPtr->*func)(arg1);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			(objPtr->*func)(arg1, arg2);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			(objPtr->*func)(arg1, arg2, arg3);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			(objPtr->*func)(arg1, arg2, arg3, arg4);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20);
			LogTraceStepReturn(0);
		}
	};






	template <typename ClassType, typename Arg1>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			(objPtr->*func)(arg1);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			(objPtr->*func)(arg1, arg2);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			(objPtr->*func)(arg1, arg2, arg3);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			(objPtr->*func)(arg1, arg2, arg3, arg4);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 2);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 3);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 4);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 5);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 6);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 7);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 8);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 9);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 10);
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19);
			LogTraceStepReturn(0);
		}
	};

	template <typename ClassType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) const >
	{
		typedef void (ClassType::*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) const;
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (ClassType::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) const >::call(lua_State * L)");
			LuaClassFunctionInfo<FunctionType> * funcInfo = (LuaClassFunctionInfo<FunctionType> *)lua_touserdata (L, lua_upvalueindex(1));
			FunctionType func = funcInfo->m_func;
			ClassType * objPtr = LuaMetatableTraits<ClassType>::userdata_to_object(L, 1);
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
			(objPtr->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20);
			LogTraceStepReturn(0);
		}
	};





	template <typename T>
	struct LuaCFunctionTraits< T (*)() >
	{
		typedef T (*FunctionType)();
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)() >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			LuaStack<const T>::push(L, func());
			LogTraceStepReturn(1);
		}
	};


	template <>
	struct LuaCFunctionTraits< void (*)() >
	{
		typedef void (*FunctionType)();
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)() >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			func();
			LogTraceStepReturn(0);
		}
	};

/*
--lua生成
function create_traits(count, isReturn)

	local str = ""

	for k = 1, count do
		local arg1, arg2, arg3, arg4 = "", "", "", ""
		for ki = 1, k do
			arg1 = string.format("%stypename Arg%d, ", arg1, ki)
			arg2 = string.format("%sArg%d, ", arg2, ki)
			arg3 = string.format("%s\t\t\tArg%d arg%d = LuaStack<const Arg%d>::get(L, %d);\n", arg3, ki, ki, ki, ki)
			arg4 = string.format("%sarg%d, ", arg4, ki)
		end
		arg1 = string.sub(arg1, 1, #arg1-2)
		arg2 = string.sub(arg2, 1, #arg2-2)
		arg4 = string.sub(arg4, 1, #arg4-2)

		local temp = {}
		table.insert(temp, "\ttemplate <")
		if isReturn then
			table.insert(temp, "typename T, ")
		else

		end
		table.insert(temp, arg1)
		table.insert(temp, ">\n\tstruct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits<")
		if isReturn then
			table.insert(temp, " T (*)(")
		else
			table.insert(temp, " void (*)(")
		end
		table.insert(temp, arg2)

		table.insert(temp, ") >\n\t{\n\t\t")

		if isReturn then
			table.insert(temp, "typedef T (*FunctionType)(")
		else
			table.insert(temp, "typedef void (*FunctionType)(")
		end
		table.insert(temp, arg2)

		table.insert(temp, ");\n\t\tstatic int call(lua_State * L)\n\t\t{\n\t\t\tLogTraceStepCall(\"int LuaCFunctionTraits<")
		if isReturn then
			table.insert(temp, " T (*)(")
		else
			table.insert(temp, " void (*)(")
		end
		table.insert(temp, arg2)
		table.insert(temp, ") >::call(lua_State * L)\");\n")
		table.insert(temp, "\t\t\tFunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));\n")
		table.insert(temp, arg3)
		if isReturn then
			table.insert(temp, "\t\t\tLuaStack<const T>::push(L, func(")
		else
			table.insert(temp, "\t\t\tfunc(")
		end
		table.insert(temp, arg4)
		if isReturn then
			table.insert(temp, "));\n\t\t\tLogTraceStepReturn(1);\n\t\t}\n\t};\n\n")
		else
			table.insert(temp, ");\n\t\t\tLogTraceStepReturn(0);\n\t\t}\n\t};\n\n")
		end

		str = str .. table.concat(temp)
	end

	print(str)

end

create_traits(20, true)
print("\n\n\n")
create_traits(20, false)
*/

	template <typename T, typename Arg1>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1) >
	{
		typedef T (*FunctionType)(Arg1);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			LuaStack<const T>::push(L, func(arg1));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2) >
	{
		typedef T (*FunctionType)(Arg1, Arg2);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			LuaStack<const T>::push(L, func(arg1, arg2));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5, arg6));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5, arg6, arg7));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 14);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 14);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 15);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 14);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 15);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 16);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 14);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 15);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 16);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 17);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 14);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 15);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 16);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 17);
			Arg18 arg18 = LuaStack<const Arg18>::get(L, 18);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 14);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 15);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 16);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 17);
			Arg18 arg18 = LuaStack<const Arg18>::get(L, 18);
			Arg19 arg19 = LuaStack<const Arg19>::get(L, 19);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19));
			LogTraceStepReturn(1);
		}
	};

	template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) >
	{
		typedef T (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< T (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 14);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 15);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 16);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 17);
			Arg18 arg18 = LuaStack<const Arg18>::get(L, 18);
			Arg19 arg19 = LuaStack<const Arg19>::get(L, 19);
			Arg20 arg20 = LuaStack<const Arg20>::get(L, 20);
			LuaStack<const T>::push(L, func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20));
			LogTraceStepReturn(1);
		}
	};






	template <typename Arg1>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1) >
	{
		typedef void (*FunctionType)(Arg1);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			func(arg1);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2) >
	{
		typedef void (*FunctionType)(Arg1, Arg2);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			func(arg1, arg2);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			func(arg1, arg2, arg3);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			func(arg1, arg2, arg3, arg4);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			func(arg1, arg2, arg3, arg4, arg5);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			func(arg1, arg2, arg3, arg4, arg5, arg6);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			func(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 14);
			func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 14);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 15);
			func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 14);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 15);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 16);
			func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 14);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 15);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 16);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 17);
			func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 14);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 15);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 16);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 17);
			Arg18 arg18 = LuaStack<const Arg18>::get(L, 18);
			func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 14);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 15);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 16);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 17);
			Arg18 arg18 = LuaStack<const Arg18>::get(L, 18);
			Arg19 arg19 = LuaStack<const Arg19>::get(L, 19);
			func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19);
			LogTraceStepReturn(0);
		}
	};

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
	struct LIGHTINK_TEMPLATE_DECL LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) >
	{
		typedef void (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20);
		static int call(lua_State * L)
		{
			LogTraceStepCall("int LuaCFunctionTraits< void (*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) >::call(lua_State * L)");
			FunctionType func = *((FunctionType *)lua_touserdata (L, lua_upvalueindex(1)));
			Arg1 arg1 = LuaStack<const Arg1>::get(L, 1);
			Arg2 arg2 = LuaStack<const Arg2>::get(L, 2);
			Arg3 arg3 = LuaStack<const Arg3>::get(L, 3);
			Arg4 arg4 = LuaStack<const Arg4>::get(L, 4);
			Arg5 arg5 = LuaStack<const Arg5>::get(L, 5);
			Arg6 arg6 = LuaStack<const Arg6>::get(L, 6);
			Arg7 arg7 = LuaStack<const Arg7>::get(L, 7);
			Arg8 arg8 = LuaStack<const Arg8>::get(L, 8);
			Arg9 arg9 = LuaStack<const Arg9>::get(L, 9);
			Arg10 arg10 = LuaStack<const Arg10>::get(L, 10);
			Arg11 arg11 = LuaStack<const Arg11>::get(L, 11);
			Arg12 arg12 = LuaStack<const Arg12>::get(L, 12);
			Arg13 arg13 = LuaStack<const Arg13>::get(L, 13);
			Arg14 arg14 = LuaStack<const Arg14>::get(L, 14);
			Arg15 arg15 = LuaStack<const Arg15>::get(L, 15);
			Arg16 arg16 = LuaStack<const Arg16>::get(L, 16);
			Arg17 arg17 = LuaStack<const Arg17>::get(L, 17);
			Arg18 arg18 = LuaStack<const Arg18>::get(L, 18);
			Arg19 arg19 = LuaStack<const Arg19>::get(L, 19);
			Arg20 arg20 = LuaStack<const Arg20>::get(L, 20);
			func(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20);
			LogTraceStepReturn(0);
		}
	};




}


#endif