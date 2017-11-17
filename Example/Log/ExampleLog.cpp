


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
#include "Log/LogOption.h"


void test_log()
{
	LightInk::LogOption lo;
	LightInk::ChannelOption<LogLockAuto, LightInk::LogChannelType::StdOutChannel> * sc = new LightInk::ChannelOption<LogLockAuto, LightInk::LogChannelType::StdOutChannel>;
	sc->set_color(true);
	lo.add_channel(sc);

	LightInkLog->init_async(1000, LightInk::AsyncMsg::Block);

	lo.set_name("CPP");
	LightInkLogCppInit(lo);
	lo.set_name("Lua");
	LightInkLogLuaInit(lo);
	
	LogMessage("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");

	LogDebug(100);

	LogMessage("{}_{}_{}", 1, 2, 3);

	std::string warn("this is warning");
	LogWarning(warn);

	LogError("this is error");

	LogFatal("this is fatal");

	LogMessage("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

}

void release_log()
{
	LightInkLog->release();
}
