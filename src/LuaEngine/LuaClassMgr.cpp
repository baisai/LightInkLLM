


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

#include "Log/Log.h"
#include "Common/CharPtrBridge.h"
#include "LuaEngine/LuaStateProtect.h"
#include "LuaEngine/LuaFixFunction.h"
#include "LuaEngine/LuaClassMgr.h"

namespace LightInk
{
	LuaClassMgr::LuaClassMgr()
	{
		LogTraceStepCall("LuaClassMgr::LuaClassMgr()");
		LogTraceStepReturnVoid;
	}

	LuaClassMgr::~LuaClassMgr()
	{
		LogTraceStepCall("LuaClassMgr::~LuaClassMgr()");
		for (std::unordered_map<const void *, LuaClass *>::const_iterator iter = m_classTable.begin(); iter != m_classTable.end(); ++iter)
		{
			delete iter->second;
		}
		m_classTable.clear();
		LogTraceStepReturnVoid;
	}

	void LuaClassMgr::register_class(const void * key)
	{
		LogTraceStepCall("void LuaClassMgr::register_class(const void * key)");
		if (m_classTable.find(key) == m_classTable.end())
		{
			LuaClass * c = new LuaClass;
			c->set_class_offset(key, 0, false);
			m_classTable.insert(std::unordered_map<const void *, LuaClass *>::value_type(key, c));
		}
		LogTraceStepReturnVoid;
	}

	bool LuaClassMgr::is_registered(const void * key)
	{
		LogTraceStepCall("bool LuaClassMgr::is_registered(const void * key)");
		LogTraceStepReturn(m_classTable.find(key) != m_classTable.end());
	}

	bool LuaClassMgr::add_base_class(const void * key, const void * base, int32 offset)
	{
		LogTraceStepCall("bool LuaClassMgr::add_base_class(const void * key, const void * base, int32 offset)");
		std::unordered_map<const void *, LuaClass *>::iterator iter = m_classTable.find(key);
		if (iter == m_classTable.end())
		{
			LogError("the child class {} is not register", key);
			LogTraceStepReturn(false);
		}

		if (key == base)
			LogTraceStepReturn(iter->second->set_class_offset(key, 0, false));

		std::unordered_map<const void *, LuaClass *>::iterator baseIter = m_classTable.find(base);
		if (baseIter == m_classTable.end())
		{
			LogError("the base class {} is not register", base);
			LogTraceStepReturn(false);
		}
		iter->second->set_class_offset(base, offset, true);
		baseIter->second->set_class_offset(key, -offset, false);
		const vector<LuaClassInfo>::type & linkClass = baseIter->second->get_link_class();
		for (size_t idx = 0u; idx < linkClass.size(); ++idx)
		{
			if (linkClass[idx].m_isBase)
			{
				iter->second->set_class_offset(linkClass[idx].m_key, offset + linkClass[idx].m_offset, true);
				std::unordered_map<const void *, LuaClass *>::iterator baseBase = m_classTable.find(linkClass[idx].m_key);
				if (baseBase != m_classTable.end())
				{
					baseBase->second->set_class_offset(key, -offset - linkClass[idx].m_offset, false);
				}
			}
		}
		LogTraceStepReturn(true);
	}

	int32 LuaClassMgr::get_class_offset(const void * fromKey, const void * toKey)
	{
		LogTraceStepCall("int32 LuaClassMgr::get_class_offset(const void * fromKey, const void * toKey)");
		std::unordered_map<const void *, LuaClass *>::iterator iter = m_classTable.find(fromKey);
		if (iter == m_classTable.end())
		{
			LogError("the child class {} is not register", fromKey);
			LogTraceStepReturn(InvalidOffset);
		}
		LogTraceStepReturn(iter->second->get_class_offset(toKey));
	}

	LuaClassMgr * LuaClassMgr::get_class_mgr(lua_State * L)
	{
		LogTraceStepCall("LuaClassMgr * LuaClassMgr::get_class_mgr(lua_State * L)");
		LuaStateProtect lsp(L, true);
		lua_pushlightuserdata(L, LuaClassMgr::get_key());
		lua_rawget(L, LUA_REGISTRYINDEX);
		if (lua_islightuserdata(L, -1))
			LogTraceStepReturn((LuaClassMgr *)lua_touserdata(L, -1));
		LuaClassMgr * pMgr = new LuaClassMgr;
		lua_pushlightuserdata(L, LuaClassMgr::get_key());
		lua_pushlightuserdata(L, pMgr);
		lua_rawset(L, LUA_REGISTRYINDEX);
		create_keep_field(L);
		LogTraceStepReturn(pMgr);
	}

	void LuaClassMgr::release_class_mgr(lua_State * L)
	{
		LogTraceStepCall("void LuaClassMgr::release_class_mgr(lua_State * L)");
		LuaStateProtect lsp(L, true);
		lua_pushlightuserdata(L, LuaClassMgr::get_key());
		lua_rawget(L, LUA_REGISTRYINDEX);
		if (lua_islightuserdata(L, -1))
		{
			LuaClassMgr * pMgr = (LuaClassMgr *)lua_touserdata(L, -1);
			delete pMgr;
			lua_pushlightuserdata(L, LuaClassMgr::get_key());
			lua_pushnil(L);
			lua_rawset(L, LUA_REGISTRYINDEX);
		}
	}
	
	int32 LuaClassMgr::calc_offset(lua_State * L, int fromIdx, int toIdx)
	{
		LogTraceStepCall("int32 LuaClassMgr::calc_offset(lua_State * L, int fromIdx, int toIdx)");
		LuaStateProtect lsp(L, true);
		fromIdx = lua_absindex(L, fromIdx);
		toIdx = lua_absindex(L, toIdx);

		if (!lua_istable(L, fromIdx))
			LogTraceStepReturn(InvalidOffset);
		//lua_pushstring (L, "key__");
		lua_pushlightuserdata(L, LuaClassMgr::get_class_key());
		lua_gettable(L, fromIdx);
		void * fromKey = lua_touserdata(L, -1);

		if (!lua_istable(L, toIdx))
			LogTraceStepReturn(InvalidOffset);
		//lua_pushstring (L, "key__");
		lua_pushlightuserdata(L, LuaClassMgr::get_class_key());
		lua_gettable(L, toIdx);
		void * toKey = lua_touserdata(L, -1);

		int32 offset = get_class_mgr(L)->get_class_offset(fromKey, toKey);
		LogTraceStepReturn(offset);
	}

	void LuaClassMgr::create_keep_field(lua_State * L)
	{
		LogTraceStepCall("void LuaClassMgr::create_keep_field(lua_State * L)");
		static const char * keepField [] = {
			"type__", /*"key__", */"is_destroy__", "destroy__", "force_destroy__", "is_auto_gc__", 
			"new__", "newcpp__", "create__", "createcpp__", "disable_new__", "disable_create__", "ctor", "is_cppobj__",
			"__index", "__newindex",
			"__gc", "__mode", "__eq",
			"__add", "__sub", "__mul", "__div", "__mod",
			"__pow", "__unm", "__len", "__lt", "__le",
			"__concat", "__call", "__tostring", "__metatable"
		};
		LuaStateProtect lsp(L, true);
		rawgetfield(L, LUA_GLOBALSINDEX, "keep_field__");
		if (!lua_istable(L, -1))
		{
			lua_createtable(L, 0, sizeof(keepField) / sizeof(keepField[0]));
			lua_pushvalue(L, -1);
			rawsetfield(L, LUA_GLOBALSINDEX, "keep_field__");
		}
		for (unsigned idx = 0; idx < sizeof(keepField) / sizeof(keepField[0]); ++idx)
		{
			lua_pushboolean(L, 1);
			rawsetfield(L, -2, keepField[idx]);
		}
		LogTraceStepReturnVoid;
	}

	bool LuaClassMgr::check_keep_field_str(lua_State * L, CharPtrBridge cpb)
	{
		LuaStateProtect lsp(L, true);
		lua_pushlstring(L, cpb.m_charPtr, cpb.m_len);
		return check_keep_field_stack(L, -1);
	}

	bool LuaClassMgr::check_keep_field_stack(lua_State * L, int fieldIdx)
	{
		fieldIdx = lua_absindex(L, fieldIdx);
		LuaStateProtect lsp(L, true);
		rawgetfield(L, LUA_GLOBALSINDEX, "keep_field__");
		if (!lua_istable(L, -1))
		{
			lua_pop(L, 1);
			create_keep_field(L);
			rawgetfield(L, LUA_GLOBALSINDEX, "keep_field__");
		}
		lua_pushvalue(L, fieldIdx);
		lua_rawget(L, -2);
		return lua_toboolean(L, -1) != 0;
	}

	bool LuaClassMgr::LuaClass::set_class_offset(const void * key, int32 offset, bool isBase)
	{
		LogTraceStepCall("bool LuaClassMgr::LuaClass::set_class_offset(const void * key, int32 offset, bool isBase)");
		for (size_t idx = 0u; idx < m_linkClass.size(); ++idx)
		{
			if (m_linkClass[idx].m_key == key)
				LogTraceStepReturn(false);
		}
		m_linkClass.push_back(LuaClassInfo(key, offset, isBase));
		LogTraceStepReturn(true);
	}

	int32 LuaClassMgr::LuaClass::get_class_offset(const void * key)
	{
		LogTraceStepCall("int32 LuaClassMgr::LuaClass::get_class_offset(const void * key)");
		for (size_t idx = 0u; idx < m_linkClass.size(); ++idx)
		{
			if (m_linkClass[idx].m_key == key)
				LogTraceStepReturn(m_linkClass[idx].m_offset);
		}
		LogTraceStepReturn(InvalidOffset);
	}

	const int32 LuaClassMgr::InvalidOffset = INT32_MAX;
	const char LuaClassMgr::ObjectKey = 0;
	const char LuaClassMgr::ClassKey = 0;
	const char LuaClassMgr::WeakKey = 0;

}
