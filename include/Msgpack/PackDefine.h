


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
#ifndef LIGHTINK_MSGPACK_PACKDEFINE_H_
#define LIGHTINK_MSGPACK_PACKDEFINE_H_
#include "DataBuffer.h"
#include "PackBuffer.h"
#include "Packer.h"
#include "Unpacker.h"

/*
--lua生成
function create_packargs(count)
	local allStr = "template <typename TBuffer>\ninline RuntimeError pack_args(TBuffer & buffer)\n{ return RE_Success; }\ntemplate <typename TBuffer>\ninline RuntimeError unpack_args(const TBuffer & buffer)\n{ return RE_Success; }\n\n"
	for k = 1, count do
		local str = "template <typename TBuffer, "
		local str1 = "inline RuntimeError pack_args(TBuffer & buffer, "
		local str2 = "{try { buffer"
		local str3 = "template <typename TBuffer, "
		local str4 = "inline RuntimeError unpack_args(TBuffer & buffer, "
		local str5 = "{try { buffer"
		for ki = 1, k do
			if ki % 10 == 0 then
				str = string.format("%stypename T%d, \n\t", str, ki)
				str1 = string.format("%sconst T%d & t%d, \n\t", str1, ki, ki)
				str2 = string.format("%s << t%d \\\n\t", str2, ki)
				str3 = string.format("%stypename T%d, \n\t", str3, ki)
				str4 = string.format("%sT%d & t%d, \n\t", str4, ki, ki)
				str5 = string.format("%s >> t%d \\\n\t", str5, ki)
			else
				str = string.format("%stypename T%d,", str, ki)
				str1 = string.format("%sconst T%d & t%d, ", str1, ki, ki)
				str2 = string.format("%s << t%d", str2, ki)
				str3 = string.format("%stypename T%d,", str3, ki)
				str4 = string.format("%sT%d & t%d, ", str4, ki, ki)
				str5 = string.format("%s >> t%d", str5, ki)
			end

		end
		local idx = string.find(str, ",", #str - 4)
		str = string.sub(str, 0, idx - 1) .. ">\n"
		idx = string.find(str1, ",", #str1 - 4)
		str1 = string.sub(str1, 0, idx - 1) .. ")\n"
		idx = string.find(str3, ",", #str3 - 4)
		str3 = string.sub(str3, 0, idx - 1) .. ">\n"
		idx = string.find(str4, ",", #str4 - 4)
		str4 = string.sub(str4, 0, idx - 1) .. ")\n"

		idx = string.find(str2, "\\", #str2 - 4)
		if idx then
			str2 = string.sub(str2, 0, idx - 1)
		end
		str2 = str2 .. ";} catch (RuntimeError & e) { return e; } return RE_Success; }\n"

		idx = string.find(str5, "\\", #str5 - 4)
		if idx then
			str5 = string.sub(str5, 0, idx - 1)
		end
		str5 = str5 .. ";} catch (RuntimeError & e) { return e; } return RE_Success; }\n"

		allStr = allStr .. str .. str1 .. str2 .. str3 .. str4 .. str5 .. "\n"

	end

	print(allStr)
end

create_packargs(56)
*/

#define LIGHTINK_MSGPACK_DEFINE(...) \
	public: \
		template <typename TBuffer> \
		::LightInk::RuntimeError pack(TBuffer & buffer) const \
		{ \
			return ::LightInk::pack_args(buffer, ##__VA_ARGS__); \
		} \
		template <typename TBuffer> \
		::LightInk::RuntimeError unpack(TBuffer & buffer) \
		{ \
			return ::LightInk::unpack_args(buffer, ##__VA_ARGS__); \
		} 

namespace LightInk
{
	template <typename TBuffer>
	inline RuntimeError pack_args(TBuffer & buffer)
	{ return RE_Success; }
	template <typename TBuffer>
	inline RuntimeError unpack_args(const TBuffer & buffer)
	{ return RE_Success; }

	template <typename TBuffer, typename T1>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1)
	{try { buffer << t1;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1)
	{try { buffer >> t1;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2)
	{try { buffer << t1 << t2;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2)
	{try { buffer >> t1 >> t2;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3)
	{try { buffer << t1 << t2 << t3;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3)
	{try { buffer >> t1 >> t2 >> t3;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4)
	{try { buffer << t1 << t2 << t3 << t4;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4)
	{try { buffer >> t1 >> t2 >> t3 >> t4;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5)
	{try { buffer << t1 << t2 << t3 << t4 << t5;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 ;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 ;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 ;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 ;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 ;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 ;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 ;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 ;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41, const T42 & t42)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41, T42 & t42)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41, const T42 & t42, const T43 & t43)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41, T42 & t42, T43 & t43)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41, const T42 & t42, const T43 & t43, const T44 & t44)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41, T42 & t42, T43 & t43, T44 & t44)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41, const T42 & t42, const T43 & t43, const T44 & t44, const T45 & t45)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41, T42 & t42, T43 & t43, T44 & t44, T45 & t45)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41, const T42 & t42, const T43 & t43, const T44 & t44, const T45 & t45, const T46 & t46)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41, T42 & t42, T43 & t43, T44 & t44, T45 & t45, T46 & t46)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41, const T42 & t42, const T43 & t43, const T44 & t44, const T45 & t45, const T46 & t46, const T47 & t47)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41, T42 & t42, T43 & t43, T44 & t44, T45 & t45, T46 & t46, T47 & t47)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41, const T42 & t42, const T43 & t43, const T44 & t44, const T45 & t45, const T46 & t46, const T47 & t47, const T48 & t48)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41, T42 & t42, T43 & t43, T44 & t44, T45 & t45, T46 & t46, T47 & t47, T48 & t48)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41, const T42 & t42, const T43 & t43, const T44 & t44, const T45 & t45, const T46 & t46, const T47 & t47, const T48 & t48, const T49 & t49)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41, T42 & t42, T43 & t43, T44 & t44, T45 & t45, T46 & t46, T47 & t47, T48 & t48, T49 & t49)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41, const T42 & t42, const T43 & t43, const T44 & t44, const T45 & t45, const T46 & t46, const T47 & t47, const T48 & t48, const T49 & t49, const T50 & t50)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49 << t50 ;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41, T42 & t42, T43 & t43, T44 & t44, T45 & t45, T46 & t46, T47 & t47, T48 & t48, T49 & t49, T50 & t50)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49 >> t50 ;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41, const T42 & t42, const T43 & t43, const T44 & t44, const T45 & t45, const T46 & t46, const T47 & t47, const T48 & t48, const T49 & t49, const T50 & t50, 
		const T51 & t51)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49 << t50 \
		 << t51;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41, T42 & t42, T43 & t43, T44 & t44, T45 & t45, T46 & t46, T47 & t47, T48 & t48, T49 & t49, T50 & t50, 
		T51 & t51)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49 >> t50 \
		 >> t51;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51,typename T52>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41, const T42 & t42, const T43 & t43, const T44 & t44, const T45 & t45, const T46 & t46, const T47 & t47, const T48 & t48, const T49 & t49, const T50 & t50, 
		const T51 & t51, const T52 & t52)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49 << t50 \
		 << t51 << t52;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51,typename T52>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41, T42 & t42, T43 & t43, T44 & t44, T45 & t45, T46 & t46, T47 & t47, T48 & t48, T49 & t49, T50 & t50, 
		T51 & t51, T52 & t52)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49 >> t50 \
		 >> t51 >> t52;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51,typename T52,typename T53>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41, const T42 & t42, const T43 & t43, const T44 & t44, const T45 & t45, const T46 & t46, const T47 & t47, const T48 & t48, const T49 & t49, const T50 & t50, 
		const T51 & t51, const T52 & t52, const T53 & t53)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49 << t50 \
		 << t51 << t52 << t53;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51,typename T52,typename T53>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41, T42 & t42, T43 & t43, T44 & t44, T45 & t45, T46 & t46, T47 & t47, T48 & t48, T49 & t49, T50 & t50, 
		T51 & t51, T52 & t52, T53 & t53)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49 >> t50 \
		 >> t51 >> t52 >> t53;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51,typename T52,typename T53,typename T54>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41, const T42 & t42, const T43 & t43, const T44 & t44, const T45 & t45, const T46 & t46, const T47 & t47, const T48 & t48, const T49 & t49, const T50 & t50, 
		const T51 & t51, const T52 & t52, const T53 & t53, const T54 & t54)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49 << t50 \
		 << t51 << t52 << t53 << t54;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51,typename T52,typename T53,typename T54>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41, T42 & t42, T43 & t43, T44 & t44, T45 & t45, T46 & t46, T47 & t47, T48 & t48, T49 & t49, T50 & t50, 
		T51 & t51, T52 & t52, T53 & t53, T54 & t54)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49 >> t50 \
		 >> t51 >> t52 >> t53 >> t54;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51,typename T52,typename T53,typename T54,typename T55>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41, const T42 & t42, const T43 & t43, const T44 & t44, const T45 & t45, const T46 & t46, const T47 & t47, const T48 & t48, const T49 & t49, const T50 & t50, 
		const T51 & t51, const T52 & t52, const T53 & t53, const T54 & t54, const T55 & t55)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49 << t50 \
		 << t51 << t52 << t53 << t54 << t55;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51,typename T52,typename T53,typename T54,typename T55>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41, T42 & t42, T43 & t43, T44 & t44, T45 & t45, T46 & t46, T47 & t47, T48 & t48, T49 & t49, T50 & t50, 
		T51 & t51, T52 & t52, T53 & t53, T54 & t54, T55 & t55)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49 >> t50 \
		 >> t51 >> t52 >> t53 >> t54 >> t55;} catch (RuntimeError & e) { return e; } return RE_Success; }

	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51,typename T52,typename T53,typename T54,typename T55,typename T56>
	inline RuntimeError pack_args(TBuffer & buffer, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9, const T10 & t10, 
		const T11 & t11, const T12 & t12, const T13 & t13, const T14 & t14, const T15 & t15, const T16 & t16, const T17 & t17, const T18 & t18, const T19 & t19, const T20 & t20, 
		const T21 & t21, const T22 & t22, const T23 & t23, const T24 & t24, const T25 & t25, const T26 & t26, const T27 & t27, const T28 & t28, const T29 & t29, const T30 & t30, 
		const T31 & t31, const T32 & t32, const T33 & t33, const T34 & t34, const T35 & t35, const T36 & t36, const T37 & t37, const T38 & t38, const T39 & t39, const T40 & t40, 
		const T41 & t41, const T42 & t42, const T43 & t43, const T44 & t44, const T45 & t45, const T46 & t46, const T47 & t47, const T48 & t48, const T49 & t49, const T50 & t50, 
		const T51 & t51, const T52 & t52, const T53 & t53, const T54 & t54, const T55 & t55, const T56 & t56)
	{try { buffer << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 \
		 << t11 << t12 << t13 << t14 << t15 << t16 << t17 << t18 << t19 << t20 \
		 << t21 << t22 << t23 << t24 << t25 << t26 << t27 << t28 << t29 << t30 \
		 << t31 << t32 << t33 << t34 << t35 << t36 << t37 << t38 << t39 << t40 \
		 << t41 << t42 << t43 << t44 << t45 << t46 << t47 << t48 << t49 << t50 \
		 << t51 << t52 << t53 << t54 << t55 << t56;} catch (RuntimeError & e) { return e; } return RE_Success; }
	template <typename TBuffer, typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, 
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20, 
		typename T21,typename T22,typename T23,typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,typename T30, 
		typename T31,typename T32,typename T33,typename T34,typename T35,typename T36,typename T37,typename T38,typename T39,typename T40, 
		typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, 
		typename T51,typename T52,typename T53,typename T54,typename T55,typename T56>
	inline RuntimeError unpack_args(TBuffer & buffer, T1 & t1, T2 & t2, T3 & t3, T4 & t4, T5 & t5, T6 & t6, T7 & t7, T8 & t8, T9 & t9, T10 & t10, 
		T11 & t11, T12 & t12, T13 & t13, T14 & t14, T15 & t15, T16 & t16, T17 & t17, T18 & t18, T19 & t19, T20 & t20, 
		T21 & t21, T22 & t22, T23 & t23, T24 & t24, T25 & t25, T26 & t26, T27 & t27, T28 & t28, T29 & t29, T30 & t30, 
		T31 & t31, T32 & t32, T33 & t33, T34 & t34, T35 & t35, T36 & t36, T37 & t37, T38 & t38, T39 & t39, T40 & t40, 
		T41 & t41, T42 & t42, T43 & t43, T44 & t44, T45 & t45, T46 & t46, T47 & t47, T48 & t48, T49 & t49, T50 & t50, 
		T51 & t51, T52 & t52, T53 & t53, T54 & t54, T55 & t55, T56 & t56)
	{try { buffer >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10 \
		 >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 \
		 >> t21 >> t22 >> t23 >> t24 >> t25 >> t26 >> t27 >> t28 >> t29 >> t30 \
		 >> t31 >> t32 >> t33 >> t34 >> t35 >> t36 >> t37 >> t38 >> t39 >> t40 \
		 >> t41 >> t42 >> t43 >> t44 >> t45 >> t46 >> t47 >> t48 >> t49 >> t50 \
		 >> t51 >> t52 >> t53 >> t54 >> t55 >> t56;} catch (RuntimeError & e) { return e; } return RE_Success; }



}

#endif