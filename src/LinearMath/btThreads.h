/*
Copyright (c) 2003-2014 Erwin Coumans  http://bullet.googlecode.com

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BT_THREADS_H
#define BT_THREADS_H

#include "btScalar.h"  // has definitions like SIMD_FORCE_INLINE

#if defined(_MSC_VER) && _MSC_VER >= 1600
// give us a compile error if any signatures of overriden methods is changed
#define BT_OVERRIDE override
#endif

#ifndef BT_OVERRIDE
#define BT_OVERRIDE
#endif

// Don't set this to larger than 64, without modifying btThreadSupportPosix
// and btThreadSupportWin32. They use UINT64 bit-masks.
const unsigned int BT_MAX_THREAD_COUNT = 64;  // only if BT_THREADSAFE is 1

// for internal use only
bool btIsMainThread();
bool btThreadsAreRunning();
unsigned int btGetCurrentThreadIndex();

///
/// btSpinMutex -- lightweight spin-mutex implemented with atomic ops, never puts
///               a thread to sleep because it is designed to be used with a task scheduler
///               which has one thread per core and the threads don't sleep until they
///               run out of tasks. Not good for general purpose use.
///
#if BT_THREADSAFE

#if defined(__GNUC__)
// GCC or CLANG
#define USE_GCC_BUILTIN_ATOMICS_OLD 1
#elif defined(_MSC_VER)
#if defined(__clang__)
// CLANG-CL
#define USE_GCC_BUILTIN_ATOMICS_OLD 1
#else
// MSVC
#define USE_MSVC_INTRINSICS 1
#endif
#else
#error Unknown Compiler
#endif

#if USE_MSVC_INTRINSICS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <intrin.h>

#define btFullMemoryFence() MemoryBarrier()

class btSpinMutex
{
	LONG volatile mLock;

public:
	inline btSpinMutex() : mLock(0)
	{
	}

	inline bool tryLock()
	{
		return (0 == mLock) && (0 == InterlockedCompareExchange(&mLock, 1, 0));
	}

	inline void lock()
	{
		while ((0 != mLock) || (0 != InterlockedCompareExchange(reinterpret_cast<long volatile *>(&mLock), 1, 0)))
		{
#if defined(_M_X64) || defined(_M_IX86)
			_mm_pause();
#elif defined(_M_ARM64) || defined(_M_ARM)
			__yield();
#else
#error Unknown Architecture
#endif
		}
	}

	inline void unlock()
	{
		btFullMemoryFence();
		mLock = 0;
	}
};

#elif USE_GCC_BUILTIN_ATOMICS_OLD

#define btFullMemoryFence() __sync_synchronize()

class btSpinMutex
{
	char volatile mLock;

public:
	inline btSpinMutex() : mLock(0)
	{
	}

	inline bool tryLock()
	{
		return (0 == mLock) && (0 == __sync_val_compare_and_swap(&mLock, static_cast<char>(0), static_cast<char>(1)));
	}

	inline void lock()
	{
		while ((0 != mLock) || (0 != __sync_val_compare_and_swap(&mLock, static_cast<char>(0), static_cast<char>(1))))
		{
#if defined(__x86_64__) || defined(__i386__)
			_mm_pause();
#elif defined(__aarch64__) || defined(__arm__)
			__yield();
#else
#error Unknown Architecture
#endif
		}
	}

	inline void unlock()
	{
		btFullMemoryFence();
		mLock = 0;
	}
};

#else // #elif USE_MSVC_INTRINSICS

#error "no threading primitives defined -- unknown platform"

#endif // #else //#elif USE_MSVC_INTRINSICS

#else // #if BT_THREADSAFE

// These should not be called ever
class btSpinMutex
{
public:
	inline bool tryLock()
	{
		btAssert(!"unimplemented btSpinMutex::tryLock() called");
		return true;
	}

	inline void lock()
	{
		btAssert(!"unimplemented btSpinMutex::lock() called");
	}

	inline void unlock()
	{
		btAssert(!"unimplemented btSpinMutex::unlock() called");
	}
};

#endif // #else //#if BT_THREADSAFE

//
// NOTE: btMutex* is for internal Bullet use only
//
// If BT_THREADSAFE is undefined or 0, should optimize away to nothing.
// This is good because for the single-threaded build of Bullet, any calls
// to these functions will be optimized out.
//
// However, for users of the multi-threaded build of Bullet this is kind
// of bad because if you call any of these functions from external code
// (where BT_THREADSAFE is undefined) you will get unexpected race conditions.
//
SIMD_FORCE_INLINE void btMutexLock(btSpinMutex* mutex)
{
#if BT_THREADSAFE
	mutex->lock();
#else
	(void)mutex;
#endif  // #if BT_THREADSAFE
}

SIMD_FORCE_INLINE void btMutexUnlock(btSpinMutex* mutex)
{
#if BT_THREADSAFE
	mutex->unlock();
#else
	(void)mutex;
#endif  // #if BT_THREADSAFE
}

SIMD_FORCE_INLINE bool btMutexTryLock(btSpinMutex* mutex)
{
#if BT_THREADSAFE
	return mutex->tryLock();
#else
	(void)mutex;
	return true;
#endif  // #if BT_THREADSAFE
}

//
// btIParallelForBody -- subclass this to express work that can be done in parallel
//
class btIParallelForBody
{
public:
	virtual ~btIParallelForBody() {}
	virtual void forLoop(int iBegin, int iEnd) const = 0;
};

//
// btIParallelSumBody -- subclass this to express work that can be done in parallel
//                       and produces a sum over all loop elements
//
class btIParallelSumBody
{
public:
	virtual ~btIParallelSumBody() {}
	virtual btScalar sumLoop(int iBegin, int iEnd) const = 0;
};

//
// btITaskScheduler -- subclass this to implement a task scheduler that can dispatch work to
//                     worker threads
//
class btITaskScheduler
{
public:
	btITaskScheduler(const char* name);
	virtual ~btITaskScheduler() {}
	const char* getName() const { return m_name; }

	virtual int getNumThreads() const = 0;
	virtual int getCurrentThreadIndex() const = 0;
	virtual void parallelFor(int iBegin, int iEnd, int grainSize, const btIParallelForBody& body) = 0;
	virtual btScalar parallelSum(int iBegin, int iEnd, int grainSize, const btIParallelSumBody& body) = 0;
	virtual void sleepWorkerThreadsHint() {}  // hint the task scheduler that we may not be using these threads for a little while

protected:
	const char* m_name;
};

// set the task scheduler to use for all calls to btParallelFor()
// NOTE: you must set this prior to using any of the multi-threaded "Mt" classes
void btSetTaskScheduler(btITaskScheduler* ts);

// get the current task scheduler
btITaskScheduler* btGetTaskScheduler();

// get non-threaded task scheduler (always available)
btITaskScheduler* btGetSequentialTaskScheduler();

// create a default task scheduler (Win32 or pthreads based)
btITaskScheduler* btCreateDefaultTaskScheduler();

// get OpenMP task scheduler (if available, otherwise returns null)
btITaskScheduler* btGetOpenMPTaskScheduler();

// get Intel TBB task scheduler (if available, otherwise returns null)
btITaskScheduler* btGetTBBTaskScheduler();

// get PPL task scheduler (if available, otherwise returns null)
btITaskScheduler* btGetPPLTaskScheduler();

// btParallelFor -- call this to dispatch work like a for-loop
//                 (iterations may be done out of order, so no dependencies are allowed)
void btParallelFor(int iBegin, int iEnd, int grainSize, const btIParallelForBody& body);

// btParallelSum -- call this to dispatch work like a for-loop, returns the sum of all iterations
//                 (iterations may be done out of order, so no dependencies are allowed)
btScalar btParallelSum(int iBegin, int iEnd, int grainSize, const btIParallelSumBody& body);

#endif
