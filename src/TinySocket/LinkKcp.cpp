

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
#include "Common/Memory/MemoryAllocator.h"
#include "Msgpack/PackBuffer.h"
#include "Msgpack/DataFixBuffer.h"
#include "TinySocket/LinkKcp.h"
#include "TinySocket/TalkLoop.h"


namespace LightInk
{
	LinkKcp::LinkKcp(TalkLoop * loop, bool ipv6) : LinkUdp(loop, ipv6), m_kcp(NULL), m_updateTime(0u)
	{
		LogTraceStepCall("LinkKcp::LinkKcp(TalkLoop * loop, bool ipv6)");
		LogTraceStepReturnVoid;
	}
	LinkKcp::~LinkKcp()
	{
		LogTraceStepCall("LinkKcp::~LinkKcp()");
		LogTraceStepReturnVoid;
	}

	bool LinkKcp::read_data()
	{
		LogTraceStepCall("bool LinkKcp::read_data()");
		if (!is_socket_valid())
		{
			m_lastErr = LE_InvalidSocket;
			LogTraceStepReturn(false);
		}
		bool needRecv = false;
		do 
		{
			needRecv = false;
			int32 len = socket_recv();
			if (len > 0)
			{
				needRecv = true;
				m_pingTimes = 0;
				if (is_connected())
				{
					ikcp_input(m_kcp, m_reader.get_data(), m_reader.get_data_size());
					m_updateTime = OsHelper::get_up_time();
					try_recv_kcp();
				}
				else
				{
					on_read(m_reader.get_data(), m_reader.get_data_size());
				}
				m_reader.clear();
			}
			else if (len == 0)
			{
				disconnect();
				LogTraceStepReturn(false);
			}
			else 
			{
				get_last_error();
				if(m_lastErr != LE_Wouldblock &&
					m_lastErr != LE_Interrupted)
				{
					LogTraceStepReturn(false);
				}
			}
		} while (needRecv);
		LogTraceStepReturn(true);
	}
	int32 LinkKcp::write_data(const void * data, int32 size)
	{
		LogTraceStepCall("int32 LinkKcp::write_data(const void * data, int32 size)");
		if (is_connected())
		{
			m_updateTime = OsHelper::get_up_time();
			LogTraceStepReturn(ikcp_send(m_kcp, static_cast<const char *>(data), size));
		}
		else
		{
			LogTraceStepReturn(LinkUdp::write_data(data, size));
		}
	}
	int LinkKcp::on_kcp_output(const char *buf, int len, struct IKCPCB *kcp, void *user)
	{
		LogTraceStepCall("int LinkKcp::on_kcp_output(const char *buf, int len, struct IKCPCB *kcp, void *user)");
		LinkKcp * self = (LinkKcp *)user;
		LogTraceStepReturn((self->LinkBase::write_data)(buf, len));
	}

	void LinkKcp::on_connect()
	{
		LogTraceStepCall("void LinkKcp::on_connect()");
		init_kcp();
		LogTraceStepReturnVoid;
	}
	void LinkKcp::on_read(const void * data, uint32 size)
	{
		LogTraceStepCall("void LinkKcp::on_read(const void * data, uint32 size)");
		if (is_connected())
		{
			LogMessage("{}", fmt::StringRef((const char *)data, size));
		}	
		else
		{
			LinkUdp::on_read(data, size);
		}
		LogTraceStepReturnVoid;
	}
	void LinkKcp::on_disconnect()
	{
		LogTraceStepCall("void LinkKcp::on_disconnect()");
		if (m_kcp)
		{
			ikcp_release(m_kcp);
			m_kcp = NULL;
		}
		LogTraceStepReturnVoid;
	}

	uint64 LinkKcp::add_tick(LinkTickType::QueueType qt, uint64 now)
	{
		LogTraceStepCall("uint64 LinkKcp::add_tick(LinkTickType::QueueType qt, uint64 now)");
		if (qt == LinkTickType::QueueTypeKcp)
		{
			const TalkConfig & config = m_loop->get_config();
			if (config.KCP_UPDATETICK > 0u)
			{
				LogTraceStepReturn(now + config.KCP_UPDATETICK * 1000);
			}
			LogTraceStepReturn(0u);
		}
		else
		{
			LogTraceStepReturn((LinkBase::add_tick(qt, now)));
		}
	}
	uint64 LinkKcp::check_tick(LinkTickType::QueueType qt, uint64 now)
	{
		LogTraceStepCall("uint64 LinkKcp::check_tick(LinkTickType::QueueType qt, uint64 now)");
		if (qt == LinkTickType::QueueTypeKcp)
		{
			if (is_connected())
			{
				uint64 now = OsHelper::get_up_time();
				if (m_updateTime <= now)
				{
					int32 t = now & 0x7fffffffu;
					ikcp_update(m_kcp, t);
					m_updateTime = (now - t + ikcp_check(m_kcp, t));
					try_recv_kcp();
				}
			}
			LogTraceStepReturn(now + m_loop->get_config().KCP_UPDATETICK);
		}
		else
		{
			LogTraceStepReturn((LinkBase::check_tick(qt, now)));
		}
	}

	bool LinkKcp::init_kcp()
	{
		LogTraceStepCall("bool LinkKcp::init_kcp()");
		if (m_kcp)
		{
			LogTraceStepReturn(true);
		}
		if (!m_key)
		{
			LogTraceStepReturn(false);
		}
		const TalkConfig & config = m_loop->get_config();
		m_kcp = ikcp_create(m_key, this);
		m_kcp->writelog = LinkKcp::kcp_log;
		ikcp_setoutput(m_kcp, LinkKcp::on_kcp_output);
		ikcp_nodelay(m_kcp, 1, config.KCP_UPDATETICK, config.KCP_FASTRESEND, config.KCP_NOCONTROL);
		ikcp_wndsize(m_kcp, config.KCP_SENDWND, config.KCP_RECVWND);
		ikcp_setmtu(m_kcp, config.UDP_MTU);
		m_kcp->rx_minrto = config.KCP_RTO;
		LogTraceStepReturn(true);
	}
	void LinkKcp::try_recv_kcp()
	{
		LogTraceStepCall("void LinkKcp::try_recv_kcp()");
		int allLen = ikcp_peeksize(m_kcp);
		char buffer[512] = { 0 };
		while (allLen > 0)
		{
			int recLen = ikcp_recv(m_kcp, buffer, sizeof(buffer));
			if (recLen > 0)
			{
				allLen -= recLen;
				on_read(buffer, recLen);
			}
		}
		LogTraceStepReturnVoid;
	}
	void LinkKcp::kcp_log(const char *log, struct IKCPCB *kcp, void *user)
	{
		LogTraceStepCall("void LinkKcp::kcp_log(const char *log, struct IKCPCB *kcp, void *user)");
		LinkKcp * self = (LinkKcp *)user;
		LogWarning("LinkID = {} --> Kcp Log \'{}\'", self->get_handle(), log);
		LogTraceStepReturnVoid;
	}



	/////////////////////////////////////////////////////////////////////////
	//KcpAllocator
	////////////////////////////////////////////////////////////////////////
	struct KcpAllocator
	{
		KcpAllocator() { ikcp_allocator(li_malloc, li_free); }
	};
	KcpAllocator ka;
}