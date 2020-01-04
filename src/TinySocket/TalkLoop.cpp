

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

#include "TinySocket/TalkLoopInline.h"

namespace LightInk
{
	///////////////////////////////////////////////////////////////////////////////
	//TalkConfig
	//////////////////////////////////////////////////////////////////////////////
	TalkConfig::TalkConfig()
	{
		LogTraceStepCall("TalkConfig::TalkConfig()");
		LINK_READERBUFFER = 8 * 1024; //读缓冲区大小
		LINK_WRITERBUFFER = 8 * 1024; //写缓冲区大小
		UDP_MTU = 1400; //mtu值(576-20-8 or 1500-20-8)

		KCP_UPDATETICK = 20; //kcp 内部的tick间隔
		KCP_FASTRESEND = 0; //kcp ack跨越次数马上重传,0为不重传
		KCP_NOCONTROL = 1; //kcp 是否关闭流控
		KCP_SENDWND = 64; //kcp 发送窗口
		KCP_RECVWND = 64; //kcp接收窗口
		KCP_RTO = 200; //延时重传

		LP_LOOPCOREMAX = 3000; //单线程初始最大连接数
		MSGP_ONEMAXMEMORY = (1024*256); //单消息最大长度
		LP_PINGINTERVAL = 10; //ping interval秒
		LP_PINGOUT = 3; //ping out times
		LogTraceStepReturnVoid;
	}

	///////////////////////////////////////////////////////////////////////////////
	//TalkLoop
	//////////////////////////////////////////////////////////////////////////////
	TalkLoop::TalkLoop()
	{
		LogTraceStepCall("TalkLoop::TalkLoop()");
		assert(LinkTickType::NodeTypeCount > 0);
		LogTraceStepReturnVoid;
	}
	TalkLoop::~TalkLoop()
	{
		LogTraceStepCall("TalkLoop::~TalkLoop()");
		release();
		LogTraceStepReturnVoid;
	}

	bool TalkLoop::init(const TalkConfig & config)
	{
		LogTraceStepCall("bool TalkLoop::init(const TalkConfig & config)");
#ifdef WIN32
		WSADATA wsa_data;
		if (WSAStartup(MAKEWORD(2, 2), &wsa_data))
		{
			LogTraceStepReturn(false);
		}
#endif // WIN32
		m_config = config;
		srand(static_cast<unsigned int>(OsHelper::get_up_time() * time(NULL)));
		LogTraceStepReturn(true);
	}
	bool TalkLoop::release()
	{
		LogTraceStepCall("bool TalkLoop::release()");
		for (LINKS_MAP::iterator iter = m_links.begin(); iter != m_links.end(); ++iter)
		{
			remove_link(iter->second->get_link());
		}
		m_links.clear();
		LogTraceStepReturn(true);
	}

	void TalkLoop::update(uint32 milliSeconds)
	{
		LogTraceStepCall("void TalkLoop::update(uint32 milliSeconds)");
		if (m_links.empty())
		{
			LogTraceStepReturnVoid;
		}
		on_tick();
		fd_set fdRecv, fdSend, fdError;
		FD_ZERO(&fdRecv);
		FD_ZERO(&fdSend);
		FD_ZERO(&fdError);
		m_processLink.clear();
		for (LINKS_MAP::iterator iter = m_links.begin(); iter != m_links.end(); ++iter)
		{
			LinkCurrentState state = iter->second->get_link()->get_state();
			switch (state)
			{
			case LightInk::LCS_Listen:
				FD_SET(iter->first, &fdRecv);
				FD_SET(iter->first, &fdError);
				iter->second->m_activeEvent = LAE_Accept | LAE_Error;
				break;
			case LightInk::LCS_Connecting:
				FD_SET(iter->first, &fdSend);
				FD_SET(iter->first,  &fdError);
				iter->second->m_activeEvent = LAE_Connect | LAE_Error;
				break;
			case LightInk::LCS_Disconnecting:
				iter->second->m_activeEvent = LAE_Error;
				m_processLink.push_back(iter->second);
				break;
			case LightInk::LCS_Connected:
				FD_SET(iter->first, &fdRecv);
				FD_SET(iter->first, &fdSend);
				FD_SET(iter->first, &fdError);
				iter->second->m_activeEvent = LAE_Read | LAE_Write | LAE_Error;
				break;
			default:
				iter->second->m_activeEvent = LAE_None;
				break;
			}
		}
		if (!m_processLink.empty())
		{
			on_event();
		}
		if (m_links.empty())
		{
			LogTraceStepReturnVoid;
		}
		struct timeval tv;
		tv.tv_sec = milliSeconds/1000;
		tv.tv_usec = ( milliSeconds%1000 )*1000;
		if (::SELECT(m_links.rbegin()->first + 1, &fdRecv, &fdSend, &fdError, &tv) > 0)
		{
			for (LINKS_MAP::iterator iter = m_links.begin(); iter != m_links.end(); ++iter)
			{
				uint32 activeEvent = 0u;
				if (iter->second->m_activeEvent & (LAE_Accept | LAE_Read))
				{
					if (FD_ISSET(iter->first, &fdRecv))
					{
						activeEvent |= (iter->second->m_activeEvent & (LAE_Accept | LAE_Read));
					}
				}
				else if (iter->second->m_activeEvent & (LAE_Connect | LAE_Write))
				{
					if (FD_ISSET(iter->first, &fdSend))
					{
						activeEvent |= (iter->second->m_activeEvent & (LAE_Connect | LAE_Write));
					}
				}
				else if (iter->second->m_activeEvent & LAE_Error)
				{
					if (FD_ISSET(iter->first, &fdError))
					{
						activeEvent |= LAE_Error;
					}
				}
				if (activeEvent != 0u)
				{
					iter->second->m_activeEvent = activeEvent;
					m_processLink.push_back(iter->second);
				}
			}
			on_event();
		}
		LogTraceStepReturnVoid;
	}

	void TalkLoop::on_event()
	{
		LogTraceStepCall("void TalkLoop::on_event()");
		for (size_t i = 0u; i < m_processLink.size(); ++i)
		{
			uint32 activeEvent = m_processLink[i]->m_activeEvent;
			LinkBase * link = m_processLink[i]->get_link();
			if (activeEvent & LAE_Accept)
			{
				LinkBase * acceptLink = link->accept();
				if (acceptLink)
				{
					add_link(acceptLink);
					acceptLink->set_state(LCS_Connected);
					acceptLink->on_connect();
				}
				else
				{
					LogError("Accept Socket Failed!!! {}", link->get_error_string());
				}
			}
			if (activeEvent & LAE_Connect)
			{
				link->set_state(LCS_Connected);
				link->on_connect();
			}
			if (activeEvent & LAE_Read)
			{
				link->read_data();
			}
			if (activeEvent & LAE_Error)
			{
				remove_link(link);
			}
		}
		m_processLink.clear();
		LogTraceStepReturnVoid;
	}

	void TalkLoop::on_tick()
	{
		LogTraceStepCall("void TalkLoop::on_tick()");
		uint64 now = OsHelper::get_up_time();
		for (int i = 0; i < LinkTickType::QueueTypeCount; ++i)
		{
			SelfDQueue<LinkTickNode> & nodeQueue = m_tickNodeQueue[i];
			for (LinkTickNode * node = nodeQueue.get_head(); node && now >= node->m_nextTime; node = nodeQueue.get_head())
			{
				nodeQueue.pop_front();
				node->m_nextTime = node->m_link->check_tick((LinkTickType::QueueType)i, now);
				if (node->m_nextTime > 0u) { nodeQueue.push_back(*node); }
			}
		}
		LogTraceStepReturnVoid;
	}

	void TalkLoop::add_link(LinkBase * link)
	{
		LogTraceStepCall("void TalkLoop::add_link(LinkBase * link)");
		LinkNode * node = new LinkNode(link);
		m_links[link->get_handle()] = node;
		uint64 now = OsHelper::get_up_time();
		for (int i = 0; i < LinkTickType::QueueTypeCount; ++i)
		{
			LinkTickNode & tickNode = node->m_tickNodes[LinkTickType::Queue2Node[i]];
			tickNode.m_nextTime = node->get_link()->add_tick((LinkTickType::QueueType)i, now);
			if (tickNode.m_nextTime > 0u)
			{
				m_tickNodeQueue[i].push_back(tickNode);
			}
		}
	}
	bool TalkLoop::remove_link(LinkBase * link)
	{
		LogTraceStepCall("bool TalkLoop::remove_link(LinkBase * link)");
		if (!link)
		{
			LogTraceStepReturn(false);
		}
		LINKS_MAP::iterator iter = m_links.find(link->get_handle());
		if (iter == m_links.end())
		{
			LogTraceStepReturn(false);
		}
		link->release();
		link->on_disconnect();
		delete iter->second;
		m_links.erase(iter);
		delete link;
		LogTraceStepReturn(true);
	}
}