


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

#include <stdlib.h>
#include <algorithm>
#include "Log/Log.h"
#include "LuaEngine/LuaDebugger.h"
#include "LuaEngine/LuaStateProtect.h"
#include "LuaEngine/LuaIterator.h"

namespace LightInk
{
	template <typename T>
	inline T Max(T t1, T t2) { return (t1 < t2 ? t2 : t1); }

	template <typename T>
	inline T Min(T t1, T t2) { return (t1 > t2 ? t2 : t1); }

	LuaDebugger::LuaDebugger() : 
		m_enableLdb(true),
		m_enableErrorCatch(true),
		m_debuggerRunning(false),
		m_lua(NULL),
		m_runningLevel(-1),
		m_breakLevel(-1),
		m_currentLevel(0),
		m_coroutine(NULL),
		m_coRunningLevel(-1),
		m_coBreakLevel(-1),
		m_coCurrentLevel(0)
	{
		LogTraceStepCall("LuaDebugger::LuaDebugger()");
		LogTraceStepReturnVoid;
	}

	LuaDebugger::~LuaDebugger()
	{
		LogTraceStepCall("LuaDebugger::~LuaDebugger()");
		release();
		LogTraceStepReturnVoid;
	}

	void LuaDebugger::init(lua_State * L)
	{
		LogTraceStepCall("void LuaDebugger::init(lua_State * L)");
		release();
		m_lua = L;
		lua_pushlightuserdata(m_lua, (void *)LuaDebugger::get_lua_debugger);
		lua_pushlightuserdata(m_lua, this);
		lua_rawset(m_lua, LUA_REGISTRYINDEX);
		lua_pushcclosure(L, LuaDebugger::ldb, 0);
		lua_setglobal(L, "ldb");
		m_runningLevel = -1;
		m_breakLevel = -1;
		m_currentLevel = 0;
		m_coroutine = NULL;
		m_coRunningLevel = -1;
		m_coBreakLevel = -1;
		m_coCurrentLevel = 0;
		LogTraceStepReturnVoid;
	}

	void LuaDebugger::release()
	{
		LogTraceStepCall("void LuaDebugger::release()");
		if (is_valid())
		{
			lua_sethook(m_lua, LuaDebugger::debug_hook, 0, 0);
			m_lua = NULL;
		}
		LogTraceStepReturnVoid;
	}

	bool LuaDebugger::debug()
	{
		LogTraceStepCall("bool LuaDebugger::debug()");
		if (!is_valid() || !is_enable_ldb() || m_debuggerRunning)
			LogTraceStepReturn(false);
		m_debuggerRunning = true;

		set_current_level(0);
		print_line(get_current_level(), get_current_line(get_current_level()), 1);

		while (block_read_cmd())
		{
			trim_string(m_cmd);
			if (m_cmd.empty())
			{
				m_cmd = m_lastCmd;
			}
			size_t idx = m_cmd.find_first_of(" \t\n");
			string cmd, param;
			if (idx != string::npos)
			{
				cmd = m_cmd.substr(0u, idx);
				param = m_cmd.substr(idx);
				trim_string(param);
			}
			else
			{
				cmd = m_cmd;
			}
			if (cmd.empty())
				continue;
			m_lastCmd = m_cmd;
			if (cmd == "help")
			{
				const char * helpInfo = "\n"
					"offldb/ol                  disable ldb\n"
					"offerr/oe                  disable catch error\n"
					"frame/f n                  goto n stack level\n"
					"up/u                       up stack level\n"
					"down/d                     down stack level\n"
					"backtrace/bt               show backtrace\n"
					"listbreak/lb               show break point list\n"
					"break/b func/file:line     add break point,func means function point，file:line means line point\n"
					"delbreak/db n              delete n index break point\n"
					"reload/rl file             realod a file\n"
					"next/n                     next call, will skip sub function\n"
					"step/s                     step call, will into sub function\n"
					"over/o                     step over, will skip out current function\n"
					"continue/c                 continue run\n"
					"list/l n                   list near n line code, normal 16 line\n"
					"print/p var                print var value\n"
					"exec/e code                exec code\n"
					;
				LogMessage(helpInfo);
			}
			else if (cmd == "offldb" || cmd == "ol")
			{
				enable_ldb(false);
			}
			else if (cmd == "offerr" || cmd == "oe")
			{
				enable_error_catch(false);
			}
			else if (cmd == "frame" || cmd == "f")
			{
				int32 index = atoi(param.c_str());
				if (!set_frame(index))
					LogScriptError(get_main_lua(), "set frame failed!!!");
				print_frame_info(get_current_level());
				print_line(get_current_level(), get_current_line(get_current_level()), 1);
			}
			else if (cmd == "up" || cmd == "u")
			{
				if (!set_frame(get_current_level() + 1))
					LogScriptError(get_main_lua(), "set frame failed!!!");
				print_frame_info(get_current_level());
				print_line(get_current_level(), get_current_line(get_current_level()), 1);
			}
			else if (cmd == "down" || cmd == "d")
			{
				if (!set_frame(get_current_level() - 1))
					LogScriptError(get_main_lua(), "set frame failed!!!");
				print_frame_info(get_current_level());
				print_line(get_current_level(), get_current_line(get_current_level()), 1);
			}
			else if (cmd == "backtrace" || cmd == "bt")
			{
				print_all_frame_info();
			}
			else if (cmd == "listbreak" || cmd == "lb")
			{
				string points;
				if (list_break_points(points))
					LogScriptMessage(points);
				else
					LogScriptMessage("Not found any break point!!!");
			}
			else if (cmd == "break" || cmd == "b")
			{
				if (!add_break_point(param))
					LogScriptError(get_main_lua(), "Add break point `{}` failed!!!", param);
			}
			else if (cmd == "delbreak" || cmd == "db")
			{
				if (!del_break_point(param))
					LogScriptError(get_main_lua(), "Del break point `{}` failed!!!", param);
			}
			else if (cmd == "reload" || cmd == "rl")
			{
				if (!reload_file(param))
					LogScriptError(get_main_lua(), "Reload file `{}` failed!!!", param);
			}
			else if (cmd == "next" || cmd == "n")
			{
				if (next_call())
					break;
				else
					LogScriptError(get_main_lua(), "Next call failed!!!");
			}
			else if (cmd == "step" || cmd == "s")
			{
				if (step_call())
					break;
				else
					LogScriptError(get_main_lua(), "Step call failed!!!");
			}
			else if (cmd == "over" || cmd == "o")
			{
				if (over_call())
					break;
				else
					LogScriptError(get_main_lua(), "Over call failed!!!");
			}
			else if (cmd == "continue" || cmd == "c")
			{
				if (has_break_point())
					lua_sethook(get_main_lua(), LuaDebugger::debug_hook, LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE, 0);

				// close all next and setp and over
				set_running_level(get_stack_level());
				set_break_level(-1);
				break;
			}
			else if (cmd == "list" || cmd == "l")
			{
				int32 lineCount = atoi(param.c_str());
				if (lineCount <= 0)
					lineCount = 16;
				print_line(get_current_level(), get_current_line(get_current_level()), lineCount);
			}
			else if (cmd == "print" || cmd == "p")
			{
				print_var(param);
			}
			else if (cmd == "exec" || cmd == "e")
			{
				exec_code(param);
			}
			else
			{
				LogScriptError(get_main_lua(), "Invalid ldb cmd `{}`", cmd);
			}
		}
		m_debuggerRunning = false;
		LogTraceStepReturn(true);
	}

	void LuaDebugger::print_frame_info(int32 level) const
	{
		LogTraceStepCall("void LuaDebugger::print_frame_info(int32 level) const");
		string info;
		if (get_frame_info(get_lua(), level, info))
			LogScriptMessage(info);
		LogTraceStepReturnVoid;
	}

	void LuaDebugger::print_all_frame_info() const
	{
		LogTraceStepCall("void LuaDebugger::print_all_frame_info() const");
		string info;
		get_all_frame_info(get_lua(), info);
		if (!info.empty())
			LogScriptMessage(info);
		LogTraceStepReturnVoid;
	}

	void LuaDebugger::print_line(int32 level, int32 currentLine, int32 lineCount) const
	{
		LogTraceStepCall("void LuaDebugger::print_line(int32 level, int32 currentLine, int32 lineCount) const");
		string source;
		if (get_current_source(level, source))
		{
			string info;
			if (list_code(source.c_str(), currentLine, lineCount, info))
			{
				LogScriptMessage(info);
			}
		}
		LogTraceStepReturnVoid;
	}

	void LuaDebugger::enable_ldb(bool enable)
	{
		LogTraceStepCall("void LuaDebugger::enable_ldb(bool enable)");
		if (is_valid())
			lua_sethook(get_main_lua(), LuaDebugger::debug_hook, 0, 0);
		m_enableLdb = enable;
		LogTraceStepReturnVoid;
	}

	void LuaDebugger::enable_error_catch(bool enable)
	{
		LogTraceStepCall("void LuaDebugger::enable_error_catch(bool enable)");
		if (is_valid())
			lua_sethook(get_main_lua(), LuaDebugger::debug_hook, 0, 0);
		m_enableErrorCatch = enable;
		LogTraceStepReturnVoid;
	}

	void LuaDebugger::catch_lua_error()
	{
		LogTraceStepCall("void LuaDebugger::catch_lua_error()");
		if (is_valid() && is_enable_error_catch())
		{
			debug();
		}
		LogTraceStepReturnVoid;
	}

	bool LuaDebugger::get_frame_info(lua_State * L, int32 level, string & info)
	{
		LogTraceStepCall("bool LuaDebugger::get_frame_info(lua_State * L, int32 level, string & info)");
		lua_Debug ld;
		if (!lua_getstack(L, level, &ld))
			LogTraceStepReturn(false);
		fmt::MemoryWriter buffer;
		lua_getinfo(L, "nSl", &ld);

		buffer.write("#{} {}", level, ld.source);
		if (ld.currentline >= 0)
			buffer.write(" : {}", ld.currentline);
		if (ld.name != NULL)
			buffer.write(" | ({} {} `{}`)\n", ld.namewhat, ld.what, ld.name);
		else
			buffer.write(" | (trunk)\n");
		info.append(buffer.data(), buffer.size());
		LogTraceStepReturn(true);
	}

	void LuaDebugger::get_all_frame_info(lua_State * L, string & info)
	{
		LogTraceStepCall("void LuaDebugger::get_all_frame_info(lua_State * L, string & info)");
		for (int i = 0; get_frame_info(L, i, info); i++)
		{ ; }
		LogTraceStepReturnVoid;
	}

	bool LuaDebugger::add_break_point(const string & point)
	{
		LogTraceStepCall("bool LuaDebugger::add_break_point(const string & point)");
		if (point.empty())
			LogTraceStepReturn(false);
		BreakPoint bp;
		size_t idx = point.find(':');
		if (idx < point.size()) // file name and line
		{
			string fileName = point.substr(0u, idx);
			string fileLine = point.substr(idx + 1);
			trim_string(fileName);
			trim_string(fileLine);
			if (fileName.empty() || fileLine.empty())
				LogTraceStepReturn(false);
			int32 line = atoi(fileLine.c_str());
			if (!bp.set_line_point(fileName, line))
				LogTraceStepReturn(false);
		}
		else
		{
			if (isdigit(point[0])) //current file line
			{
				string data = point;
				trim_string(data);
				int32 line = atoi(data.c_str());
				if (!get_current_source(0, data) || !bp.set_line_point(data, line))
					LogTraceStepReturn(false);
			}
			else //function name
			{
				string name = point;
				trim_string(name);
				if (!bp.set_func_point(name))
					LogTraceStepReturn(false);
			}
		}
		m_breakPoints.push_back(bp);
		LogTraceStepReturn(true);
	}

	bool LuaDebugger::del_break_point(const string & index)
	{
		LogTraceStepCall("bool LuaDebugger::del_break_point(const string & index)");
		int32 idx = atoi(index.c_str());
		if (idx <= 0 || idx >= (int32)m_breakPoints.size())
			LogTraceStepReturn(false);
		m_breakPoints.erase(m_breakPoints.begin() + idx);
		LogTraceStepReturn(true);
	}

	bool LuaDebugger::list_break_points(string & points)
	{
		LogTraceStepCall("bool LuaDebugger::list_break_points(string & points)");
		if (m_breakPoints.empty())
			LogTraceStepReturn(false);
		int32 index = 1;
		fmt::MemoryWriter buffer;
		for (vector<BreakPoint>::type::const_iterator iter = m_breakPoints.begin(); iter != m_breakPoints.end(); ++iter, ++index)
		{
			if (iter->is_function())
				buffer.write("{}\t function : {}\n", index, iter->m_name);
			else
				buffer.write("{}\t {} : {}\n", index, iter->m_name, iter->m_lineNum);
		}
		points.append(buffer.data(), buffer.size());
		LogTraceStepReturn(true);
	}

	bool LuaDebugger::reload_file(const string & path)
	{
		LogTraceStepCall("bool LuaDebugger::reload_file(const string & path)");
		if (!is_valid())
			LogTraceStepReturn(false);
		if (path.empty())
		{
			string source;
			if (!get_current_source(0, source) || source.empty() || source[0] != '@')
				LogTraceStepReturn(false);
			if (luaL_dofile(get_main_lua(), source.c_str() + 1))
			{
				LogScriptError(get_main_lua(), lua_tostring(get_main_lua(), -1));
				LogTraceStepReturn(false);
			}
		}
		else
		{
			if (luaL_dofile(get_main_lua(), path.c_str()))
			{
				LogScriptError(get_main_lua(), lua_tostring(get_main_lua(), -1));
				LogTraceStepReturn(false);
			}
		}
		LogTraceStepReturn(true);
	}

	bool LuaDebugger::next_call()
	{
		LogTraceStepCall("bool LuaDebugger::next_call()");
		if (is_valid())
		{
			lua_sethook(get_main_lua(), LuaDebugger::debug_hook, LUA_MASKLINE | LUA_MASKCALL | LUA_MASKRET, 0);
			set_running_level(get_stack_level());
			set_break_level(get_running_level());
			LogTraceStepReturn(true);
		}
		LogTraceStepReturn(false);
	}

	bool LuaDebugger::step_call()
	{
		LogTraceStepCall("bool LuaDebugger::step_call()");
		if (is_valid())
		{
			lua_sethook(get_main_lua(), LuaDebugger::debug_hook, LUA_MASKLINE | LUA_MASKCALL | LUA_MASKRET, 0);
			set_running_level(get_stack_level());
			set_break_level(INT32_MAX);
			LogTraceStepReturn(true);
		}
		LogTraceStepReturn(false);
	}

	bool LuaDebugger::over_call()
	{
		LogTraceStepCall("bool LuaDebugger::over_call()");
		if (is_valid())
		{
			lua_sethook(get_main_lua(), LuaDebugger::debug_hook, LUA_MASKLINE | LUA_MASKCALL | LUA_MASKRET, 0);
			set_running_level(get_stack_level());
			set_break_level(get_running_level() - 1);
			LogTraceStepReturn(true);
		}
		LogTraceStepReturn(false);
	}

	bool LuaDebugger::list_code(const char * path, int32 currentLine, int32 lineCount, string & code) const
	{
		LogTraceStepCall("bool LuaDebugger::list_code(const char * path, int32 currentLine, int32 lineCount, string & code) const");
		if (currentLine <= 0)
		{
			code.append("0 >>");
			code.append(path);
			LogTraceStepReturn(true);
		}
		
		FILE * fp = fopen(path, "rb");
		if (fp == NULL)
		{
			LogError("Source `{}` not found!!!", path);
			LogTraceStepReturn(false);
		}
		bool result = true;
		int c = fgetc(fp);
		if (c == '#' || c == LUA_SIGNATURE[0])
		{
			LogError("Not support lua bin code `{}`", path);
			result = false;
		}
		else
		{
			fseek(fp, 0, SEEK_END);
			long size = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			string fullCode;
			fullCode.resize(size + 2);
			long index = 0;
			vector<string>::type lines;
			while (!feof(fp))
			{
				lines.push_back(fgets((char *)fullCode.c_str() + index, fullCode.size() - index, fp));
				index += lines.back().size();
			}
			if (currentLine > (int32)lines.size())
			{
				LogError("Source line {} is invalid, the file `{}` has {} lines!!!", currentLine, path, lines.size());
				result = false;
			}
			else
			{
				--currentLine;
				int startLine = Max(0, currentLine - (lineCount >> 1));
				int endLine = Min((int32)lines.size(), currentLine + (lineCount >> 1) + 1);
				fmt::MemoryWriter buffer;
				while (startLine < endLine)
				{
					if (startLine == currentLine)
						buffer.write("{} >>", startLine + 1);
					else
						buffer.write("{}   ", startLine + 1);
					buffer.write("{}", lines[startLine]);
					++startLine;
				}
				code.append(buffer.data(), buffer.size());
			}
		}
		fclose(fp);
		LogTraceStepReturn(result);
	}

	bool LuaDebugger::set_frame(int32 level)
	{
		LogTraceStepCall("bool LuaDebugger::set_frame(int32 level)");
		if (!is_valid() || level < 0)
			LogTraceStepReturn(false);
		lua_Debug ld;
		if (!lua_getstack(get_lua(), level, &ld))
			LogTraceStepReturn(false);
		set_current_level(level);
		LogTraceStepReturn(true);
	}

	bool LuaDebugger::print_var(const string & varName) const
	{
		LogTraceStepCall("bool LuaDebugger::print_var(const string & varName) const");
		if (!is_valid())
			LogTraceStepReturn(false);
		lua_Debug ld;
		if (lua_getstack(get_lua(), get_current_level(), &ld))
		{
			LuaStateProtect lsp(get_lua(), true);
			lua_pushcclosure(get_lua(), LuaDebugger::print_table, 0);
			if (!push_var(&ld, varName))
				LogTraceStepReturn(false);
			lua_call(get_lua(), 1, 1);
			LogTraceStepReturn(true);
		}
		LogTraceStepReturn(false);
	}

	bool LuaDebugger::exec_code(const string & code) const
	{
		LogTraceStepCall("bool LuaDebugger::exec_code(const string & code) const");
		if (!is_valid())
			LogTraceStepReturn(false);
		if (luaL_dostring(get_lua(), code.c_str()))
		{
			const char * errMsg = lua_tostring(get_lua(), -1);
			LogScriptError(get_lua(), errMsg);
			LogTraceStepReturn(false);
		}
		LogTraceStepReturn(true);
	}

	int LuaDebugger::print_table(lua_State * L)
	{
		LogTraceStepCall("int LuaDebugger::print_table(lua_State * L)");
		int n = lua_gettop(L) + 1;  /* number of arguments */
		lua_getglobal(L, "tostring");
	
		const char * s = NULL;

		fmt::MemoryWriter str;
	
		for (int i = 1; i < n; i++)
		{
			lua_pushvalue(L, -1);  /* function to be called */
			lua_pushvalue(L, i);   /* value to print */
			lua_call(L, 1, 1);
		
			s = lua_tostring(L, -1);  /* get result */
			if (s == NULL)
				LogTraceStepReturn(luaL_error(L, "\'tostring\' must return a string to \'print\'"));

			if (lua_istable(L, i))
			{
				str << s << "\n{\n";
				for (LuaIterator iter(L, i); !iter.is_over(); ++iter)
				{
					lua_pushvalue(L, n); // tostring
					iter.copy_key(); //key
					lua_call(L, 1, 1);
					s = lua_tostring(L, -1);  /* get result */
					if (s == NULL)
						LogTraceStepReturn(luaL_error(L, "\'tostring\' must return a string to \'print\'"));
					str << '\t' << s << " = ";

					lua_pushvalue(L, n); // tostring
					iter.copy_value(); //value
					lua_call(L, 1, 1);
					s = lua_tostring(L, -1);  /* get result */
					if (s == NULL)
						LogTraceStepReturn(luaL_error(L, "\'tostring\' must return a string to \'print\'"));
					str << s << '\n';
					iter.clean_stack();
				}
				str << '}';
			}
			else
				str << s << '\t';
			lua_pop(L, 1);  /* pop result */
		}
		LogScriptMessage(fmt::StringRef(str.data(), str.size()));
		LogTraceStepReturn(0);
	}

	int LuaDebugger::ldb(lua_State * L)
	{
		LogTraceStepCall("int LuaDebugger::ldb(lua_State * L)");
		LuaDebugger * ld = get_lua_debugger(L);
		bool result = false;
		if (ld != NULL)
		{
			ld->print_all_frame_info();
			result = ld->debug();
		}
		lua_pushboolean(L, result ? 1 : 0);
		LogTraceStepReturn(1);
	}

	LuaDebugger * LuaDebugger::get_lua_debugger(lua_State * L)
	{
		LogTraceStepCall("LuaDebugger * LuaDebugger::get_lua_debugger(lua_State * L)");
		LuaStateProtect lsp(L, true);
		lua_pushlightuserdata(L, (void *)LuaDebugger::get_lua_debugger);
		lua_rawget(L, LUA_REGISTRYINDEX);
		if (lua_islightuserdata(L, -1))
			LogTraceStepReturn((LuaDebugger *)lua_touserdata(L, -1));
		LogTraceStepReturn(NULL);
	}

	bool LuaDebugger::block_read_cmd()
	{
		LogTraceStepCall("bool LuaDebugger::block_read_cmd()");
		LogMessage("ldb : ");
		char tmp[128] = { 0 };
		m_cmd.clear();
		while (fgets(tmp, sizeof(tmp), stdin))
		{
			m_cmd.append(tmp);
			if (!m_cmd.empty() && m_cmd[m_cmd.size() - 1u] == '\n')
				LogTraceStepReturn(true);
		}
		m_cmd.clear();
		LogTraceStepReturn(false);
	}

	void LuaDebugger::trim_string(string & str)
	{
		LogTraceStepCall("LuaDebugger::trim_string(string & str)");
		if (str.empty())
			LogTraceStepReturnVoid;
		for (size_t i = str.size(); i > 0u; )
		{
			--i;
			if (str[i] != ' ' && str[i] != '\t' && str[i] != '\r' && str[i] != '\n')
			{
				str.resize(i + 1);
				break;
			}
		}
		if (str.empty())
			LogTraceStepReturnVoid;
		char back = str[str.size() - 1u];
		if (back == ' ' || back == '\t' || back == '\r' || back == '\n')
		{
			str.clear();
			LogTraceStepReturnVoid;
		}
		for (size_t i = 0u; i < str.size(); ++i)
		{
			if (str[i] != ' ' && str[i] != '\t' && str[i] != '\r' && str[i] != '\n')
			{
				str.assign(str.substr(i));
				break;
			}
		}
		LogTraceStepReturnVoid;
	}

	bool LuaDebugger::push_var(lua_Debug * ld, const string & varName) const
	{
		LogTraceStepCall("bool LuaDebugger::push_var(lua_Debug * ld, const string & varName) const");
		vector<string>::type fields;
		if (!split_fields(varName, fields) || fields.empty())
		{
			LogScriptError(get_lua(), "Split var failed!!!");
			LogTraceStepReturn(false);
		}
		int32 idx = 0;
		bool found = false;
		const char * name = NULL;
		while (!found && (name = lua_getlocal(get_lua(), ld, ++idx)) != NULL)
		{
			found = fields[0] == name;
			if (found)
			{
				if (!push_sub_var(ld, 1u, fields))
					LogTraceStepReturn(false);
				LogTraceStepReturn(true);
			}
			else
				lua_pop(get_lua(), 1);
		}

		idx = 0;
		if (!lua_getinfo(get_lua(), "f", ld))
		{
			LogScriptError(get_lua(), "Get upvalue failed!!!");
			LogTraceStepReturn(false);
		}
		while (!found && (name = lua_getupvalue(get_lua(), -1, ++idx)) != NULL)
		{
			found = fields[0] == name;
			if (found)
			{
				if (!push_sub_var(ld, 1u, fields))
					LogTraceStepReturn(false);
				LogTraceStepReturn(true);
			}
			else
				lua_pop(get_lua(), 1);
		}
		lua_pop(get_lua(), 1);

		if (!found)
		{
			lua_getglobal(get_lua(), fields[0].c_str());
			if (!lua_isnoneornil(get_lua(), -1))
			{
				found = true;
				if (!push_sub_var(ld, 1u, fields))
					LogTraceStepReturn(false);
				LogTraceStepReturn(true);
			}
			else
				lua_pop(get_lua(), 1);
		}
		LogTraceStepReturn(false);
	}

	bool LuaDebugger::push_sub_var(lua_Debug * ld, size_t startIdx, const vector<string>::type & fields) const
	{
		LogTraceStepCall("bool LuaDebugger::push_sub_var(lua_Debug * ld, size_t startIdx, const vector<string>::type & fields) const");
		for (size_t i = startIdx; i < fields.size(); ++i)
		{
			if (fields[i].empty())
				continue;
			if (fields[i][0] == '[')
			{
				string field(fields[i].substr(1u, fields[i].size() - 2u));
				trim_string(field);
				if (field.empty())
				{
					LogScriptError(get_lua(), "`{}` field is invalid!!!", fields[i]);
					LogTraceStepReturn(false);
				}
				if (field[0] == '\"')
				{
					if (field.size() <= 2u || field[field.size() - 1u] != '\"')
					{
						LogScriptError(get_lua(), "`{}` field is invalid, the \" is not match!!!", fields[i]);
						LogTraceStepReturn(false);
					}
					field = field.substr(1u, field.size() - 2u);
					lua_pushlstring(get_lua(), field.c_str(), field.size());
					lua_gettable(get_lua(), -2);
					lua_remove(get_lua(), -2);
				}
				else if (isdigit(field[0]))
				{
					double num = atof(field.c_str());
					lua_pushnumber(get_lua(), num);
					lua_gettable(get_lua(), -2);
					lua_remove(get_lua(), -2);
				}
				else
				{
					if (!push_var(ld, field))
						LogTraceStepReturn(false);
					lua_gettable(get_lua(), -2);
					lua_remove(get_lua(), -2);
				}
			}
			else
			{
				lua_pushlstring(get_lua(), fields[i].c_str(), fields[i].size());
				lua_gettable(get_lua(), -2);
				lua_remove(get_lua(), -2);
			}
		}
		LogTraceStepReturn(true);
	}

	bool LuaDebugger::split_fields(const string & varName, vector<string>::type & fields) const
	{
		LogTraceStepCall("bool LuaDebugger::split_fields(const string & varName, vector<string>::type & fields) const");
		size_t startIdx = 0u, endIdx = varName.size();
		while ((endIdx = varName.find_first_of(".[", startIdx)) != string::npos)
		{
			if (endIdx - startIdx == 0)
			{
				++startIdx;
				continue;
			}
			fields.push_back(varName.substr(startIdx, endIdx - startIdx));
			trim_string(fields.back());
			startIdx = endIdx + 1u;

			if (varName[endIdx] == '[')
			{
				int subCount = 1;
				size_t subStartIdx = startIdx, subEndIdx = endIdx;
				while (subCount > 0 && (subEndIdx = varName.find_first_of("[]", subStartIdx)) != string::npos)
				{
					subStartIdx = subEndIdx + 1u;
					if (varName[subEndIdx] == '[')
						++subCount;
					else
						--subCount;
				}
				if (subCount > 0)
					LogTraceStepReturn(false);
				fields.push_back(varName.substr(endIdx, subEndIdx - endIdx + 1u));
				trim_string(fields.back());
				startIdx = subEndIdx + 1u;
			}
		}
		if (startIdx < varName.size() && endIdx > startIdx)
		{
			fields.push_back(varName.substr(startIdx, endIdx - startIdx));
			trim_string(fields.back());
		}
		LogTraceStepReturn(true);
	}

	int32 LuaDebugger::get_current_line(int32 level) const
	{
		LogTraceStepCall("int32 LuaDebugger::get_current_line(int32 level) const");
		if (is_valid())
		{
			lua_Debug ld;
			lua_getstack(get_lua(), level, &ld);
			lua_getinfo(get_lua(), "l", &ld);
			LogTraceStepReturn(ld.currentline);
		}
		LogTraceStepReturn(-1);
	}

	int32 LuaDebugger::get_stack_level() const
	{
		LogTraceStepCall("int32 LuaDebugger::get_stack_level() const");
		if (is_valid())
		{
			lua_Debug ld;
			int32 level = 0;
			while (lua_getstack(get_lua(), level, &ld))
			{ ++level; }
			LogTraceStepReturn(level);
		}
		LogTraceStepReturn(-1);
	}

	bool LuaDebugger::get_current_source(int32 level, string & source) const
	{
		LogTraceStepCall("bool LuaDebugger::get_current_source(int32 level, string & source) const");
		if (is_valid())
		{
			lua_Debug ld;
			lua_getstack(get_lua(), level, &ld);
			lua_getinfo(get_lua(), "S", &ld);
			if (ld.source[0] == '@')
				source.assign(ld.source + 1);
			else
				source.assign(ld.source);
			LogTraceStepReturn(true);
		}
		LogTraceStepReturn(false);
	}

	bool LuaDebugger::check_line_point(lua_Debug * ld) const
	{
		LogTraceStepCall("bool LuaDebugger::check_line_point(lua_Debug * ld) const");
		if (is_valid() && !m_breakPoints.empty() && lua_getinfo(get_lua(), "Sl", ld) && ld->source != NULL && ld->currentline >= 0)
		{
			BreakPoint bp;
			if (ld->source[0] == '@')
				bp.m_name = ld->source + 1;
			else
				bp.m_name = ld->source;
			bp.m_lineNum = ld->currentline;
			LogTraceStepReturn(std::find(m_breakPoints.begin(), m_breakPoints.end(), bp) != m_breakPoints.end());
		}
		LogTraceStepReturn(false);
	}

	bool LuaDebugger::check_func_point(lua_Debug * ld) const
	{
		LogTraceStepCall("bool LuaDebugger::check_func_point(lua_Debug * ld) const");
		if (is_valid() && !m_breakPoints.empty() && lua_getinfo(get_lua(), "n", ld) && ld->name != NULL)
		{
			BreakPoint bp;
			bp.m_name = ld->name;
			LogTraceStepReturn(std::find(m_breakPoints.begin(), m_breakPoints.end(), bp) != m_breakPoints.end());
		}
		LogTraceStepReturn(false);
	}

	void LuaDebugger::debug_hook(lua_State * L, lua_Debug * ld)
	{
		LogTraceStepCall("void LuaDebugger::debug_hook(lua_State * L, lua_Debug * ld)");
		LuaDebugger * self = LuaDebugger::get_lua_debugger(L);
		if (self != NULL)
		{
			if (self->m_lua == L)
				self->reset_coroutine(NULL);
			else
				self->reset_coroutine(L);
			switch (ld->event)
			{
			case LUA_HOOKCALL:
				self->set_running_level(self->get_stack_level());
				if (self->get_break_level() < self->get_running_level() && !self->check_func_point(ld))
					LogTraceStepReturnVoid;
				break;
			case LUA_HOOKRET:
				self->set_running_level(self->get_stack_level() - 1);
				if (self->get_break_level() <= self->get_running_level())
					LogTraceStepReturnVoid;
				break;
			case LUA_HOOKLINE:
				if (self->get_break_level() < self->get_running_level() && !self->check_line_point(ld))
					LogTraceStepReturnVoid;
				break;
			case LUA_HOOKCOUNT:
			case LUA_HOOKTAILRET:
				LogTraceStepReturnVoid;
				break;
			default:
				LogScriptError(self->get_main_lua(), "Invalid lua hook event {}", ld->event);
				LogTraceStepReturnVoid;
			}
			lua_sethook(self->get_main_lua(), LuaDebugger::debug_hook, 0, 0);
			self->debug();
		}
		LogTraceStepReturnVoid;
	}

	void LuaDebugger::reset_coroutine(lua_State * L)
	{
		LogTraceStepCall("void LuaDebugger::reset_coroutine(lua_State * L)");
		if (m_coroutine != L)
		{
			m_coroutine = L;
			m_coRunningLevel = -1;
			m_coBreakLevel = -1;
			m_coCurrentLevel = 0;
		}
		LogTraceStepReturnVoid;
	}

	LuaDebugger::BreakPoint::BreakPoint() :  m_lineNum(-1)
	{
		LogTraceStepCall("LuaDebugger::BreakPoint::BreakPoint()");
		LogTraceStepReturnVoid;
	}

	bool LuaDebugger::BreakPoint::set_func_point(const string & funcName)
	{
		LogTraceStepCall("bool LuaDebugger::BreakPoint::set_func_point(const string & funcName)");
		if (funcName.empty())
			LogTraceStepReturn(false);
		m_name = funcName;
		m_lineNum = -1;
		LogTraceStepReturn(true);
	}

	bool LuaDebugger::BreakPoint::set_line_point(const string & fileName, int32 lineNum)
	{
		LogTraceStepCall("bool LuaDebugger::BreakPoint::set_line_point(const char * fileName, int32 lineNum)");
		if (fileName.empty())
			LogTraceStepReturn(false);
		m_name = fileName;
		for (size_t i = 0u; i < m_name.size(); ++i)
		{
			if (m_name[i] == '\\')
			{
				m_name[i] = '/';
			}
		}
		for (size_t idx = m_name.find("//"); idx < m_name.size(); idx = m_name.find("//"))
		{
			m_name.replace(idx, 2, "/");
		}
		if (m_name.empty())
			LogTraceStepReturn(false);
		m_lineNum = lineNum;
		LogTraceStepReturn(true);
	}

	bool LuaDebugger::BreakPoint::operator== (const BreakPoint & right) const
	{
		LogTraceStepCall("bool LuaDebugger::BreakPoint::operator== (const BreakPoint & right) const");
		if (m_lineNum != right.m_lineNum)
			LogTraceStepReturn(false);
		LogTraceStepReturn(m_name.find(right.m_name) != string::npos || right.m_name.find(m_name) != string::npos);
	}


}