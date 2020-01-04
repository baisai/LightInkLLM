

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
#ifndef LIGHTINK_TINYSOCKET_LINKUDP_H_
#define LIGHTINK_TINYSOCKET_LINKUDP_H_

#include "TinySocket/LinkBase.h"

namespace LightInk
{
	class LIGHTINK_DECL LinkUdp : public LinkBase
	{
	public:
		LinkUdp(TalkLoop * loop, bool ipv6);
		virtual ~LinkUdp();

		virtual bool init(uint32 readerSize, uint32 writerSize);

		virtual LinkError connect(const char * ip, uint16 port);
		virtual LinkError listen(const char * ip, uint16 port);
		virtual LinkBase * accept();

		virtual void on_connect();
		virtual void on_read(const void * data, uint32 size);
		virtual void on_disconnect();

		virtual IPAddrV4V6 * get_remote_addr();

		virtual bool get_remote_ip(char * ip, size_t ipLen, uint16 * port);
		virtual bool set_remote_ip(const char * ip, uint16 port);

		virtual bool is_connected();

	protected:
		virtual int32 socket_recv();
		virtual int32 socket_send(const void * data, int32 size);

		virtual LinkBase * new_link();

		void set_key(uint32 key);

	protected:
		IPAddrV4V6 m_remoteAddr;
		uint32 m_key;
	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline IPAddrV4V6 * LinkUdp::get_remote_addr() { return &m_remoteAddr; }

	inline bool LinkUdp::is_connected() { return m_key != 0 && m_state == LCS_Connected; }

	inline LinkBase * LinkUdp::new_link() { return new LinkUdp(m_loop, m_ipV6); }

	inline void LinkUdp::set_key(uint32 key) { m_key = key; }

}


#endif
