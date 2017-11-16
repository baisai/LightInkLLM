


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

#ifndef LIGHTINK_LUAENGINE_LUAINT64_H_
#define LIGHTINK_LUAENGINE_LUAINT64_H_

#include "Common/Type.h"
#include "LuaEngine/LuaLib.h"
#include "Common/CharPtrBridge.h"

namespace LightInk
{
	class LuaUint64;
	class LIGHTINK_DECL LuaInt64
	{
	public:
		LuaInt64(lua_Number number);
		LuaInt64(int64 number);

		static LuaInt64 new_from_string(const CharPtrBridge & number);
		static LuaInt64 new_from_st(const LuaInt64 & number);

		const  string to_string();
		lua_Number to_number();
		LuaUint64 to_uint64();

		void set_number(lua_Number number);
		void set_string(const CharPtrBridge & number);
		void set_st(const LuaInt64 & number);

		LuaInt64 add_number(lua_Number number);
		LuaInt64 sub_number(lua_Number number);
		LuaInt64 mul_number(lua_Number number);
		LuaInt64 div_number(lua_Number number);
		LuaInt64 mod_number(lua_Number number);
		LuaInt64 pow_number(lua_Number number);
		LuaInt64 unm_number();
		bool eq_number(lua_Number number);
		bool lt_number(lua_Number number);
		bool le_number(lua_Number number);


		LuaInt64 add_st(const LuaInt64 & number);
		LuaInt64 sub_st(const LuaInt64 & number);
		LuaInt64 mul_st(const LuaInt64 & number);
		LuaInt64 div_st(const LuaInt64 & number);
		LuaInt64 mod_st(const LuaInt64 & number);
		LuaInt64 pow_st(const LuaInt64 & number);
		bool eq_st(const LuaInt64 & number);
		bool lt_st(const LuaInt64 & number);
		bool le_st(const LuaInt64 & number);

		void add_set_number(lua_Number number);
		void sub_set_number(lua_Number number);
		void mul_set_number(lua_Number number);
		void div_set_number(lua_Number number);
		void mod_set_number(lua_Number number);
		void pow_set_number(lua_Number number);
		void unm_set_number();
		void add_set_st(const LuaInt64 & number);
		void sub_set_st(const LuaInt64 & number);
		void mul_set_st(const LuaInt64 & number);
		void div_set_st(const LuaInt64 & number);
		void mod_set_st(const LuaInt64 & number);
		void pow_set_st(const LuaInt64 & number);

		int64 m_number;
	};

	class LIGHTINK_DECL LuaUint64
	{
	public:
		LuaUint64(lua_Number number);
		LuaUint64(uint64 number);

		static LuaUint64 new_from_string(const CharPtrBridge & number);
		static LuaUint64 new_from_st(const LuaUint64 & number);

		const string to_string();
		lua_Number to_number();
		LuaInt64 to_int64();

		void set_number(lua_Number number);
		void set_string(const CharPtrBridge & number);
		void set_st(const LuaUint64 & number);

		LuaUint64 add_number(lua_Number number);
		LuaUint64 sub_number(lua_Number number);
		LuaUint64 mul_number(lua_Number number);
		LuaUint64 div_number(lua_Number number);
		LuaUint64 mod_number(lua_Number number);
		LuaUint64 pow_number(lua_Number number);
		LuaInt64 unm_number();
		bool eq_number(lua_Number number);
		bool lt_number(lua_Number number);
		bool le_number(lua_Number number);


		LuaUint64 add_st(const LuaUint64 & number);
		LuaUint64 sub_st(const LuaUint64 & number);
		LuaUint64 mul_st(const LuaUint64 & number);
		LuaUint64 div_st(const LuaUint64 & number);
		LuaUint64 mod_st(const LuaUint64 & number);
		LuaUint64 pow_st(const LuaUint64 & number);
		bool eq_st(const LuaUint64 & number);
		bool lt_st(const LuaUint64 & number);
		bool le_st(const LuaUint64 & number);

		void add_set_number(lua_Number number);
		void sub_set_number(lua_Number number);
		void mul_set_number(lua_Number number);
		void div_set_number(lua_Number number);
		void mod_set_number(lua_Number number);
		void pow_set_number(lua_Number number);
		void unm_set_number();
		void add_set_st(const LuaUint64 & number);
		void sub_set_st(const LuaUint64 & number);
		void mul_set_st(const LuaUint64 & number);
		void div_set_st(const LuaUint64 & number);
		void mod_set_st(const LuaUint64 & number);
		void pow_set_st(const LuaUint64 & number);

		uint64 m_number;
	};


}


#endif


