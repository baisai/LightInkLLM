


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

#ifndef LIGHTINK_LUAENGINE_LUACLASSMGR_H_
#define LIGHTINK_LUAENGINE_LUACLASSMGR_H_

#include "Common/Type.h"
#include <unordered_map>
#include "LuaEngine/LuaLib.h"

namespace LightInk
{
	struct CharPtrBridge;
	class LIGHTINK_DECL LuaClassMgr
	{
	public:
		LuaClassMgr();
		~LuaClassMgr();

		void register_class(const void * key);
		bool is_registered(const void * key);

		bool add_base_class(const void * key, const void * base, int32 offset);

		int32 get_class_offset(const void * fromKey, const void * toKey);

		//LuaClassMgr in lua key
		inline static void * get_key() {  return (void *)&InvalidOffset;}
		//Object in lua Class table key
		inline static void * get_object_key() {  return (void *)&ObjectKey;}
		//Class key in lua Class table key
		inline static void * get_class_key() {  return (void *)&ClassKey;}
		//weak object table key
		inline static void * get_weak_key() { return (void *)&WeakKey; }

		static LuaClassMgr * get_class_mgr(lua_State * L);
		static void release_class_mgr(lua_State * L);
		static int32 calc_offset(lua_State * L, int fromIdx, int toIdx);

		static void create_keep_field(lua_State * L);
		static bool check_keep_field_str(lua_State * L, CharPtrBridge cpb);
		static bool check_keep_field_stack(lua_State * L, int fieldIdx);

	private:
		struct LuaClassInfo
		{
			LuaClassInfo(const void * key, int32 offset, bool isBase) : m_key(key), m_offset(offset), m_isBase(isBase) {  }
			const void * m_key;
			int32 m_offset;
			bool m_isBase;
		};
		class LuaClass
		{
		public:
			bool set_class_offset(const void * key, int32 offset, bool isBase);
			int32 get_class_offset(const void * key);
			inline const vector<LuaClassInfo>::type & get_link_class() const { return m_linkClass; }
		private:
			vector<LuaClassInfo>::type m_linkClass;
		};	
	private:
		std::unordered_map<const void *, LuaClass *> m_classTable;
	public:
		static const int32 InvalidOffset;
		static const char ObjectKey;
		static const char ClassKey;
		static const char WeakKey;
	};

}

#endif