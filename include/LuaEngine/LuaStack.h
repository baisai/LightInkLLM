


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

#ifndef LIGHTINK_LUAENGINE_LUASTACK_H_
#define LIGHTINK_LUAENGINE_LUASTACK_H_

#include "Common/Type.h"
#include "Log/Log.h"
#include "LuaEngine/LuaUserdata.h"
#include "Common/STLType.h"
#include "Common/RuntimeError.h"
#include "LuaEngine/LuaFixFunction.h"
#include "Common/CharPtrBridge.h"
#include "Common/TypeTool/TypeTool.h"
#include "Common/Ptr/SharedPtr.h"


#define LightInkLuaEnumType(ET) \
	template <> \
	struct LIGHTINK_TEMPLATE_DECL LuaStack <ET> \
	{ \
		static inline void push(lua_State * L, ET e) \
		{ \
			LogTraceStepCall("void LuaStack<int>::push(lua_State * L, " #ET " e)"); \
			lua_pushinteger(L, static_cast <lua_Integer> (e)); \
			LogTraceStepReturnVoid; \
		} \
		static inline ET get(lua_State * L, int idx) \
		{ \
			LogTraceStepCall("int LuaStack<" #ET ">::get(lua_State * L, int idx)"); \
			LogTraceStepReturn(static_cast<ET>(luaL_checkinteger(L, idx))); \
		} \
	}; \
	template <> \
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const ET> \
	{ \
		static inline void push(lua_State * L, const ET e) \
		{ \
			LogTraceStepCall("void LuaStack<int>::push(lua_State * L, " #ET " e)"); \
			lua_pushinteger(L, static_cast <lua_Integer> (e)); \
			LogTraceStepReturnVoid; \
		} \
		static inline ET get(lua_State * L, int idx) \
		{ \
			LogTraceStepCall("int LuaStack<" #ET ">::get(lua_State * L, int idx)"); \
			LogTraceStepReturn(static_cast<ET>(luaL_checkinteger(L, idx))); \
		} \
	}; 

namespace LightInk
{
	struct LIGHTINK_DECL LuaTypeNil
	{  };

	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack;

/*
--lua生成

local type_str = {
	["char"] = "\t\t\tlua_pushinteger(L, static_cast <lua_Integer> (value));\n",
	["signed char"] = "\t\t\tlua_pushinteger(L, static_cast <lua_Integer> (value));\n",
	["unsigned char"] = "\t\t\tlua_pushinteger(L, static_cast <lua_Integer> (value));\n",
	["short"] = "\t\t\tlua_pushinteger(L, static_cast <lua_Integer> (value));\n",
	["unsigned short"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["int"] = "\t\t\tlua_pushinteger(L, static_cast <lua_Integer> (value));\n",
	["unsigned int"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["long"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["unsigned long"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["long long"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["unsigned long long"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["float"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["double"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["bool"] = "\t\t\tlua_pushboolean(L, value ? 1 : 0);\n",

	["const char"] = "\t\t\tlua_pushinteger(L, static_cast <lua_Integer> (value));\n",
	["const signed char"] = "\t\t\tlua_pushinteger(L, static_cast <lua_Integer> (value));\n",
	["const unsigned char"] = "\t\t\tlua_pushinteger(L, static_cast <lua_Integer> (value));\n",
	["const short"] = "\t\t\tlua_pushinteger(L, static_cast <lua_Integer> (value));\n",
	["const unsigned short"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["const int"] = "\t\t\tlua_pushinteger(L, static_cast <lua_Integer> (value));\n",
	["const unsigned int"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["const long"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["const unsigned long"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["const long long"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["const unsigned long long"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["const float"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["const double"] = "\t\t\tlua_pushnumber(L, static_cast <lua_Number> (value));\n",
	["const bool"] = "\t\t\tlua_pushboolean(L, value ? 1 : 0);\n",
}

local check_str = {
	["char"] = "\t\t\tLogTraceStepReturn(static_cast<char>(luaL_checkinteger(L, idx)));\n",
	["signed char"] = "\t\t\tLogTraceStepReturn(static_cast<signed char>(luaL_checkinteger(L, idx)));\n",
	["unsigned char"] = "\t\t\tLogTraceStepReturn(static_cast<unsigned char>(luaL_checkinteger(L, idx)));\n",
	["short"] = "\t\t\tLogTraceStepReturn(static_cast<short>(luaL_checkinteger(L, idx)));\n",
	["unsigned short"] = "\t\t\tLogTraceStepReturn(static_cast<unsigned short>(luaL_checknumber(L, idx)));\n",
	["int"] = "\t\t\tLogTraceStepReturn(static_cast<int>(luaL_checkinteger(L, idx)));\n",
	["unsigned int"] = "\t\t\tLogTraceStepReturn(static_cast<unsigned int>(luaL_checknumber(L, idx)));\n",
	["long"] = "\t\t\tLogTraceStepReturn(static_cast<long>(luaL_checknumber(L, idx)));\n",
	["unsigned long"] = "\t\t\tLogTraceStepReturn(static_cast<unsigned long>(luaL_checknumber(L, idx)));\n",
	["long long"] = "\t\t\tLogTraceStepReturn(static_cast<long long>(luaL_checknumber(L, idx)));\n",
	["unsigned long long"] = "\t\t\tLogTraceStepReturn(static_cast<unsigned long long>(luaL_checknumber(L, idx)));\n",
	["float"] = "\t\t\tLogTraceStepReturn(static_cast<float>(luaL_checknumber(L, idx)));\n",
	["double"] = "\t\t\tLogTraceStepReturn(static_cast<double>(luaL_checknumber(L, idx)));\n",
	["bool"] = "\t\t\tLogTraceStepReturn(lua_toboolean (L, idx) ? true : false);\n",

	["const char"] = "\t\t\tLogTraceStepReturn(static_cast<char>(luaL_checkinteger(L, idx)));\n",
	["const signed char"] = "\t\t\tLogTraceStepReturn(static_cast<signed char>(luaL_checkinteger(L, idx)));\n",
	["const unsigned char"] = "\t\t\tLogTraceStepReturn(static_cast<unsigned char>(luaL_checkinteger(L, idx)));\n",
	["const short"] = "\t\t\tLogTraceStepReturn(static_cast<short>(luaL_checkinteger(L, idx)));\n",
	["const unsigned short"] = "\t\t\tLogTraceStepReturn(static_cast<unsigned short>(luaL_checknumber(L, idx)));\n",
	["const int"] = "\t\t\tLogTraceStepReturn(static_cast<int>(luaL_checkinteger(L, idx)));\n",
	["const unsigned int"] = "\t\t\tLogTraceStepReturn(static_cast<unsigned int>(luaL_checknumber(L, idx)));\n",
	["const long"] = "\t\t\tLogTraceStepReturn(static_cast<long>(luaL_checknumber(L, idx)));\n",
	["const unsigned long"] = "\t\t\tLogTraceStepReturn(static_cast<unsigned long>(luaL_checknumber(L, idx)));\n",
	["const long long"] = "\t\t\tLogTraceStepReturn(static_cast<long long>(luaL_checknumber(L, idx)));\n",
	["const unsigned long long"] = "\t\t\tLogTraceStepReturn(static_cast<unsigned long long>(luaL_checknumber(L, idx)));\n",
	["const float"] = "\t\t\tLogTraceStepReturn(static_cast<float>(luaL_checknumber(L, idx)));\n",
	["const double"] = "\t\t\tLogTraceStepReturn(static_cast<double>(luaL_checknumber(L, idx)));\n",
	["const bool"] = "\t\t\tLogTraceStepReturn(lua_toboolean (L, idx) ? true : false);\n",
}



function create_traits(typeStr)
	local temp = {}
	table.insert(temp, "\t//")
	table.insert(temp, typeStr)
	table.insert(temp, "\n\ttemplate <>\n\tstruct LIGHTINK_TEMPLATE_DECL LuaStack <")
	table.insert(temp, typeStr)
	table.insert(temp, ">\n\t{\n\t\tstatic inline void push(lua_State * L, ")
	table.insert(temp, typeStr)
	table.insert(temp, " value)\n\t\t{\n\t\t\tLogTraceStepCall(\"void LuaStack<")
	table.insert(temp, typeStr)
	table.insert(temp, ">::push(lua_State * L, ")
	table.insert(temp, typeStr)
	table.insert(temp, " value)\");\n")
	table.insert(temp, type_str[typeStr])
	table.insert(temp, "\t\t\tLogTraceStepReturnVoid;\n\t\t}\n\n")

	table.insert(temp, "\t\tstatic inline ")
	table.insert(temp, typeStr)
	table.insert(temp, " get(lua_State * L, int idx)\n\t\t{\n\t\t\tLogTraceStepCall(\"")
	table.insert(temp, typeStr)
	table.insert(temp, " LuaStack<")
	table.insert(temp, typeStr)
	table.insert(temp, ">::get(lua_State * L, int idx)\");\n")
	table.insert(temp, check_str[typeStr])
	table.insert(temp, "\t\t}\n\t};\n\n\n")

	print(table.concat(temp))

end

for k, v in pairs(type_str) do
	create_traits(k)
end
*/
	//const long long
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const long long>
	{
		static inline void push(lua_State * L, const long long value)
		{
			LogTraceStepCall("void LuaStack<const long long>::push(lua_State * L, const long long value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline long long get(lua_State * L, int idx)
		{
			LogTraceStepCall("long long LuaStack<const long long>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<long long>(luaL_checknumber(L, idx)));
		}
	};



	//const unsigned int
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const unsigned int>
	{
		static inline void push(lua_State * L, const unsigned int value)
		{
			LogTraceStepCall("void LuaStack<const unsigned int>::push(lua_State * L, const unsigned int value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline unsigned int get(lua_State * L, int idx)
		{
			LogTraceStepCall("unsigned int LuaStack<const unsigned int>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<unsigned int>(luaL_checknumber(L, idx)));
		}
	};



	//const signed char
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const signed char>
	{
		static inline void push(lua_State * L, const signed char value)
		{
			LogTraceStepCall("void LuaStack<const signed char>::push(lua_State * L, const signed char value)");
			lua_pushinteger(L, static_cast <lua_Integer> (value));
			LogTraceStepReturnVoid;
		}

		static inline signed char get(lua_State * L, int idx)
		{
			LogTraceStepCall("signed char LuaStack<const signed char>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<signed char>(luaL_checkinteger(L, idx)));
		}
	};



	//unsigned short
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <unsigned short>
	{
		static inline void push(lua_State * L, unsigned short value)
		{
			LogTraceStepCall("void LuaStack<unsigned short>::push(lua_State * L, unsigned short value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline unsigned short get(lua_State * L, int idx)
		{
			LogTraceStepCall("unsigned short LuaStack<unsigned short>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<unsigned short>(luaL_checknumber(L, idx)));
		}
	};



	//long
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <long>
	{
		static inline void push(lua_State * L, long value)
		{
			LogTraceStepCall("void LuaStack<long>::push(lua_State * L, long value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline long get(lua_State * L, int idx)
		{
			LogTraceStepCall("long LuaStack<long>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<long>(luaL_checknumber(L, idx)));
		}
	};



	//const bool
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const bool>
	{
		static inline void push(lua_State * L, const bool value)
		{
			LogTraceStepCall("void LuaStack<const bool>::push(lua_State * L, const bool value)");
			lua_pushboolean(L, value ? 1 : 0);
			LogTraceStepReturnVoid;
		}

		static inline bool get(lua_State * L, int idx)
		{
			LogTraceStepCall("bool LuaStack<const bool>::get(lua_State * L, int idx)");
			LogTraceStepReturn(lua_toboolean (L, idx) ? true : false);
		}
	};



	//int
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <int>
	{
		static inline void push(lua_State * L, int value)
		{
			LogTraceStepCall("void LuaStack<int>::push(lua_State * L, int value)");
			lua_pushinteger(L, static_cast <lua_Integer> (value));
			LogTraceStepReturnVoid;
		}

		static inline int get(lua_State * L, int idx)
		{
			LogTraceStepCall("int LuaStack<int>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<int>(luaL_checkinteger(L, idx)));
		}
	};



	//unsigned long long
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <unsigned long long>
	{
		static inline void push(lua_State * L, unsigned long long value)
		{
			LogTraceStepCall("void LuaStack<unsigned long long>::push(lua_State * L, unsigned long long value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline unsigned long long get(lua_State * L, int idx)
		{
			LogTraceStepCall("unsigned long long LuaStack<unsigned long long>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<unsigned long long>(luaL_checknumber(L, idx)));
		}
	};



	//float
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <float>
	{
		static inline void push(lua_State * L, float value)
		{
			LogTraceStepCall("void LuaStack<float>::push(lua_State * L, float value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline float get(lua_State * L, int idx)
		{
			LogTraceStepCall("float LuaStack<float>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<float>(luaL_checknumber(L, idx)));
		}
	};



	//const short
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const short>
	{
		static inline void push(lua_State * L, const short value)
		{
			LogTraceStepCall("void LuaStack<const short>::push(lua_State * L, const short value)");
			lua_pushinteger(L, static_cast <lua_Integer> (value));
			LogTraceStepReturnVoid;
		}

		static inline short get(lua_State * L, int idx)
		{
			LogTraceStepCall("short LuaStack<const short>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<short>(luaL_checkinteger(L, idx)));
		}
	};



	//unsigned int
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <unsigned int>
	{
		static inline void push(lua_State * L, unsigned int value)
		{
			LogTraceStepCall("void LuaStack<unsigned int>::push(lua_State * L, unsigned int value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline unsigned int get(lua_State * L, int idx)
		{
			LogTraceStepCall("unsigned int LuaStack<unsigned int>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<unsigned int>(luaL_checknumber(L, idx)));
		}
	};



	//const char
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const char>
	{
		static inline void push(lua_State * L, const char value)
		{
			LogTraceStepCall("void LuaStack<const char>::push(lua_State * L, const char value)");
			lua_pushinteger(L, static_cast <lua_Integer> (value));
			LogTraceStepReturnVoid;
		}

		static inline char get(lua_State * L, int idx)
		{
			LogTraceStepCall("char LuaStack<const char>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<char>(luaL_checkinteger(L, idx)));
		}
	};



	//bool
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <bool>
	{
		static inline void push(lua_State * L, bool value)
		{
			LogTraceStepCall("void LuaStack<bool>::push(lua_State * L, bool value)");
			lua_pushboolean(L, value ? 1 : 0);
			LogTraceStepReturnVoid;
		}

		static inline bool get(lua_State * L, int idx)
		{
			LogTraceStepCall("bool LuaStack<bool>::get(lua_State * L, int idx)");
			LogTraceStepReturn(lua_toboolean (L, idx) ? true : false);
		}
	};



	//const float
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const float>
	{
		static inline void push(lua_State * L, const float value)
		{
			LogTraceStepCall("void LuaStack<const float>::push(lua_State * L, const float value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline float get(lua_State * L, int idx)
		{
			LogTraceStepCall("float LuaStack<const float>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<float>(luaL_checknumber(L, idx)));
		}
	};



	//const unsigned long
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const unsigned long>
	{
		static inline void push(lua_State * L, const unsigned long value)
		{
			LogTraceStepCall("void LuaStack<const unsigned long>::push(lua_State * L, const unsigned long value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline unsigned long get(lua_State * L, int idx)
		{
			LogTraceStepCall("unsigned long LuaStack<const unsigned long>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<unsigned long>(luaL_checknumber(L, idx)));
		}
	};



	//const unsigned long long
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const unsigned long long>
	{
		static inline void push(lua_State * L, const unsigned long long value)
		{
			LogTraceStepCall("void LuaStack<const unsigned long long>::push(lua_State * L, const unsigned long long value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline unsigned long long get(lua_State * L, int idx)
		{
			LogTraceStepCall("unsigned long long LuaStack<const unsigned long long>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<unsigned long long>(luaL_checknumber(L, idx)));
		}
	};



	//short
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <short>
	{
		static inline void push(lua_State * L, short value)
		{
			LogTraceStepCall("void LuaStack<short>::push(lua_State * L, short value)");
			lua_pushinteger(L, static_cast <lua_Integer> (value));
			LogTraceStepReturnVoid;
		}

		static inline short get(lua_State * L, int idx)
		{
			LogTraceStepCall("short LuaStack<short>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<short>(luaL_checkinteger(L, idx)));
		}
	};



	//unsigned char
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <unsigned char>
	{
		static inline void push(lua_State * L, unsigned char value)
		{
			LogTraceStepCall("void LuaStack<unsigned char>::push(lua_State * L, unsigned char value)");
			lua_pushinteger(L, static_cast <lua_Integer> (value));
			LogTraceStepReturnVoid;
		}

		static inline unsigned char get(lua_State * L, int idx)
		{
			LogTraceStepCall("unsigned char LuaStack<unsigned char>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<unsigned char>(luaL_checkinteger(L, idx)));
		}
	};



	//const int
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const int>
	{
		static inline void push(lua_State * L, const int value)
		{
			LogTraceStepCall("void LuaStack<const int>::push(lua_State * L, const int value)");
			lua_pushinteger(L, static_cast <lua_Integer> (value));
			LogTraceStepReturnVoid;
		}

		static inline int get(lua_State * L, int idx)
		{
			LogTraceStepCall("int LuaStack<const int>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<int>(luaL_checkinteger(L, idx)));
		}
	};



	//const long
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const long>
	{
		static inline void push(lua_State * L, const long value)
		{
			LogTraceStepCall("void LuaStack<const long>::push(lua_State * L, const long value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline long get(lua_State * L, int idx)
		{
			LogTraceStepCall("long LuaStack<const long>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<long>(luaL_checknumber(L, idx)));
		}
	};



	//char
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <char>
	{
		static inline void push(lua_State * L, char value)
		{
			LogTraceStepCall("void LuaStack<char>::push(lua_State * L, char value)");
			lua_pushinteger(L, static_cast <lua_Integer> (value));
			LogTraceStepReturnVoid;
		}

		static inline char get(lua_State * L, int idx)
		{
			LogTraceStepCall("char LuaStack<char>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<char>(luaL_checkinteger(L, idx)));
		}
	};



	//unsigned long
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <unsigned long>
	{
		static inline void push(lua_State * L, unsigned long value)
		{
			LogTraceStepCall("void LuaStack<unsigned long>::push(lua_State * L, unsigned long value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline unsigned long get(lua_State * L, int idx)
		{
			LogTraceStepCall("unsigned long LuaStack<unsigned long>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<unsigned long>(luaL_checknumber(L, idx)));
		}
	};



	//signed char
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <signed char>
	{
		static inline void push(lua_State * L, signed char value)
		{
			LogTraceStepCall("void LuaStack<signed char>::push(lua_State * L, signed char value)");
			lua_pushinteger(L, static_cast <lua_Integer> (value));
			LogTraceStepReturnVoid;
		}

		static inline signed char get(lua_State * L, int idx)
		{
			LogTraceStepCall("signed char LuaStack<signed char>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<signed char>(luaL_checkinteger(L, idx)));
		}
	};



	//const unsigned short
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const unsigned short>
	{
		static inline void push(lua_State * L, const unsigned short value)
		{
			LogTraceStepCall("void LuaStack<const unsigned short>::push(lua_State * L, const unsigned short value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline unsigned short get(lua_State * L, int idx)
		{
			LogTraceStepCall("unsigned short LuaStack<const unsigned short>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<unsigned short>(luaL_checknumber(L, idx)));
		}
	};



	//long long
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <long long>
	{
		static inline void push(lua_State * L, long long value)
		{
			LogTraceStepCall("void LuaStack<long long>::push(lua_State * L, long long value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline long long get(lua_State * L, int idx)
		{
			LogTraceStepCall("long long LuaStack<long long>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<long long>(luaL_checknumber(L, idx)));
		}
	};



	//const unsigned char
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const unsigned char>
	{
		static inline void push(lua_State * L, const unsigned char value)
		{
			LogTraceStepCall("void LuaStack<const unsigned char>::push(lua_State * L, const unsigned char value)");
			lua_pushinteger(L, static_cast <lua_Integer> (value));
			LogTraceStepReturnVoid;
		}

		static inline unsigned char get(lua_State * L, int idx)
		{
			LogTraceStepCall("unsigned char LuaStack<const unsigned char>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<unsigned char>(luaL_checkinteger(L, idx)));
		}
	};



	//double
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <double>
	{
		static inline void push(lua_State * L, double value)
		{
			LogTraceStepCall("void LuaStack<double>::push(lua_State * L, double value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline double get(lua_State * L, int idx)
		{
			LogTraceStepCall("double LuaStack<double>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<double>(luaL_checknumber(L, idx)));
		}
	};



	//const double
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const double>
	{
		static inline void push(lua_State * L, const double value)
		{
			LogTraceStepCall("void LuaStack<const double>::push(lua_State * L, const double value)");
			lua_pushnumber(L, static_cast <lua_Number> (value));
			LogTraceStepReturnVoid;
		}

		static inline double get(lua_State * L, int idx)
		{
			LogTraceStepCall("double LuaStack<const double>::get(lua_State * L, int idx)");
			LogTraceStepReturn(static_cast<double>(luaL_checknumber(L, idx)));
		}
	};


	//Push a lua_CFunction.
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <lua_CFunction>
	{
		static inline void push(lua_State * L, lua_CFunction f)
		{
			LogTraceStepCall("void LuaStack<lua_CFunction>::push(lua_State * L, lua_CFunction f)");
			lua_pushcfunction (L, f);
			LogTraceStepReturnVoid;
		}

		static inline lua_CFunction get(lua_State * L, int idx)
		{
			LogTraceStepCall("lua_CFunction LuaStack<lua_CFunction>::get(lua_State * L, int idx)");
			LogTraceStepReturn(lua_tocfunction (L, idx));
		}
	};

	//RuntimeError
	LightInkLuaEnumType(RuntimeError)

	//nil
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <LuaTypeNil>
	{
		static inline void push(lua_State * L, LuaTypeNil)
		{
			LogTraceStepCall("void LuaStack<LuaTypeNil>::push(lua_State * L, LuaTypeNil)");
			lua_pushnil (L);
			LogTraceStepReturnVoid;
		}

		static inline LuaTypeNil get(lua_State * L, int idx)
		{
			LogTraceStepCall("LuaTypeNil LuaStack<LuaTypeNil>::get(lua_State * L, int idx)");
			LogTraceStepReturn(LuaTypeNil());
		}
	};

	//const char *
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const char * const>
	{
		static inline void push(lua_State * L, const char * str)
		{
			LogTraceStepCall("void LuaStack<const char *>::push(lua_State * L, const char * str)");
			if (str)
			{
				lua_pushstring(L, str);
			}
			else
			{
				lua_pushnil(L);
			}
			LogTraceStepReturnVoid;
		}

		static inline const char * get(lua_State * L, int idx)
		{
			LogTraceStepCall("const char * LuaStack<const char *>::get(lua_State * L, int idx)");
			LogTraceStepReturn((lua_isnil(L, idx) ? NULL : luaL_checkstring(L, idx)));
		}
	};

	//const char[size]
	template <uint32 size>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const char [size]>
	{
		static inline void push(lua_State * L, const char str[size])
		{
			LogTraceStepCall("void LuaStack<const char *>::push(lua_State * L, const char str[size])");
			if (str)
			{
				lua_pushlstring(L, str, strnlen(str, size));
			}
			else
			{
				lua_pushnil(L);
			}
			LogTraceStepReturnVoid;
		}

		static inline const char * get(lua_State * L, int idx)
		{
			LogTraceStepCall("const char * LuaStack<const char *>::get(lua_State * L, int idx)");
			LogTraceStepReturn((lua_isnil(L, idx) ? NULL : luaL_checkstring(L, idx)));
		}
	};

	//char[size]
	template <uint32 size>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <char [size]>
	{
		static inline void push(lua_State * L, char str[size])
		{
			LogTraceStepCall("void LuaStack<const char *>::push(lua_State * L, char str[size])");
			if (str)
			{
				lua_pushlstring(L, str, strnlen(str, size));
			}
			else
			{
				lua_pushnil(L);
			}
			LogTraceStepReturnVoid;
		}

		static inline const char * get(lua_State * L, int idx)
		{
			LogTraceStepCall("const char * LuaStack<const char *>::get(lua_State * L, int idx)");
			LogTraceStepReturn((lua_isnil(L, idx) ? NULL : luaL_checkstring(L, idx)));
		}
	};

	//char *
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <char *>
	{
		static inline void push(lua_State * L, char * str)
		{
			LogTraceStepCall("void LuaStack<char *>::push(lua_State * L, char * str)");
			if (str)
			{
				lua_pushstring(L, str);
			}
			else
			{
				lua_pushnil(L);
			}
			LogTraceStepReturnVoid;
		}

		static inline const char * get(lua_State * L, int idx)
		{
			LogTraceStepCall("const char * LuaStack<char *>::get(lua_State * L, int idx)");
			LogTraceStepReturn((lua_isnil(L, idx) ? NULL : luaL_checkstring(L, idx)));
		}
	};


	//CharPtrBridge
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <CharPtrBridge>
	{
		static inline void push(lua_State * L, CharPtrBridge & str)
		{
			LogTraceStepCall("void LuaStack<CharPtrBridge>::push(lua_State * L, CharPtrBridge & str)");
			lua_pushlstring (L, str.m_charPtr, str.m_len);
			LogTraceStepReturnVoid;
		}

		static inline CharPtrBridge get(lua_State * L, int idx)
		{
			LogTraceStepCall("CharPtrBridge LuaStack<CharPtrBridge>::get(lua_State * L, int idx)");
			size_t len = 0;
			const char * str = luaL_checklstring(L, idx, &len);
			LogTraceStepReturn(CharPtrBridge(str, len));
		}
	};

	//const CharPtrBridge
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const CharPtrBridge>
	{
		static inline void push(lua_State * L, const CharPtrBridge & str)
		{
			LogTraceStepCall("void LuaStack<const CharPtrBridge>::push(lua_State * L, const CharPtrBridge & str)");
			lua_pushlstring (L, str.m_charPtr, str.m_len);
			LogTraceStepReturnVoid;
		}

		static inline CharPtrBridge get(lua_State * L, int idx)
		{
			LogTraceStepCall("CharPtrBridge LuaStack<const CharPtrBridge &>::get(lua_State * L, int idx)");
			size_t len = 0;
			const char * str = luaL_checklstring(L, idx, &len);
			LogTraceStepReturn(CharPtrBridge(str, len));
		}
	};

	//const CharPtrBridge &
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const CharPtrBridge &>
	{
		static inline void push(lua_State * L, const CharPtrBridge & str)
		{
			LogTraceStepCall("void LuaStack<const CharPtrBridge &>::push(lua_State * L, const CharPtrBridge & str)");
			lua_pushlstring (L, str.m_charPtr, str.m_len);
			LogTraceStepReturnVoid;
		}

		static inline CharPtrBridge get(lua_State * L, int idx)
		{
			LogTraceStepCall("CharPtrBridge LuaStack<const CharPtrBridge &>::get(lua_State * L, int idx)");
			size_t len = 0;
			const char * str = luaL_checklstring(L, idx, &len);
			LogTraceStepReturn(CharPtrBridge(str, len));
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	//stl
	///////////////////////////////////////////////////////////////////////////////////////////
	//std::string
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <std::string>
	{
		static inline void push(lua_State * L, std::string & str)
		{
			LogTraceStepCall("void LuaStack<std::string>::push(lua_State * L, const std::string & str)");
			lua_pushlstring (L, str.c_str (), str.size());
			LogTraceStepReturnVoid;
		}

		static inline std::string get(lua_State * L, int idx)
		{
			LogTraceStepCall("std::string LuaStack<std::string>::get(lua_State * L, int idx)");
			size_t len = 0;
			const char * str = luaL_checklstring(L, idx, &len);
			LogTraceStepReturn(std::string(str, len));
		}
	};

	//const std::string
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const std::string>
	{
		static inline void push(lua_State * L, const std::string & str)
		{
			LogTraceStepCall("void LuaStack<const std::string>::push(lua_State * L, const std::string & str)");
			lua_pushlstring (L, str.c_str (), str.size());
			LogTraceStepReturnVoid;
		}

		static inline std::string get(lua_State * L, int idx)
		{
			LogTraceStepCall("std::string LuaStack<const std::string &>::get(lua_State * L, int idx)");
			size_t len = 0;
			const char * str = luaL_checklstring(L, idx, &len);
			LogTraceStepReturn(std::string(str, len));
		}
	};

	//const std::string &
	template <>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const std::string &>
	{
		static inline void push(lua_State * L, const std::string & str)
		{
			LogTraceStepCall("void LuaStack<const std::string &>::push(lua_State * L, const std::string & str)");
			lua_pushlstring (L, str.c_str (), str.size());
			LogTraceStepReturnVoid;
		}

		static inline std::string get(lua_State * L, int idx)
		{
			LogTraceStepCall("std::string LuaStack<const std::string &>::get(lua_State * L, int idx)");
			size_t len = 0;
			const char * str = luaL_checklstring(L, idx, &len);
			LogTraceStepReturn(std::string(str, len));
		}
	};

	//std::vector
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <std::vector<T> >
	{
		static inline void push(lua_State * L, std::vector<T> & vec)
		{
			LogTraceStepCall("void LuaStack<std::vector<T>>::push(lua_State * L, const std::vector<T> & vec)");
			lua_createtable(L, vec.size(), 0);
			for (size_t i = 0; i < vec.size(); ++i)
			{
				LuaStack<const T>::push(L, vec[i]);
				lua_rawseti(L, -2, i+1);
			}
			LogTraceStepReturnVoid;
		}

		static inline std::vector<T> get(lua_State * L, int idx)
		{
			LogTraceStepCall("std::vector<T> LuaStack<std::vector<T> >::get(lua_State * L, int idx)");
			if (!lua_istable(L, idx))
			{
				LogScriptErrorJump(L, "Error!!!The {} data is not a table, convert vector failed!!!", idx);
			}
			idx = lua_absindex(L, idx);
			size_t len = lua_objlen(L, idx);
			std::vector<T> vec;
			vec.reserve(len);
			for (size_t i = 1; i <= len; i++)
			{
				lua_rawgeti(L, idx, i);
				vec.push_back(LuaStack<const T>::get(L, -1));
				lua_pop(L, 1);
			}
			LogTraceStepReturn(vec);
		}
	};

	//const std::vector
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const std::vector<T> >
	{
		static inline void push(lua_State * L, const std::vector<T> & vec)
		{
			LogTraceStepCall("void LuaStack<const std::vector<T>>::push(lua_State * L, const std::vector<T> & const vec)");
			lua_createtable(L, vec.size(), 0);
			for (size_t i = 0; i < vec.size(); ++i)
			{
				LuaStack<const T>::push(L, vec[i]);
				lua_rawseti(L, -2, i+1);
			}
			LogTraceStepReturnVoid;
		}

		static inline std::vector<T> get(lua_State * L, int idx)
		{
			LogTraceStepCall("std::vector<T> LuaStack<const std::vector<T> &>::get(lua_State * L, int idx)");
			if (!lua_istable(L, idx))
			{
				LogScriptErrorJump(L, "Error!!!The {} data is not a table, convert vector failed!!!", idx);
			}
			idx = lua_absindex(L, idx);
			size_t len = lua_objlen(L, idx);
			std::vector<T> vec;
			vec.reserve(len);
			for (size_t i = 1; i <= len; i++)
			{
				lua_rawgeti(L, idx, i);
				vec.push_back(LuaStack<const T>::get(L, -1));
				lua_pop(L, 1);
			}
			LogTraceStepReturn(vec);
		}
	};

	//const std::vector &
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const std::vector<T> &>
	{
		static inline void push(lua_State * L, const std::vector<T> & vec)
		{
			LogTraceStepCall("void LuaStack<const std::vector<T> &>::push(lua_State * L, const std::vector<T> & vec)");
			lua_createtable(L, vec.size(), 0);
			for (size_t i = 0; i < vec.size(); ++i)
			{
				LuaStack<const T>::push(L, vec[i]);
				lua_rawseti(L, -2, i+1);
			}
			LogTraceStepReturnVoid;
		}

		static inline std::vector<T> get(lua_State * L, int idx)
		{
			LogTraceStepCall("std::vector<T> LuaStack<const std::vector<T> &>::get(lua_State * L, int idx)");
			if (!lua_istable(L, idx))
			{
				LogScriptErrorJump(L, "Error!!!The {} data is not a table, convert vector failed!!!", idx);
			}
			idx = lua_absindex(L, idx);
			size_t len = lua_objlen(L, idx);
			std::vector<T> vec;
			vec.reserve(len);
			for (size_t i = 1; i <= len; i++)
			{
				lua_rawgeti(L, idx, i);
				vec.push_back(LuaStack<const T>::get(L, -1));
				lua_pop(L, 1);
			}
			LogTraceStepReturn(vec);
		}
	};



	//std::map
	template <typename K, typename V>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <std::map<K, V> >
	{
		static inline void push(lua_State * L, std::map<K, V> & m)
		{
			LogTraceStepCall("void LuaStack<std::map<K, V>>::push(lua_State * L, const std::map<K, V> & m)");
			lua_createtable(L, 0, m.size());
			for (typename std::map<K, V>::iterator iter = m.begin(); iter != m.end(); ++iter)
			{
				LuaStack<const K>::push(L, iter->first);
				LuaStack<const V>::push(L, iter->second);
				lua_rawset(L, -3);
			}
			LogTraceStepReturnVoid;
		}

		static inline std::map<K, V> get(lua_State * L, int idx)
		{
			LogTraceStepCall("std::map<K, V> LuaStack<std::map<K, V>>::get(lua_State * L, int idx)");
			if (!lua_istable(L, idx))
			{
				LogScriptErrorJump(L, "Error!!!The {} data is not a table, convert map failed!!!", idx);
			}
			std::map<K, V> m;
			idx = lua_absindex(L, idx);

			lua_pushnil(L);
			while (lua_next(L, idx))
			{
				K key = LuaStack<const K>::get(L, -2);
				V val = LuaStack<const V>::get(L, -1);
				m.insert(typename std::map<K, V>::value_type(key, val));
				lua_pop(L, 1);
			}
			LogTraceStepReturn(m);
		}
	};
	
	//const std::map
	template <typename K, typename V>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const std::map<K, V> >
	{
		static inline void push(lua_State * L, const std::map<K, V> & m)
		{
			LogTraceStepCall("void LuaStack<const std::map<K, V> &>::push(lua_State * L, const std::map<K, V> & m)");
			lua_createtable(L, 0, m.size());
			for (typename std::map<K, V>::const_iterator iter = m.begin(); iter != m.end(); ++iter)
			{
				LuaStack<const K>::push(L, iter->first);
				LuaStack<const V>::push(L, iter->second);
				lua_rawset(L, -3);
			}
			LogTraceStepReturnVoid;
		}

		static inline std::map<K, V> get(lua_State * L, int idx)
		{
			LogTraceStepCall("std::map<K, V> LuaStack<const std::map<K, V> &>::get(lua_State * L, int idx)");
			if (!lua_istable(L, idx))
			{
				LogScriptErrorJump(L, "Error!!!The {} data is not a table, convert map failed!!!", idx);
			}
			std::map<K, V> m;
			idx = lua_absindex(L, idx);

			lua_pushnil(L);
			while (lua_next(L, idx))
			{
				K key = LuaStack<const K>::get(L, -2);
				V val = LuaStack<const V>::get(L, -1);
				m.insert(typename std::map<K, V>::value_type(key, val));
				lua_pop(L, 1);
			}
			LogTraceStepReturn(m);
		}
	};

	//const std::map &
	template <typename K, typename V>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const std::map<K, V> & >
	{
		static inline void push(lua_State * L, const std::map<K, V> & m)
		{
			LogTraceStepCall("void LuaStack<const std::map<K, V> &>::push(lua_State * L, const std::map<K, V> & m)");
			lua_createtable(L, 0, m.size());
			for (typename std::map<K, V>::const_iterator iter = m.begin(); iter != m.end(); ++iter)
			{
				LuaStack<const K>::push(L, iter->first);
				LuaStack<const V>::push(L, iter->second);
				lua_rawset(L, -3);
			}
			LogTraceStepReturnVoid;
		}

		static inline std::map<K, V> get(lua_State * L, int idx)
		{
			LogTraceStepCall("std::map<K, V> LuaStack<const std::map<K, V> &>::get(lua_State * L, int idx)");
			if (!lua_istable(L, idx))
			{
				LogScriptErrorJump(L, "Error!!!The {} data is not a table, convert map failed!!!", idx);
			}
			std::map<K, V> m;
			idx = lua_absindex(L, idx);

			lua_pushnil(L);
			while (lua_next(L, idx))
			{
				K key = LuaStack<const K>::get(L, -2);
				V val = LuaStack<const V>::get(L, -1);
				m.insert(typename std::map<K, V>::value_type(key, val));
				lua_pop(L, 1);
			}
			LogTraceStepReturn(m);
		}
	};


	//std::set
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <std::set<T> >
	{
		static inline void push(lua_State * L, std::set<T> & s)
		{
			LogTraceStepCall("void LuaStack<std::set<T>>::push(lua_State * L, const std::set<T> & s)");
			lua_createtable(L, 0, s.size());
			for (typename std::set<T>::iterator iter = s.begin(); iter != s.end(); ++iter)
			{
				LuaStack<const T>::push(L, *iter);
				LuaStack<bool>::push(L, true);
				lua_rawset(L, -3);
			}
			LogTraceStepReturnVoid;
		}

		static inline std::set<T> get(lua_State * L, int idx)
		{
			LogTraceStepCall("std::set<T> LuaStack<std::set<T>>::get(lua_State * L, int idx)");
			if (!lua_istable(L, idx))
			{
				LogScriptErrorJump(L, "Error!!!The {} data is not a table, convert set failed!!!", idx);
			}
			std::set<T> s;
			idx = lua_absindex(L, idx);

			lua_pushnil(L);
			while (lua_next(L, idx))
			{
				T key = LuaStack<const T>::get(L, -2);
				s.insert(key);
				lua_pop(L, 1);
			}
			LogTraceStepReturn(s);
		}
	};

	//const std::set
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const std::set<T> >
	{
		static inline void push(lua_State * L, const std::set<T> & s)
		{
			LogTraceStepCall("void LuaStack<const std::set<T> &>::push(lua_State * L, const std::set<T> & s)");
			lua_createtable(L, 0, s.size());
			for (typename std::set<T>::const_iterator iter = s.begin(); iter != s.end(); ++iter)
			{
				LuaStack<const T>::push(L, *iter);
				LuaStack<bool>::push(L, true);
				lua_rawset(L, -3);
			}
			LogTraceStepReturnVoid;
		}

		static inline std::set<T> get(lua_State * L, int idx)
		{
			LogTraceStepCall("std::set<T> LuaStack<const std::set<T> &>::get(lua_State * L, int idx)");
			if (!lua_istable(L, idx))
			{
				LogScriptErrorJump(L, "Error!!!The {} data is not a table, convert set failed!!!", idx);
			}
			std::set<T> s;
			idx = lua_absindex(L, idx);

			lua_pushnil(L);
			while (lua_next(L, idx))
			{
				T key = LuaStack<const T>::get(L, -2);
				s.insert(key);
				lua_pop(L, 1);
			}
			LogTraceStepReturn(s);
		}
	};

	//const std::set &
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const std::set<T> & >
	{
		static inline void push(lua_State * L, const std::set<T> & s)
		{
			LogTraceStepCall("void LuaStack<const std::set<T> &>::push(lua_State * L, const std::set<T> & s)");
			lua_createtable(L, 0, s.size());
			for (typename std::set<T>::const_iterator iter = s.begin(); iter != s.end(); ++iter)
			{
				LuaStack<const T>::push(L, *iter);
				LuaStack<bool>::push(L, true);
				lua_rawset(L, -3);
			}
			LogTraceStepReturnVoid;
		}

		static inline std::set<T> get(lua_State * L, int idx)
		{
			LogTraceStepCall("std::set<T> LuaStack<const std::set<T> &>::get(lua_State * L, int idx)");
			if (!lua_istable(L, idx))
			{
				LogScriptErrorJump(L, "Error!!!The {} data is not a table, convert set failed!!!", idx);
			}
			std::set<T> s;
			idx = lua_absindex(L, idx);

			lua_pushnil(L);
			while (lua_next(L, idx))
			{
				T key = LuaStack<const T>::get(L, -2);
				s.insert(key);
				lua_pop(L, 1);
			}
			LogTraceStepReturn(s);
		}
	};


	// pointer move
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <LuaStackPtrMove<T> >
	{
		static inline void push(lua_State* L, LuaStackPtrMove<T> & pm)
		{
			LogTraceStepCall("void LuaStack<LuaStackPtrMove<T> &>::push(lua_State * L, LuaStackPtrMove<T> & pm)");
			LuaUserdataPtrMove::push<T>(L, pm);
			LogTraceStepReturnVoid;
		}

		static inline LuaStackPtrMove<T> get (lua_State * L, int idx)
		{
			LogTraceStepCall("LuaStackPtrMove<T> LuaStack<LuaStackPtrMove<T> &>::get(lua_State * L, int idx)");
			LogTraceStepReturn(LuaUserdataPtrMove::get<T> (L, idx));
		}
	};

	//const pointer move
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const LuaStackPtrMove<T> >
	{
		static inline void push(lua_State* L, const LuaStackPtrMove<T> & pm)
		{
			LogTraceStepCall("void LuaStack<const LuaStackPtrMove<T> &>::push(lua_State * L, const LuaStackPtrMove<T> & pm)");
			LuaUserdataPtrMove::push<T>(L, pm);
			LogTraceStepReturnVoid;
		}

		static inline LuaStackPtrMove<T> get (lua_State * L, int idx)
		{
			LogTraceStepCall("LuaStackPtrMove<T> LuaStack<const LuaStackPtrMove<T> &>::get(lua_State * L, int idx)");
			LogTraceStepReturn(LuaUserdataPtrMove::get<T> (L, idx));
		}
	};

	//const pointer move &
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const LuaStackPtrMove<T> &>
	{
		static inline void push(lua_State* L, const LuaStackPtrMove<T> & pm)
		{
			LogTraceStepCall("void LuaStack<const LuaStackPtrMove<T> &>::push(lua_State * L, const LuaStackPtrMove<T> & pm)");
			LuaUserdataPtrMove::push<T>(L, pm);
			LogTraceStepReturnVoid;
		}

		static inline LuaStackPtrMove<T> get (lua_State * L, int idx)
		{
			LogTraceStepCall("LuaStackPtrMove<T> LuaStack<const LuaStackPtrMove<T> &>::get(lua_State * L, int idx)");
			LogTraceStepReturn(LuaUserdataPtrMove::get<T> (L, idx));
		}
	};


	

	template <typename T, typename ContextType>
	inline static void gc_shared_ptr(T * obj, void * context)
	{
		LogTraceStepCall("void GCSharedPtr::gc_shared_ptr(T * obj, void * context)");
		ContextType * c = (ContextType *)context;
		delete c;
		LogTraceStepReturnVoid;
	}
	// pointer shared
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <SharedPtrWrapper<T, RefCounter<SmallObject>, PtrDelStrategy, SmallObject> >
	{
		static inline void push(lua_State* L, typename SharedPtr<T>::type & sp)
		{
			LogTraceStepCall("void LuaStack<typename SharedPtr<T>::type &>::push(lua_State * L, typename SharedPtr<T>::type & sp)");
			typedef typename SharedPtr<T>::type AdaptorType;
			AdaptorType * nsp = new AdaptorType(sp);
			LuaUserdataSharedPtr::push<T>(L, sp.get(), gc_shared_ptr<T, AdaptorType>, nsp);
			LogTraceStepReturnVoid;
		}

		static inline typename SharedPtr<T>::type get (lua_State * L, int idx)
		{
			LogTraceStepCall("typename SharedPtr<T>::type LuaStack<typename SharedPtr<T>::type &>::get(lua_State * L, int idx)");
			typedef typename SharedPtr<T>::type AdaptorType;
			AdaptorType * sp = (AdaptorType *) LuaUserdataSharedPtr::get<T>(L, idx);
			LogTraceStepReturn(*sp);
		}
	};

	//const pointer shared
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const SharedPtrWrapper<T, RefCounter<SmallObject>, PtrDelStrategy, SmallObject> >
	{
		static inline void push(lua_State* L, const typename SharedPtr<T>::type & sp)
		{
			LogTraceStepCall("void LuaStack<const typename SharedPtr<T>::type &>::push(lua_State * L, const typename SharedPtr<T>::type & sp)");
			typedef typename SharedPtr<T>::type AdaptorType;
			AdaptorType * nsp = new AdaptorType(sp);
			LuaUserdataSharedPtr::push<T>(L, sp.get(), gc_shared_ptr<T, AdaptorType>, nsp);
			LogTraceStepReturnVoid;
		}

		static inline typename SharedPtr<T>::type get (lua_State * L, int idx)
		{
			LogTraceStepCall("typename SharedPtr<T>::type LuaStack<const typename SharedPtr<T>::type &>::get(lua_State * L, int idx)");
			typedef typename SharedPtr<T>::type AdaptorType;
			AdaptorType * sp = (AdaptorType *) LuaUserdataSharedPtr::get<T>(L, idx);
			LogTraceStepReturn(*sp);
		}
	};

	//const pointer shared &
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const SharedPtrWrapper<T, RefCounter<SmallObject>, PtrDelStrategy, SmallObject> &>
	{
		static inline void push(lua_State* L, const typename SharedPtr<T>::type & sp)
		{
			LogTraceStepCall("void LuaStack<const typename SharedPtr<T>::type &>::push(lua_State * L, const typename SharedPtr<T>::type & sp)");
			typedef typename SharedPtr<T>::type AdaptorType;
			AdaptorType * nsp = new AdaptorType(sp);
			LuaUserdataSharedPtr::push<T>(L, sp.get(), gc_shared_ptr<T, AdaptorType>, nsp);
			LogTraceStepReturnVoid;
		}

		static inline typename SharedPtr<T>::type get (lua_State * L, int idx)
		{
			LogTraceStepCall("typename SharedPtr<T>::type LuaStack<const typename SharedPtr<T>::type &>::get(lua_State * L, int idx)");
			typedef typename SharedPtr<T>::type AdaptorType;
			AdaptorType * sp = (AdaptorType *) LuaUserdataSharedPtr::get<T>(L, idx);
			LogTraceStepReturn(*sp);
		}
	};

	// pointer ts shared
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <SharedPtrWrapper<T, RefCounterTS<SmallObject>, PtrDelStrategy, SmallObject> >
	{
		static inline void push(lua_State* L, typename SharedPtrTS<T>::type & sp)
		{
			LogTraceStepCall("void LuaStack<typename SharedPtrTS<T>::type &>::push(lua_State * L, typename SharedPtrTS<T>::type & sp)");
			typedef typename SharedPtrTS<T>::type AdaptorType;
			AdaptorType * nsp = new AdaptorType(sp);
			LuaUserdataSharedPtr::push<T>(L, sp.get(), gc_shared_ptr<T, AdaptorType>, nsp);
			LogTraceStepReturnVoid;
		}

		static inline typename SharedPtrTS<T>::type get (lua_State * L, int idx)
		{
			LogTraceStepCall("typename SharedPtrTS<T>::type LuaStack<typename SharedPtrTS<T>::type &>::get(lua_State * L, int idx)");
			typedef typename SharedPtrTS<T>::type AdaptorType;
			AdaptorType * sp = (AdaptorType *) LuaUserdataSharedPtr::get<T>(L, idx);
			LogTraceStepReturn(*sp);
		}
	};

	//const pointer ts shared
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const SharedPtrWrapper<T, RefCounterTS<SmallObject>, PtrDelStrategy, SmallObject> >
	{
		static inline void push(lua_State* L, const typename SharedPtrTS<T>::type & sp)
		{
			LogTraceStepCall("void LuaStack<const typename SharedPtrTS<T>::type &>::push(lua_State * L, const typename SharedPtrTS<T>::type & sp)");
			typedef typename SharedPtrTS<T>::type AdaptorType;
			AdaptorType * nsp = new AdaptorType(sp);
			LuaUserdataSharedPtr::push<T>(L, sp.get(), gc_shared_ptr<T, AdaptorType>, nsp);
			LogTraceStepReturnVoid;
		}

		static inline typename SharedPtrTS<T>::type get (lua_State * L, int idx)
		{
			LogTraceStepCall("typename SharedPtrTS<T>::type LuaStack<const typename SharedPtrTS<T>::type &>::get(lua_State * L, int idx)");
			typedef typename SharedPtrTS<T>::type AdaptorType;
			AdaptorType * sp = (AdaptorType *) LuaUserdataSharedPtr::get<T>(L, idx);
			LogTraceStepReturn(*sp);
		}
	};

	//const pointer ts shared &
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const SharedPtrWrapper<T, RefCounterTS<SmallObject>, PtrDelStrategy, SmallObject> &>
	{
		static inline void push(lua_State* L, const typename SharedPtrTS<T>::type & sp)
		{
			LogTraceStepCall("void LuaStack<const typename SharedPtrTS<T>::type &>::push(lua_State * L, const typename SharedPtrTS<T>::type & sp)");
			typedef typename SharedPtrTS<T>::type AdaptorType;
			AdaptorType * nsp = new AdaptorType(sp);
			LuaUserdataSharedPtr::push<T>(L, sp.get(), gc_shared_ptr<T, AdaptorType>, nsp);
			LogTraceStepReturnVoid;
		}

		static inline typename SharedPtrTS<T>::type get (lua_State * L, int idx)
		{
			LogTraceStepCall("typename SharedPtrTS<T>::type LuaStack<const typename SharedPtrTS<T>::type &>::get(lua_State * L, int idx)");
			typedef typename SharedPtrTS<T>::type AdaptorType;
			AdaptorType * sp = (AdaptorType *) LuaUserdataSharedPtr::get<T>(L, idx);
			LogTraceStepReturn(*sp);
		}
	};


	// pointer
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <T * const>
	{
		static inline void push(lua_State* L, T * const p)
		{
			LogTraceStepCall("void LuaStack<T *>::push(lua_State * L, T * const p)");
			LuaUserdataPtr::push<T>(L, p);
			LogTraceStepReturnVoid;
		}

		static inline T * get (lua_State * L, int idx)
		{
			LogTraceStepCall("T * LuaStack<T *>::get(lua_State * L, int idx)");
			LogTraceStepReturn(LuaUserdataPtr::get<T> (L, idx));
		}
	};

	// const pointer
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const T *>
	{
		static inline void push(lua_State* L, const T * p)
		{
			LogTraceStepCall("void LuaStack<T *>::push(lua_State * L, const T * p)");
			LuaUserdataPtr::push<T>(L, p);
			LogTraceStepReturnVoid;
		}

		static inline T * get (lua_State * L, int idx)
		{
			LogTraceStepCall("T * LuaStack<T *>::get(lua_State * L, int idx)");
			LogTraceStepReturn(LuaUserdataPtr::get<T> (L, idx));
		}
	};

	//const pointer const
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack <const T * const>
	{
		static inline void push(lua_State* L, const T * const p)
		{
			LogTraceStepCall("void LuaStack<T *>::push(lua_State * L, const T * const p)");
			LuaUserdataPtr::push<T>(L, p);
			LogTraceStepReturnVoid;
		}

		static inline const T * get (lua_State * L, int idx)
		{
			LogTraceStepCall("const T * LuaStack<T *>::get(lua_State * L, int idx)");
			LogTraceStepReturn(LuaUserdataPtr::get<T> (L, idx));
		}
	};

	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack<const T &>
	{
		static inline void push(lua_State * L, const T & t)
		{
			LogTraceStepCall("void LuaStack<T>::push(lua_State * L, const T & t)");
			LuaUserdataPtr::push<T>(L, &t);
			LogTraceStepReturnVoid;
		}

		static inline const T & get(lua_State * L, int idx)
		{
			LogTraceStepCall("const T & LuaStack<T>::get(lua_State * L, int idx)");
			LogTraceStepReturn(*LuaUserdataPtr::get <T> (L, idx));
		}
	};

	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack<T &>
	{
		static inline void push(lua_State * L, T & t)
		{
			LogTraceStepCall("void LuaStack<T>::push(lua_State * L, T & t)");
			LuaUserdataPtr::push<T>(L, &t);
			LogTraceStepReturnVoid;
		}

		static inline T & get(lua_State * L, int idx)
		{
			LogTraceStepCall("T & LuaStack<T>::get(lua_State * L, int idx)");
			LogTraceStepReturn(*LuaUserdataPtr::get <T> (L, idx));
		}
	};

	// Lua stack conversions for const class objects passed by value.
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack<const T>
	{
		static inline void push(lua_State * L, const T & t)
		{
			LogTraceStepCall("void LuaStack<T>::push(lua_State * L, const T & t)");
			LuaUserdata::push<T>(L, t);
			LogTraceStepReturnVoid;
		}

		static inline const T get(lua_State * L, int idx)
		{
			LogTraceStepCall("T LuaStack<T>::get(lua_State * L, int idx)");
			LogTraceStepReturn(LuaUserdata::get <T> (L, idx));
		}
	};

	// Lua stack conversions for class objects passed by value.
	template <typename T>
	struct LIGHTINK_TEMPLATE_DECL LuaStack
	{
		static inline void push(lua_State * L, T & t)
		{
			LogTraceStepCall("void LuaStack<T>::push(lua_State * L, T & t)");
			LuaUserdata::push<T>(L, t);
			LogTraceStepReturnVoid;
		}

		static inline T get(lua_State * L, int idx)
		{
			LogTraceStepCall("T LuaStack<T>::get(lua_State * L, int idx)");
			LogTraceStepReturn(LuaUserdata::get <T> (L, idx));
		}
	};

}


#endif