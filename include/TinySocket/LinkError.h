

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

#ifndef LIGHTINK_TINYSOCKET_LINKERROR_H_
#define LIGHTINK_TINYSOCKET_LINKERROR_H_

namespace LightInk
{
	enum LinkError
	{
		LE_Success = 0, //Success
		LE_Error = -1, //Not Get Error Info
		LE_InvalidSocket = -2, //Invalid Socket Handle
		LE_InvalidAddress = -3, //Invalid Address
		LE_InvalidPort = -4, //Invalid Port
		LE_ConnectionRefused = -5, //Connect Refused
		LE_Timedout = -6, //Timed Out
		LE_Wouldblock = -7, //Operation Would Block If Socket Was Blocking
		LE_Notconnected = -8, //The Link Was Disconnected
		LE_Einprogress = -9, //The Link Not Completed Immediately
		LE_Interrupted = -10, //Call Interrupted By Signal
		LE_ConnectionAborted = -11, //The Link Aborted
		LE_ProtocolError = -12, //Invalid Link Protocol
		LE_FirewallError = -13, //Firewall Rules Forbid Link
		LE_InvalidSocketBuffer = -14, //The Buffer Invalid
		LE_ConnectionReset = -15, //Connection Was Closed By The Remote Host
		LE_AddressInUse = -16, //Address Already Use
		LE_InvalidPointer = -17, //Pointer Type Invalid
		LE_BufferLess = -18, //Buffer Less
		LE_Unknown = -19, //Unknown Error
	};

	LIGHTINK_DECL LinkError get_last_error();
	LIGHTINK_DECL const char * get_error_string(LinkError err);
}


#define LINK_ERROR_MAP(XX) \
	XX(LightInk::LE_Success, "Success") \
	XX(LightInk::LE_Error, "Not Get Error Info") \
	XX(LightInk::LE_InvalidSocket, "Invalid Socket Handle") \
	XX(LightInk::LE_InvalidAddress, "Invalid Address") \
	XX(LightInk::LE_InvalidPort, "Invalid Port") \
	XX(LightInk::LE_ConnectionRefused, "Connect Refused") \
	XX(LightInk::LE_Timedout, "Timed Out") \
	XX(LightInk::LE_Wouldblock, "Operation Would Block If Socket Was Blocking") \
	XX(LightInk::LE_Notconnected, "The Link Was Disconnected") \
	XX(LightInk::LE_Einprogress, "The Link Not Completed Immediately") \
	XX(LightInk::LE_Interrupted, "Call Interrupted By Signal") \
	XX(LightInk::LE_ConnectionAborted, "The Link Aborted") \
	XX(LightInk::LE_ProtocolError, "Invalid Link Protocol") \
	XX(LightInk::LE_FirewallError, "Firewall Rules Forbid Link") \
	XX(LightInk::LE_InvalidSocketBuffer, "The Buffer Invalid") \
	XX(LightInk::LE_ConnectionReset, "Connection Was Closed By The Remote Host") \
	XX(LightInk::LE_AddressInUse, "Address Already Use") \
	XX(LightInk::LE_InvalidPointer, "Pointer Type Invalid") \
	XX(LightInk::LE_BufferLess, "Buffer Less") \
	XX(LightInk::LE_Unknown, "Unknown Error")

#endif