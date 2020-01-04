

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
#include "Msgpack/PackBuffer.h"
#include "Msgpack/DataFixBuffer.h"
#include "TinySocket/LinkUdp.h"
#include "TinySocket/TalkLoop.h"


namespace LightInk
{
	LinkUdp::LinkUdp(TalkLoop * loop, bool ipv6) : LinkBase(loop, ipv6), m_key(0u)
	{
		LogTraceStepCall("LinkUdp::LinkUdp(TalkLoop * loop, bool ipv6)");
		m_remoteAddr.set_ip_v6(ipv6);
		LogTraceStepReturnVoid;
	}
	LinkUdp::~LinkUdp()
	{
		LogTraceStepCall("LinkUdp::~LinkUdp()");
		LogTraceStepReturnVoid;
	}

	bool LinkUdp::init(uint32 readerSize, uint32 writerSize)
	{
		LogTraceStepCall("bool LinkUdp::init(uint32 readerSize, uint32 writerSize)");
		SOCKET handle = ::socket(is_ip_v6() ? AF_INET6 : AF_INET, SOCK_DGRAM, 0);
		if (handle == INVALID_SOCKET)
		{
			m_lastErr = LE_InvalidSocket;
			LogTraceStepReturn(false);
		}
		LogTraceStepReturn(init_with_handle(handle, readerSize, writerSize));
	}

	LinkError LinkUdp::connect(const char * ip, uint16 port)
	{
		LogTraceStepCall("LinkError LinkUdp::connect(const char * ip, uint16 port)");
		if (!set_remote_ip(ip, port))
		{
			LogTraceStepReturn(LE_InvalidAddress);
		}
		if (!init(m_loop->get_config().UDP_MTU, m_loop->get_config().UDP_MTU))
		{
			LogTraceStepReturn(LE_InvalidSocket);
		}
		IPAddrV4V6 addr; addr.set_ip_v6(is_ip_v6());
		if (::bind(get_handle(), addr.get_sockaddr(), addr.get_addr_len()))
		{
			LinkError err = LightInk::get_last_error();
			release();
			LogTraceStepReturn(err);
		}
		if (socket_send(UDPHands::HandsStr, UDPHands::HandsStrLen) <= 0)
		{
			get_last_error();
			release();
			LogTraceStepReturn(m_lastErr);
		}
		set_server(false);
		set_state(LCS_Connected);
		LogTraceStepReturn(LE_Success);
	}
	LinkError LinkUdp::listen(const char * ip, uint16 port)
	{
		LogTraceStepCall("LinkError LinkUdp::listen(const char * ip, uint16 port)");
		IPAddrV4V6 addr;
		LinkError err = addr.set_addr(is_ip_v6(), ip, port);
		if (err != LE_Success)
		{
			LogTraceStepReturn(err);
		}
		if (!init(16, 16))
		{
			LogTraceStepReturn(LE_InvalidSocket);
		}
		if (::bind(get_handle(), addr.get_sockaddr(), addr.get_addr_len()))
		{
			LinkError err = LightInk::get_last_error();
			release();
			LogTraceStepReturn(err);
		}
		set_state(LCS_Listen);
		LogTraceStepReturn(LE_Success);
	}

	LinkBase * LinkUdp::accept()
	{
		LogTraceStepCall("LinkBase * LinkUdp::accept()");
		m_reader.clear();
		if (socket_recv() < 0)
		{
			get_last_error();
			LogTraceStepReturn(NULL);
		}
		if (m_reader.get_data_size() != UDPHands::HandsStrLen || strncmp(static_cast<const char *>(m_reader.get_data()), UDPHands::HandsStr, UDPHands::HandsStrLen))
		{
			m_lastErr = LE_ProtocolError;
			LogTraceStepReturn(NULL);
		}
		LinkUdp * link = static_cast<LinkUdp *>(new_link());
		if (link && link->get_remote_addr() && link->init(m_loop->get_config().LINK_READERBUFFER, m_loop->get_config().LINK_WRITERBUFFER))
		{
			IPAddrV4V6 addr; addr.set_ip_v6(is_ip_v6());
			if (::bind(link->get_handle(), addr.get_sockaddr(), addr.get_addr_len()) == 0)
			{
				*(link->get_remote_addr()) = m_remoteAddr;
				link->set_server(true);
				uint32 key = rand(); key *= static_cast<uint32>(time(NULL));
				link->set_key(key);
				PackBuffer<DataFixBuffer<16> > temp;
				temp.pack(UDPHands::HandsNum);
				temp.pack(key);
				if (link->socket_send(temp.data(), temp.size()) > 0)
				{
					LogTraceStepReturn(link);
				}
			}
			get_last_error();
			link->release();
		}
		delete link;
		LogTraceStepReturn(NULL);
	}

	void LinkUdp::on_connect()
	{
		LogTraceStepCall("void LinkUdp::on_connect()");

		LogTraceStepReturnVoid;
	}
	void LinkUdp::on_read(const void * data, uint32 size)
	{
		LogTraceStepCall("void LinkUdp::on_read(const void * data, uint32 size)");
		if (is_connected())
		{
			LogMessage("{}", fmt::StringRef((const char *)data, size));
		}
		else
		{
			PackBuffer<DataFixBuffer<16> > temp;
			temp.write(data, size);
			uint32 num = 0;
			if (temp.unpack(num) == RE_Success && num == UDPHands::HandsNum && temp.unpack(m_key) == RE_Success)
			{
				on_connect();
				LogTraceStepReturnVoid;
			}
			m_key = 0u;
		}
		LogTraceStepReturnVoid;
	}
	void LinkUdp::on_disconnect()
	{
		LogTraceStepCall("void LinkUdp::on_disconnect()");

		LogTraceStepReturnVoid;
	}

	bool LinkUdp::get_remote_ip(char * ip, size_t ipLen, uint16 * port)
	{
		LogTraceStepCall("bool LinkUdp::get_remote_ip(char * ip, size_t ipLen, uint16 * port)");
		LogTraceStepReturn((m_lastErr = m_remoteAddr.get_addr(ip, ipLen, port)) == LE_Success);
	}
	bool LinkUdp::set_remote_ip(const char * ip, uint16 port)
	{
		LogTraceStepCall("bool LinkUdp::set_remote_ip(const char * ip, uint16 port)");
		LogTraceStepReturn((m_lastErr = m_remoteAddr.set_addr(is_ip_v6(), ip, port)) == LE_Success);
	}

	int32 LinkUdp::socket_recv() 
	{
		LogTraceStepCall("int32 LinkUdp::socket_recv() ");
		socklen_t addrLen = m_remoteAddr.get_addr_len();
		int32 len = ::RECVFROM(m_handle, m_reader.get_free(), m_reader.get_free_size(), 0, m_remoteAddr.get_sockaddr(), &addrLen);
		if (len > 0) { m_reader.add_write_pos(len); }
		LogTraceStepReturn(len);
	}
	int32 LinkUdp::socket_send(const void * data, int32 size)
	{
		LogTraceStepCall("int32 LinkUdp::socket_send(const void * data, int32 size)");
		LogTraceStepReturn(::SENDTO(m_handle, data, size, 0, m_remoteAddr.get_sockaddr(), m_remoteAddr.get_addr_len()));
	}
}