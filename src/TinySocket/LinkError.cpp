

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
#include "TinySocket/PlatformHost.h"
#include "TinySocket/LinkError.h"

namespace LightInk
{
	LinkError get_last_error()
	{
		LogTraceStepCall("LinkError get_last_error()");
		LinkError err = LE_Unknown;
#if defined(_LINUX) || defined(_DARWIN)
		switch (errno)
		{
		case EXIT_SUCCESS:
			err = LE_Success;
			break;
		case ENOTCONN:
			err = LE_Notconnected;
			break;
		case EAFNOSUPPORT:
			err = LE_ProtocolError;
			break;
		case ENOTSOCK:
		case EBADF:
		case EACCES:
		case EMFILE:
		case ENFILE:
		case ENOBUFS:
		case ENOMEM:
		case EPROTONOSUPPORT:
		case EPIPE:
			err = LE_InvalidSocket;
			break;
		case ECONNREFUSED :
			err = LE_ConnectionRefused;
			break;
		case ETIMEDOUT:
			err = LE_Timedout;
			break;
		case EINPROGRESS:
			err = LE_Einprogress;
			break;
		case EWOULDBLOCK:
			err = LE_Wouldblock;
			break;
		case EINTR:
			err = LE_Interrupted;
			break;
		case ECONNABORTED:
			err = LE_ConnectionAborted;
			break;
		case EINVAL:
		case EPROTO:
			err = LE_ProtocolError;
			break;
		case EPERM:
			err = LE_FirewallError;
			break;
		case EFAULT:
			err = LE_InvalidSocketBuffer;
			break;
		case ECONNRESET:
		case ENOPROTOOPT:
			err = LE_ConnectionReset;
			break;
		case EMSGSIZE:
			err = LE_BufferLess;
			break;
		default:
			err = LE_Unknown;
			break;
		}
#elif defined(WIN32)
		int32 nError = ::WSAGetLastError();
		switch (nError)
		{
		case EXIT_SUCCESS:
			err = LE_Success;
			break;
		case WSAEBADF:
		case WSAENOTCONN:
			err = LE_Notconnected;
			break;
		case WSAEINTR:
			err = LE_Interrupted;
			break;
		case WSAEAFNOSUPPORT:
			err = LE_ProtocolError;
			break;
		case WSAEACCES:
		case WSAEINVAL:
		case WSAEMFILE:
		case WSAENOBUFS:
		case WSAEPROTONOSUPPORT:
		case WSAENOTSOCK:
			err = LE_InvalidSocket;
			break;
		case WSAECONNREFUSED :
			err = LE_ConnectionRefused;
			break;
		case WSAETIMEDOUT:
			err = LE_Timedout;
			break;
		case WSAEINPROGRESS:
			err = LE_Einprogress;
			break;
		case WSAECONNABORTED:
			err = LE_ConnectionAborted;
			break;
		case WSAEWOULDBLOCK:
			err = LE_Wouldblock;
			break;
		case WSAECONNRESET:
			err = LE_ConnectionReset;
			break;
		case WSANO_DATA:
			err = LE_InvalidAddress;
			break;
		case WSAEADDRINUSE:
			err = LE_AddressInUse;
			break;
		case WSAEFAULT:
			err = LE_InvalidPointer;
			break;
		case WSAEMSGSIZE:
			err = LE_BufferLess;
			break;
		default:
			err = LE_Unknown;
			break;
		}
#endif
		LogTraceStepReturn(err);
	}
	const char * get_error_string(LinkError err)
	{
		LogTraceStepCall("const char * get_error_string(LinkError err)");
#define XX(e, s) \
		case e: \
		return s; \
		break;
		switch (err)
		{
			LINK_ERROR_MAP(XX)
		default:
			break;
		}
#undef XX
		return "Unknown Error";
	}
}