


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
#include "TinySocket/TalkLoopInline.h"
#include "TinySocket/LinkTcp.h"
#include "TinySocket/LinkKcp.h"

void test_tinysocket()
{
	LightInk::TalkLoop loop;
	loop.init(LightInk::TalkConfig());
	
	LightInk::LinkError err = loop.add_connect<LightInk::LinkTcp, false>("23.83.239.90", 80);
	if (err != LightInk::LE_Success)
	{
		LogError(LightInk::get_error_string(err));
	}
	/*err = loop.add_connect<LightInk::LinkKcp, false>("127.0.0.1", 1111);
	if (err != LightInk::LE_Success)
	{
		LogError(LightInk::get_error_string(err));
	}*/
	err = loop.add_listen<LightInk::LinkTcp, false>("0.0.0.0", 1111);
	if (err != LightInk::LE_Success)
	{
		LogError(LightInk::get_error_string(err));
	}
	err = loop.add_listen<LightInk::LinkKcp, false>("0.0.0.0", 1111);
	if (err != LightInk::LE_Success)
	{
		LogError(LightInk::get_error_string(err));
	}
	while (true)
	{
		loop.update(10);
	}

	loop.release();
}