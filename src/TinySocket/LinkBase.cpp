

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
#include "TinySocket/LinkBase.h"
#include "TinySocket/TalkLoopInline.h"

namespace LightInk
{
	namespace LinkTickType
	{
		const int Queue2Node[QueueTypeCount] = 
		{
			NodeTypePing, NodeTypeUdp,
		};
	}
	namespace UDPHands
	{
		const uint32 HandsNum = 0x025cd520;
		const unsigned char HandsUStr [sizeof(HandsNum)] = { 0x02, 0x5c, 0xd5, 0x20 };
		const char * const HandsStr = (const char *)HandsUStr;
		const size_t HandsStrLen = sizeof(HandsNum);
	}

	IPAddrV4V6::IPAddrV4V6() : m_ipV6(false)
	{
		LogTraceStepCall("IPAddrV4V6::IPAddrV4V6()");
		memset(&m_addr, 0, sizeof(m_addr));
		m_addr.addrv4.sin_family = AF_INET;
		LogTraceStepReturnVoid;
	}
	void IPAddrV4V6::clear()
	{
		LogTraceStepCall("void IPAddrV4V6::clear()");
		m_ipV6 = false;
		memset(&m_addr, 0, sizeof(m_addr));
		m_addr.addrv4.sin_family = AF_INET;
		LogTraceStepReturnVoid;
	}
	LinkError IPAddrV4V6::get_local_addr(bool ipV6, SOCKET handle)
	{
		LogTraceStepCall("LinkError IPAddrV4V6::get_local_addr(bool ipV6, SOCKET handle)");
		if (ipV6)
		{
			socklen_t addrLen = sizeof(m_addr.addrv6);
			if (::getsockname(handle, (struct sockaddr *) &m_addr.addrv6, &addrLen) != LE_Success)
			{
				LogTraceStepReturn(get_last_error());
			}
		}
		else
		{
			socklen_t addrLen = sizeof(m_addr.addrv4);
			if (::getsockname(handle, (struct sockaddr *) &m_addr.addrv4, &addrLen) != LE_Success)
			{
				LogTraceStepReturn(get_last_error());
			}
		}
		m_ipV6 = ipV6;
		LogTraceStepReturn(LE_Success);

	}
	LinkError IPAddrV4V6::get_remote_addr(bool ipV6, SOCKET handle)
	{
		LogTraceStepCall("LinkError IPAddrV4V6::get_remote_addr(bool ipV6, SOCKET handle)");
		if (ipV6)
		{
			socklen_t addrLen = sizeof(m_addr.addrv6);
			if (::getpeername(handle, (struct sockaddr *) &m_addr.addrv6, &addrLen) != LE_Success)
			{
				LogTraceStepReturn(get_last_error());
			}
		}
		else
		{
			socklen_t addrLen = sizeof(m_addr.addrv4);
			if (::getpeername(handle, (struct sockaddr *) &m_addr.addrv4, &addrLen) != LE_Success)
			{
				LogTraceStepReturn(get_last_error());
			}
		}
		m_ipV6 = ipV6;
		LogTraceStepReturn(LE_Success);
	}

	bool IPAddrV4V6::is_ip_v6()
	{
		LogTraceStepCall("bool IPAddrV4V6::is_ip_v6()");
		LogTraceStepReturn(m_ipV6);
	}
	void IPAddrV4V6::set_ip_v6(bool ipV6)
	{
		LogTraceStepCall("void IPAddrV4V6::set_ip_v6(bool ipV6)");
		if (ipV6) { m_addr.addrv6.sin6_family = AF_INET6; }
		else { m_addr.addrv4.sin_family = AF_INET; }
		m_ipV6 = ipV6;
		LogTraceStepReturnVoid;
	}

	LinkError IPAddrV4V6::set_addr(bool ipV6, const char * ip, uint16 port)
	{
		LogTraceStepCall("LinkError IPAddrV4V6::set_addr(bool ipV6, const char * ip, uint16 port)");
		if (ipV6)
		{
			if (::inet_pton(AF_INET6, ip, &m_addr.addrv6.sin6_addr) <= 0)
			{
				LogTraceStepReturn(LE_InvalidAddress);
			}
			m_addr.addrv6.sin6_family = AF_INET6;
			m_addr.addrv6.sin6_port = ::htons(port);
		}
		else
		{
			if (::inet_pton(AF_INET, ip, &m_addr.addrv4.sin_addr) <= 0)
			{
				LogTraceStepReturn(LE_InvalidAddress);
			}
			m_addr.addrv4.sin_family = AF_INET;
			m_addr.addrv4.sin_port = ::htons(port);
		}
		m_ipV6 = ipV6;
		LogTraceStepReturn(LE_Success);
	}
	LinkError IPAddrV4V6::get_addr(char * ip, size_t ipLen, uint16 * port)
	{
		LogTraceStepCall("LinkError IPAddrV4V6::get_addr(char * ip, size_t ipLen, uint16 * port)");
		if (m_ipV6)
		{
			if (::inet_ntop(AF_INET6, &m_addr.addrv6.sin6_addr, ip, ipLen) == NULL)
			{
				LogTraceStepReturn(LE_InvalidAddress);
			}
			*port = ::ntohs(m_addr.addrv6.sin6_port);
		}
		else
		{
			if (::inet_ntop(AF_INET, &m_addr.addrv4.sin_addr, ip, ipLen) == NULL)
			{
				LogTraceStepReturn(LE_InvalidAddress);
			}
			*port = ::ntohs(m_addr.addrv4.sin_port);
		}
		LogTraceStepReturn(LE_Success);
	}
	const struct sockaddr * IPAddrV4V6::get_sockaddr()
	{
		LogTraceStepCall("const struct sockaddr * IPAddrV4V6::get_sockaddr()");
		if (m_ipV6) { LogTraceStepReturn((const struct sockaddr *) &m_addr.addrv6); }
		else { LogTraceStepReturn((const struct sockaddr *) &m_addr.addrv4); }
	}
	socklen_t IPAddrV4V6::get_addr_len()
	{
		LogTraceStepCall("socklen_t IPAddrV4V6::get_addr_len()");
		if (m_ipV6) { LogTraceStepReturn(sizeof(m_addr.addrv6)); }
		else { LogTraceStepReturn(sizeof(m_addr.addrv4)); }
	}



	LinkBase::LinkBase(TalkLoop * loop, bool ipv6) : 
		m_loop(loop), m_handle(INVALID_SOCKET), m_ipV6(ipv6), 
		m_isBlocking(false), m_lastErr(LE_Success), m_state(LCS_None), m_isServer(false), m_pingTimes(0)
	{
		LogTraceStepCall("LinkBase::LinkBase(int32 af)");
		LogTraceStepReturnVoid;
	}

	LinkBase::~LinkBase()
	{
		LogTraceStepCall("LinkBase::~LinkBase()");
		LogTraceStepReturnVoid;
	}

	bool LinkBase::init_with_handle(SOCKET handle, uint32 readerSize, uint32 writerSize)
	{
		LogTraceStepCall("bool LinkBase::init_with_handle(SOCKET handle, uint32 readerSize, uint32 writerSize)");
		if (is_socket_valid())
		{
			::shutdown(handle, LinkShutDownMode::All);
			::CLOSE(handle);
			LogTraceStepReturn(false);
		}
		m_handle = handle;
		if (!set_blocking(false) || !set_reuse_addr(true) || 
			!m_reader.init(readerSize) || !m_writer.init(writerSize))
		{
			release();
			LogTraceStepReturn(false);
		}
		LogTraceStepReturn(true);
	}
	bool LinkBase::release()
	{
		LogTraceStepCall("bool LinkBase::release()");
		shutdown();
		m_reader.clear();
		m_writer.clear();
		m_loop = NULL;
		set_state(LCS_Disconnected);
		LogTraceStepReturn(true);
	}

	bool LinkBase::close()
	{
		LogTraceStepCall("bool LinkBase::close()");
		if (is_socket_valid())
		{
			int err = ::CLOSE(m_handle);
			if (err != LE_Success && err != LE_Wouldblock)
			{
				get_last_error();
				LogTraceStepReturn(false);
			}
			m_handle = INVALID_SOCKET;
		}
		LogTraceStepReturn(true);
	}

	bool LinkBase::is_socket_valid()
	{
		LogTraceStepCall("bool LinkBase::is_socket_valid()");
		LogTraceStepReturn(m_handle != INVALID_SOCKET);
	}

	const char * LinkBase::get_error_string()
	{
		LogTraceStepCall("const char * LinkBase::get_error_string()");
		LogTraceStepReturn(LightInk::get_error_string(m_lastErr));
	}
	LinkError LinkBase::get_error()
	{
		LogTraceStepCall("LinkError LinkBase::get_error()");
		LogTraceStepReturn(m_lastErr);
	}

	bool LinkBase::read_data()
	{
		LogTraceStepCall("bool LinkBase::read_data()");
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
				on_read(m_reader.get_data(), m_reader.get_data_size());
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
	int32 LinkBase::write_data(const void * data, int32 size)
	{
		LogTraceStepCall("int32 LinkBase::write_data(const void * data, int32 size)");
		int32 sendSize = 0;
		if (!is_socket_valid())
		{
			m_lastErr = LE_InvalidSocket;
			LogTraceStepReturn(sendSize);
		}
		bool needSend = false;
		do 
		{
			needSend = false;
			if (m_writer.is_empty())
			{
				int32 len = socket_send((const char *)data + sendSize, size - sendSize);
				if (len >= 0)
				{
					m_pingTimes = 0;
					sendSize += len;
					if (sendSize < size)
					{
						needSend = true;
					}
				}
				else
				{
					sendSize += m_writer.set_data(static_cast<const char *>(data) + sendSize, size - sendSize);
				}
			}
			else
			{
				char * a = NULL; uint32 sizeA = 0u;
				char * b = NULL; uint32 sizeB = 0u;
				m_writer.get_ab_data(&a, sizeA, &b, sizeB);
				int32 len = socket_send(a, sizeA);
				if (len >= 0)
				{
					needSend = true;
					m_writer.remove(len);
				}
				else
				{
					sendSize = m_writer.set_data(static_cast<const char *>(data), size);
				}
			}
		} while (needSend);
		LogTraceStepReturn(sendSize);
	}

	void LinkBase::ping()
	{
		LogTraceStepCall("void LinkBase::ping()");
		LogTraceStepReturnVoid;
	}
	void LinkBase::on_ping()
	{
		LogTraceStepCall("void LinkBase::on_ping()");
		m_pingTimes = 0;
		if (is_server())
		{
			ping();
		}
		LogTraceStepReturnVoid;
	}
	void LinkBase::on_ping_tick()
	{
		LogTraceStepCall("void LinkBase::on_ping_tick()");
		++m_pingTimes;
		if (m_pingTimes > m_loop->get_config().LP_PINGOUT)
		{
			LogError("Call LinkBase::on_ping_tick() Ping Out {}", m_pingTimes);
			disconnect();
			LogTraceStepReturnVoid;
		}
		if (m_pingTimes > 1 && is_connected() && !is_server())
		{
			ping();
		}
		LogTraceStepReturnVoid;
	}

	bool LinkBase::is_blocking()
	{
		LogTraceStepCall("bool LinkBase::is_blocking()");
		LogTraceStepReturn(m_isBlocking);
	}
	bool LinkBase::set_blocking(bool blocking)
	{
		LogTraceStepCall("bool LinkBase::set_blocking(bool blocking)");
		if (!is_socket_valid())
		{
			m_lastErr = LE_InvalidSocket;
			LogTraceStepReturn(false);
		}
#ifdef WIN32
		u_long flag = blocking ? 0u : 1u;
		if (::ioctlsocket(m_handle, FIONBIO, &flag) != LE_Success)
		{
			get_last_error();
			LogTraceStepReturn(false);
		}
#else
		int flag = ::fcntl(m_handle, F_GETFL);
		if (flag < 0)
		{
			get_last_error();
			LogTraceStepReturn(false);
		}
		if (blocking) { flag &= (~O_NONBLOCK); }
		else { flag |= O_NONBLOCK; }
		if (::fcntl(m_handle, F_SETFL, flag) != LE_Success)
		{
			get_last_error();
			LogTraceStepReturn(false);
		}
		
#endif
		m_isBlocking = blocking;
		LogTraceStepReturn(true);
	}

	bool LinkBase::set_linger(bool enable, uint16 closeDelay)
	{
		LogTraceStepCall("bool LinkBase::set_linger(bool enable, uint16 closeDelay)");
		if (!is_socket_valid())
		{
			m_lastErr = LE_InvalidSocket;
			LogTraceStepReturn(false);
		}
		struct linger op;
		op.l_onoff = enable ? 1 : 0;
		op.l_linger = closeDelay;
		if (::SETSOCKOPT(m_handle, SOL_SOCKET, SO_LINGER, &op, sizeof(op)) != LE_Success)
		{
			get_last_error();
			LogTraceStepReturn(false);
		}
		LogTraceStepReturn(true);
	}
	bool LinkBase::set_nagle(bool enable)
	{
		LogTraceStepCall("bool LinkBase::set_nagle(bool enable)");
		if (!is_socket_valid())
		{
			m_lastErr = LE_InvalidSocket;
			LogTraceStepReturn(false);
		}
		int noDelay = enable ? 0 : 1;
		if (::SETSOCKOPT(m_handle, IPPROTO_TCP, TCP_NODELAY, &noDelay, sizeof(noDelay)) != LE_Success)
		{
			get_last_error();
			LogTraceStepReturn(false);
		}
		LogTraceStepReturn(true);
	}
	bool LinkBase::set_reuse_addr(bool enable)
	{
		LogTraceStepCall("bool LinkBase::set_reuse_addr(bool enable)");
		if (!is_socket_valid())
		{
			m_lastErr = LE_InvalidSocket;
			LogTraceStepReturn(false);
		}
		int reuse = enable ? 0 : 1;
		if (::SETSOCKOPT(m_handle, SOL_SOCKET, TCP_NODELAY, &reuse, sizeof(reuse)) != LE_Success)
		{
			get_last_error();
			LogTraceStepReturn(false);
		}
		LogTraceStepReturn(true);
	}

	bool LinkBase::set_qos(LinkIPPriorty::__ ipPri, LinkServiceType::__ ipService)
	{
		LogTraceStepCall("bool LinkBase::set_qos(LinkIPPriorty::__ ipPri, LinkServiceType::__ ipService)");
		if (!is_socket_valid())
		{
			m_lastErr = LE_InvalidSocket;
			LogTraceStepReturn(false);
		}
		unsigned char serviceType = static_cast<unsigned char>((ipPri << 4) | ipService);
		if (::SETSOCKOPT(m_handle, IPPROTO_IP, IP_TOS, &serviceType, sizeof(serviceType)) != LE_Success)
		{
			get_last_error();
			LogTraceStepReturn(false);
		}
		LogTraceStepReturn(true);
	}
	int LinkBase::get_qos()
	{
		LogTraceStepCall("int LinkBase::get_qos()");
		if (!is_socket_valid())
		{
			m_lastErr = LE_InvalidSocket;
			LogTraceStepReturn(LE_Error);
		}
		unsigned char serviceType = 0u;
		int len = sizeof(serviceType);
		if (::GETSOCKOPT(m_handle, IPPROTO_IP, IP_TOS, &serviceType, &len) != LE_Success)
		{
			get_last_error();
			LogTraceStepReturn(LE_Error);
		}
		LogTraceStepReturn(serviceType);
	}
	LinkIPPriorty::__ LinkBase::qos_to_ip_priorty(int qos)
	{
		LogTraceStepCall("LinkIPPriorty::__ LinkBase::qos_to_ip_priorty(int qos)");
		if (qos < 256 && qos >= 0)
		{
			qos >>= 4;
			LogTraceStepReturn((LinkIPPriorty::__)qos);
		}
		LogTraceStepReturn(LinkIPPriorty::Routine);
	}
	LinkServiceType::__ LinkBase::qos_to_service_type(int qos)
	{
		LogTraceStepCall("LinkServiceType::__ LinkBase::qos_to_service_type(int qos)");
		if (qos < 256 && qos >= 0)
		{
			qos &= 0xfu;
			LogTraceStepReturn((LinkServiceType::__)qos);
		}
		LogTraceStepReturn(LinkServiceType::Normal);
	}

	bool LinkBase::get_remote_ip(char * ip, size_t ipLen, uint16 * port)
	{
		LogTraceStepCall("bool LinkBase::get_remote_ip(char * ip, size_t ipLen, uint16 * port)");
		IPAddrV4V6 addr;
		if ((m_lastErr = addr.get_remote_addr(is_ip_v6(), m_handle)) == LE_Success && (m_lastErr = addr.get_addr(ip, ipLen, port)) == LE_Success)
		{
			LogTraceStepReturn(true);
		}
		LogTraceStepReturn(false);
	}

	bool LinkBase::set_remote_ip(const char * ip, uint16 port)
	{
		LogTraceStepCall("bool LinkBase::set_remote_ip(const char * ip, uint16 port)");
		LogTraceStepReturn(true);
	}
	bool LinkBase::get_local_ip(char * ip, size_t ipLen, uint16 * port)
	{
		LogTraceStepCall("bool LinkBase::get_local_ip(char * ip, size_t ipLen, uint16 * port)");
		IPAddrV4V6 addr;
		if ((m_lastErr = addr.get_local_addr(is_ip_v6(), m_handle)) == LE_Success && (m_lastErr = addr.get_addr(ip, ipLen, port)) == LE_Success)
		{
			LogTraceStepReturn(true);
		}
		LogTraceStepReturn(false);
	}

	uint64 LinkBase::add_tick(LinkTickType::QueueType qt, uint64 now)
	{
		LogTraceStepCall("uint64 LinkBase::add_tick(LinkTickType::QueueType qt, uint64 now)");
		if (qt == LinkTickType::QueueTypePing && m_state != LCS_Listen)
		{
			const TalkConfig & config = m_loop->get_config();
			if (config.LP_PINGINTERVAL > 0u)
			{
				LogTraceStepReturn(now + config.LP_PINGINTERVAL * 1000);
			}
		}
		LogTraceStepReturn(0u);
	}
	uint64 LinkBase::check_tick(LinkTickType::QueueType qt, uint64 now)
	{
		LogTraceStepCall("uint64 LinkBase::check_tick(LinkTickType::QueueType qt, uint64 now)");
		if (qt == LinkTickType::QueueTypePing && m_state == LCS_Connected)
		{
			on_ping_tick();
			LogTraceStepReturn(now + m_loop->get_config().LP_PINGINTERVAL * 1000);
		}
		LogTraceStepReturn(0u);
	}

	bool LinkBase::shutdown_by_mode(LinkShutDownMode::__ mode)
	{
		LogTraceStepCall("bool LinkBase::shutdown_by_mode(LinkShutDownMode::__ mode)");
		if (is_socket_valid())
		{
			if (::shutdown(m_handle, mode) != LE_Success)
			{
				LogTraceStepReturn(false);
			}
		}
		LogTraceStepReturn(close());
	}

	void LinkBase::get_last_error()
	{
		LogTraceStepCall("void LinkBase::get_last_error()");
		m_lastErr = LightInk::get_last_error();
		LogTraceStepReturnVoid;
	}
}