/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  https://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "btAlignedAllocator.h"
#include <string.h>

static btAllocFunc *sAllocFunc = NULL;
static btFreeFunc *sFreeFunc = NULL;

void btAlignedAllocSetCustom(btAllocFunc *allocFunc, btFreeFunc *freeFunc)
{
	sAllocFunc = allocFunc;
	sFreeFunc = freeFunc;
}

static btAlignedAllocFunc *sAlignedAllocFunc = NULL;
static btAlignedFreeFunc *sAlignedFreeFunc = NULL;

void btAlignedAllocSetCustomAligned(btAlignedAllocFunc *allocFunc, btAlignedFreeFunc *freeFunc)
{
	sAlignedAllocFunc = allocFunc;
	sAlignedFreeFunc = freeFunc;
}

// detect memory leaks
#ifdef BT_DEBUG_MEMORY_ALLOCATIONS

#include "btThreads.h"

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

static LONG volatile gNumAlignedAllocs = 0;
static LONG volatile gNumAlignedFree = 0;
static LONG volatile gTotalBytesAlignedAllocs = 0;  

static inline LONG btFetchAndAdd(LONG volatile *ptr, LONG value)
{
	return InterlockedAdd(ptr, value) - value;
}

static inline LONG btAddAndFetch(LONG volatile *ptr, LONG value)
{
	return InterlockedAdd(ptr, value);
}

static inline LONG btAcquire(LONG volatile *ptr)
{
	LONG value = (*ptr);
	btFullMemoryFence();
	return value;
}

static LONG volatile gAllocId = 0;

#elif USE_GCC_BUILTIN_ATOMICS_OLD

static int volatile gNumAlignedAllocs = 0;
static int volatile gNumAlignedFree = 0;
static int volatile gTotalBytesAlignedAllocs = 0;  

static inline int btFetchAndAdd(int volatile *ptr, int value)
{
	return __sync_fetch_and_add(ptr, value);
}

static inline int btAddAndFetch(int volatile *ptr, int value)
{
	return __sync_add_and_fetch(ptr, value);
}

static inline int btAcquire(int volatile *ptr)
{
	int value = (*ptr);
	btFullMemoryFence();
	return value;
}

static int volatile gAllocId = 0;

#else

#error "no threading primitives defined -- unknown platform"

#endif

#else

static int gNumAlignedAllocs = 0;
static int gNumAlignedFree = 0;
static int gTotalBytesAlignedAllocs = 0;  

static inline int btFetchAndAdd(int *ptr, int value)
{
	int temp = (*ptr);
	(*ptr) += value;
	return temp;
}

static inline int btAddAndFetch(int *ptr, int value)
{
	(*ptr) += value;
	return (*ptr);
}

static inline int btAcquire(int *ptr)
{
	int value = (*ptr);
	return value;
}

static int gAllocId = 0;

#endif


static int allocations_id[0X100000];
static int allocations_bytes[0X100000];
static int mynumallocs = 0;
static btSpinMutex allocations_mutex;

#include <stdio.h>

int btDumpMemoryLeaks()
{
	int NumAlignedAllocs = btAcquire(&gNumAlignedAllocs);
	int NumAlignedFree = btAcquire(&gNumAlignedFree);
	int TotalBytesAlignedAllocs = btAcquire(&gTotalBytesAlignedAllocs);
	printf("total remain = %d in %d non-freed allocations)\n", TotalBytesAlignedAllocs, (NumAlignedAllocs - NumAlignedFree));

	btMutexLock(&allocations_mutex);
	int totalLeak = 0;
	for (int i = 0; i < mynumallocs; i++)
	{
		printf("Error: leaked memory of allocation #%d (%d bytes)\n", allocations_id[i], allocations_bytes[i]);
		totalLeak += allocations_bytes[i];
	}
	if (totalLeak)
	{
		printf("Error: memory leaks: %d allocations were not freed and leaked together %d bytes\n", mynumallocs, totalLeak);
	}
	btMutexUnlock(&allocations_mutex);

	btAssert(totalLeak == TotalBytesAlignedAllocs);
	return totalLeak;
}

// this generic allocator provides the total allocated number of bytes
struct btDebugPtrMagic
{
	union {
		void **vptrptr;
		void *vptr;
		int *iptr;
		char *cptr;
	};
};

void *btAlignedAllocInternal(size_t size, int alignment, int line, const char *filename)
{
	if (btUnlikely(0U == size))
	{
		return NULL;
	}

	// to find some particular memory leak, you could do something like this:
	//	if (allocId==172)
	//	{
	//		printf("catch me!\n");
	//	}
	//	if (size>1024*1024)
	//	{
	//		printf("big alloc!%d\n", size);
	//	}

	btAddAndFetch(&gTotalBytesAlignedAllocs, static_cast<int>(size));
	btAddAndFetch(&gNumAlignedAllocs, 1);

	int sz4prt = 4 * sizeof(void *);

	char *real = (char *)sAllocFunc(size + sz4prt + (alignment - 1));

	// id zero is invalid
	int AllocId = btFetchAndAdd(&gAllocId, 1);

	void *ret;
	if (real)
	{
		ret = (void *)btAlignPointer(real + sz4prt, alignment);
		btDebugPtrMagic p;
		p.vptr = ret;
		p.cptr -= sizeof(void *);
		*p.vptrptr = (void *)real;
		p.cptr -= sizeof(void *);
		*p.iptr = size;
		p.cptr -= sizeof(void *);
		*p.iptr = AllocId;

		btMutexLock(&allocations_mutex);
		btAssert(mynumallocs < 0X100000);
		allocations_id[mynumallocs] = AllocId;
		allocations_bytes[mynumallocs] = size;
		mynumallocs += 1;
		btMutexUnlock(&allocations_mutex);
	}
	else
	{
		btAssert(false);
		ret = (void *)(real); //??
	}

	int *ptr = (int *)ret;
	*ptr = 12;
	return (ret);
}

void btAlignedFreeInternal(void *ptr, int line, const char *filename)
{
	if (btUnlikely(NULL == ptr))
	{
		return;
	}

	btDebugPtrMagic p;
	p.vptr = ptr;
	p.cptr -= sizeof(void *);
	void *real = *p.vptrptr;
	p.cptr -= sizeof(void *);
	int size = *p.iptr;
	p.cptr -= sizeof(void *);
	int allocId = *p.iptr;

	btMutexLock(&allocations_mutex);
	bool found = false;
	for (int i = 0; i < mynumallocs; ++i)
	{
		if (allocations_id[i] == allocId)
		{
			allocations_id[i] = allocations_id[mynumallocs - 1];
			allocations_bytes[i] = allocations_bytes[mynumallocs - 1];
			mynumallocs += (-1);
			found = true;
			break;
		}
	}
	btAssert(found);
	btMutexUnlock(&allocations_mutex);

	btAddAndFetch(&gTotalBytesAlignedAllocs, -static_cast<int>(size));
	btAddAndFetch(&gNumAlignedFree, 1);

	sFreeFunc(real);
}

#else  //BT_DEBUG_MEMORY_ALLOCATIONS

void *btAlignedAllocInternal(size_t size, int alignment)
{
	if (btUnlikely(0U == size))
	{
		return NULL;
	}

	return sAlignedAllocFunc(size, alignment);
}

void btAlignedFreeInternal(void *ptr)
{
	if (btUnlikely(NULL == ptr))
	{
		return;
	}

	sAlignedFreeFunc(ptr);
}

#endif  //BT_DEBUG_MEMORY_ALLOCATIONS
