


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
#ifndef LIGHTINK_THREAD_GUARD_H_
#define LIGHTINK_THREAD_GUARD_H_

namespace LightInk
{
	template<typename T>
	class Guard
	{
	public:
		typedef T MutexType;

		explicit Guard(MutexType & mt) : m_mutex(mt) { m_mutex.lock(); }

		~Guard() { m_mutex.unlock(); }

	private:
		MutexType & m_mutex;
	};

	template<typename T>
	class ReadGuard
	{
	public:
		typedef T MutexType;

		explicit ReadGuard(MutexType & mt) : m_mutex(mt) { m_mutex.read_lock(); }

		ReadGuard() { m_mutex.read_unlock(); }

	private:
		MutexType & m_mutex;
	};

	template<typename T>
	class WriteGuard
	{
	public:
		typedef T MutexType;

		explicit WriteGuard(MutexType & mt) : m_mutex(mt) { m_mutex.write_lock(); }

		~WriteGuard() { m_mutex.write_unlock(); }

	private:
		MutexType & m_mutex;
	};
}



#endif