/* ------------------------------------------------------------------
 * Copyright (C) 1998-2009 PacketVideo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ M E M _ M E M P O O L

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclmemory OSCL Memory
 *
 * @{
 */


/*! \file oscl_mem_mempool.h
    \brief This file contains the definition of memory pool allocators
*/


#ifndef OSCL_MEM_MEMPOOL_H_INCLUDED
#define OSCL_MEM_MEMPOOL_H_INCLUDED

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif


/** \class allocator
** A memory allocator class which allocates and deallocates from a fixed size memory pool;
** The memory pool is a multiple of fixed chunk size and does not grow. All allocation size must be the same as this chunk size.
**
*/

class OsclMemPoolFixedChunkAllocatorObserver
{
    public:
        virtual void freechunkavailable(OsclAny* aContextData) = 0;
        virtual ~OsclMemPoolFixedChunkAllocatorObserver() {}
};


class OsclMemPoolFixedChunkAllocator : public Oscl_DefAlloc
{
    public:
        /** This API throws an exception when the memory allocation for pool fails
          * If numchunk and chunksize parameters are not set, memory pool of 1 chunk will be created in the first call to allocate.
          * The chunk size will be set to the n passed in for allocate().
          * If numchunk parameter is set to 0, the memory pool will use 1 for numchunk.
          * If chunkalignment is set to 0, memory pool will use default allocator alignment (8-byte)
          * If chunkalignment is > 0, memory pool will align all buffers in the mempool to the specified alignment. Alignment should be a power of 2
          *
          * @return void
          *
          */
        OSCL_IMPORT_REF OsclMemPoolFixedChunkAllocator(const uint32 numchunk = 1, const uint32 chunksize = 0, Oscl_DefAlloc* gen_alloc = NULL, const uint32 chunkalignment = 0);

        /** This API will disable exceptions in case the memory pool runs out of memory
          * Instead of doing "OSCL_LEAVE(OsclErrNoResources)" allocate API will return
          * NULL.
          *
          * @return void
          *
          */
        OSCL_IMPORT_REF virtual void enablenullpointerreturn();

        /** The destructor for the memory pool
          */
        OSCL_IMPORT_REF virtual ~OsclMemPoolFixedChunkAllocator();

        /** This API throws an exception when n is greater than the fixed chunk size or there are no free chunk available in the pool,
          * if "enablenullpointerreturn" has not been called.
          * If the memory pool hasn't been created yet, the pool will be created with chunk size equal to n so n must be greater than 0.
          * Exception will be thrown if memory allocation for the memory pool fails.
          *
          * @return pointer to available chunk from memory pool
          *
          */
        OSCL_IMPORT_REF virtual OsclAny* allocate(const uint32 n);

        /** This API throws an exception when the pointer p passed in is not part of the memory pool.
           * Exception will be thrown if the memory pool is not set up yet.
           *
           * @return void
           *
           */
        OSCL_IMPORT_REF virtual void deallocate(OsclAny* p);

        /** This API will set the flag to send a callback via specified observer object when the
          * next memory chunk is deallocated by deallocate() call..
          *
          * @return void
          *
          */
        OSCL_IMPORT_REF virtual void notifyfreechunkavailable(OsclMemPoolFixedChunkAllocatorObserver& obs, OsclAny* aContextData = NULL);

        /** This API will cancel any past callback requests..
          *
          * @return void
          *
          */
        OSCL_IMPORT_REF virtual void CancelFreeChunkAvailableCallback();

        /** Increments the reference count for this memory pool allocator
          *
          * @return void
          *
          */
        OSCL_IMPORT_REF void addRef();

        /** Decrements the reference count for this memory pool allocator
          * When the reference count goes to 0, this instance of the memory pool object is deleted
          *
          * @return void
          *
          */
        OSCL_IMPORT_REF void removeRef();

    protected:
        OSCL_IMPORT_REF virtual void createmempool();
        OSCL_IMPORT_REF virtual void destroymempool();

        uint32 iNumChunk;
        uint32 iChunkSize;
        uint32 iChunkSizeMemAligned;
        uint32 iChunkAlignment;
        Oscl_DefAlloc* iMemPoolAllocator;
        OsclAny* iMemPool;
        OsclAny* iMemPoolAligned;

        Oscl_Vector<OsclAny*, OsclMemAllocator> iFreeMemChunkList;

        bool iCheckNextAvailableFreeChunk;
        OsclMemPoolFixedChunkAllocatorObserver* iObserver;
        OsclAny* iNextAvailableContextData;

        int32 iRefCount;
        bool iEnableNullPtrReturn;
};


/** \class allocator
** A memory allocator class which allocates and deallocates from a fixed size memory pool;
** The memory pool is one block of memory and allocations are not fixed in size. The memory pool
** also has the capability of growing by allocating more block one at a time. This memory pool
** also provides the capability of returning the tail end of memory previously allocated from
** the memory pool
**
*/

class OsclMemPoolResizableAllocatorObserver
{
    public:
        virtual void freeblockavailable(OsclAny* aContextData) = 0;
        virtual ~OsclMemPoolResizableAllocatorObserver() {}
};

class OsclMemPoolResizableAllocatorMemoryObserver
{
    public:
        virtual void freememoryavailable(OsclAny* aContextData) = 0;
        virtual ~OsclMemPoolResizableAllocatorMemoryObserver() {}
};

class OsclMemPoolResizableAllocator : public Oscl_DefAlloc
{
    public:
        /** Create the memory pool allocator with resizing functionality.
          * The size of the memory pool buffer needs to be passed-in. The maximum number of
          * memory pool buffers, expected number of blocks in a memory pool buffer, and outside allocator are optional.
          * This API throws an exception when the memory allocation for the pool buffer fails.
          * If memory pool buffer number limit parameter is not set, the assumption is that
          * there is no limit and memory pool will grow as needed.
          * If the expected number of blocks is not set or not known, the memory pool will use
          * a default value to 10 to allocate extra memory for the block info header.
          *
          * @return void
          *
          */
        OSCL_IMPORT_REF OsclMemPoolResizableAllocator(uint32 aMemPoolBufferSize, uint32 aMemPoolBufferNumLimit = 0, uint32 aExpectedNumBlocksPerBuffer = 0, Oscl_DefAlloc* gen_alloc = NULL);

        /** This API will disable exceptions in case the memory pool runs out of memory
          * Instead of doing "OSCL_LEAVE(OsclErrNoResources)" allocate API will return
          * NULL.
          *
          * @return void
          *
          */
        OSCL_IMPORT_REF virtual void enablenullpointerreturn();

        /** Allocates a block from the memory pool that is at least in size requested
          * This API throws an exception if there isn't enough memory (if "enablenullpointerreturn" has not been called)
          * for the requested amount in the pool or if the extra pool buffer cannot be allocated.
          *
          * @return Pointer to memory buffer from memory pool
          *
          */
        OSCL_IMPORT_REF virtual OsclAny* allocate(const uint32 aNumBytes);

        /** Deallocates and returns a block back to the memory pool
          * This API throws an exception if the pointer passed in is not part of the memory pool, aligned, or has corrupted block header.
          *
          * @return void
          *
          */
        OSCL_IMPORT_REF virtual void deallocate(OsclAny* aPtr);

        /** Returns a tail segment of a previously allocated memory block back to the memory pool. The passed-in
          * pointer to the memory buffer is still valid after the call completes but the buffer size is smaller by
          * by specified amount that was freed.
          * This function allows the user to allocate a larger size block initially when the amount needed is unknown
          * and then return the unused portion of the block when the amount becomes known.
          * This API throws an exception if the pointer passed in is not part of the memory pool or the
          * size to return is bigger than the size of the passed-in block.
          * Exception will be thrown if the memory pool is not set up yet.
          *
          * @return bool True if trim operation successful. False if the block wasn't trimmed
          *
          */
        OSCL_IMPORT_REF virtual bool trim(OsclAny* aPtr, uint32 aBytesToFree);

        /** Returns the size of the buffer <including the overhead bytes that may be allocated by the allocater>
          */
        OSCL_IMPORT_REF uint32 getBufferSize() const;

        /** Returns the number of bytes allocated from the buffer<including the overhead bytes that may be
          * allocated by the allocater to keep track of the chunks allocated>
          */
        OSCL_IMPORT_REF virtual uint32 getAllocatedSize() const;

        /** Returns the number of bytes available with the buffer
          */
        OSCL_IMPORT_REF virtual uint32 getAvailableSize() const;

        /** Returns the size of the largest available chunk in the memory.
          */
        OSCL_IMPORT_REF virtual uint32 getLargestContiguousFreeBlockSize() const;

        OSCL_IMPORT_REF virtual bool setMaxSzForNewMemPoolBuffer(uint32 aMaxNewMemPoolBufferSz);

        /** This API will set the flag to send a callback via specified observer object when the
          * next memory block is deallocated by deallocate() call. If the optional requested size
          * parameter is set, the callback is sent when a free memory space of requested size becomes available.
          * The optional context data is returned with the callback and can be used by the user to
          * differentiate.between different instances of memory pool objects.
          * This memory pool only allows one notify to be queued. Another call to this function will just overwrite
          * the previous call.
          *
          * @return void
          *
          */
        OSCL_IMPORT_REF virtual void notifyfreeblockavailable(OsclMemPoolResizableAllocatorObserver& aObserver, uint32 aRequestedSize = 0, OsclAny* aContextData = NULL);

        /** This API will cancel any past callback requests..
          *
          * @return void
          *
          */
        OSCL_IMPORT_REF virtual void CancelFreeChunkAvailableCallback();

        OSCL_IMPORT_REF virtual void notifyfreememoryavailable(OsclMemPoolResizableAllocatorMemoryObserver& aObserver, uint32 aRequestedSize = 0, OsclAny* aContextData = NULL);
        OSCL_IMPORT_REF void CancelFreeMemoryAvailableCallback();

        /** Increments the reference count for this memory pool allocator
          *
          * @return void
          *
          */
        OSCL_IMPORT_REF void addRef();

        /** Decrements the reference count for this memory pool allocator
          * When the reference count goes to 0, this instance of the memory pool object is deleted
          *
          * @return void
          *
          */
        OSCL_IMPORT_REF void removeRef();


        struct MemPoolBlockInfo;

        struct MemPoolBufferInfo
        {
            uint32 iBufferPreFence;     // Pre-fence to check for memory corruption
            OsclAny* iStartAddr;        // Starting memory address of the memory pool buffer after the buffer info
            OsclAny* iEndAddr;          // Ending memory address of the memory pool buffer
            uint32 iBufferSize;         // Total size of the memory pool buffer including the buffer info header
            uint32 iNumOutstanding;     // Number of outstanding blocks from this memory pool buffer
            MemPoolBlockInfo* iNextFreeBlock; // Pointer to the next free memory block
            uint32 iAllocatedSz;        //Number of butes allocated from the mempool
            uint32 iBufferPostFence;    // Post-fence to check for memory corruption
        };

        struct MemPoolBlockInfo
        {
            uint32 iBlockPreFence;      // Pre-fence to check for memory corruption
            MemPoolBlockInfo* iNextFreeBlock; // Pointer to the next (right neighbor) free block in the buffer. NULL if none.
            MemPoolBlockInfo* iPrevFreeBlock; // Pointer to the previous (left neighbor) free block in the buffer. NULL if first free block
            uint32 iBlockSize;          // Total size of the block including the block info header
            uint8* iBlockBuffer;        // Pointer to the buffer area of the block
            MemPoolBufferInfo* iParentBuffer; // Pointer to the block's parent memory pool buffer
            uint32 iBlockPostFence;     // Post-fence to check for memory corruption
        };

    protected:

        /** The destructor for the memory pool. Should not be called directly. Use removeRef() instead.
          */
        OSCL_IMPORT_REF virtual ~OsclMemPoolResizableAllocator();

        MemPoolBufferInfo* addnewmempoolbuffer(uint32 aBufferSize);
        void destroyallmempoolbuffers();
        MemPoolBlockInfo* findfreeblock(uint32 aBlockSize);
        OsclAny* allocateblock(MemPoolBlockInfo& aBlockPtr, uint32 aNumBytes);
        void deallocateblock(MemPoolBlockInfo& aBlockPtr);
        bool validateblock(OsclAny* aBlockBufPtr);

        uint32 iMemPoolBufferSize;
        uint32 iMemPoolBufferNumLimit;
        uint32 iExpectedNumBlocksPerBuffer;
        uint32 iMaxNewMemPoolBufferSz;
        Oscl_DefAlloc* iMemPoolBufferAllocator;
        Oscl_Vector<MemPoolBufferInfo*, OsclMemAllocator> iMemPoolBufferList;

        uint32 iBufferInfoAlignedSize;
        uint32 iBlockInfoAlignedSize;

        bool iCheckNextAvailable;
        uint32 iRequestedNextAvailableSize;
        OsclAny* iNextAvailableContextData;
        OsclMemPoolResizableAllocatorObserver* iObserver;

        bool iCheckFreeMemoryAvailable;
        uint32  iRequestedAvailableFreeMemSize;
        OsclAny* iFreeMemContextData;
        OsclMemPoolResizableAllocatorMemoryObserver* iFreeMemPoolObserver;

        int32 iRefCount;
        bool iEnableNullPtrReturn;
        //To compute the size of the buffer, excluding the extra memory in the buffer for its management
        uint32 getMemPoolBufferSize(MemPoolBufferInfo* aBufferInfo) const;
        //To compute the number of bytes allocated from the buffer
        uint32 getMemPoolBufferAllocatedSize(MemPoolBufferInfo* aBufferInfo) const;
        //To compute the addition bytes which were allocated while createing the memory pool for the buffer.
        uint32 memoryPoolBufferMgmtOverhead() const;
};

#endif

/*! @} */
