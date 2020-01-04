

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
#include "TinySocket/LinkError.h"
#include "TinySocket/LinkTcp.h"
#include "TinySocket/TalkLoop.h"

namespace LightInk
{
	LinkTcp::LinkTcp(TalkLoop * loop, bool ipv6) : LinkBase(loop, ipv6)
	{
		LogTraceStepCall("LinkTcp::LinkTcp(TalkLoop * loop, bool ipv6)");
		LogTraceStepReturnVoid;
	}
	LinkTcp::~LinkTcp()
	{
		LogTraceStepCall("LinkTcp::~LinkTcp()");
		LogTraceStepReturnVoid;
	}

	bool LinkTcp::init(uint32 readerSize, uint32 writerSize)
	{
		LogTraceStepCall("bool LinkTcp::init(uint32 readerSize, uint32 writerSize)");
		SOCKET handle = ::socket(is_ip_v6() ? AF_INET6 : AF_INET, SOCK_STREAM, 0);
		if (handle == INVALID_SOCKET)
		{
			m_lastErr = LE_InvalidSocket;
			LogTraceStepReturn(false);
		}
		if (!init_with_handle(handle, readerSize, writerSize))
		{
			LogTraceStepReturn(false);
		}
		if (!set_nagle(false))
		{
			release();
			LogTraceStepReturn(false);
		}
		LogTraceStepReturn(true);
	}

	LinkError LinkTcp::connect(const char * ip, uint16 port)
	{
		LogTraceStepCall("LinkError LinkTcp::connect(const char * ip, uint16 port)");
		IPAddrV4V6 addr;
		LinkError err = addr.set_addr(is_ip_v6(), ip, port);
		if (err != LE_Success)
		{
			LogTraceStepReturn(err);
		}
		if (!init(m_loop->get_config().LINK_READERBUFFER, m_loop->get_config().LINK_WRITERBUFFER))
		{
			LogTraceStepReturn(LE_InvalidSocket);
		}
		if (::CONNECT(get_handle(), addr.get_sockaddr(), addr.get_addr_len()))
		{
			LinkError err = LightInk::get_last_error();
			if (err != LE_Wouldblock)
			{
				release();
				LogTraceStepReturn(err);
			}
		}
		set_state(LCS_Connecting);
		set_server(false);
		LogTraceStepReturn(LE_Success);
	}
	LinkError LinkTcp::listen(const char * ip, uint16 port)
	{
		LogTraceStepCall("LinkError LinkTcp::listen(const char * ip, uint16 port)");
		IPAddrV4V6 addr;
		LinkError err = addr.set_addr(is_ip_v6(), ip, port);
		if (err != LE_Success)
		{
			LogTraceStepReturn(err);
		}
		if (!init(0, 0))
		{
			LogTraceStepReturn(LE_InvalidSocket);
		}
		if (::bind(get_handle(), addr.get_sockaddr(), addr.get_addr_len()))
		{
			LinkError err = LightInk::get_last_error();
			release();
			LogTraceStepReturn(err);
		}
		if (::listen(get_handle(), SOMAXCONN))
		{
			LinkError err = LightInk::get_last_error();
			release();
			LogTraceStepReturn(err);
		}
		set_state(LCS_Listen);
		LogTraceStepReturn(LE_Success);
	}

	LinkBase * LinkTcp::accept()
	{
		LogTraceStepCall("LinkBase * LinkTcp::accept()");
		SOCKET sock = ::accept(m_handle, NULL, NULL);
		if (sock == INVALID_SOCKET)
		{
			get_last_error();
			LogTraceStepReturn(NULL);
		}
		LinkBase * link = new_link();
		if (link->init_with_handle(sock, m_loop->get_config().LINK_READERBUFFER, m_loop->get_config().LINK_WRITERBUFFER))
		{
			link->set_server(true);
			LogTraceStepReturn(link);
		}
		delete link;
		LogTraceStepReturn(NULL);
	}

	void LinkTcp::on_connect()
	{
		LogTraceStepCall("void LinkTcp::on_connect()");
		LogTraceStepReturnVoid;
	}
	void LinkTcp::on_read(const void * data, uint32 size)
	{
		LogTraceStepCall("void LinkTcp::on_read(const void * data, uint32 size)");
		LogMessage("{}", fmt::StringRef((const char *)data, size));
		LogTraceStepReturnVoid;
	}
	void LinkTcp::on_disconnect()
	{
		LogTraceStepCall("void LinkTcp::on_disconnect()");

		LogTraceStepReturnVoid;
	}
}