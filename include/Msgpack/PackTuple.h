



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
#ifndef LIGHTINK_MSGPACK_PACKTUPLE_H_
#define LIGHTINK_MSGPACK_PACKTUPLE_H_

#include "Common/TypeListDefine.h"

/*
--lua生成
function create_tuple(count)
	local allStr = "template <"
	for k = 1, count + 1 do
		if k % 5 == 0 then
			allStr = string.format("%stypename T%d = void,\n\t", allStr, k)
		else
			allStr = string.format("%stypename T%d = void,", allStr, k)
		end
	end
	local idx = string.find(allStr, ",", #allStr - 4)
	allStr = string.sub(allStr, 0, idx - 1) .. [[>
struct PackTuple;

template <>
struct PackTuple<>
{
	typedef PackTuple<> value_type;

	PackTuple() { ; }
};
]]

	for k = 1, count do
		local str = "template <"
		local str1 = "struct PackTuple<"
		local str2 = string.format("typedef LIGHTINK_TYPELIST_%d(", k)
		local str3 = "typedef PackTuple<"
		local str4 = "PackTuple("
		local str5 = " : "
		local str6 = "template <typename TBuffer>\n\tRuntimeError pack(TBuffer buffer)\n\t{ try {buffer"
		local str7 = "template <typename TBuffer>\n\tRuntimeError unpack(TBuffer buffer)\n\t{ try {buffer"
		local str8 = ""
		for ki = 1, k do
			if ki % 10 == 0 then
				str = string.format("%stypename T%d, \n\t", str, ki)
				str1 = string.format("%sT%d, \n\t", str1, ki)
				str2 = string.format("%sT%d, \n\t", str2, ki)
				str3 = string.format("%sT%d, \n\t", str3, ki)
				str4 = string.format("%sconst T%d & _t%d, \n\t", str4, ki, ki)
				str5 = string.format("%st%d(_t%d), \n\t", str5, ki, ki)
				str6 = string.format("%s << t%d \\\n\t", str6, ki)
				str7 = string.format("%s >> t%d \\\n\t", str7, ki)
			else
				str = string.format("%stypename T%d,", str, ki)
				str1 = string.format("%sT%d, ", str1, ki)
				str2 = string.format("%sT%d, ", str2, ki)
				str3 = string.format("%sT%d, ", str3, ki)
				str4 = string.format("%sconst T%d & _t%d, ", str4, ki, ki)
				str5 = string.format("%st%d(_t%d), ", str5, ki, ki)
				str6 = string.format("%s << t%d", str6, ki)
				str7 = string.format("%s >> t%d", str7, ki)
			end
			str8 = string.format("%s\tTYPEAT(%du, t%d)\n", str8, ki, ki)
		end
		idx = string.find(str, ",", #str - 4)
		str = string.sub(str, 0, idx - 1) .. ">\n"

		idx = string.find(str1, ",", #str1 - 4)
		str1 = string.sub(str1, 0, idx - 1) .. ">\n"

		idx = string.find(str2, ",", #str2 - 4)
		str2 = string.sub(str2, 0, idx - 1) .. ") value_type_list;\n"

		idx = string.find(str3, ",", #str3 - 4)
		str3 = string.sub(str3, 0, idx - 1) .. "> value_type;\n"

		idx = string.find(str4, ",", #str4 - 4)
		str4 = string.sub(str4, 0, idx - 1) .. ")"

		idx = string.find(str5, ",", #str5 - 4)
		str5 = string.sub(str5, 0, idx - 1) .. " {;}\n"

		idx = string.find(str6, "\\", #str6 - 4)
		if idx then
			str6 = string.sub(str6, 0, idx - 1)
		end
		str6 = str6 .. ";} catch (RuntimeError & e) { return e; } return RE_Success; }\n"

		idx = string.find(str7, "\\", #str7 - 4)
		if idx then
			str7 = string.sub(str7, 0, idx - 1)
		end
		str7 = str7 .. ";} catch (RuntimeError & e) { return e; } return RE_Success; }\n"

		allStr = allStr .. str .. str1 .. "{\n\n"
		allStr = allStr .. "\t" .. str2 .. "\n"
		allStr = allStr .. "\t" .. str3 .. "\n"
		allStr = allStr .. "\t" .. str4 .. str5 .. "\n"
		allStr = allStr .. "\t" .. str6 .. "\n"
		allStr = allStr .. "\t" .. str7 .. "\n"
		allStr = allStr .. "\tPackTuple() { ; }\n\n\ttemplate<unsigned int N>\n\ttypename TypeListAt<value_type_list, N>::Result & get();\n\n"
		allStr = allStr .. str8 .. "\n};\n\n"
	end



	print(allStr)
end

create_tuple(56)

*/

#define TYPEAT(i, arg) typename TypeListAt<value_type_list, i>::Result arg; \
	template<> \
	typename TypeListAt<value_type_list, i>::Result & get<i>() \
	{ return arg; }

namespace LightInk
{
	template <typename T1 = void,typename T2 = void,typename T3 = void,typename T4 = void,typename T5 = void,
		typename T6 = void,typename T7 = void,typename T8 = void,typename T9 = void,typename T10 = void,
		typename T11 = void,typename T12 = void,typename T13 = void,typename T14 = void,typename T15 = void,
		typename T16 = void,typename T17 = void,typename T18 = void,typename T19 = void,typename T20 = void,
		typename T21 = void,typename T22 = void,typename T23 = void,typename T24 = void,typename T25 = void,
		typename T26 = void,typename T27 = void,typename T28 = void,typename T29 = void,typename T30 = void,
		typename T31 = void,typename T32 = void,typename T33 = void,typename T34 = void,typename T35 = void,
		typename T36 = void,typename T37 = void,typename T38 = void,typename T39 = void,typename T40 = void,
		typename T41 = void,typename T42 = void,typename T43 = void,typename T44 = void,typename T45 = void,
		typename T46 = void,typename T47 = void,typename T48 = void,typename T49 = void,typename T50 = void,
		typename T51 = void,typename T52 = void,typename T53 = void,typename T54 = void,typename T55 = void,
		typename T56 = void,typename T57 = void>
	struct PackTuple;

	template <>
	struct PackTuple<>
	{
		typedef PackTuple<> value_type;

		PackTuple() { ; }
	};
	template <typename T1>
	struct PackTuple<T1>
	{

		typedef LIGHTINK_TYPELIST_1(T1) value_type_list;

		typedef PackTuple<T1> value_type;

		PackTuple(const T1 & _t1) : t1(_t1) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)

	};

	template <typename T1,typename T2>
	struct PackTuple<T1, T2>
	{

		typedef LIGHTINK_TYPELIST_2(T1, T2) value_type_list;

		typedef PackTuple<T1, T2> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2) : t1(_t1), t2(_t2) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)

	};

	template <typename T1,typename T2,typename T3>
	struct PackTuple<T1, T2, T3>
	{

		typedef LIGHTINK_TYPELIST_3(T1, T2, T3) value_type_list;

		typedef PackTuple<T1, T2, T3> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3) : t1(_t1), t2(_t2), t3(_t3) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)

	};

	template <typename T1,typename T2,typename T3,typename T4>
	struct PackTuple<T1, T2, T3, T4>
	{

		typedef LIGHTINK_TYPELIST_4(T1, T2, T3, T4) value_type_list;

		typedef PackTuple<T1, T2, T3, T4> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4) : t1(_t1), t2(_t2), t3(_t3), t4(_t4) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5>
	struct PackTuple<T1, T2, T3, T4, T5>
	{

		typedef LIGHTINK_TYPELIST_5(T1, T2, T3, T4, T5) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
	struct PackTuple<T1, T2, T3, T4, T5, T6>
	{

		typedef LIGHTINK_TYPELIST_6(T1, T2, T3, T4, T5, T6) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7>
	{

		typedef LIGHTINK_TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8>
	{

		typedef LIGHTINK_TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9>
	{

		typedef LIGHTINK_TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>
	{

		typedef LIGHTINK_TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 ;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 ;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11>
	{

		typedef LIGHTINK_TYPELIST_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12>
	{

		typedef LIGHTINK_TYPELIST_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13>
	{

		typedef LIGHTINK_TYPELIST_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14>
	{

		typedef LIGHTINK_TYPELIST_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15>
	{

		typedef LIGHTINK_TYPELIST_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16>
	{

		typedef LIGHTINK_TYPELIST_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17>
	{

		typedef LIGHTINK_TYPELIST_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18>
	{

		typedef LIGHTINK_TYPELIST_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19>
	{

		typedef LIGHTINK_TYPELIST_19(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20>
	{

		typedef LIGHTINK_TYPELIST_20(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 ;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 ;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21>
	{

		typedef LIGHTINK_TYPELIST_21(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22>
	{

		typedef LIGHTINK_TYPELIST_22(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23>
	{

		typedef LIGHTINK_TYPELIST_23(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24>
	{

		typedef LIGHTINK_TYPELIST_24(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25>
	{

		typedef LIGHTINK_TYPELIST_25(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26>
	{

		typedef LIGHTINK_TYPELIST_26(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27>
	{

		typedef LIGHTINK_TYPELIST_27(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28>
	{

		typedef LIGHTINK_TYPELIST_28(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29>
	{

		typedef LIGHTINK_TYPELIST_29(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30>
	{

		typedef LIGHTINK_TYPELIST_30(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 ;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 ;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31>
	{

		typedef LIGHTINK_TYPELIST_31(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32>
	{

		typedef LIGHTINK_TYPELIST_32(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33>
	{

		typedef LIGHTINK_TYPELIST_33(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34>
	{

		typedef LIGHTINK_TYPELIST_34(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35>
	{

		typedef LIGHTINK_TYPELIST_35(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36>
	{

		typedef LIGHTINK_TYPELIST_36(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37>
	{

		typedef LIGHTINK_TYPELIST_37(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38>
	{

		typedef LIGHTINK_TYPELIST_38(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39>
	{

		typedef LIGHTINK_TYPELIST_39(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40>
	{

		typedef LIGHTINK_TYPELIST_40(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 ;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 ;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41>
	{

		typedef LIGHTINK_TYPELIST_41(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42>
	{

		typedef LIGHTINK_TYPELIST_42(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41, const T42 & _t42) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41), t42(_t42) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)
		TYPEAT(42u, t42)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43>
	{

		typedef LIGHTINK_TYPELIST_43(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41, const T42 & _t42, const T43 & _t43) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41), t42(_t42), t43(_t43) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)
		TYPEAT(42u, t42)
		TYPEAT(43u, t43)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44>
	{

		typedef LIGHTINK_TYPELIST_44(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41, const T42 & _t42, const T43 & _t43, const T44 & _t44) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41), t42(_t42), t43(_t43), t44(_t44) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)
		TYPEAT(42u, t42)
		TYPEAT(43u, t43)
		TYPEAT(44u, t44)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45>
	{

		typedef LIGHTINK_TYPELIST_45(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41, const T42 & _t42, const T43 & _t43, const T44 & _t44, const T45 & _t45) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41), t42(_t42), t43(_t43), t44(_t44), t45(_t45) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)
		TYPEAT(42u, t42)
		TYPEAT(43u, t43)
		TYPEAT(44u, t44)
		TYPEAT(45u, t45)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46>
	{

		typedef LIGHTINK_TYPELIST_46(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41, const T42 & _t42, const T43 & _t43, const T44 & _t44, const T45 & _t45, const T46 & _t46) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41), t42(_t42), t43(_t43), t44(_t44), t45(_t45), t46(_t46) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)
		TYPEAT(42u, t42)
		TYPEAT(43u, t43)
		TYPEAT(44u, t44)
		TYPEAT(45u, t45)
		TYPEAT(46u, t46)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47>
	{

		typedef LIGHTINK_TYPELIST_47(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41, const T42 & _t42, const T43 & _t43, const T44 & _t44, const T45 & _t45, const T46 & _t46, const T47 & _t47) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41), t42(_t42), t43(_t43), t44(_t44), t45(_t45), t46(_t46), t47(_t47) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)
		TYPEAT(42u, t42)
		TYPEAT(43u, t43)
		TYPEAT(44u, t44)
		TYPEAT(45u, t45)
		TYPEAT(46u, t46)
		TYPEAT(47u, t47)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48>
	{

		typedef LIGHTINK_TYPELIST_48(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41, const T42 & _t42, const T43 & _t43, const T44 & _t44, const T45 & _t45, const T46 & _t46, const T47 & _t47, const T48 & _t48) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41), t42(_t42), t43(_t43), t44(_t44), t45(_t45), t46(_t46), t47(_t47), t48(_t48) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)
		TYPEAT(42u, t42)
		TYPEAT(43u, t43)
		TYPEAT(44u, t44)
		TYPEAT(45u, t45)
		TYPEAT(46u, t46)
		TYPEAT(47u, t47)
		TYPEAT(48u, t48)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49>
	{

		typedef LIGHTINK_TYPELIST_49(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41, const T42 & _t42, const T43 & _t43, const T44 & _t44, const T45 & _t45, const T46 & _t46, const T47 & _t47, const T48 & _t48, const T49 & _t49) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41), t42(_t42), t43(_t43), t44(_t44), t45(_t45), t46(_t46), t47(_t47), t48(_t48), t49(_t49) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)
		TYPEAT(42u, t42)
		TYPEAT(43u, t43)
		TYPEAT(44u, t44)
		TYPEAT(45u, t45)
		TYPEAT(46u, t46)
		TYPEAT(47u, t47)
		TYPEAT(48u, t48)
		TYPEAT(49u, t49)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50>
	{

		typedef LIGHTINK_TYPELIST_50(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41, const T42 & _t42, const T43 & _t43, const T44 & _t44, const T45 & _t45, const T46 & _t46, const T47 & _t47, const T48 & _t48, const T49 & _t49, const T50 & _t50) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41), t42(_t42), t43(_t43), t44(_t44), t45(_t45), t46(_t46), t47(_t47), t48(_t48), t49(_t49), t50(_t50) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49 << t50 ;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49 >> t50 ;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)
		TYPEAT(42u, t42)
		TYPEAT(43u, t43)
		TYPEAT(44u, t44)
		TYPEAT(45u, t45)
		TYPEAT(46u, t46)
		TYPEAT(47u, t47)
		TYPEAT(48u, t48)
		TYPEAT(49u, t49)
		TYPEAT(50u, t50)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51>
	{

		typedef LIGHTINK_TYPELIST_51(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41, const T42 & _t42, const T43 & _t43, const T44 & _t44, const T45 & _t45, const T46 & _t46, const T47 & _t47, const T48 & _t48, const T49 & _t49, const T50 & _t50, 
		const T51 & _t51) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41), t42(_t42), t43(_t43), t44(_t44), t45(_t45), t46(_t46), t47(_t47), t48(_t48), t49(_t49), t50(_t50), 
		t51(_t51) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49 << t50 \
		 << t51;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49 >> t50 \
		 >> t51;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)
		TYPEAT(42u, t42)
		TYPEAT(43u, t43)
		TYPEAT(44u, t44)
		TYPEAT(45u, t45)
		TYPEAT(46u, t46)
		TYPEAT(47u, t47)
		TYPEAT(48u, t48)
		TYPEAT(49u, t49)
		TYPEAT(50u, t50)
		TYPEAT(51u, t51)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51,typename T52>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51, T52>
	{

		typedef LIGHTINK_TYPELIST_52(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51, T52) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51, T52> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41, const T42 & _t42, const T43 & _t43, const T44 & _t44, const T45 & _t45, const T46 & _t46, const T47 & _t47, const T48 & _t48, const T49 & _t49, const T50 & _t50, 
		const T51 & _t51, const T52 & _t52) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41), t42(_t42), t43(_t43), t44(_t44), t45(_t45), t46(_t46), t47(_t47), t48(_t48), t49(_t49), t50(_t50), 
		t51(_t51), t52(_t52) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49 << t50 \
		 << t51 << t52;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49 >> t50 \
		 >> t51 >> t52;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)
		TYPEAT(42u, t42)
		TYPEAT(43u, t43)
		TYPEAT(44u, t44)
		TYPEAT(45u, t45)
		TYPEAT(46u, t46)
		TYPEAT(47u, t47)
		TYPEAT(48u, t48)
		TYPEAT(49u, t49)
		TYPEAT(50u, t50)
		TYPEAT(51u, t51)
		TYPEAT(52u, t52)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51,typename T52,typename T53>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51, T52, T53>
	{

		typedef LIGHTINK_TYPELIST_53(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51, T52, T53) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51, T52, T53> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41, const T42 & _t42, const T43 & _t43, const T44 & _t44, const T45 & _t45, const T46 & _t46, const T47 & _t47, const T48 & _t48, const T49 & _t49, const T50 & _t50, 
		const T51 & _t51, const T52 & _t52, const T53 & _t53) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41), t42(_t42), t43(_t43), t44(_t44), t45(_t45), t46(_t46), t47(_t47), t48(_t48), t49(_t49), t50(_t50), 
		t51(_t51), t52(_t52), t53(_t53) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49 << t50 \
		 << t51 << t52 << t53;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49 >> t50 \
		 >> t51 >> t52 >> t53;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)
		TYPEAT(42u, t42)
		TYPEAT(43u, t43)
		TYPEAT(44u, t44)
		TYPEAT(45u, t45)
		TYPEAT(46u, t46)
		TYPEAT(47u, t47)
		TYPEAT(48u, t48)
		TYPEAT(49u, t49)
		TYPEAT(50u, t50)
		TYPEAT(51u, t51)
		TYPEAT(52u, t52)
		TYPEAT(53u, t53)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51,typename T52,typename T53,typename T54>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51, T52, T53, T54>
	{

		typedef LIGHTINK_TYPELIST_54(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51, T52, T53, T54) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51, T52, T53, T54> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41, const T42 & _t42, const T43 & _t43, const T44 & _t44, const T45 & _t45, const T46 & _t46, const T47 & _t47, const T48 & _t48, const T49 & _t49, const T50 & _t50, 
		const T51 & _t51, const T52 & _t52, const T53 & _t53, const T54 & _t54) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41), t42(_t42), t43(_t43), t44(_t44), t45(_t45), t46(_t46), t47(_t47), t48(_t48), t49(_t49), t50(_t50), 
		t51(_t51), t52(_t52), t53(_t53), t54(_t54) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49 << t50 \
		 << t51 << t52 << t53 << t54;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49 >> t50 \
		 >> t51 >> t52 >> t53 >> t54;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)
		TYPEAT(42u, t42)
		TYPEAT(43u, t43)
		TYPEAT(44u, t44)
		TYPEAT(45u, t45)
		TYPEAT(46u, t46)
		TYPEAT(47u, t47)
		TYPEAT(48u, t48)
		TYPEAT(49u, t49)
		TYPEAT(50u, t50)
		TYPEAT(51u, t51)
		TYPEAT(52u, t52)
		TYPEAT(53u, t53)
		TYPEAT(54u, t54)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51,typename T52,typename T53,typename T54,typename T55>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51, T52, T53, T54, T55>
	{

		typedef LIGHTINK_TYPELIST_55(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51, T52, T53, T54, T55) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51, T52, T53, T54, T55> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41, const T42 & _t42, const T43 & _t43, const T44 & _t44, const T45 & _t45, const T46 & _t46, const T47 & _t47, const T48 & _t48, const T49 & _t49, const T50 & _t50, 
		const T51 & _t51, const T52 & _t52, const T53 & _t53, const T54 & _t54, const T55 & _t55) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41), t42(_t42), t43(_t43), t44(_t44), t45(_t45), t46(_t46), t47(_t47), t48(_t48), t49(_t49), t50(_t50), 
		t51(_t51), t52(_t52), t53(_t53), t54(_t54), t55(_t55) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49 << t50 \
		 << t51 << t52 << t53 << t54 << t55;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49 >> t50 \
		 >> t51 >> t52 >> t53 >> t54 >> t55;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)
		TYPEAT(42u, t42)
		TYPEAT(43u, t43)
		TYPEAT(44u, t44)
		TYPEAT(45u, t45)
		TYPEAT(46u, t46)
		TYPEAT(47u, t47)
		TYPEAT(48u, t48)
		TYPEAT(49u, t49)
		TYPEAT(50u, t50)
		TYPEAT(51u, t51)
		TYPEAT(52u, t52)
		TYPEAT(53u, t53)
		TYPEAT(54u, t54)
		TYPEAT(55u, t55)

	};

	template <typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51,typename T52,typename T53,typename T54,typename T55,typename T56>
	struct PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51, T52, T53, T54, T55, T56>
	{

		typedef LIGHTINK_TYPELIST_56(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51, T52, T53, T54, T55, T56) value_type_list;

		typedef PackTuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
		T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, 
		T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, 
		T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, 
		T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, 
		T51, T52, T53, T54, T55, T56> value_type;

		PackTuple(const T1 & _t1, const T2 & _t2, const T3 & _t3, const T4 & _t4, const T5 & _t5, const T6 & _t6, const T7 & _t7, const T8 & _t8, const T9 & _t9, const T10 & _t10, 
		const T11 & _t11, const T12 & _t12, const T13 & _t13, const T14 & _t14, const T15 & _t15, const T16 & _t16, const T17 & _t17, const T18 & _t18, const T19 & _t19, const T20 & _t20, 
		const T21 & _t21, const T22 & _t22, const T23 & _t23, const T24 & _t24, const T25 & _t25, const T26 & _t26, const T27 & _t27, const T28 & _t28, const T29 & _t29, const T30 & _t30, 
		const T31 & _t31, const T32 & _t32, const T33 & _t33, const T34 & _t34, const T35 & _t35, const T36 & _t36, const T37 & _t37, const T38 & _t38, const T39 & _t39, const T40 & _t40, 
		const T41 & _t41, const T42 & _t42, const T43 & _t43, const T44 & _t44, const T45 & _t45, const T46 & _t46, const T47 & _t47, const T48 & _t48, const T49 & _t49, const T50 & _t50, 
		const T51 & _t51, const T52 & _t52, const T53 & _t53, const T54 & _t54, const T55 & _t55, const T56 & _t56) : t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7), t8(_t8), t9(_t9), t10(_t10), 
		t11(_t11), t12(_t12), t13(_t13), t14(_t14), t15(_t15), t16(_t16), t17(_t17), t18(_t18), t19(_t19), t20(_t20), 
		t21(_t21), t22(_t22), t23(_t23), t24(_t24), t25(_t25), t26(_t26), t27(_t27), t28(_t28), t29(_t29), t30(_t30), 
		t31(_t31), t32(_t32), t33(_t33), t34(_t34), t35(_t35), t36(_t36), t37(_t37), t38(_t38), t39(_t39), t40(_t40), 
		t41(_t41), t42(_t42), t43(_t43), t44(_t44), t45(_t45), t46(_t46), t47(_t47), t48(_t48), t49(_t49), t50(_t50), 
		t51(_t51), t52(_t52), t53(_t53), t54(_t54), t55(_t55), t56(_t56) {;}

		template <typename TBuffer>
		RuntimeError pack(TBuffer buffer)
		{ try {buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49 << t50 \
		 << t51 << t52 << t53 << t54 << t55 << t56;} catch (RuntimeError & e) { return e; } return RE_Success; }

		template <typename TBuffer>
		RuntimeError unpack(TBuffer buffer)
		{ try {buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49 >> t50 \
		 >> t51 >> t52 >> t53 >> t54 >> t55 >> t56;} catch (RuntimeError & e) { return e; } return RE_Success; }

		PackTuple() { ; }

		template<unsigned int N>
		typename TypeListAt<value_type_list, N>::Result & get();

		TYPEAT(1u, t1)
		TYPEAT(2u, t2)
		TYPEAT(3u, t3)
		TYPEAT(4u, t4)
		TYPEAT(5u, t5)
		TYPEAT(6u, t6)
		TYPEAT(7u, t7)
		TYPEAT(8u, t8)
		TYPEAT(9u, t9)
		TYPEAT(10u, t10)
		TYPEAT(11u, t11)
		TYPEAT(12u, t12)
		TYPEAT(13u, t13)
		TYPEAT(14u, t14)
		TYPEAT(15u, t15)
		TYPEAT(16u, t16)
		TYPEAT(17u, t17)
		TYPEAT(18u, t18)
		TYPEAT(19u, t19)
		TYPEAT(20u, t20)
		TYPEAT(21u, t21)
		TYPEAT(22u, t22)
		TYPEAT(23u, t23)
		TYPEAT(24u, t24)
		TYPEAT(25u, t25)
		TYPEAT(26u, t26)
		TYPEAT(27u, t27)
		TYPEAT(28u, t28)
		TYPEAT(29u, t29)
		TYPEAT(30u, t30)
		TYPEAT(31u, t31)
		TYPEAT(32u, t32)
		TYPEAT(33u, t33)
		TYPEAT(34u, t34)
		TYPEAT(35u, t35)
		TYPEAT(36u, t36)
		TYPEAT(37u, t37)
		TYPEAT(38u, t38)
		TYPEAT(39u, t39)
		TYPEAT(40u, t40)
		TYPEAT(41u, t41)
		TYPEAT(42u, t42)
		TYPEAT(43u, t43)
		TYPEAT(44u, t44)
		TYPEAT(45u, t45)
		TYPEAT(46u, t46)
		TYPEAT(47u, t47)
		TYPEAT(48u, t48)
		TYPEAT(49u, t49)
		TYPEAT(50u, t50)
		TYPEAT(51u, t51)
		TYPEAT(52u, t52)
		TYPEAT(53u, t53)
		TYPEAT(54u, t54)
		TYPEAT(55u, t55)
		TYPEAT(56u, t56)

	};

}
#undef TYPEAT

#endif