

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

#ifndef LIGHTINK_TINYSOCKET_LINKTCP_H_
#define LIGHTINK_TINYSOCKET_LINKTCP_H_

#include "TinySocket/LinkBase.h"


namespace LightInk
{
	class LIGHTINK_DECL LinkTcp : public LinkBase
	{
	public:
		LinkTcp(TalkLoop * loop, bool ipv6);
		virtual ~LinkTcp();

		virtual bool init(uint32 readerSize, uint32 writerSize);

		virtual LinkError connect(const char * ip, uint16 port);
		virtual LinkError listen(const char * ip, uint16 port);
		virtual LinkBase * accept();

		virtual void on_connect();
		virtual void on_read(const void * data, uint32 size);
		virtual void on_disconnect();

	protected:
		virtual int32 socket_recv();
		virtual int32 socket_send(const void * data, int32 size);

		virtual LinkBase * new_link();

	};
	///////////////////////////////////////////////////////////////////////
	//inline method
	//////////////////////////////////////////////////////////////////////
	inline int32 LinkTcp::socket_recv() 
	{ 
		int32 len = ::RECV(m_handle, m_reader.get_free(), m_reader.get_free_size(), 0); 
		if (len > 0) { m_reader.add_write_pos(len); }
		return len;
	}
	inline int32 LinkTcp::socket_send(const void * data, int32 size) { return ::SEND(m_handle, data, size, 0); }

	inline LinkBase * LinkTcp::new_link() { return new LinkTcp(m_loop, m_ipV6); }

}

#endif