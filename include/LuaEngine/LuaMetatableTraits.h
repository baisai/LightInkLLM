


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
#ifndef LIGHTINK_LUAENGINE_LUAMETATABLETRAITS_H_
#define LIGHTINK_LUAENGINE_LUAMETATABLETRAITS_H_

#include "Common/Type.h"
#include "Log/Log.h"
#include "LuaEngine/LuaClassInfo.h"
#include "LuaEngine/LuaUserdataForClass.h"
#include "LuaEngine/LuaFixFunction.h"
#include "LuaEngine/LuaMetatable.h"

namespace LightInk
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//LuaMetatableTraits
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 生成一些元方法的实现类
	template<typename ClassType>
	struct LuaMetatableTraits
	{
		static LuaUserdataForClass * userdata_to_imp(lua_State * L, int idx)
		{
			LogTraceStepCall("LuaUserdataForClass * LuaMetatableTraits<ClassType>::userdata_to_imp(lua_State * L, int idx)");
			LuaUserdataForClass * p = LuaMetatable::get_userdata(L, idx);
			if (p == NULL)
			{
				//LogScriptWarning("Warning!!!The arg {} is a ({}) object, but not found userdata!!!", idx, LuaClassInfo<ClassType>::get_class_name());
				LogTraceStepReturn(NULL);
			}
			int32 offset = LuaClassInfo<ClassType>::calc_from_offset(L, idx);
			if (offset == LuaClassMgr::InvalidOffset)
			{
				LogScriptError(L, "Error!!!The arg {} is a object, but it is not link {}!!!", idx, LuaClassInfo<ClassType>::get_class_name());
				LogTraceStepReturn(NULL);
			}
			p->set_class_offset(offset);
			LogTraceStepReturn(p);
		}

		static ClassType * userdata_to_object(lua_State* L, int idx)
		{
			LogTraceStepCall("ClassType * LuaMetatableTraits<ClassType>::userdata_to_object(lua_State * L, int idx)");
			LuaUserdataForClass * p = userdata_to_imp(L, idx);
			if (p) { LogTraceStepReturn(p->get_object_t<ClassType>()); }
			LogTraceStepReturn(NULL);
		}

		static ClassType * userdata_to_object_move(lua_State* L, int idx)
		{
			LogTraceStepCall("ClassType * LuaMetatableTraits<ClassType>::userdata_to_object_move(lua_State* L, int idx)");
			LuaUserdataForClass * p = userdata_to_imp(L, idx);
			if (p) { LogTraceStepReturn(p->move_object_t<ClassType>()); }
			LogTraceStepReturn(NULL);
		}

		static int mt_is_destroy_function(lua_State * L)
		{
			LogTraceStepCall("int LuaMetatableTraits<ClassType>::mt_is_destroy_function(lua_State * L)");
			LuaUserdataForClass * p = LuaMetatable::get_userdata(L, 1);
			if (p && p->get_object()) { lua_pushboolean(L, 0); }
			else { lua_pushboolean(L, 1); }
			LogTraceStepReturn(1);
		}

		static int mt_destroy_function(lua_State * L)
		{
			LogTraceStepCall("int LuaMetatableTraits<ClassType>::mt_destroy_function(lua_State * L)");
			LuaUserdataForClass * p = LuaMetatable::get_userdata(L, 1);
			if (p) { p->user_gc(); }
			LogTraceStepReturn(0);
		}

		static int mt_force_destroy_function(lua_State * L)
		{
			LogTraceStepCall("int LuaMetatableTraits<ClassType>::mt_force_destroy_function(lua_State * L)");
			LuaUserdataForClass * p = LuaMetatable::get_userdata(L, 1);
			if (p) { p->force_gc(); }
			LogTraceStepReturn(0);
		}

		static int mt_is_auto_gc_funtcion(lua_State * L)
		{
			LogTraceStepCall("int LuaMetatableTraits<ClassType>::mt_set_auto_gc_funtcion(lua_State * L)");
			int autoGC = 0;
			LuaUserdataForClass * p = LuaMetatable::get_userdata(L, 1);
			if (p) { autoGC = p->is_auto_gc() ? 1 : 0; }
			lua_pushboolean(L, autoGC);
			LogTraceStepReturn(1);
		}

		static int mt_create___function(lua_State * L)
		{
			LogTraceStepCall("int LuaMetatableTraits<ClassType>::mt_create___function(lua_State * L)");
			if (LuaMetatable::mt_new___function(L) == 1)
			{
				LuaUserdataForClass * p = LuaMetatable::get_userdata(L, -1);
				if (p) { p->set_atuto_gc(false); }
				LogTraceStepReturn(1);
			}
			LogScriptError(L, "Call LuaMetatable::mt_new___function Error!!! create failed!!!");
			LogTraceStepReturn(0);
		}

		static int mt_set_auto_gc_funtcion(lua_State * L)
		{
			LogTraceStepCall("int LuaMetatableTraits<ClassType>::mt_set_auto_gc_funtcion(lua_State * L)");
			LuaUserdataForClass * p = userdata_to_imp(L, 1);
			if (p)
			{
				bool autoGC = lua_toboolean(L, 2);
				p->set_atuto_gc(autoGC);
			}
			LogTraceStepReturn(0);
		}
	};
}


#endif