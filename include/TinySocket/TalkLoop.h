

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

#ifndef LIGHTINK_TINYSOCKET_TALKLOOP_H_
#define LIGHTINK_TINYSOCKET_TALKLOOP_H_

#include <list>
#include <map>
#include "Common/SelfDQueue.h"
#include "TinySocket/LinkBase.h"

namespace LightInk
{
	struct LIGHTINK_DECL TalkConfig
	{
		TalkConfig();

		uint32 LINK_READERBUFFER; /* 读缓冲区大小 */ 
		uint32 LINK_WRITERBUFFER; /* 写缓冲区大小 */ 
		uint32 UDP_MTU; /* mtu值 */ 

		uint32 KCP_UPDATETICK; /* kcp 内部的tick间隔 */ 
		uint32 KCP_FASTRESEND; /* kcp ack跨越次数马上重传,0为不重传 */ 
		uint32 KCP_NOCONTROL; /* kcp 是否关闭流控 */ 
		uint32 KCP_SENDWND; /* kcp 发送窗口 */ 
		uint32 KCP_RECVWND; /* kcp接收窗口 */ 
		uint32 KCP_RTO; /* 延时重传 */ 

		uint32 LP_LOOPCOREMAX; /* 单线程初始最大连接数 */ 
		uint32 MSGP_ONEMAXMEMORY; /* 单消息最大长度 */ 
		uint32 LP_PINGINTERVAL; //ping interval 
		uint16 LP_PINGOUT; //ping out times 
	};

	class LinkTickNode : public SelfDQueue<LinkTickNode>::NodeType
	{
	public:
		LinkTickNode() : m_nextTime(0u), m_link(NULL) {  }
		uint64 m_nextTime;
		LinkBase * m_link;
	};
	struct LinkNode
	{
		LinkNode(LinkBase * link) { for (int i = 0; i < LinkTickType::NodeTypeCount; ++i) { m_tickNodes[i].m_link = link; } }
		inline LinkBase * get_link() { return m_tickNodes[0].m_link; }

		LinkTickNode m_tickNodes[LinkTickType::NodeTypeCount];
		uint32 m_activeEvent;
	};

	class LIGHTINK_DECL TalkLoop
	{
	public:
		TalkLoop();
		virtual ~TalkLoop();

		virtual bool init(const TalkConfig & config);
		virtual bool release();

		const TalkConfig & get_config();

		virtual void update(uint32 milliSeconds);

		template <typename LinkType, bool IpV6>
		LinkError add_listen(const char * host, uint16 port);

		template <typename LinkType, bool IpV6>
		LinkError add_connect(const char * host, uint16 port);

	protected:
		virtual void on_event();
		virtual void on_tick();

		void add_link(LinkBase * link);
		bool remove_link(LinkBase * link);

	protected:
		typedef std::map<SOCKET, LinkNode *> LINKS_MAP;
	protected:
		TalkConfig m_config;
		LINKS_MAP m_links;
		std::vector<LinkNode *> m_processLink;
		SelfDQueue<LinkTickNode> m_tickNodeQueue[LinkTickType::QueueTypeCount];
	};
}

#include "TinySocket/TalkLoopInline.h"

#endif