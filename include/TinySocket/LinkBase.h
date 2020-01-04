

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

#ifndef LIGHTINK_TINYSOCKET_LINKBASE_H_
#define LIGHTINK_TINYSOCKET_LINKBASE_H_

#include "TinySocket/PlatformHost.h"

#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#if defined(_LINUX) || defined (_DARWIN)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netdb.h>
#endif
#ifdef _LINUX
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if.h>
#include <sys/sendfile.h>
#endif
#ifdef _DARWIN
#include <net/if.h>
#endif
#if defined(_LINUX) || defined (_DARWIN)
#include <sys/time.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#ifdef _WIN32
#include <io.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#endif

#include "Common/Type.h"
#include "Common/Memory/BaseBuffer.h"
#include "Common/Memory/RingBuffer.h"
#include "TinySocket/LinkError.h"


namespace LightInk
{
	namespace LinkShutDownMode
	{
		enum __
		{
			Read = SHUT_RD,
			Write = SHUT_WR,
			All = SHUT_RDWR,
		};
	}

	namespace LinkIPPriorty
	{
		enum __
		{
			Routine = 0x0,
			Priorty = 0x1,
			Immediate = 0x2,
			Flash = 0x3,
			FlashOverride = 0x4,
			Critic = 0x5,
			InternetControl = 0x6,
			NetworkControl = 0x7,
		};
	}

	namespace LinkServiceType
	{
		enum __
		{
			MinDelay = 0x8,
			MaxThroughput = 0x4,
			MaxReliable = 0x2,
			MinCost = 0x1,
			Normal = 0x0,
		};
	}

	enum LinkActiveEvent
	{
		LAE_None = 0x0u,
		LAE_Accept = 0x1u,
		LAE_Connect = 0x2u,
		LAE_Read = 0x4u,
		LAE_Write = 0x8u,
		LAE_Error = 0x10u,
	};

	enum LinkCurrentState
	{
		LCS_None,
		LCS_Listen,
		LCS_Connecting,
		LCS_Connected,
		LCS_Disconnecting,
		LCS_Disconnected,
	};

	namespace LinkTickType
	{
		enum NodeType
		{
			NodeTypePing,
			NodeTypeUdp,

			NodeTypeCount,
		};
		enum QueueType
		{
			QueueTypePing,
			QueueTypeKcp,

			QueueTypeCount,
		};

		extern const int Queue2Node[QueueTypeCount];
	}
	namespace UDPHands
	{
		extern const uint32 HandsNum;
		extern const unsigned char HandsUStr [sizeof(HandsNum)];
		extern const char * const HandsStr;
		extern const size_t HandsStrLen;
	}

	class LIGHTINK_DECL IPAddrV4V6
	{
	public:
		IPAddrV4V6();
		void clear();
		LinkError get_local_addr(bool ipV6, SOCKET handle);
		LinkError get_remote_addr(bool ipV6, SOCKET handle);
		bool is_ip_v6();
		void set_ip_v6(bool ipV6);
		LinkError set_addr(bool ipV6, const char * ip, uint16 port);
		LinkError get_addr(char * ip, size_t ipLen, uint16 * port);
		const struct sockaddr * get_sockaddr();
		socklen_t get_addr_len();

	private:
		bool m_ipV6;
		union
		{
			struct sockaddr_in addrv4;
			struct sockaddr_in6 addrv6;
		} m_addr;
	};

	class TalkLoop;
	class LIGHTINK_DECL LinkBase
	{
	public:
		LinkBase(TalkLoop * loop, bool ipv6);
		virtual ~LinkBase();

		virtual bool init(uint32 readerSize, uint32 writerSize) = 0;
		virtual bool init_with_handle(SOCKET handle, uint32 readerSize, uint32 writerSize);
		virtual bool release();

		virtual LinkError connect(const char * ip, uint16 port) = 0;
		virtual LinkError listen(const char * ip, uint16 port) = 0;
		virtual LinkBase * accept() = 0;

		virtual bool close();
		virtual bool shutdown();

		void disconnect();

		SOCKET get_handle();

		virtual bool is_socket_valid();

		const char * get_error_string();
		LinkError get_error();

		virtual bool read_data();
		virtual int32 write_data(const void * data, int32 size);

		virtual void on_connect() = 0;
		virtual void on_read(const void * data, uint32 size) = 0;
		virtual void on_disconnect() = 0;

		virtual void ping();
		virtual void on_ping();
		virtual void on_ping_tick();

		bool is_blocking();
		bool set_blocking(bool blocking);
		bool is_ip_v6();

		bool set_linger(bool enable, uint16 closeDelay);
		bool set_nagle(bool enable);
		bool set_reuse_addr(bool enable);

		bool set_qos(LinkIPPriorty::__ ipPri, LinkServiceType::__ ipService);
		int get_qos();
		LinkIPPriorty::__ qos_to_ip_priorty(int qos);
		LinkServiceType::__ qos_to_service_type(int qos);

		void set_state(LinkCurrentState state);
		LinkCurrentState get_state();

		virtual IPAddrV4V6 * get_remote_addr();

		virtual bool get_remote_ip(char * ip, size_t ipLen, uint16 * port);
		virtual bool set_remote_ip(const char * ip, uint16 port);
		virtual bool get_local_ip(char * ip, size_t ipLen, uint16 * port);

		virtual uint64 add_tick(LinkTickType::QueueType qt, uint64 now);
		virtual uint64 check_tick(LinkTickType::QueueType qt, uint64 now);

		bool is_server();
		void set_server(bool isServer);

		virtual bool is_connected();

	protected:
		virtual bool shutdown_by_mode(LinkShutDownMode::__ mode);
		void get_last_error();

		virtual int32 socket_recv() = 0;
		virtual int32 socket_send(const void * data, int32 size) = 0;

		virtual LinkBase * new_link() = 0;

	protected:
		TalkLoop * m_loop;
		SOCKET m_handle;
		bool m_ipV6;
		bool m_isBlocking;
		LinkError m_lastErr;
		BaseBuffer m_reader;
		RingBuffer m_writer;
		LinkCurrentState m_state;
		bool m_isServer;
		uint16 m_pingTimes;

	LIGHTINK_DISABLE_COPY(LinkBase)
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline bool LinkBase::shutdown() { return shutdown_by_mode(LinkShutDownMode::All); }

	inline void LinkBase::disconnect() { set_state(LCS_Disconnecting); }

	inline SOCKET LinkBase::get_handle() { return m_handle; }

	inline bool LinkBase::is_ip_v6() { return m_ipV6; }

	inline void LinkBase::set_state(LinkCurrentState state) { m_state = state; }
	inline LinkCurrentState LinkBase::get_state() { return m_state; }

	inline IPAddrV4V6 * LinkBase::get_remote_addr() { return NULL; }

	inline bool LinkBase::is_server() { return m_isServer; }
	inline void LinkBase::set_server(bool isServer) { m_isServer = isServer; }

	inline bool LinkBase::is_connected() { return m_state == LCS_Connected; }

}


#endif

