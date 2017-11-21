


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
#ifndef LIGHTINK_LUAENGINE_LUAUSERDATAFORCLASS_H_
#define LIGHTINK_LUAENGINE_LUAUSERDATAFORCLASS_H_

#include "Common/Type.h"
#include "Log/Log.h"

namespace LightInk
{
	template<typename T>
	class LuaUserdataForClass
	{
	public:
		typedef void (* GCCallback)(T *, void *);
		inline static void gc_delete(T * obj, void * context) { delete obj; }
		LuaUserdataForClass(const T * p, bool autoGC) : m_obj((T*)p), m_autoGC(autoGC), m_luaGC(gc_delete), m_context(NULL)
		{
			LogTraceStepCall("LuaUserdataForClass::LuaUserdataForClass(T * p, bool luaGC, GCCallback luaGC)");
			LogTraceStepReturnVoid;
		}
		LuaUserdataForClass(const T * p, bool autoGC, GCCallback luaGC, void * context) : m_obj((T*)p), m_autoGC(autoGC), m_luaGC(luaGC), m_context(context)
		{
			LogTraceStepCall("LuaUserdataForClass::LuaUserdataForClass(T * p, bool luaGC, GCCallback luaGC, void * context)");
			LogTraceStepReturnVoid;
		}
		~LuaUserdataForClass()
		{
			LogTraceStepCall("LuaUserdataForClass::~LuaUserdataForClass()");
			if (m_obj && m_autoGC) { m_luaGC(m_obj, m_context); }
			m_obj = NULL;
			m_context = NULL;
			LogTraceStepReturnVoid;
		}
		inline void user_gc() 
		{ 
			LogTraceStepCall("void LuaUserdataForClass::user_gc()");
			if (m_obj && !m_autoGC) { m_luaGC(m_obj, m_context); m_obj = NULL; m_context = NULL; }
			LogTraceStepReturnVoid;
		}
		inline void force_gc()
		{
			LogTraceStepCall("void LuaUserdataForClass::force_gc()");
			if (m_obj) { m_luaGC(m_obj, m_context); m_obj = NULL; m_context = NULL; }
			LogTraceStepReturnVoid;
		}
		inline T * get_object() 
		{ 
			LogTraceStepCall("T * LuaUserdataForClass::get_object() ");
			LogTraceStepReturn(m_obj);
		}
		inline T * move_object()
		{
			LogTraceStepCall("T * LuaUserdataForClass::move_object()");
			T * t = m_obj; m_obj = NULL;
			LogTraceStepReturn(t);
		}
		inline void * get_context()
		{ 
			LogTraceStepCall("T * LuaUserdataForClass::get_context() ");
			LogTraceStepReturn(m_context);
		}
	private:
		T * m_obj;
		bool m_autoGC;
		GCCallback m_luaGC;
		void * m_context;
	};

}


#endif