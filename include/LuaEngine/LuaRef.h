


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

#ifndef LIGHTINK_LUAENGINE_LUAREF_H_
#define LIGHTINK_LUAENGINE_LUAREF_H_

#include "Common/Type.h"
#include "Log/Log.h"
#include "LuaEngine/LuaStateProtect.h"
#include "LuaEngine/LuaStack.h"
#include "LuaEngine/LuaTableRef.h"


namespace LightInk
{
	class LIGHTINK_DECL LuaRef : public SmallObject
	{
	public:
		LuaRef(lua_State * L);
		LuaRef(lua_State * L, bool fromStack);
		LuaRef(lua_State * L, int idx);
		template <typename T>
		LuaRef(lua_State * L, const T & v) : m_L(L)
		{
			LogTraceStepCall("LuaRef::LuaRef<T>(lua_State * L, const T v)");
			LuaStack<const T>::push(m_L, v);
			m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
			LogTraceStepReturnVoid;
		}
		LuaRef(const LuaTableRef & v);
		LuaRef(const LuaRef & cp);
		virtual ~LuaRef();

		void clear_lua();

		int create_ref() const;

		void set_nil();

		LuaRef & operator = (const LuaRef & right);
		LuaRef & operator = (const LuaTableRef & right);

		template <typename T>
		LuaRef & operator = (const T & right)
		{
			LogTraceStepCall("LuaRef & LuaRef::operator = <T>(const T & right)");
			luaL_unref(m_L, LUA_REGISTRYINDEX, m_ref);
			LuaStack<const T>::push(m_L, right);
			m_ref = luaL_ref(m_L, LUA_REGISTRYINDEX);
			LogTraceStepReturn(*this);
		}


		static LuaRef new_table(lua_State * L, int narr = 0, int nrec = 0);
		static LuaRef get_global_var(lua_State * L, const char * name);


		string to_string() const;
		void print() const;
		
		lua_State * state() const;
		void push() const;
		void pop();
		int type() const;

		bool is_nil() const;
		bool is_number() const;
		bool is_string() const;
		bool is_table() const;
		bool is_function() const;
		bool is_userdata() const;
		bool is_thread() const;
		bool is_lightuserdata() const;
		bool is_bool() const;
		bool is_cfunction() const;

		template <typename T>
		T cast() const
		{
			LogTraceStepCall("T LuaRef::cast<T>() const");
			LuaStateProtect lsp(m_L);
			push();
			T t(LuaStack<const T>::get(m_L, -1));
			lsp.reset();
			LogTraceStepReturn(t);
		}
		template <typename T>
		inline operator T () const
		{ LogTraceStepCall("LuaRef::operator T<T> () const"); LogTraceStepReturn(cast<T>()); }


		template <typename T>
		bool operator == (const T & right) const
		{
			LogTraceStepCall("bool LuaRef::operator ==<T> (const T & right) const");
			LuaStateProtect lsp(m_L);
			push();
			LuaStack<const T>::push(m_L, right);
			bool eq = lua_equal(m_L, -1, -2) == 1;
			lsp.reset();
			LogTraceStepReturn(eq);
		}
		template <typename T>
		inline bool operator != (const T & right) const
		{
			LogTraceStepCall("bool LuaRef::operator !=<T> (const T & right) const");
			LogTraceStepReturn(!((*this) == right));
		}
		template <typename T>
		bool operator < (const T & right) const
		{
			LogTraceStepCall("bool LuaRef::operator <<T> (const T & right) const");
			LuaStateProtect lsp(m_L);
			push();
			LuaStack<const T>::push(m_L, right);
			bool lt = lua_lessthan(m_L, -2, -1) == 1;
			lsp.reset();
			LogTraceStepReturn(lt);
		}
		template <typename T>
		bool operator <= (const T & right) const
		{
			LogTraceStepCall("bool LuaRef::operator <=<T> (const T & right) const");
			LuaStateProtect lsp(m_L);
			push();
			LuaStack<const T>::push(m_L, right);
			bool elt = lua_equal(m_L, -2, -1) == 1 || lua_lessthan(m_L, -2, -1) == 1;
			lsp.reset();
			LogTraceStepReturn(elt);
		}
		template <typename T>
		bool operator > (const T & right) const
		{
			LogTraceStepCall("bool LuaRef::operator ><T> (const T & right) const");
			LuaStateProtect lsp(m_L);
			push();
			LuaStack<const T>::push(m_L, right);
			bool mt = lua_lessthan(m_L, -1, -2) == 1;
			lsp.reset();
			LogTraceStepReturn(mt);
		}
		template <typename T>
		bool operator >= (const T & right) const
		{
			LogTraceStepCall("bool LuaRef::operator >=<T> (const T & right) const");
			LuaStateProtect lsp(m_L);
			push();
			LuaStack<const T>::push(m_L, right);
			bool emt = lua_equal(m_L, -1, -2) == 1 || lua_lessthan(m_L, -1, -2) == 1;
			lsp.reset();
			LogTraceStepReturn(emt);
		}
		template <typename T>
		bool rawequal(const T & right) const
		{
			LogTraceStepCall("bool LuaRef::rawequal<T>(const T & right) const");
			LuaStateProtect lsp(m_L);
			push();
			LuaStack<const T>::push(m_L, right);
			bool req = lua_rawequal(m_L, -1, -2) == 1;
			lsp.reset();
			LogTraceStepReturn(req);
		}


		template <typename T>
		void append(const T & v) const
		{
			LogTraceStepCall("LuaRef::append<T>(const T & v) const");
			LuaStateProtect lsp(m_L);
			push();
			LuaStack<const T>::push(m_L, v);
			luaL_ref(m_L, -2);
			lsp.reset();
			LogTraceStepReturnVoid;
		}

		size_t length() const;

		template <typename T>
		LuaRef rawget(const T & key) const
		{
			LogTraceStepCall("LuaRef LuaRef::rawget<T>(const T & key) const");
			LuaStateProtect lsp(m_L);
			push();
			LuaStack<const T>::push(m_L, key);
			lua_rawget(m_L, -2);
			LuaRef t(m_L, true);
			lsp.reset();
			LogTraceStepReturn(t);
		}

		template <typename T>
		LuaTableRef operator[] (const T & key) const
		{
			LogTraceStepCall("LuaTableRef LuaRef::operator [] (const T & key) const");
			LuaStack<const T>::push(m_L, key);
			LogTraceStepReturn(LuaTableRef(*this));
		}
		const LuaRef operator () () const;
/*
--lua生成
function create_traits(count)

	local str = ""

	for k = 1, count do
		local arg1, arg2, arg3 = "", "", ""
		for ki = 1, k do
			arg1 = string.format("%stypename Arg%d, ", arg1, ki)
			arg2 = string.format("%sconst Arg%d & arg%d, ", arg2, ki, ki)
			arg3 = string.format("%s\t\t\tLuaStack<const Arg%d>::push(m_L, arg%d);\n", arg3, ki, ki)
		end
		arg1 = string.sub(arg1, 1, #arg1-2)
		arg2 = string.sub(arg2, 1, #arg2-2)


		local temp = {}
		table.insert(temp, "\t\ttemplate <")
		table.insert(temp, arg1)
		table.insert(temp, ">\n\t\tconst LuaRef operator()(")
		table.insert(temp, arg2)
		table.insert(temp, ") const\n\t\t{\n\t\t\tLogTraceStepCall(\"const LuaRef ")
		table.insert(temp, "LuaRef::operator()(")
		table.insert(temp, arg2)
		table.insert(temp, ") const\");\n\t\t\tpush();\n")
		table.insert(temp, arg3)
		table.insert(temp, string.format("\t\t\tif (lua_pcall(m_L, %d, 1, 0))\n\t\t\t{\n", k))
		table.insert(temp, "\t\t\t\tconst char * errStr = lua_tostring(m_L, -1);\n\t\t\t\tif (errStr) LogScriptError(errStr);\n")
		table.insert(temp, "\t\t\t\tLogTraceStepReturn(LuaRef(m_L));\n\t\t\t}\n\t\t\tLogTraceStepReturn(LuaRef(m_L, true));\n\t\t}\n\n")

		str = str .. table.concat(temp)
	end

	print(str)

end

create_traits(20)
*/		
		template <typename Arg1>
		const LuaRef operator()(const Arg1 & arg1) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			if (lua_pcall(m_L, 1, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			if (lua_pcall(m_L, 2, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			if (lua_pcall(m_L, 3, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			if (lua_pcall(m_L, 4, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			if (lua_pcall(m_L, 5, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			LuaStack<const Arg6>::push(m_L, arg6);
			if (lua_pcall(m_L, 6, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			LuaStack<const Arg6>::push(m_L, arg6);
			LuaStack<const Arg7>::push(m_L, arg7);
			if (lua_pcall(m_L, 7, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			LuaStack<const Arg6>::push(m_L, arg6);
			LuaStack<const Arg7>::push(m_L, arg7);
			LuaStack<const Arg8>::push(m_L, arg8);
			if (lua_pcall(m_L, 8, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			LuaStack<const Arg6>::push(m_L, arg6);
			LuaStack<const Arg7>::push(m_L, arg7);
			LuaStack<const Arg8>::push(m_L, arg8);
			LuaStack<const Arg9>::push(m_L, arg9);
			if (lua_pcall(m_L, 9, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			LuaStack<const Arg6>::push(m_L, arg6);
			LuaStack<const Arg7>::push(m_L, arg7);
			LuaStack<const Arg8>::push(m_L, arg8);
			LuaStack<const Arg9>::push(m_L, arg9);
			LuaStack<const Arg10>::push(m_L, arg10);
			if (lua_pcall(m_L, 10, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			LuaStack<const Arg6>::push(m_L, arg6);
			LuaStack<const Arg7>::push(m_L, arg7);
			LuaStack<const Arg8>::push(m_L, arg8);
			LuaStack<const Arg9>::push(m_L, arg9);
			LuaStack<const Arg10>::push(m_L, arg10);
			LuaStack<const Arg11>::push(m_L, arg11);
			if (lua_pcall(m_L, 11, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			LuaStack<const Arg6>::push(m_L, arg6);
			LuaStack<const Arg7>::push(m_L, arg7);
			LuaStack<const Arg8>::push(m_L, arg8);
			LuaStack<const Arg9>::push(m_L, arg9);
			LuaStack<const Arg10>::push(m_L, arg10);
			LuaStack<const Arg11>::push(m_L, arg11);
			LuaStack<const Arg12>::push(m_L, arg12);
			if (lua_pcall(m_L, 12, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			LuaStack<const Arg6>::push(m_L, arg6);
			LuaStack<const Arg7>::push(m_L, arg7);
			LuaStack<const Arg8>::push(m_L, arg8);
			LuaStack<const Arg9>::push(m_L, arg9);
			LuaStack<const Arg10>::push(m_L, arg10);
			LuaStack<const Arg11>::push(m_L, arg11);
			LuaStack<const Arg12>::push(m_L, arg12);
			LuaStack<const Arg13>::push(m_L, arg13);
			if (lua_pcall(m_L, 13, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			LuaStack<const Arg6>::push(m_L, arg6);
			LuaStack<const Arg7>::push(m_L, arg7);
			LuaStack<const Arg8>::push(m_L, arg8);
			LuaStack<const Arg9>::push(m_L, arg9);
			LuaStack<const Arg10>::push(m_L, arg10);
			LuaStack<const Arg11>::push(m_L, arg11);
			LuaStack<const Arg12>::push(m_L, arg12);
			LuaStack<const Arg13>::push(m_L, arg13);
			LuaStack<const Arg14>::push(m_L, arg14);
			if (lua_pcall(m_L, 14, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			LuaStack<const Arg6>::push(m_L, arg6);
			LuaStack<const Arg7>::push(m_L, arg7);
			LuaStack<const Arg8>::push(m_L, arg8);
			LuaStack<const Arg9>::push(m_L, arg9);
			LuaStack<const Arg10>::push(m_L, arg10);
			LuaStack<const Arg11>::push(m_L, arg11);
			LuaStack<const Arg12>::push(m_L, arg12);
			LuaStack<const Arg13>::push(m_L, arg13);
			LuaStack<const Arg14>::push(m_L, arg14);
			LuaStack<const Arg15>::push(m_L, arg15);
			if (lua_pcall(m_L, 15, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			LuaStack<const Arg6>::push(m_L, arg6);
			LuaStack<const Arg7>::push(m_L, arg7);
			LuaStack<const Arg8>::push(m_L, arg8);
			LuaStack<const Arg9>::push(m_L, arg9);
			LuaStack<const Arg10>::push(m_L, arg10);
			LuaStack<const Arg11>::push(m_L, arg11);
			LuaStack<const Arg12>::push(m_L, arg12);
			LuaStack<const Arg13>::push(m_L, arg13);
			LuaStack<const Arg14>::push(m_L, arg14);
			LuaStack<const Arg15>::push(m_L, arg15);
			LuaStack<const Arg16>::push(m_L, arg16);
			if (lua_pcall(m_L, 16, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			LuaStack<const Arg6>::push(m_L, arg6);
			LuaStack<const Arg7>::push(m_L, arg7);
			LuaStack<const Arg8>::push(m_L, arg8);
			LuaStack<const Arg9>::push(m_L, arg9);
			LuaStack<const Arg10>::push(m_L, arg10);
			LuaStack<const Arg11>::push(m_L, arg11);
			LuaStack<const Arg12>::push(m_L, arg12);
			LuaStack<const Arg13>::push(m_L, arg13);
			LuaStack<const Arg14>::push(m_L, arg14);
			LuaStack<const Arg15>::push(m_L, arg15);
			LuaStack<const Arg16>::push(m_L, arg16);
			LuaStack<const Arg17>::push(m_L, arg17);
			if (lua_pcall(m_L, 17, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17, const Arg18 & arg18) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17, const Arg18 & arg18) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			LuaStack<const Arg6>::push(m_L, arg6);
			LuaStack<const Arg7>::push(m_L, arg7);
			LuaStack<const Arg8>::push(m_L, arg8);
			LuaStack<const Arg9>::push(m_L, arg9);
			LuaStack<const Arg10>::push(m_L, arg10);
			LuaStack<const Arg11>::push(m_L, arg11);
			LuaStack<const Arg12>::push(m_L, arg12);
			LuaStack<const Arg13>::push(m_L, arg13);
			LuaStack<const Arg14>::push(m_L, arg14);
			LuaStack<const Arg15>::push(m_L, arg15);
			LuaStack<const Arg16>::push(m_L, arg16);
			LuaStack<const Arg17>::push(m_L, arg17);
			LuaStack<const Arg18>::push(m_L, arg18);
			if (lua_pcall(m_L, 18, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17, const Arg18 & arg18, const Arg19 & arg19) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17, const Arg18 & arg18, const Arg19 & arg19) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			LuaStack<const Arg6>::push(m_L, arg6);
			LuaStack<const Arg7>::push(m_L, arg7);
			LuaStack<const Arg8>::push(m_L, arg8);
			LuaStack<const Arg9>::push(m_L, arg9);
			LuaStack<const Arg10>::push(m_L, arg10);
			LuaStack<const Arg11>::push(m_L, arg11);
			LuaStack<const Arg12>::push(m_L, arg12);
			LuaStack<const Arg13>::push(m_L, arg13);
			LuaStack<const Arg14>::push(m_L, arg14);
			LuaStack<const Arg15>::push(m_L, arg15);
			LuaStack<const Arg16>::push(m_L, arg16);
			LuaStack<const Arg17>::push(m_L, arg17);
			LuaStack<const Arg18>::push(m_L, arg18);
			LuaStack<const Arg19>::push(m_L, arg19);
			if (lua_pcall(m_L, 19, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
		const LuaRef operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17, const Arg18 & arg18, const Arg19 & arg19, const Arg20 & arg20) const
		{
			LogTraceStepCall("const LuaRef LuaRef::operator()(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, const Arg6 & arg6, const Arg7 & arg7, const Arg8 & arg8, const Arg9 & arg9, const Arg10 & arg10, const Arg11 & arg11, const Arg12 & arg12, const Arg13 & arg13, const Arg14 & arg14, const Arg15 & arg15, const Arg16 & arg16, const Arg17 & arg17, const Arg18 & arg18, const Arg19 & arg19, const Arg20 & arg20) const");
			push();
			LuaStack<const Arg1>::push(m_L, arg1);
			LuaStack<const Arg2>::push(m_L, arg2);
			LuaStack<const Arg3>::push(m_L, arg3);
			LuaStack<const Arg4>::push(m_L, arg4);
			LuaStack<const Arg5>::push(m_L, arg5);
			LuaStack<const Arg6>::push(m_L, arg6);
			LuaStack<const Arg7>::push(m_L, arg7);
			LuaStack<const Arg8>::push(m_L, arg8);
			LuaStack<const Arg9>::push(m_L, arg9);
			LuaStack<const Arg10>::push(m_L, arg10);
			LuaStack<const Arg11>::push(m_L, arg11);
			LuaStack<const Arg12>::push(m_L, arg12);
			LuaStack<const Arg13>::push(m_L, arg13);
			LuaStack<const Arg14>::push(m_L, arg14);
			LuaStack<const Arg15>::push(m_L, arg15);
			LuaStack<const Arg16>::push(m_L, arg16);
			LuaStack<const Arg17>::push(m_L, arg17);
			LuaStack<const Arg18>::push(m_L, arg18);
			LuaStack<const Arg19>::push(m_L, arg19);
			LuaStack<const Arg20>::push(m_L, arg20);
			if (lua_pcall(m_L, 20, 1, 0))
			{
				const char * errStr = lua_tostring(m_L, -1);
				if (errStr) LogScriptError(errStr);
				LogTraceStepReturn(LuaRef(m_L));
			}
			LogTraceStepReturn(LuaRef(m_L, true));
		}

	private:
		lua_State * m_L;
		int m_ref;

	};


	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const LuaRef>
	{
		static inline void push(lua_State * L, const LuaRef & v)
		{
			LogTraceStepCall("LuaStack<const LuaRef>::push(lua_State * L, LuaRef & v)");
			if (L != v.state())
			{
				LogScriptErrorJump(L, "Error!!!The LuaStack<LuaRef>::push vm is not LuaRef vm!!!");
			}
			v.push();
			LogTraceStepReturnVoid;
		}
		
		static inline LuaRef get(lua_State * L, int idx)
		{
			LogTraceStepCall("LuaRef LuaStack<const LuaRef>::get(lua_State * L, int idx)");
			LogTraceStepReturn(LuaRef(L, idx));
		}
	};

	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const LuaRef &>
	{
		static inline void push(lua_State * L, const LuaRef & v)
		{
			LogTraceStepCall("LuaStack<const LuaRef &>::push(lua_State * L, LuaRef & v)");
			if (L != v.state())
			{
				LogScriptErrorJump(L, "Error!!!The LuaStack<LuaRef>::push vm is not LuaRef vm!!!");
			}
			v.push();
			LogTraceStepReturnVoid;
		}

		static inline LuaRef get(lua_State * L, int idx)
		{
			LogTraceStepCall("LuaRef LuaStack<const LuaRef&>::get(lua_State * L, int idx)");
			LogTraceStepReturn(LuaRef(L, idx));
		}
	};

}

#endif