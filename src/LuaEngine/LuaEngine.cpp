


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
#include "LuaEngine/LuaEngine.h"
#include "LuaEngine/LuaDefTool.h"
#include "LuaEngine/LuaInt64.h"
#include "LuaEngine/LuaTableRef.h"

namespace LightInk
{
	LuaEngine::LuaEngine() : m_lua(NULL)
	{
		LogTraceStepCall("LuaEngine::LuaEngine()");
		LogTraceStepReturnVoid;
	}

	LuaEngine::~LuaEngine()
	{
		LogTraceStepCall("LuaEngine::~LuaEngine()");
		close();
		LogTraceStepReturnVoid;
	}

	lua_State * LuaEngine::get_lua_state()
	{
		LogTraceStepCall("lua_State * LuaEngine::get_lua_state()");
		LogTraceStepReturn(m_lua);
	}

	bool LuaEngine::is_inited()
	{
		LogTraceStepCall("bool LuaEngine::is_inited()");
		LogTraceStepReturn(m_lua != NULL);
	}

	RuntimeError LuaEngine::init()
	{
		LogTraceStepCall("RuntimeError LuaEngine::init()");
		if (m_lua)
		{
			LogTraceStepReturn(RE_Lua_ThisStateInited);
		}
#if defined(LIGHTINK_LUAJIT) && defined(LIGHTINK_X64)
		m_lua = luaL_newstate();
#else
		m_lua = lua_newstate(LuaEngine::lua_allocator, this);
#endif
		if (!m_lua)
		{
			LogTraceStepReturn(RE_Lua_NewStateFailed);
		}

		lua_atpanic(m_lua, LuaEngine::lua_error_catch);
		luaL_openlibs(m_lua);

		m_debugger.init(m_lua);

		LuaClassMgr::get_class_mgr(m_lua);

		register_global_object("get_runtime_error", get_runtime_error);
		lua_getglobal(m_lua, "print");
		lua_setglobal(m_lua, "__print");
		register_global_object("print", &lua_replace_print);
		register_global_object("print_debug", &lua_print_debug);
		register_global_object("print_warning", &lua_print_warning);
		register_global_object("print_error", &lua_print_error);
		register_global_object("print_fatal", &lua_print_fatal);

		register_global_object("bit_nega", &lua_num_bit_nega);
		register_global_object("bit_and", &lua_num_bit_and);
		register_global_object("bit_or", &lua_num_bit_or);
		register_global_object("bit_xor", &lua_num_bit_xor);
		register_global_object("bit_left", &lua_num_bit_left);
		register_global_object("bit_right", &lua_num_bit_right);

		load_class_script(m_lua);

		lua_pushvalue(m_lua, LUA_GLOBALSINDEX);
		LuaRef lrf(m_lua, true);
		LuaModule(m_lua, "_G", lrf)
		[
			LuaRegister<LuaTableRef, void(const LuaRef &, const LuaRef &)>(m_lua, "LuaTableRef")
				.def(&LuaTableRef::get_ref, "get_ref")
			<=
			LuaRegister<LuaUint64, void(lua_Number)>(m_lua, "Uint64")
				.def(LuaUint64::new_from_string, "new_from_string")
				.def(LuaUint64::new_from_st, "new_from_st")
				.def(&LuaUint64::to_string, "to_string")
				.def(&LuaUint64::to_number, "to_number")
				.def(&LuaUint64::to_int64, "to_int64")
				.def(&LuaUint64::set_number, "set_number")
				.def(&LuaUint64::set_string, "set_string")
				.def(&LuaUint64::set_st, "set_st")
				.def(&LuaUint64::add_number, "__add")
				.def(&LuaUint64::sub_number, "__sub")
				.def(&LuaUint64::mul_number, "__mul")
				.def(&LuaUint64::div_number, "__div")
				.def(&LuaUint64::mod_number, "__mod")
				.def(&LuaUint64::pow_number, "__pow")
				.def(&LuaUint64::unm_number, "__unm")
				.def(&LuaUint64::eq_number, "__eq")
				.def(&LuaUint64::lt_number, "__lt")
				.def(&LuaUint64::le_number, "__le")
				.def(&LuaUint64::add_st, "add_st")
				.def(&LuaUint64::sub_st, "sub_st")
				.def(&LuaUint64::mul_st, "mul_st")
				.def(&LuaUint64::div_st, "div_st")
				.def(&LuaUint64::mod_st, "mod_st")
				.def(&LuaUint64::pow_st, "pow_st")
				.def(&LuaUint64::eq_st, "eq_st")
				.def(&LuaUint64::lt_st, "lt_st")
				.def(&LuaUint64::le_st, "le_st")
				.def(&LuaUint64::add_set_number, "add_set_number")
				.def(&LuaUint64::sub_set_number, "sub_set_number")
				.def(&LuaUint64::mul_set_number, "mul_set_number")
				.def(&LuaUint64::div_set_number, "div_set_number")
				.def(&LuaUint64::mod_set_number, "mod_set_number")
				.def(&LuaUint64::pow_set_number, "pow_set_number")
				.def(&LuaUint64::unm_set_number, "unm_set_number")
				.def(&LuaUint64::add_set_st, "add_set_st")
				.def(&LuaUint64::sub_set_st, "sub_set_st")
				.def(&LuaUint64::mul_set_st, "mul_set_st")
				.def(&LuaUint64::div_set_st, "div_set_st")
				.def(&LuaUint64::mod_set_st, "mod_set_st")
				.def(&LuaUint64::pow_set_st, "pow_set_st")

			<=
			LuaRegister<LuaInt64, void(lua_Number)>(m_lua, "Int64")
				.def(LuaInt64::new_from_string, "new_from_string")
				.def(LuaInt64::new_from_st, "new_from_st")
				.def(&LuaInt64::to_string, "to_string")
				.def(&LuaInt64::to_number, "to_number")
				.def(&LuaInt64::to_uint64, "to_uint64")
				.def(&LuaInt64::set_number, "set_number")
				.def(&LuaInt64::set_string, "set_string")
				.def(&LuaInt64::set_st, "set_st")
				.def(&LuaInt64::add_number, "__add")
				.def(&LuaInt64::sub_number, "__sub")
				.def(&LuaInt64::mul_number, "__mul")
				.def(&LuaInt64::div_number, "__div")
				.def(&LuaInt64::mod_number, "__mod")
				.def(&LuaInt64::pow_number, "__pow")
				.def(&LuaInt64::unm_number, "__unm")
				.def(&LuaInt64::eq_number, "__eq")
				.def(&LuaInt64::lt_number, "__lt")
				.def(&LuaInt64::le_number, "__le")
				.def(&LuaInt64::add_st, "add_st")
				.def(&LuaInt64::sub_st, "sub_st")
				.def(&LuaInt64::mul_st, "mul_st")
				.def(&LuaInt64::div_st, "div_st")
				.def(&LuaInt64::mod_st, "mod_st")
				.def(&LuaInt64::pow_st, "pow_st")
				.def(&LuaInt64::eq_st, "eq_st")
				.def(&LuaInt64::lt_st, "lt_st")
				.def(&LuaInt64::le_st, "le_st")
				.def(&LuaInt64::add_set_number, "add_set_number")
				.def(&LuaInt64::sub_set_number, "sub_set_number")
				.def(&LuaInt64::mul_set_number, "mul_set_number")
				.def(&LuaInt64::div_set_number, "div_set_number")
				.def(&LuaInt64::mod_set_number, "mod_set_number")
				.def(&LuaInt64::pow_set_number, "pow_set_number")
				.def(&LuaInt64::unm_set_number, "unm_set_number")
				.def(&LuaInt64::add_set_st, "add_set_st")
				.def(&LuaInt64::sub_set_st, "sub_set_st")
				.def(&LuaInt64::mul_set_st, "mul_set_st")
				.def(&LuaInt64::div_set_st, "div_set_st")
				.def(&LuaInt64::mod_set_st, "mod_set_st")
				.def(&LuaInt64::pow_set_st, "pow_set_st")

		];
		LogTraceStepReturn(do_init());
	}

	void LuaEngine::close()
	{
		LogTraceStepCall("void LuaEngine::close()");
		m_debugger.release();
		if (m_lua)
		{
			LuaClassMgr * pMgr = LuaClassMgr::get_class_mgr(m_lua);
			do_close();
			lua_close(m_lua);
			m_lua = NULL;
			delete pMgr;
		}
		LogTraceStepReturnVoid;
	}

	void * LuaEngine::lua_allocator(void * ud, void * ptr, size_t osize, size_t nsize)
	{
		LogTraceStepCall("void * LuaEngine::lua_allocator(void * ud, void * ptr, size_t osize, size_t nsize)");
		LuaEngine * p = (LuaEngine *)ud;
		LogTraceStepReturn(p->do_lua_allocator(ptr, osize, nsize));
	}

	int LuaEngine::lua_error_catch(lua_State * L)
	{
		LogTraceStepCall("int LuaEngine::lua_error_catch(lua_State * L)");
		const char * errStr = lua_tostring(L, -1);
		LogScriptError(L, "Error!!!Lua Error Catch \"{}\"", errStr);
		LuaDebugger * ld = LuaDebugger::get_lua_debugger(L);
		if (ld != NULL)
		{
			ld->catch_lua_error();
		}
		throw RE_Lua_RuntimeError;
		LogTraceStepReturn(0);
	}

	int LuaEngine::log_string(lua_State * L, LogLevel::LEVEL level)
	{
		LogTraceStepCall("int LuaEngine::log_string(lua_State * L, LogMsg::LEVEL level)");
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
			{
				LogTraceStepReturn(luaL_error(L, "\'tostring\' must return a string to \'print\'"));
			}

			str << s << '\t';
		
			lua_pop(L, 1);  /* pop result */
		}
	
		switch (level)
		{
		case LogLevel::LogMsg_Debug:
			LogScriptDebug(fmt::StringRef(str.data(), str.size()));
			break;
		case LogLevel::LogMsg_Message:
			LogScriptMessage(fmt::StringRef(str.data(), str.size()));
			break;
		case LogLevel::LogMsg_Warning:
			LogScriptWarning(fmt::StringRef(str.data(), str.size()));
			break;
		case LogLevel::LogMsg_Error:
			LogScriptError(L, fmt::StringRef(str.data(), str.size()));
			break;
		case LogLevel::LogMsg_Fatal:
			LogScriptFatal(L, fmt::StringRef(str.data(), str.size()));
			break;
		default:
			break;
		}
		LogTraceStepReturn(0);
	}

	int LuaEngine::lua_print_debug(lua_State * L)
	{
		LogTraceStepCall("int LuaEngine::lua_print_debug(lua_State * L)");
		LogTraceStepReturn(LuaEngine::log_string(L, LogLevel::LogMsg_Debug));
	}
	int LuaEngine::lua_replace_print(lua_State * L)
	{
		LogTraceStepCall("int LuaEngine::lua_replace_print(lua_State * L)");
		LogTraceStepReturn(LuaEngine::log_string(L, LogLevel::LogMsg_Message));
	}
	int LuaEngine::lua_print_warning(lua_State * L)
	{
		LogTraceStepCall("int LuaEngine::lua_print_warning(lua_State * L)");
		LogTraceStepReturn(LuaEngine::log_string(L, LogLevel::LogMsg_Warning));
	}
	int LuaEngine::lua_print_error(lua_State * L)
	{
		LogTraceStepCall("int LuaEngine::lua_print_error(lua_State * L)");
		LogTraceStepReturn(LuaEngine::log_string(L, LogLevel::LogMsg_Error));
	}
	int LuaEngine::lua_print_fatal(lua_State * L)
	{
		LogTraceStepCall("int LuaEngine::lua_print_fatal(lua_State * L)");
		LogTraceStepReturn(LuaEngine::log_string(L, LogLevel::LogMsg_Fatal));
	}

	uint32 LuaEngine::lua_num_bit_nega(uint32 num)
	{
		LogTraceStepCall("uint32 LuaEngine::lua_num_bit_nega(uint32 num)");
		LogTraceStepReturn(~num);
	}
	uint32 LuaEngine::lua_num_bit_and(uint32 num1, uint32 num2)
	{
		LogTraceStepCall("uint32 LuaEngine::lua_num_bit_and(uint32 num1, uint32 num2)");
		LogTraceStepReturn(num1 & num2);
	}
	uint32 LuaEngine::lua_num_bit_or(uint32 num1, uint32 num2)
	{
		LogTraceStepCall("uint32 LuaEngine::lua_num_bit_or(uint32 num1, uint32 num2)");
		LogTraceStepReturn(num1 | num2);
	}
	uint32 LuaEngine::lua_num_bit_xor(uint32 num1, uint32 num2)
	{
		LogTraceStepCall("uint32 LuaEngine::lua_num_bit_xor(uint32 num1, uint32 num2)");
		LogTraceStepReturn(num1 ^ num2);
	}

	uint32 LuaEngine::lua_num_bit_left(uint32 num, int16 leftBit)
	{
		LogTraceStepCall("uint32 LuaEngine::lua_num_bit_left(uint32 num, int16 leftBit)");
		LogTraceStepReturn(num << leftBit);
	}
	uint32 LuaEngine::lua_num_bit_right(uint32 num, int16 rightBit)
	{
		LogTraceStepCall("uint32 LuaEngine::lua_num_bit_right(uint32 num, int16 rightBit)");
		LogTraceStepReturn(num >> rightBit);
	}

	void * LuaEngine::do_lua_allocator(void * ptr, size_t osize, size_t nsize)
	{
		LogTraceStepCall("void * LuaEngine::do_lua_allocator(void * ptr, size_t osize, size_t nsize)");
		LogTraceStepReturn(m_allocator.reallocate(ptr, osize, nsize));
	}

	void LuaEngine::allocator_gc(bool all)
	{
		LogTraceStepCall("void LuaEngine::allocator_gc(bool all)");
		m_allocator.gc(all);
		LogTraceStepReturnVoid;
	}

	RuntimeError LuaEngine::do_init()
	{
		LogTraceStepCall("RuntimeError LuaEngine::do_init()");
		LogTraceStepReturn(RE_Success);
	}
	
	void LuaEngine::do_close()
	{
		LogTraceStepCall("void LuaEngine::do_close()");
		LogTraceStepReturnVoid;
	}

	
	RuntimeError LuaEngine::add_package_path(const string & path)
	{
		LogTraceStepCall("RuntimeError LuaEngine::add_package_path(const string & path)");
		string new_path = "package = package or {};package.path = (package.path or \";./?.lua\") .. \"";
        if (path.empty())
        {
            LogTraceStepReturn(RE_Lua_StringEmpty);
        }

        if (path[0] != ';')
        {
           new_path.push_back(';');
        }

        new_path.append(path);
		new_path.append("\"");

		do_string(new_path.c_str());
		LogTraceStepReturn(RE_Success);
	}

	void LuaEngine::clear_package_path()
	{
		LogTraceStepCall("void LuaEngine::clear_package_path()");
		do_string("package = package or {};package.path = \";./?.lua\"");
		LogTraceStepReturnVoid;
	}

	string LuaEngine::dump_stack(lua_State * L)
	{
		LogTraceStepCall("string LuaEngine::dump_stack(lua_State * L)");
		string dumpStr;
		int top = lua_gettop(L);
        for (int i = 1; i <= top; i++)
        {
            int t = lua_type(L, i);
            switch (t)
            {
			case LUA_TSTRING:
				{
					dumpStr.append(lua_typename(L, lua_type(L, i)));
					dumpStr.append(" - `");
					dumpStr.append(lua_tostring(L, i));
					dumpStr.push_back('`');
				}
				break;
                case LUA_TBOOLEAN:
                {
					dumpStr.append(lua_typename(L, lua_type(L, i)));
					dumpStr.append(" - ");
					dumpStr.append(lua_toboolean(L, i) ? "true" : "false");
                }
                break;
                case LUA_TNUMBER:
                {
					dumpStr.append(lua_typename(L, lua_type(L, i)));
					dumpStr.append(" - ");
					dumpStr.append(fmt::format("`{}`", lua_tonumber(L, i)));
                }
                break;
                case LUA_TTABLE:
                {
					{
						LuaStateProtect lsp(L, true);
						lua_getglobal(L, "tostring");
						lua_pushvalue(L, i);   /* value to print */
						const char * s = NULL;
						if (!lua_pcall(L, 1, 1, 0))
							s = lua_tostring(L, -1);  /* get result */
						if (s == NULL)
							dumpStr.append("table");
						else
							dumpStr.append(s);
					}
                	dumpStr.append(" begin\n");
                    lua_pushnil(L);
                    while (lua_next(L, i) != 0)
					{
						const char * key = NULL;
						if (lua_isstring(L, -2))
						{
							key = lua_tostring(L, -2);
							dumpStr.append(fmt::format("\t{}({}) - {}\n",
								lua_typename(L, lua_type(L, -2)), key,
								lua_typename(L, lua_type(L, -1))));
						}
						else
						{
							dumpStr.append(fmt::format("\t{} - {}\n",
								lua_typename(L, lua_type(L, -2)),
								lua_typename(L, lua_type(L, -1))));
						}
						lua_pop(L, 1);
                    }
					dumpStr.append("table end");
                }
                break;
                default:
                {
					dumpStr.append(lua_typename(L, lua_type(L, i)));
					dumpStr.append(" - `");
					{
						LuaStateProtect lsp(L, true);
						lua_getglobal(L, "tostring");
						lua_pushvalue(L, i);   /* value to print */
						const char * s = NULL;
						if (!lua_pcall(L, 1, 1, 0))
							s = lua_tostring(L, -1);  /* get result */
						if (s != NULL)
							dumpStr.append(s);
					}
					dumpStr.push_back('`');
                }
                break;
            }
			dumpStr.push_back('\n');
        }
		dumpStr.push_back('\n');
		lua_settop(L, top);
		LogTraceStepReturn(dumpStr);
	}

	RuntimeError LuaEngine::do_string(const char * chunk)
	{
		LogTraceStepCall("RuntimeError LuaEngine::do_string(const char * chunk)");
		if (luaL_dostring(m_lua, chunk))
		{
			const char * err = lua_tostring(m_lua, -1);
			LogScriptError(m_lua, "Call LuaEngine::do_string Error!!! error is \"{}\"", err);
			lua_pop(m_lua, 1); //pop error
			LogTraceStepReturn(RE_Lua_RuntimeError);
		}
		LogTraceStepReturn(RE_Success);
	}

	RuntimeError LuaEngine::do_chunk(const char * chunk, const char * name)
	{
		LogTraceStepCall("RuntimeError LuaEngine::do_chunk(const char * chunk, const char * name)");
		if (luaL_loadbuffer(m_lua, chunk, strlen(chunk), name) || lua_pcall(m_lua, 0, LUA_MULTRET, 0))
		{
			const char * err = lua_tostring(m_lua, -1);
			LogScriptError(m_lua, "Call LuaEngine::do_chunk Error!!! error is \"{}\"", err);
			lua_pop(m_lua, 1); //pop error
			LogTraceStepReturn(RE_Lua_RuntimeError);
		}
		LogTraceStepReturn(RE_Success);
	}

	RuntimeError LuaEngine::do_file(const char * fileName)
	{
		LogTraceStepCall("RuntimeError LuaEngine::do_file(const char * fileName)");
		if (luaL_dofile(m_lua, fileName))
		{
			const char * err = lua_tostring(m_lua, -1);
			LogScriptError(m_lua, "Call LuaEngine::do_file Error!!! error is \"{}\" \n chunk is \"{}\"\n", err, fileName);
			lua_pop(m_lua, 1); //pop error
			LogTraceStepReturn(RE_Lua_RuntimeError);
		}
		LogTraceStepReturn(RE_Success);
	}

	RuntimeError LuaEngine::require_file(const char * fileName)
	{
		LogTraceStepCall("RuntimeError LuaEngine::require_file(const char * fileName)");
		LuaStateProtect lsp(m_lua, true);
		lua_getglobal(m_lua, "require");
		lua_pushstring(m_lua, fileName);
		if (lua_pcall(m_lua, 1, 0, 0))
		{
			const char * errStr = lua_tostring(m_lua, -1);
			if (errStr) LogScriptError(m_lua, errStr);
			LogTraceStepReturn(RE_Lua_RuntimeError);
		}
		LogTraceStepReturn(RE_Success);
	}

	LuaRef LuaEngine::get_global_ref(const char * name)
	{
		LogTraceStepCall("LuaRef LuaEngine::get_global_ref(const char * name)");
		lua_getglobal(m_lua, name);
		LogTraceStepReturn(LuaRef(m_lua, true));
	}

	void LuaEngine::register_module(void(*func)(lua_State * lua))
	{
		LogTraceStepCall("void LuaEngine::register_module(void(*func)(lua_State * lua))");
		func(m_lua);
		LogTraceStepReturnVoid;
	}

	const char * LuaEngine::get_class_name(lua_State * L, int idx)
	{
		LogTraceStepCall("const char * LuaEngine::get_class_name(lua_State * L, int idx)");
		const char * className = "";
		if (lua_isuserdata(L, idx) && lua_getmetatable(L, idx))
		{
			rawgetfield(L, -1, "type__");
			if (lua_isstring(L, -1))
			{
				className = lua_tostring(L, -1);
			}
			lua_pop(L, 2);
		}
		LogTraceStepReturn(className);
	}

	bool LuaEngine::load_class_script(lua_State * L)
	{
		LogTraceStepCall("bool LuaEngine::load_class_script(lua_State * L)");
		lua_pushcclosure(L, LuaMetatable::mt_new___function, 0);
		lua_setglobal(L, "__mt_new__");
		const char * class_script = 
			"function class__(BaseClass)\n"
			"	local LuaClass = {}\n"
			"	if BaseClass and BaseClass.__call then\n"
			"		setmetatable(LuaClass, BaseClass)\n"
			"	else\n"
			"		setmetatable(LuaClass, {__call = __mt_new__})\n"
			"	end\n"
			"	LuaClass.__index = LuaClass\n"
			"	LuaClass.__call = __mt_new__\n"
			"	LuaClass.new__ = __mt_new__\n"
			"	return LuaClass\n"
			"end\n";
		if (luaL_loadbuffer(L, class_script, strlen(class_script), "class__") || lua_pcall(L, 0, LUA_MULTRET, 0))
		{
			const char * err = lua_tostring(L, -1);
			LogScriptError(L, "Call LuaEngine::load_class_script Error!!! error is \"{}\"", err);
			lua_pop(L, 1); //pop error
			LogTraceStepReturn(false);
		}
		LogTraceStepReturn(true);
	}
}