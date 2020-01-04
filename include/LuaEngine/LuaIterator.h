


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

#ifndef LIGHTINK_LUAENGINE_LUAITERATOR_H_
#define LIGHTINK_LUAENGINE_LUAITERATOR_H_

#include "Common/Type.h"
#include "LuaEngine/LuaLib.h"
#include "LuaEngine/LuaRef.h"

namespace LightInk
{
	class LIGHTINK_DECL LuaIterator
	{
	public:
		LuaIterator(lua_State * L, int idx);

		void set_over();
		lua_State * state();
		bool is_over();
		
		template <typename T>
		inline T get_key()
		{ LogTraceStepCall("T LuaIterator::get_key<T>()"); LogTraceStepReturn(LuaStack<const T>::get(m_L, m_keyIdx)); }
		template <typename T>
		inline T get_value()
		{ LogTraceStepCall("T LuaIterator::get_value<T>()"); LogTraceStepReturn(LuaStack<const T>::get(m_L, m_keyIdx + 1)); }
		template <typename T>
		inline T operator * ()
		{ LogTraceStepCall("T LuaIterator::operator *<T> ()"); LogTraceStepReturn(LuaStack<const T>::get(m_L, m_keyIdx + 1)); }
		template <typename T>
		inline T operator -> ()
		{ LogTraceStepCall("T LuaIterator::operator -><T> ()"); LogTraceStepReturn(LuaStack<const T>::get(m_L, m_keyIdx + 1)); }

		inline int key_type() { LogTraceStepCall("int LuaIterator::key_type()"); LogTraceStepReturn(lua_type(m_L, m_keyIdx)); }
		inline int value_type() { LogTraceStepCall("int LuaIterator::value_type()"); LogTraceStepReturn(lua_type(m_L, m_keyIdx + 1)); }

		inline void copy_key() { LogTraceStepCall("void LuaIterator::copy_key()"); lua_pushvalue(m_L, m_keyIdx); ++m_copyCount; LogTraceStepReturnVoid; }
		inline void copy_value() { LogTraceStepCall("void LuaIterator::copy_value()"); lua_pushvalue(m_L, m_keyIdx + 1); ++m_copyCount; LogTraceStepReturnVoid; }
		inline void pop_copy() { LogTraceStepCall("void LuaIterator::pop_copy()"); lua_pop(m_L, m_copyCount); m_copyCount = 0; LogTraceStepReturnVoid; }
		inline void clean_stack() { LogTraceStepCall("void LuaIterator::clean_stack()"); lua_settop(m_L, m_keyIdx + 1); m_copyCount = 0; LogTraceStepReturnVoid; }

		LuaIterator & operator ++ ();
		LuaIterator & operator ++ (int);

	private:
		void next();

	private:
		lua_State * m_L;
		int m_top;
		bool m_over;
		int m_keyIdx;
		int m_copyCount;
	};


	class LIGHTINK_DECL LuaRefIterator
	{
	public:
		explicit LuaRefIterator(const LuaRef & table);

		lua_State * state();
		bool is_over();

		LuaRef & get_key();

		LuaRef & get_value();

		LuaRef & operator * ();

		LuaRef & operator -> ();

		LuaRefIterator & operator ++ ();

		LuaRefIterator & operator ++ (int);

	private:
		void next();
	private:
		lua_State * m_L;
		LuaRef m_table;
		LuaRef m_key;
		LuaRef m_value;
	};


}


#endif


