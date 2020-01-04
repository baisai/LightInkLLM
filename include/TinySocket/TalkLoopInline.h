

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

#ifndef LIGHTINK_TINYSOCKET_TALKLOOPINLINE_H_
#define LIGHTINK_TINYSOCKET_TALKLOOPINLINE_H_

#include "Log/Log.h"
#include "TinySocket/TalkLoop.h"


namespace LightInk
{
	inline const TalkConfig & TalkLoop::get_config()
	{
		LogTraceStepCall("const TalkConfig & TalkLoop::get_config()");
		LogTraceStepReturn(m_config);
	}

	template <typename LinkType, bool IpV6>
	LinkError TalkLoop::add_listen(const char * host, uint16 port)
	{
		LogTraceStepCall("LinkError TalkLoop::add_listen(const char * host, uint16 port)");
		LinkType * link = new LinkType(this, IpV6);
		if (link == NULL)
		{
			LogTraceStepReturn(LE_InvalidPointer);
		}
		LinkError err = link->listen(host, port);
		if (err != LE_Success)
		{
			delete link;
			LogTraceStepReturn(err);
		}
		add_link(link);
		LogTraceStepReturn(LE_Success);
	}

	template <typename LinkType, bool IpV6>
	LinkError TalkLoop::add_connect(const char * host, uint16 port)
	{
		LogTraceStepCall("LinkError TalkLoop::add_connect(const char * host, uint16 port)");
		LinkType * link = new LinkType(this, IpV6);
		if (link == NULL)
		{
			LogTraceStepReturn(LE_InvalidSocket);
		}
		LinkError err = link->connect(host, port);
		if (err != LE_Success)
		{
			delete link;
			LogTraceStepReturn(err);
		}
		add_link(link);
		LogTraceStepReturn(LE_Success);
	}

}




#endif