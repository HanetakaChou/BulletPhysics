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

#include "btThreads.h"
#include "btQuickprof.h"
#include <algorithm>  // for min and max

static btITaskScheduler* gBtTaskScheduler=0;
static int gThreadsRunningCounter = 0;  // useful for detecting if we are trying to do nested parallel-for calls
static btSpinMutex gThreadsRunningCounterMutex;

//
// BT_DETECT_BAD_THREAD_INDEX tries to detect when there are multiple threads assigned the same thread index.
//
// BT_DETECT_BAD_THREAD_INDEX is a developer option to test if
// certain assumptions about how the task scheduler manages its threads
// holds true.
// The main assumption is:
//   - when the threadpool is resized, the task scheduler either
//      1. destroys all worker threads and creates all new ones in the correct number, OR
//      2. never destroys a worker thread
//
// We make that assumption because we can't easily enumerate the worker threads of a task scheduler
// to assign nice sequential thread-indexes. We also do not get notified if a worker thread is destroyed,
// so we can't tell when a thread-index is no longer being used.
// We allocate thread-indexes as needed with a sequential global thread counter.
//
// Our simple thread-counting scheme falls apart if the task scheduler destroys some threads but
// continues to re-use other threads and the application repeatedly resizes the thread pool of the
// task scheduler.
// In order to prevent the thread-counter from exceeding the global max (BT_MAX_THREAD_COUNT), we
// wrap the thread counter back to 1. This should only happen if the worker threads have all been
// destroyed and re-created.
//
// BT_DETECT_BAD_THREAD_INDEX only works for Win32 right now,
// but could be adapted to work with pthreads
#define BT_DETECT_BAD_THREAD_INDEX 0

#if BT_DETECT_BAD_THREAD_INDEX

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

typedef DWORD ThreadId_t;
const static ThreadId_t kInvalidThreadId = 0;
ThreadId_t gDebugThreadIds[BT_MAX_THREAD_COUNT];

static ThreadId_t getDebugThreadId()
{
	return GetCurrentThreadId();
}

#endif  // #if BT_DETECT_BAD_THREAD_INDEX

// return a unique index per thread, main thread is 0, worker threads are in [1, BT_MAX_THREAD_COUNT)
unsigned int btGetCurrentThreadIndex()
{
#if BT_THREADSAFE
	btAssert(gBtTaskScheduler != NULL);  // call btSetTaskScheduler() with a valid task scheduler first!
	 unsigned int sThreadIndex = gBtTaskScheduler->getCurrentThreadIndex();
#if BT_DETECT_BAD_THREAD_INDEX
	if (gBtTaskScheduler && sThreadIndex > 0)
	{
		if(sThreadIndex >= gBtTaskScheduler->getNumThreads())
		{
			__debugbreak();
		}

		ThreadId_t tid = getDebugThreadId();
		// if not set
		if (gDebugThreadIds[sThreadIndex] == kInvalidThreadId)
		{
			// set it
			gDebugThreadIds[sThreadIndex] = tid;
		}
		else
		{
			if (gDebugThreadIds[sThreadIndex] != tid)
			{
				// this could indicate the task scheduler is breaking our assumptions about
				// how threads are managed when threadpool is resized
				
				// there are 2 or more threads with the same thread-index!
				__debugbreak();
			}
		}
	}
#endif  // #if BT_DETECT_BAD_THREAD_INDEX
	return sThreadIndex;
#else
	return 0;
#endif
}

bool btIsMainThread()
{
	return btGetCurrentThreadIndex() == 0;
}

btITaskScheduler::btITaskScheduler(const char* name)
{
	m_name = name;
}

void btPushThreadsAreRunning()
{
	gThreadsRunningCounterMutex.lock();
	gThreadsRunningCounter++;
	gThreadsRunningCounterMutex.unlock();
}

void btPopThreadsAreRunning()
{
	gThreadsRunningCounterMutex.lock();
	gThreadsRunningCounter--;
	gThreadsRunningCounterMutex.unlock();
}

bool btThreadsAreRunning()
{
	return gThreadsRunningCounter != 0;
}

void btSetTaskScheduler(btITaskScheduler* ts)
{
	btAssert(!gBtTaskScheduler);
	gBtTaskScheduler = ts;
}

btITaskScheduler* btGetTaskScheduler()
{
	return gBtTaskScheduler;
}

void btParallelFor(int iBegin, int iEnd, int grainSize, const btIParallelForBody& body)
{
#if BT_THREADSAFE

#if BT_DETECT_BAD_THREAD_INDEX
	if (!btThreadsAreRunning())
	{
		// clear out thread ids
		for (int i = 0; i < BT_MAX_THREAD_COUNT; ++i)
		{
			gDebugThreadIds[i] = kInvalidThreadId;
		}
	}
#endif  // #if BT_DETECT_BAD_THREAD_INDEX

	btAssert(gBtTaskScheduler != NULL);  // call btSetTaskScheduler() with a valid task scheduler first!
	gBtTaskScheduler->parallelFor(iBegin, iEnd, grainSize, body);

#else  // #if BT_THREADSAFE

	// non-parallel version of btParallelFor
	btAssert(!"called btParallelFor in non-threadsafe build. enable BT_THREADSAFE");
	body.forLoop(iBegin, iEnd);

#endif  // #if BT_THREADSAFE
}

btScalar btParallelSum(int iBegin, int iEnd, int grainSize, const btIParallelSumBody& body)
{
#if BT_THREADSAFE

#if BT_DETECT_BAD_THREAD_INDEX
	if (!btThreadsAreRunning())
	{
		// clear out thread ids
		for (int i = 0; i < BT_MAX_THREAD_COUNT; ++i)
		{
			gDebugThreadIds[i] = kInvalidThreadId;
		}
	}
#endif  // #if BT_DETECT_BAD_THREAD_INDEX

	btAssert(gBtTaskScheduler != NULL);  // call btSetTaskScheduler() with a valid task scheduler first!
	return gBtTaskScheduler->parallelSum(iBegin, iEnd, grainSize, body);

#else  // #if BT_THREADSAFE

	// non-parallel version of btParallelSum
	btAssert(!"called btParallelFor in non-threadsafe build. enable BT_THREADSAFE");
	return body.sumLoop(iBegin, iEnd);

#endif  //#else // #if BT_THREADSAFE
}
