


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
#ifndef LIGHTINK_LUAENGINE_LUADEBUGGER_H_
#define LIGHTINK_LUAENGINE_LUADEBUGGER_H_

#include "Common/Type.h"
#include "Common/STLType.h"
#include "LuaEngine/LuaLib.h"

namespace LightInk
{
	class LIGHTINK_DECL LuaDebugger
	{
	public :
		LuaDebugger();
		virtual ~LuaDebugger();

		void init(lua_State * L);
		void release();

		bool debug();

		void print_frame_info(int32 level) const;
		void print_all_frame_info() const;
		void print_line(int32 level, int32 currentLine, int32 lineCount) const;

		bool is_valid() const;

		bool is_enable_ldb() const;
		void enable_ldb(bool enable);

		bool is_enable_error_catch() const;
		void enable_error_catch(bool enable);

		void catch_lua_error();

		static bool get_frame_info(lua_State * L, int32 level, string & info);
		static void get_all_frame_info(lua_State * L, string & info);

		bool has_break_point() const;
		bool add_break_point(const string & point);
		bool del_break_point(const string & index);
		bool list_break_points(string & points);

		bool reload_file(const string & path);

		bool next_call();
		bool step_call();
		bool over_call();

		bool list_code(const char * path, int32 currentLine, int32 lineCount, string & code) const;

		bool set_frame(int32 level);

		bool print_var(const string & varName) const;

		bool exec_code(const string & code) const;

		static int print_table(lua_State * L);
		static int ldb(lua_State * L);
		static LuaDebugger * get_lua_debugger(lua_State * L);

	protected:
		bool block_read_cmd();
		static void trim_string(string & str);

		bool push_var(lua_Debug * ld, const string & varName) const;
		bool push_sub_var(lua_Debug * ld, size_t startIdx, const vector<string>::type & fields) const;
		bool split_fields(const string & varName, vector<string>::type & fields) const;

		int32 get_current_line(int32 level) const;
		int32 get_stack_level() const;
		bool get_current_source(int32 level, string & source) const;

		bool check_line_point(lua_Debug * ld) const;
		bool check_func_point(lua_Debug * ld) const;

		static void debug_hook(lua_State * L, lua_Debug * ld);

		void reset_coroutine(lua_State * L);

		lua_State * get_lua() const;
		lua_State * get_main_lua() const;

		int32 get_running_level() const;
		void set_running_level(int32 level);

		int32 get_break_level() const;
		void set_break_level(int32 level);

		int32 get_current_level() const;
		void set_current_level(int32 level);

	protected:
		struct BreakPoint
		{
			string m_name;
			int32 m_lineNum;

			BreakPoint();

			bool set_func_point(const string & funcName);
			bool set_line_point(const string & fileName, int32 lineNum);
			bool is_function() const;
			bool operator== (const BreakPoint & right) const;
		};

	protected:
		bool m_enableLdb;
		bool m_enableErrorCatch;
		bool m_debuggerRunning;

		lua_State * m_lua;
		int32 m_runningLevel;
		int32 m_breakLevel;
		int32 m_currentLevel;

		lua_State * m_coroutine;
		int32 m_coRunningLevel;
		int32 m_coBreakLevel;
		int32 m_coCurrentLevel;

		string m_cmd;
		string m_lastCmd;
		vector<BreakPoint>::type m_breakPoints;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline bool LuaDebugger::is_valid() const
	{
		return m_lua != NULL && (lua_gethook(m_lua) == NULL || lua_gethook(m_lua) == LuaDebugger::debug_hook);
	}
	inline bool LuaDebugger::is_enable_ldb() const
	{
		return m_enableLdb;
	}
	inline bool LuaDebugger::is_enable_error_catch() const
	{
		return m_enableErrorCatch;
	}
	inline bool LuaDebugger::has_break_point() const
	{
		return !m_breakPoints.empty();
	}
	inline lua_State * LuaDebugger::get_lua() const
	{
		return m_coroutine == NULL ? m_lua : m_coroutine;
	}
	inline lua_State * LuaDebugger::get_main_lua() const
	{
		return m_lua;
	}
	inline int32 LuaDebugger::get_running_level() const
	{
		return m_coroutine == NULL ? m_runningLevel : m_coRunningLevel;
	}
	inline void LuaDebugger::set_running_level(int32 level)
	{
		if (m_coroutine == NULL)
			m_runningLevel = level;
		else
			m_coRunningLevel = level;
	}
	inline int32 LuaDebugger::get_break_level() const
	{
		return m_coroutine == NULL ? m_breakLevel : m_coBreakLevel;
	}
	inline void LuaDebugger::set_break_level(int32 level)
	{
		if (m_coroutine == NULL)
			m_breakLevel = level;
		else
			m_coBreakLevel = level;
	}
	inline int32 LuaDebugger::get_current_level() const
	{
		return m_coroutine == NULL ? m_currentLevel : m_coCurrentLevel;
	}
	inline void LuaDebugger::set_current_level(int32 level)
	{
		if (m_coroutine == NULL)
			m_currentLevel = level;
		else
			m_coCurrentLevel = level;
	}

	inline bool LuaDebugger::BreakPoint::is_function() const
	{
		return m_lineNum == -1;
	}
}

#endif