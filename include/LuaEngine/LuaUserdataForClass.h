


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
	class GCCallback
	{
	public:
		virtual ~GCCallback() {  }
		virtual void call(void *, void *) const = 0;
	};
	template <typename T>
	class GCCallbackT : public GCCallback
	{ 
	public:
		typedef void (* CallbackType)(T *, void *);

		GCCallbackT() : m_call(gc_delete) {  }
		GCCallbackT(CallbackType call) : m_call(call) { if (!m_call) m_call = gc_delete; }
		virtual ~GCCallbackT() {  }

		inline virtual void call(void * obj, void * context) const
		{ T * t = (T *)obj; m_call(t, context); }

	private:
		inline static void gc_delete(T * obj, void * context) { delete obj; }
	private:
		CallbackType m_call;
	};

	class LuaUserdataForClass
	{
	public:
		template <typename T>
		LuaUserdataForClass(const T * p, int offset, bool autoGC) : 
			m_classOffset(0), 
			m_obj(((char *)p) + offset),
			m_autoGC(autoGC), 
			m_luaGC(new GCCallbackT<T>), 
			m_context(NULL)
		{
			LogTraceStepCall("LuaUserdataForClass::LuaUserdataForClass(T * p, int offset, bool luaGC)");
			LogTraceStepReturnVoid;
		}

		template <typename T>
		LuaUserdataForClass(const T * p, int offset, bool autoGC, typename GCCallbackT<T>::CallbackType luaGC, void * context) : 
			m_classOffset(0), 
			m_obj(((char *)p) + offset),
			m_autoGC(autoGC), 
			m_luaGC(new GCCallbackT<T>(luaGC)), 
			m_context(context)
		{
			LogTraceStepCall("LuaUserdataForClass::LuaUserdataForClass(const T * p, int offset, bool autoGC, typename GCCallbackT<T>::CallbackType luaGC, void * context)");
			LogTraceStepReturnVoid;
		}
		virtual ~LuaUserdataForClass()
		{
			LogTraceStepCall("LuaUserdataForClass::~LuaUserdataForClass()");
			if (m_obj && m_autoGC) { m_luaGC->call(m_obj, m_context); }
			delete m_luaGC;
			m_luaGC = NULL;
			m_obj = NULL;
			m_context = NULL;
			
			LogTraceStepReturnVoid;
		}

		inline void offset_object(int32 offset)
		{
			LogTraceStepCall("LuaUserdataForClass::offset_object(int32 offset)");
			m_obj = static_cast<char *>(m_obj) + offset;
			LogTraceStepReturnVoid;
		}

		inline void set_class_offset(int32 offset)
		{ 
			LogTraceStepCall("void LuaUserdataForClass::set_class_offset(int32 offset)");
			m_classOffset = offset;
			LogTraceStepReturnVoid;
		}

		template <typename T>
		inline T * get_object_t()
		{ 
			LogTraceStepCall("T * LuaUserdataForClass::get_object_t()");
			char * p = (char *)get_object() + m_classOffset;
			m_classOffset = 0;
			LogTraceStepReturn((T *)p);
		}

		template <typename T>
		inline T * move_object_t()
		{
			LogTraceStepCall("T * LuaUserdataForClass::move_object_t()");
			char * p = (char *)move_object() + m_classOffset;
			m_classOffset = 0;
			LogTraceStepReturn((T *)p);
		}

		virtual inline void user_gc() 
		{ 
			LogTraceStepCall("void LuaUserdataForClass::user_gc()");
			if (m_obj && !m_autoGC) { m_luaGC->call(m_obj, m_context); m_obj = NULL; m_context = NULL; }
			LogTraceStepReturnVoid;
		}
		virtual inline void force_gc()
		{
			LogTraceStepCall("void LuaUserdataForClass::force_gc()");
			if (m_obj) { m_luaGC->call(m_obj, m_context); m_obj = NULL; m_context = NULL; }
			LogTraceStepReturnVoid;
		}
		virtual inline void * get_object() const
		{ 
			LogTraceStepCall("void * LuaUserdataForClass::get_object() const");
			LogTraceStepReturn(m_obj);
		}
		virtual inline void * move_object()
		{
			LogTraceStepCall("void * LuaUserdataForClass::move_object()");
			void * t = m_obj; m_obj = NULL;
			LogTraceStepReturn(t);
		}
		virtual inline void * get_context() const
		{ 
			LogTraceStepCall("T * LuaUserdataForClass::get_context() const");
			LogTraceStepReturn(m_context);
		}

		inline bool is_auto_gc() const
		{
			LogTraceStepCall("bool LuaUserdataForClass::is_auto_gc() const");
			LogTraceStepReturn(m_autoGC);
		}

		inline void set_atuto_gc(bool autoGC)
		{
			LogTraceStepCall("bool LuaUserdataForClass::set_atuto_gc()");
			m_autoGC = autoGC;
			LogTraceStepReturnVoid;
		}

	private:
		int32 m_classOffset;
		void * m_obj;
		bool m_autoGC;
		GCCallback * m_luaGC;
		void * m_context;
	};

}


#endif