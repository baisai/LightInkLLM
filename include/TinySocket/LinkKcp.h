

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
#ifndef LIGHTINK_TINYSOCKET_LINKKCP_H_
#define LIGHTINK_TINYSOCKET_LINKKCP_H_

#include "TinySocket/LinkUdp.h"
#include "kcp/ikcp.h"

namespace LightInk
{
	class LIGHTINK_DECL LinkKcp : public LinkUdp
	{
	public:
		LinkKcp(TalkLoop * loop, bool ipv6);
		virtual ~LinkKcp();

		virtual bool read_data();
		virtual int32 write_data(const void * data, int32 size);
		static int on_kcp_output(const char *buf, int len, struct IKCPCB *kcp, void *user);

		virtual void on_connect();
		virtual void on_read(const void * data, uint32 size);
		virtual void on_disconnect();

		virtual bool is_connected();

		virtual uint64 add_tick(LinkTickType::QueueType qt, uint64 now);
		virtual uint64 check_tick(LinkTickType::QueueType qt, uint64 now);

	protected:
		bool init_kcp();
		void try_recv_kcp();
		static void kcp_log(const char *log, struct IKCPCB *kcp, void *user);

		virtual LinkBase * new_link();

	protected:
		ikcpcb * m_kcp;
		uint64 m_updateTime;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline bool LinkKcp::is_connected() { return m_kcp != NULL && m_key != 0 && m_state == LCS_Connected; }

	inline LinkBase * LinkKcp::new_link() { return new LinkKcp(m_loop, m_ipV6); }

}


#endif
