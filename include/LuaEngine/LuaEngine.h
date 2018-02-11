


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

#ifndef LIGHTINK_LUAENGINE_LUAENGINE_H_
#define LIGHTINK_LUAENGINE_LUAENGINE_H_

#include "Common/RuntimeError.h"
#include "Common/STLType.h"
#include "LuaEngine/LuaLib.h"
#include "Log/Log.h"
#include "Log/LoggerMgr.h"
#include "LuaEngine/LuaRef.h"
#include "LuaEngine/LuaRegister.h"
#include "LuaEngine/LuaModule.h"
#include "LuaEngine/LuaDefAutoTool.h"

namespace LightInk
{
	class LIGHTINK_DECL LuaEngine : public SmallObject
	{
	public:
		LuaEngine();
		~LuaEngine();
		lua_State * get_lua_state();
		bool is_inited();
		RuntimeError init();
		void close();
		virtual void * do_lua_allocator(void * ptr, size_t osize, size_t nsize);
		virtual RuntimeError do_init();
		virtual void do_close();
		virtual RuntimeError add_package_path(const string & path);
		virtual void clear_package_path();
		static void dump_stack(lua_State * L);
		virtual RuntimeError do_string(const char * chunk);
		virtual RuntimeError do_file(const char * fileName);
		virtual RuntimeError require_file(const char * fileName);
		RuntimeError register_global_func(const char * func, lua_CFunction f);
		LuaRef get_global_ref(const char * name);

		void register_module(void(*func)(lua_State * lua));

		static const char * get_class_name(lua_State * L, int idx);

	private:
		static void * lua_allocator(void * ud, void * ptr, size_t osize, size_t nsize);
		static int lua_error_catch(lua_State * L);
		static int log_string(lua_State * L, LogLevel::LEVEL level);
		static int lua_print_debug(lua_State * L);
		static int lua_replace_print(lua_State * L);
		static int lua_print_warning(lua_State * L);
		static int lua_print_error(lua_State * L);
		static int lua_print_fatal(lua_State * L);
		
	protected:
		lua_State * m_lua;

	public:
		template <typename T>
		RuntimeError open_module(T m)
		{
			LogTraceStepCall("LuaEngine::open_module<T>(T m)");
			m(m_lua);
			LogTraceStepReturn(RE_Success);
		}

		template <typename T>
		RuntimeError get_global_var(const char * name, T & var)
		{
			LogTraceStepCall("RuntimeError LuaEngine::get_global_var<T>(const char * name, T & var)");
			LuaStateProtect lsp(m_lua);
			lua_getglobal(m_lua, name);
			var = LuaStack<const T>::get(m_lua, -1);
			lsp.reset();
			LogTraceStepReturn(RE_Success);
		}

		template <typename T>
		RuntimeError set_global_var(const char * name, const T & var)
		{
			LogTraceStepCall("RuntimeError LuaEngine::set_global_var<T>(const char * name, T & var)");
			LuaStack<const T>::push(m_lua, var);
			lua_setglobal(m_lua, name);
			LogTraceStepReturn(RE_Success);
		}

		template<typename R>
		RuntimeError call(const char * name, R & result)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(R & result)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			if (lua_pcall(m_lua, 0, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{ LogTraceStepReturn(e); }
			
			LogTraceStepReturn(RE_Success);
		}
/*
--lua生成
function create_traits(count)

	local str = ""

	for k = 1, count do
		local arg1, arg2, arg3 = "", "", ""
		for ki = 1, k do
			arg1 = string.format("%stypename Arg%d, ", arg1, ki)
			arg2 = string.format("%sconst Arg%d & arg%d, ", arg2, ki, ki)
			arg3 = string.format("%s\t\t\tLuaStack<const Arg%d>::push(m_lua, arg%d);\n", arg3, ki, ki)
		end
		arg1 = string.sub(arg1, 1, #arg1-2)
		arg2 = string.sub(arg2, 1, #arg2-2)


		local temp = {}
		table.insert(temp, "\t\ttemplate <typename R, ")
		table.insert(temp, arg1)
		table.insert(temp, ">\n\t\tRuntimeError call(const char * name, R & result, ")
		table.insert(temp, arg2)
		table.insert(temp, ")\n\t\t{\n\t\t\tLogTraceStepCall(\"RuntimeError LuaEngine::call(const char * name, R & result, ")
		table.insert(temp, arg2)
		table.insert(temp, ")\");\n")
		table.insert(temp, "\t\t\tLuaStateProtect lsp(m_lua, true);\n\t\t\tlua_getglobal(m_lua, name);\n")
		table.insert(temp, arg3)
		table.insert(temp, string.format("\t\t\tif (lua_pcall(m_lua, %d, 1, 0))\n\t\t\t{\n", k))
		table.insert(temp, "\t\t\t\tconst char * errStr = lua_tostring(m_lua, -1);\n\t\t\t\tif (errStr) LogScriptError(errStr);\n")
		table.insert(temp, "\t\t\t\tLogTraceStepReturn(RE_Lua_RuntimeError);\n\t\t\t}\n")
		table.insert(temp, "\t\t\ttry { result = LuaStack<const R>::get(m_lua, -1); }\n\t\t\tcatch (RuntimeError e)\n\t\t\t{  LogTraceStepReturn(e); }\n\t\t\tLogTraceStepReturn(RE_Success);\n\t\t}\n\n")

		str = str .. table.concat(temp)
	end

	print(str)

end

create_traits(20)
*/
		template <typename R, typename Arg1>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			if (lua_pcall(m_lua, 1, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			if (lua_pcall(m_lua, 2, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			if (lua_pcall(m_lua, 3, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			if (lua_pcall(m_lua, 4, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			if (lua_pcall(m_lua, 5, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			LuaStack<const Arg6>::push(m_lua, arg6);
			if (lua_pcall(m_lua, 6, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			LuaStack<const Arg6>::push(m_lua, arg6);
			LuaStack<const Arg7>::push(m_lua, arg7);
			if (lua_pcall(m_lua, 7, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			LuaStack<const Arg6>::push(m_lua, arg6);
			LuaStack<const Arg7>::push(m_lua, arg7);
			LuaStack<const Arg8>::push(m_lua, arg8);
			if (lua_pcall(m_lua, 8, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			LuaStack<const Arg6>::push(m_lua, arg6);
			LuaStack<const Arg7>::push(m_lua, arg7);
			LuaStack<const Arg8>::push(m_lua, arg8);
			LuaStack<const Arg9>::push(m_lua, arg9);
			if (lua_pcall(m_lua, 9, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			LuaStack<const Arg6>::push(m_lua, arg6);
			LuaStack<const Arg7>::push(m_lua, arg7);
			LuaStack<const Arg8>::push(m_lua, arg8);
			LuaStack<const Arg9>::push(m_lua, arg9);
			LuaStack<const Arg10>::push(m_lua, arg10);
			if (lua_pcall(m_lua, 10, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			LuaStack<const Arg6>::push(m_lua, arg6);
			LuaStack<const Arg7>::push(m_lua, arg7);
			LuaStack<const Arg8>::push(m_lua, arg8);
			LuaStack<const Arg9>::push(m_lua, arg9);
			LuaStack<const Arg10>::push(m_lua, arg10);
			LuaStack<const Arg11>::push(m_lua, arg11);
			if (lua_pcall(m_lua, 11, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			LuaStack<const Arg6>::push(m_lua, arg6);
			LuaStack<const Arg7>::push(m_lua, arg7);
			LuaStack<const Arg8>::push(m_lua, arg8);
			LuaStack<const Arg9>::push(m_lua, arg9);
			LuaStack<const Arg10>::push(m_lua, arg10);
			LuaStack<const Arg11>::push(m_lua, arg11);
			LuaStack<const Arg12>::push(m_lua, arg12);
			if (lua_pcall(m_lua, 12, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			LuaStack<const Arg6>::push(m_lua, arg6);
			LuaStack<const Arg7>::push(m_lua, arg7);
			LuaStack<const Arg8>::push(m_lua, arg8);
			LuaStack<const Arg9>::push(m_lua, arg9);
			LuaStack<const Arg10>::push(m_lua, arg10);
			LuaStack<const Arg11>::push(m_lua, arg11);
			LuaStack<const Arg12>::push(m_lua, arg12);
			LuaStack<const Arg13>::push(m_lua, arg13);
			if (lua_pcall(m_lua, 13, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			LuaStack<const Arg6>::push(m_lua, arg6);
			LuaStack<const Arg7>::push(m_lua, arg7);
			LuaStack<const Arg8>::push(m_lua, arg8);
			LuaStack<const Arg9>::push(m_lua, arg9);
			LuaStack<const Arg10>::push(m_lua, arg10);
			LuaStack<const Arg11>::push(m_lua, arg11);
			LuaStack<const Arg12>::push(m_lua, arg12);
			LuaStack<const Arg13>::push(m_lua, arg13);
			LuaStack<const Arg14>::push(m_lua, arg14);
			if (lua_pcall(m_lua, 14, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			LuaStack<const Arg6>::push(m_lua, arg6);
			LuaStack<const Arg7>::push(m_lua, arg7);
			LuaStack<const Arg8>::push(m_lua, arg8);
			LuaStack<const Arg9>::push(m_lua, arg9);
			LuaStack<const Arg10>::push(m_lua, arg10);
			LuaStack<const Arg11>::push(m_lua, arg11);
			LuaStack<const Arg12>::push(m_lua, arg12);
			LuaStack<const Arg13>::push(m_lua, arg13);
			LuaStack<const Arg14>::push(m_lua, arg14);
			LuaStack<const Arg15>::push(m_lua, arg15);
			if (lua_pcall(m_lua, 15, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			LuaStack<const Arg6>::push(m_lua, arg6);
			LuaStack<const Arg7>::push(m_lua, arg7);
			LuaStack<const Arg8>::push(m_lua, arg8);
			LuaStack<const Arg9>::push(m_lua, arg9);
			LuaStack<const Arg10>::push(m_lua, arg10);
			LuaStack<const Arg11>::push(m_lua, arg11);
			LuaStack<const Arg12>::push(m_lua, arg12);
			LuaStack<const Arg13>::push(m_lua, arg13);
			LuaStack<const Arg14>::push(m_lua, arg14);
			LuaStack<const Arg15>::push(m_lua, arg15);
			LuaStack<const Arg16>::push(m_lua, arg16);
			if (lua_pcall(m_lua, 16, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			LuaStack<const Arg6>::push(m_lua, arg6);
			LuaStack<const Arg7>::push(m_lua, arg7);
			LuaStack<const Arg8>::push(m_lua, arg8);
			LuaStack<const Arg9>::push(m_lua, arg9);
			LuaStack<const Arg10>::push(m_lua, arg10);
			LuaStack<const Arg11>::push(m_lua, arg11);
			LuaStack<const Arg12>::push(m_lua, arg12);
			LuaStack<const Arg13>::push(m_lua, arg13);
			LuaStack<const Arg14>::push(m_lua, arg14);
			LuaStack<const Arg15>::push(m_lua, arg15);
			LuaStack<const Arg16>::push(m_lua, arg16);
			LuaStack<const Arg17>::push(m_lua, arg17);
			if (lua_pcall(m_lua, 17, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17, const Arg18 & arg18)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17, const Arg18 & arg18)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			LuaStack<const Arg6>::push(m_lua, arg6);
			LuaStack<const Arg7>::push(m_lua, arg7);
			LuaStack<const Arg8>::push(m_lua, arg8);
			LuaStack<const Arg9>::push(m_lua, arg9);
			LuaStack<const Arg10>::push(m_lua, arg10);
			LuaStack<const Arg11>::push(m_lua, arg11);
			LuaStack<const Arg12>::push(m_lua, arg12);
			LuaStack<const Arg13>::push(m_lua, arg13);
			LuaStack<const Arg14>::push(m_lua, arg14);
			LuaStack<const Arg15>::push(m_lua, arg15);
			LuaStack<const Arg16>::push(m_lua, arg16);
			LuaStack<const Arg17>::push(m_lua, arg17);
			LuaStack<const Arg18>::push(m_lua, arg18);
			if (lua_pcall(m_lua, 18, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17, const Arg18 & arg18, const Arg19 & arg19)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17, const Arg18 & arg18, const Arg19 & arg19)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			LuaStack<const Arg6>::push(m_lua, arg6);
			LuaStack<const Arg7>::push(m_lua, arg7);
			LuaStack<const Arg8>::push(m_lua, arg8);
			LuaStack<const Arg9>::push(m_lua, arg9);
			LuaStack<const Arg10>::push(m_lua, arg10);
			LuaStack<const Arg11>::push(m_lua, arg11);
			LuaStack<const Arg12>::push(m_lua, arg12);
			LuaStack<const Arg13>::push(m_lua, arg13);
			LuaStack<const Arg14>::push(m_lua, arg14);
			LuaStack<const Arg15>::push(m_lua, arg15);
			LuaStack<const Arg16>::push(m_lua, arg16);
			LuaStack<const Arg17>::push(m_lua, arg17);
			LuaStack<const Arg18>::push(m_lua, arg18);
			LuaStack<const Arg19>::push(m_lua, arg19);
			if (lua_pcall(m_lua, 19, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}

		template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
		RuntimeError call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17, const Arg18 & arg18, const Arg19 & arg19, const Arg20 & arg20)
		{
			LogTraceStepCall("RuntimeError LuaEngine::call(const char * name, R & result, const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17, const Arg18 & arg18, const Arg19 & arg19, const Arg20 & arg20)");
			LuaStateProtect lsp(m_lua, true);
			lua_getglobal(m_lua, name);
			LuaStack<const Arg1>::push(m_lua, arg1);
			LuaStack<const Arg2>::push(m_lua, arg2);
			LuaStack<const Arg3>::push(m_lua, arg3);
			LuaStack<const Arg4>::push(m_lua, arg4);
			LuaStack<const Arg5>::push(m_lua, arg5);
			LuaStack<const Arg6>::push(m_lua, arg6);
			LuaStack<const Arg7>::push(m_lua, arg7);
			LuaStack<const Arg8>::push(m_lua, arg8);
			LuaStack<const Arg9>::push(m_lua, arg9);
			LuaStack<const Arg10>::push(m_lua, arg10);
			LuaStack<const Arg11>::push(m_lua, arg11);
			LuaStack<const Arg12>::push(m_lua, arg12);
			LuaStack<const Arg13>::push(m_lua, arg13);
			LuaStack<const Arg14>::push(m_lua, arg14);
			LuaStack<const Arg15>::push(m_lua, arg15);
			LuaStack<const Arg16>::push(m_lua, arg16);
			LuaStack<const Arg17>::push(m_lua, arg17);
			LuaStack<const Arg18>::push(m_lua, arg18);
			LuaStack<const Arg19>::push(m_lua, arg19);
			LuaStack<const Arg20>::push(m_lua, arg20);
			if (lua_pcall(m_lua, 20, 1, 0))
			{
				const char * errStr = lua_tostring(m_lua, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(RE_Lua_RuntimeError);
			}
			try { result = LuaStack<const R>::get(m_lua, -1); }
			catch (RuntimeError e)
			{  LogTraceStepReturn(e); }
			LogTraceStepReturn(RE_Success);
		}
	};
}

#endif