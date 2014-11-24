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

//                   O S C L _ M E M _ A U D I T

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclmemory OSCL Memory
 *
 * @{
 */


/*! \file oscl_mem_audit.h
    \brief This file contains the definition and partial implementation of MM_Audit class
*/

#ifndef OSCL_MEM_AUDIT_H_INCLUDED
#define OSCL_MEM_AUDIT_H_INCLUDED

#ifndef OSCL_LOCK_BASE_H_INCLUDED
#include "oscl_lock_base.h"
#endif

#ifndef OSCL_BASE_ALLOC_H_INCLUDED
#include "oscl_base_alloc.h"
#endif

#ifndef OSCL_TAGTREE_H_INCLUDED
#include "oscl_tagtree.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OSCL_MEM_AUTO_PTR_H_INCLUDED
#include "oscl_mem_auto_ptr.h"
#endif

#define OSCL_DISABLE_WARNING_TRUNCATE_DEBUG_MESSAGE
#include "osclconfig_compiler_warnings.h"


/* DATA TYPES */
struct MM_Stats_t
{
    uint32  numBytes;
    uint32  peakNumBytes;
    uint32  numAllocs;
    uint32  peakNumAllocs;
    uint32  numAllocFails;
    uint32  totalNumAllocs;
    uint32  totalNumBytes;

    MM_Stats_t()
    {
        oscl_memset(this, 0, sizeof(MM_Stats_t));
    };
    MM_Stats_t(uint32 sizeIn)
    {
        numBytes = peakNumBytes = sizeIn;
        peakNumAllocs = numAllocs = 1;
        numAllocFails = 0;
        totalNumAllocs = 1;
        totalNumBytes = sizeIn;
    };

    void reset()
    {
        oscl_memset(this, 0, sizeof(MM_Stats_t));
    };

    void update(const MM_Stats_t& delta, bool add)
    {
        if (add)
        {
            numBytes += delta.numBytes;
            numAllocs += delta.numAllocs;
            numAllocFails += delta.numAllocFails;
            if (numBytes > peakNumBytes)
            {
                peakNumBytes = numBytes;
            }
            if (numAllocs > peakNumAllocs)
            {
                peakNumAllocs = numAllocs;
            }

            totalNumAllocs += delta.numAllocs;
            totalNumBytes += delta.numBytes;

        }
        else
        {
            numBytes -= delta.numBytes;
            numAllocs -= delta.numAllocs;
            numAllocFails -= delta.numAllocFails;
        }
    }

    void *operator new(oscl_memsize_t size)
    {
        OSCL_UNUSED_ARG(size);
        Oscl_TAlloc<MM_Stats_t, OsclMemBasicAllocator> statsAlloc;
        MM_Stats_t *ptr = statsAlloc.allocate(1);
        return ptr;
    };

    void *operator new(oscl_memsize_t size, MM_Stats_t* ptr)
    {
        OSCL_UNUSED_ARG(size);
        return ptr;
    };


    void operator delete(void *ptr) throw()
    {
        Oscl_TAlloc<MM_Stats_t, OsclMemBasicAllocator> statsAlloc;
        statsAlloc.deallocate((MM_Stats_t*)ptr);
    };

    /* won't build on ADS
    void operator delete(void *ptr, MM_Stats_t* mptr) {
         OSCL_UNUSED_ARG(ptr);
         OSCL_UNUSED_ARG(mptr);
     };
         */
};

struct MM_FailInsertParam
{
    uint32 nAllocNum;
    uint16 xsubi[3];
    MM_FailInsertParam(): nAllocNum(0)
    {
        oscl_memset(xsubi, 0, 3*sizeof(xsubi[0]));
    }

    void reset()
    {
        nAllocNum = 0;
        oscl_memset(xsubi, 0, 3*sizeof(xsubi[0]));
    };

    void *operator new(oscl_memsize_t size)
    {
        OSCL_UNUSED_ARG(size);
        Oscl_TAlloc<MM_FailInsertParam, OsclMemBasicAllocator> failAlloc;
        MM_FailInsertParam *ptr = failAlloc.allocate(1);
        return ptr;
    };

    void *operator new(oscl_memsize_t size, MM_FailInsertParam* ptr)
    {
        OSCL_UNUSED_ARG(size);
        return ptr;
    };


    void operator delete(void *ptr) throw()
    {
        Oscl_TAlloc<MM_FailInsertParam, OsclMemBasicAllocator> failAlloc;
        failAlloc.deallocate((MM_FailInsertParam*)ptr);
    };

    /* won't build on ADS
    void operator delete(void *ptr, MM_FailInsertParam* mptr) {
        OSCL_UNUSED_ARG(ptr);
        OSCL_UNUSED_ARG(mptr);
    };*/
};

class OsclMemStatsNode
{
    public:

        MM_Stats_t *pMMStats;
        MM_FailInsertParam *pMMFIParam;

        /* set a link to map, especially for getting the tags of children nodes */
        char *tag;

        OsclMemStatsNode()
        {
            pMMStats = NULL;
            pMMFIParam = NULL;
            tag = NULL;
        }

        void reset()
        {
            if (pMMStats) pMMStats->reset();
            if (pMMFIParam) pMMFIParam->reset();
        };

        ~OsclMemStatsNode()
        {
            OSCL_DELETE(pMMStats);
            OSCL_DELETE(pMMFIParam);
            Oscl_TAlloc<char, OsclMemBasicAllocator> charAlloc;
            charAlloc.deallocate(tag);
        }

        void *operator new(oscl_memsize_t size)
        {
            OSCL_UNUSED_ARG(size);
            Oscl_TAlloc<OsclMemStatsNode, OsclMemBasicAllocator> statsNodeAlloc;
            OsclMemStatsNode *ptr = statsNodeAlloc.allocate(1);
            return ptr;
        };

        void *operator new(oscl_memsize_t size, OsclMemStatsNode* ptr)
        {
            OSCL_UNUSED_ARG(size);
            return ptr;
        };


        void operator delete(void *ptr) throw()
        {
            Oscl_TAlloc<OsclMemStatsNode, OsclMemBasicAllocator> statsNodeAlloc;
            statsNodeAlloc.deallocate((OsclMemStatsNode*)ptr);
        };

        /* won't build on ADS
        void operator delete(void *ptr, OsclMemStatsNode* mptr) {
            OSCL_UNUSED_ARG(ptr);
            OSCL_UNUSED_ARG(mptr);
        };*/
};

struct MM_Stats_CB
{
    const char *tag;
    const MM_Stats_t *pStats;
    uint32 num_child_nodes;

    MM_Stats_CB()
    {
        oscl_memset(this, 0, sizeof(MM_Stats_CB));
    }

    void *operator new(oscl_memsize_t size)
    {
        OSCL_UNUSED_ARG(size);
        Oscl_TAlloc<MM_Stats_CB, OsclMemBasicAllocator> statsCBAlloc;
        MM_Stats_CB *ptr = statsCBAlloc.allocate(1);
        return ptr;
    };

    void *operator new(oscl_memsize_t size, MM_Stats_CB* ptr)
    {
        OSCL_UNUSED_ARG(size);
        return ptr;
    };


    void operator delete(void *ptr) throw()
    {
        Oscl_TAlloc<MM_Stats_CB, OsclMemBasicAllocator> statsCBAlloc;
        statsCBAlloc.deallocate((MM_Stats_CB*)ptr);
    };

    /* won't build on ADS
    void operator delete(void *ptr, MM_Stats_CB* mptr) {
        OSCL_UNUSED_ARG(ptr);
        OSCL_UNUSED_ARG(mptr);
    };*/
};

#define MM_ALLOC_MAX_QUERY_FILENAME_LEN 128
#define MM_ALLOC_MAX_QUERY_TAG_LEN 64

struct MM_AllocQueryInfo
{
    uint32 allocNum;
    char fileName[MM_ALLOC_MAX_QUERY_FILENAME_LEN];
    uint32 lineNo;
    uint32 size;
    const void *pMemBlock;
    char tag[MM_ALLOC_MAX_QUERY_TAG_LEN];
};


struct MM_AllocInfo
{
    uint32 allocNum;
    char *pFileName;
    uint32 lineNo;
    uint32 size;
    void *pMemBlock;
    OsclMemStatsNode *pStatsNode;
    bool bSetFailure;

    MM_AllocInfo()
    {
        oscl_memset(this, 0, sizeof(MM_AllocInfo));
    }

    ~MM_AllocInfo()
    {
        Oscl_TAlloc<char, OsclMemBasicAllocator> charAlloc;
        charAlloc.deallocate(pFileName);
    }


    void *operator new(oscl_memsize_t size)
    {
        OSCL_UNUSED_ARG(size);
        Oscl_TAlloc<MM_AllocInfo, OsclMemBasicAllocator> allocInfoAlloc;
        MM_AllocInfo *ptr = allocInfoAlloc.allocate(1);
        return ptr;
    };

    void *operator new(oscl_memsize_t size, MM_AllocInfo* ptr)
    {
        OSCL_UNUSED_ARG(size);
        return ptr;
    };


    void operator delete(void *ptr) throw()
    {
        Oscl_TAlloc<MM_AllocInfo, OsclMemBasicAllocator> allocInfoAlloc;
        allocInfoAlloc.deallocate((MM_AllocInfo*)ptr);
    };

    /* won't build on ADS
    void operator delete(void *ptr, MM_AllocInfo* mptr) {
        OSCL_UNUSED_ARG(ptr);
        OSCL_UNUSED_ARG(mptr);
    };*/
};


struct MM_AllocNode
{
    MM_AllocInfo *pAllocInfo;

    MM_AllocNode *pPrev;
    MM_AllocNode *pNext;
    MM_AllocNode()
    {
        oscl_memset(this, 0, sizeof(MM_AllocNode));
    }

    ~MM_AllocNode()
    {
        OSCL_DELETE(pAllocInfo);
    }

    void *operator new(oscl_memsize_t size)
    {
        OSCL_UNUSED_ARG(size);
        Oscl_TAlloc<MM_AllocNode, OsclMemBasicAllocator> allocNodeAlloc;
        MM_AllocNode *ptr = allocNodeAlloc.allocate(1);
        return ptr;
    };

    void *operator new(oscl_memsize_t size, MM_AllocNode* ptr)
    {
        OSCL_UNUSED_ARG(size);
        return ptr;
    };


    void operator delete(void *ptr) throw()
    {
        Oscl_TAlloc<MM_AllocNode, OsclMemBasicAllocator> allocNodeAlloc;
        allocNodeAlloc.deallocate((MM_AllocNode*)ptr);
    };

    /* won't build on ADS
    void operator delete(void *ptr, MM_AllocNode* mptr) {
        OSCL_UNUSED_ARG(ptr);
        OSCL_UNUSED_ARG(mptr);
    };*/
};


/* Define internal auto ptr classes that don't go through mem mgmt */
typedef OSCLMemAutoPtr<char, Oscl_TAlloc<char, OsclMemBasicAllocator> > MMAuditCharAutoPtr;
typedef OSCLMemAutoPtr<uint8, Oscl_TAlloc<uint8, _OsclBasicAllocator> > MMAuditUint8AutoPtr;
typedef OSCLMemAutoPtr<MM_AllocNode, Oscl_TAlloc<MM_AllocNode, OsclMemBasicAllocator> > MM_AllocNodeAutoPtr;

typedef OSCLMemAutoPtr<OsclMemStatsNode, Oscl_TAlloc<OsclMemStatsNode, OsclMemBasicAllocator> > MM_StatsNodeTagTreeType;
typedef OSCLMemAutoPtr<OsclMemStatsNode, Oscl_TAlloc<OsclMemStatsNode, OsclMemBasicAllocator> > OsclMemStatsNodeAutoPtr;
typedef Oscl_TAlloc<MM_StatsNodeTagTreeType, OsclMemBasicAllocator> TagTree_Allocator;
typedef Oscl_TagTree<MM_StatsNodeTagTreeType, TagTree_Allocator> OsclTagTreeType;


#define MM_AUDIT_VALIDATE_BLOCK 1
#define MM_AUDIT_PREFILL_FLAG 0x1
#define MM_AUDIT_POSTFILL_FLAG 0x2
#define MM_AUDIT_VALIDATE_ALL_HEAP_FLAG 0x4
#define MM_AUDIT_VALIDATE_ON_FREE_FLAG 0x8
#define MM_AUDIT_ALLOC_NODE_ENABLE_FLAG 0x10
#define MM_AUDIT_SUPPRESS_FILENAME_FLAG 0x20

#if(PVMEM_INST_LEVEL>0)
#define DEFAULT_MM_AUDIT_MODE (MM_AUDIT_PREFILL_FLAG | \
                               MM_AUDIT_POSTFILL_FLAG | \
                               MM_AUDIT_VALIDATE_ON_FREE_FLAG | \
                               MM_AUDIT_ALLOC_NODE_ENABLE_FLAG )



#else
#define DEFAULT_MM_AUDIT_MODE 0
#endif


struct MM_AuditOverheadStats
{
    uint32 per_allocation_overhead;
    uint32 stats_overhead;
};

#if OSCL_BYPASS_MEMMGT
//an empty class for compilation only
class MM_Audit_Imp
{
    public:
};
#else
class MM_Audit_Imp
{
    public:

        /**
        *   Constructor, create the root node in statistics table
        */
        MM_Audit_Imp();

        /**
        * A destructor, remove all the nodes in allocation andstatistics table
        */
        ~MM_Audit_Imp();

        /**
        *  The following are APIs
        t __nothrow_/
        * const __nothrow_
        * @returns the memory pointer if operation succeeds.
        */
        OSCL_IMPORT_REF void *MM_allocate(const OsclMemStatsNode*  statsNode,
                                          uint32 sizeIn,
                                          const char *pFileName,
                                          uint32 lineNumber,
                                          bool allocNodeTracking = false);

        /**
        *   @returns true if operation succeeds;
        */
        OSCL_IMPORT_REF bool MM_deallocate(void *pMemBlockIn);

        /**
        *   API to get memory statistics through context string(tag)
        *   @returns statistics pointer if operation succeeds
        */
        OSCL_IMPORT_REF MM_Stats_t * MM_GetStats(const char * const  tagIn);

        /**
        *   API to get memory statistics in detail through context string(tag) including its subtree
        *   @returns statistics pointer array and actual number of nodes if operation succeeds
        */
        OSCL_IMPORT_REF uint32 MM_GetStatsInDepth(const char *  tagIn, MM_Stats_CB *array_ptr, uint32 max_nodes);

        /**
        *   API to get the number of tree nodes including the tag node and its subtree
        *   @param tagIn input tag
        *   @returns the number of tree nodes ; 0 means no tag node
        */
        OSCL_IMPORT_REF uint32 MM_GetTreeNodes(const char *  tagIn);

        /**
        *   API to add a node and zero out its counters; Note that this tag should be re-used
        *   @param tagIn input tag
        *   @returns true if operation succeeds;
        */
        OSCL_IMPORT_REF bool MM_AddTag(const char * tagIn)
        {
            return (MM_GetTagNode(tagIn) != NULL);
        };

        /**
        *   API to add a node and zero out its counters;
        *    Note that this tag should be re-used
        *   @param tagIn input tag
        *   @returns pointer to OsclMemStatsNode which should be passed to MM_allocate
        */
        OSCL_IMPORT_REF const OsclMemStatsNode* MM_GetTagNode(const char * tagIn);


        /**
        *   API to add a node and zero out its counters;
        *   Note that this tag should be re-used
        *   @param tagIn input tag
        *   @returns true if operation succeeds;
        */
        OSCL_IMPORT_REF const OsclMemStatsNode* MM_GetExistingTag(const char * tagIn);


        OSCL_IMPORT_REF const OsclMemStatsNode* MM_GetRootNode()
        {
            return mpStatsNode;
        };

        /**
        ** These APIs will allocate and release space for alloc node info, to be
        ** used with the MM_GetAllocNodeInfo API.
        */
        OSCL_IMPORT_REF MM_AllocQueryInfo* MM_CreateAllocNodeInfo(uint32 max_array_size);
        OSCL_IMPORT_REF void MM_ReleaseAllocNodeInfo(MM_AllocQueryInfo* info);

        /**
        *   API to query the list of alloc nodes.  It copies the information
        *    into the provided output array.
        *   @param output_array the array where the data will be written
        *    @param max_array_size the max number of output array elements
        *    @param offset the offset into the alloc node list from which the
        *     data should begin.
        *   @returns the number of valid nodes in the output array
        */
        OSCL_IMPORT_REF uint32 MM_GetAllocNodeInfo(MM_AllocQueryInfo* output_array,
                uint32 max_array_size, uint32 offset);

        /**
        *   API to check the input pointer is a valid pointer to a chunk of memory
        *   @param ptrIn input pointer to be validated
        *   @returns true if operation succeeds;
        */
        OSCL_IMPORT_REF bool MM_Validate(const void *ptrIn);

        /**
        *   API to get the current allocation number
        *   @returns the current allocation number
        */
        uint32 MM_GetAllocNo(void)
        {
            return mnAllocNum;
        }

        /**
        *   API to get the overhead statistics for the
        *   memory used by the mm_audit class.
        */
        void MM_GetOverheadStats(MM_AuditOverheadStats& stats)
        {
            stats.per_allocation_overhead = mm_audit_per_block_overhead;
            stats.stats_overhead = mm_audit_stats_overhead;
        }

        /**
        *   API to get the number of allocation nodes (records)
        *   for allocations that are being tracked individually.
        */
        uint32 MM_GetNumAllocNodes()
        {
            return mNumAllocNodes;
        };

        /**
        *   API to get the operating mode of the mm_audit class.
        */
        uint32 MM_GetMode(void)
        {
            return mode;
        }

        /**
        *   API to get the prefill pattern.  The pattern is used
        *    to fill the memory before returning it to the caller.
        */
        uint8 MM_GetPrefillPattern(void)
        {
            return prefill_pattern;
        }

        /**
        *   API to get the postfill pattern.  The pattern is used
        *    to fill the memory before freeing it.
        */
        uint32 MM_GetPostfillPattern(void)
        {
            return postfill_pattern;
        }

        /**
        *   API to set the operating mode of the mm_audit class.
        */
        OSCL_IMPORT_REF void MM_SetMode(uint32 inMode);

        /**
        *   API to set the prefill pattern.
        */
        OSCL_IMPORT_REF void MM_SetPrefillPattern(uint8 pattern);

        /**
        *   API to set the postfill pattern.
        */
        OSCL_IMPORT_REF void MM_SetPostfillPattern(uint8 pattern);

        /**
        *   API to set the maximum tag level,i.e. tag level for a.b.c.d = 4
        *   @param level input tag level to be set
        */
        OSCL_IMPORT_REF void MM_SetTagLevel(uint32 level);

        /**
        *   API to insert allocation failure deterministically according to allocation number associated with tag
        *   @param tagIn input tag
        *   @param alloc_number allocation number associated with tag
        *   @returns true if operation succeeds;
        */
        OSCL_IMPORT_REF bool MM_SetFailurePoint(const char * tagIn, uint32 alloc_number);

        /**
        *   API to cancel the allocation failure point associated with tag
        *   @param tagIn input tag
        */
        OSCL_IMPORT_REF void MM_UnsetFailurePoint(const char * tagIn);


        /*
        * The following are private member functions
        */


        /**
        *    @returns true if operation succeeds;
        */
        MM_AllocNode* addAllocNode(void *pMem, uint32 sizeIn, OsclMemStatsNode* pStatsNode, const char *pFileName, uint32 lineNumber);
        /**
        *    @returns true if operation succeeds;
        */
        OsclMemStatsNode* removeAllocNode(void *pMemBlockIn, uint32& size);
        void removeALLAllocNodes();

        /**
        *    @returns true if operation succeeds;
        */
        OsclMemStatsNode* createStatsNode(const char *  tagIn);
        /**
        *    @returns true if operation succeeds;
        */
        bool updateStatsNode(OsclMemStatsNode *pCurrStatsNode,
                             const MM_Stats_t& pDelta, bool bAdd);
        /**
        *    @returns true if operation succeeds;
        */
        bool updateStatsNodeInFailure(const char * tagIn);
        bool updateStatsNodeInFailure(OsclMemStatsNode * pStatsNode);
        /**
        *    @returns true if operation succeeds;
        */
        bool pruneSubtree(OsclMemStatsNode *pNode);
        bool pruneSubtree(const char * tagIn);

        void retrieveParentTag(char *tag);
        /**
        *    @returns the length of a immediate parent tag for the input tag
        */
        int32 retrieveParentTagLength(const char *tag, int32 bound);
        /**
        *    @returns a valid tag; NULL will be converted into root tag
        */
        void makeValidTag(const char * tagIn, MMAuditCharAutoPtr& autoptr);
        /**
        *    @returns the size of the truncated tag; 0 means NO truncation
        */
        uint32 getTagActualSize(const char * tagIn);
        /**
        *    @returns true if operation succeeds;
        */
        /**
        *    @returns true if operation succeeds;
        */
        bool isSetFailure(const char * tagIn);

        bool isSetFailure(OsclMemStatsNode * statsNode);
        /**
        *    @returns true if operation succeeds;
        */
        static bool validate(void *ptrIn);

        /**
        *    @returns audit root pointer.
        */
        static OsclMemAudit* getAuditRoot(void *ptrIn);

        /**
        *  @returns original block size.  leaves if bad pointer.
        */
        static uint32 getSize(void *ptrIn);

        /**
        *    @returns true if operation succeeds;
        */
        bool validate_all_heap();

    private:

        void populateChildren(const char *tagIn,
                              MM_Stats_CB *array_ptr,
                              uint32 &curr_array_index,
                              uint32 max_nodes);

        /*
        * The following are private data members
        */

        /**
        * mpAllocNode is for the head of the linked list tracking allocated
            * nodes.  mpCurrAllocNode is the tail of the list.
        */
        MM_AllocNode *mpAllocNode, *mpCurrAllocNode;

        /**
         * mNumAllocNodes is for the number of allocated nodes on the
         * linked list.
         */
        uint32 mNumAllocNodes;

        /**
        * mpStatsNode is for storing memory statistics table
        */
        OsclMemStatsNode *mpStatsNode;
        /**
        * a MAP container, mTagTree (key=Tag, value=node addr),
        *   is for fast search in memory statistics table given the input Tag
        */
        OsclTagTreeType          mTagTree;
        /**
        * tag for the root node in memory statistics table
        */
        uint32 mnAllocNum;
        uint32 mnMaxTagLevel;
        uint32 mode;

        uint32 mm_audit_per_block_overhead;
        uint32 mm_audit_stats_overhead;

        uint8 prefill_pattern;
        uint8 postfill_pattern;

};
#endif

#if OSCL_BYPASS_MEMMGT
//an empty class for compilation only.
class OsclMemAudit
{
    public:
};
#else
class OsclMemAudit
{
    public:
        /**
        *   Constructor, create the root node in statistics table
        */
        OsclMemAudit()
        {
            void * p = _oscl_malloc(sizeof(MM_Audit_Imp));
            OsclError::LeaveIfNull(p);

            // this will invoke system placement new operator
            _pMM_Audit_Imp = OSCL_PLACEMENT_NEW(p, MM_Audit_Imp());
            iLock = &iSingletonLock;
            iRefCount = 1;
        };

        /**
        * A destructor, remove all the nodes in allocation andstatistics table
        */
        ~OsclMemAudit()
        {
            _pMM_Audit_Imp->~MM_Audit_Imp();
            _oscl_free((void *) _pMM_Audit_Imp);
        };

        /**
        *  The following are APIs
        t __nothrow_/
        * const __nothrow_
        * @returns the memory pointer if operation succeeds.
        */
        void *MM_allocate(const OsclMemStatsNode*  statsNode,
                          uint32 sizeIn,
                          const char *pFileName,
                          uint32 lineNumber,
                          bool allocNodeTracking = false)
        {
            // make sure lock is acquired and freed up upon return
            iLock->Lock();

            void* result = _pMM_Audit_Imp->MM_allocate(statsNode, sizeIn, pFileName, lineNumber, allocNodeTracking);

            iLock->Unlock();
            return result;
        };

        /**
        *   @returns true if operation succeeds;
        */
        bool MM_deallocate(void *pMemBlockIn)
        {
            // make sure lock is acquired and freed up upon return
            iLock->Lock();

            bool result = _pMM_Audit_Imp->MM_deallocate(pMemBlockIn);

            iLock->Unlock();
            return result;
        };

        /**
        *   API to get memory statistics through context string(tag)
        *   @returns statistics pointer if operation succeeds
        */
        MM_Stats_t * MM_GetStats(const char * const  tagIn)
        {
            // make sure lock is acquired and freed up upon return
            iLock->Lock();

            MM_Stats_t* result = (_pMM_Audit_Imp->MM_GetStats(tagIn));

            iLock->Unlock();
            return result;
        };

        /**
        *   API to get memory statistics in detail through context string(tag) including its subtree
        *   @returns statistics pointer array and actual number of nodes if operation succeeds
        */
        uint32 MM_GetStatsInDepth(const char *  tagIn, MM_Stats_CB *array_ptr, uint32 max_nodes)
        {
            // make sure lock is acquired and freed up upon return
            iLock->Lock();

            uint32 result = (_pMM_Audit_Imp->MM_GetStatsInDepth(tagIn, array_ptr, max_nodes));

            iLock->Unlock();
            return result;
        };

        /**
        *   API to get the number of tree nodes including the tag node and its subtree
        *   @param tagIn input tag
        *   @returns the number of tree nodes ; 0 means no tag node
        */
        uint32 MM_GetTreeNodes(const char *  tagIn)
        {
            // make sure lock is acquired and freed up upon return
            iLock->Lock();

            uint32 result = (_pMM_Audit_Imp->MM_GetTreeNodes(tagIn));

            iLock->Unlock();
            return result;
        };

        /**
        *   API to add a node and zero out its counters; Note that this tag should be re-used
        *   @param tagIn input tag
        *   @returns true if operation succeeds;
        */
        bool MM_AddTag(const char * tagIn)
        {
            return (MM_GetTagNode(tagIn) != NULL);
        };

        /**
        *   API to add a node and zero out its counters;
        *    Note that this tag should be re-used
        *   @param tagIn input tag
        *   @returns pointer to OsclMemStatsNode which should be passed to MM_allocate
        */
        const OsclMemStatsNode* MM_GetTagNode(const char * tagIn)
        {
            // make sure lock is acquired and freed up upon return
            iLock->Lock();

            const OsclMemStatsNode* result = (_pMM_Audit_Imp->MM_GetTagNode(tagIn));

            iLock->Unlock();
            return result;
        };


        /**
        *   API to add a node and zero out its counters;
        *   Note that this tag should be re-used
        *   @param tagIn input tag
        *   @returns true if operation succeeds;
        */
        const OsclMemStatsNode* MM_GetExistingTag(const char * tagIn)
        {
            // make sure lock is acquired and freed up upon return
            iLock->Lock();

            const OsclMemStatsNode* result = (_pMM_Audit_Imp->MM_GetExistingTag(tagIn));

            iLock->Unlock();
            return result;
        };


        const OsclMemStatsNode* MM_GetRootNode()
        {
            return (_pMM_Audit_Imp->MM_GetRootNode());
        };

        /**
        *   API to query the list of alloc nodes.  It copies the information
        *    into the provided output array.
        *   @param output_array the array where the data will be written
        *    @param max_array_size the max number of output array elements
        *    @param offset the offset into the alloc node list from which the
        *     data should begin.
        *   @returns the number of valid nodes in the output array
        */
        uint32 MM_GetAllocNodeInfo(MM_AllocQueryInfo* output_array,
                                   uint32 max_array_size, uint32 offset)
        {
            // make sure lock is acquired and freed up upon return
            iLock->Lock();

            uint32 result = (_pMM_Audit_Imp->MM_GetAllocNodeInfo(output_array, max_array_size, offset));

            iLock->Unlock();
            return result;
        };

        MM_AllocQueryInfo* MM_CreateAllocNodeInfo(uint32 max_array_size)
        {
            return _pMM_Audit_Imp->MM_CreateAllocNodeInfo(max_array_size);
        }
        void MM_ReleaseAllocNodeInfo(MM_AllocQueryInfo* info)
        {
            _pMM_Audit_Imp->MM_ReleaseAllocNodeInfo(info);
        }

        /**
        *   API to check the input pointer is a valid pointer to a chunk of memory
        *   @param ptrIn input pointer to be validated
        *   @returns true if operation succeeds;
        */
        bool MM_Validate(const void *ptrIn)
        {
            // make sure lock is acquired and freed up upon return
            iLock->Lock();

            bool result = (_pMM_Audit_Imp->MM_Validate(ptrIn));

            iLock->Unlock();
            return result;
        };

        /**
        *   API to get the current allocation number
        *   @returns the current allocation number
        */
        uint32 MM_GetAllocNo(void)
        {
            return (_pMM_Audit_Imp->MM_GetAllocNo());
        };

        /**
        *   API to get the overhead statistics for the
        *   memory used by the mm_audit class.
        */
        void MM_GetOverheadStats(MM_AuditOverheadStats& stats)
        {
            _pMM_Audit_Imp->MM_GetOverheadStats(stats);
        };

        /**
        *   API to get the number of allocation nodes (records)
        *   for allocations that are being tracked individually.
        */
        uint32 MM_GetNumAllocNodes()
        {
            return (_pMM_Audit_Imp->MM_GetNumAllocNodes());
        };

        /**
        *   API to get the operating mode of the mm_audit class.
        */
        uint32 MM_GetMode(void)
        {
            return (_pMM_Audit_Imp->MM_GetMode());
        };

        /**
        *   API to get the prefill pattern.  The pattern is used
        *    to fill the memory before returning it to the caller.
        */
        uint8 MM_GetPrefillPattern(void)
        {
            return (_pMM_Audit_Imp->MM_GetPrefillPattern());
        };

        /**
        *   API to get the postfill pattern.  The pattern is used
        *    to fill the memory before freeing it.
        */
        uint32 MM_GetPostfillPattern(void)
        {
            return (_pMM_Audit_Imp->MM_GetPostfillPattern());
        };

        /**
        *   API to set the operating mode of the mm_audit class.
        */
        void MM_SetMode(uint32 inMode)
        {
            // make sure lock is acquired and freed up upon return
            iLock->Lock();

            _pMM_Audit_Imp->MM_SetMode(inMode);

            iLock->Unlock();
        };

        /**
        *   API to set the prefill pattern.
        */
        void MM_SetPrefillPattern(uint8 pattern)
        {
            // make sure lock is acquired and freed up upon return
            iLock->Lock();

            _pMM_Audit_Imp->MM_SetPrefillPattern(pattern);

            iLock->Unlock();
        };

        /**
        *   API to set the postfill pattern.
        */
        void MM_SetPostfillPattern(uint8 pattern)
        {
            // make sure lock is acquired and freed up upon return
            iLock->Lock();

            _pMM_Audit_Imp->MM_SetPostfillPattern(pattern);

            iLock->Unlock();
        };

        /**
        *   API to set the maximum tag level,i.e. tag level for a.b.c.d = 4
        *   @param level input tag level to be set
        */
        void MM_SetTagLevel(uint32 level)
        {
            // make sure lock is acquired and freed up upon return
            iLock->Lock();

            _pMM_Audit_Imp->MM_SetTagLevel(level);

            iLock->Unlock();
        };

        /**
        *   API to insert allocation failure deterministically according to allocation number associated with tag
        *   @param tagIn input tag
        *   @param alloc_number allocation number associated with tag
        *   @returns true if operation succeeds;
        */
        bool MM_SetFailurePoint(const char * tagIn, uint32 alloc_number)
        {
            // make sure lock is acquired and freed up upon return
            iLock->Lock();

            bool result = (_pMM_Audit_Imp->MM_SetFailurePoint(tagIn, alloc_number));

            iLock->Unlock();
            return result;
        };


        /**
        *   API to cancel the allocation failure point associated with tag
        *   @param tagIn input tag
        */
        void MM_UnsetFailurePoint(const char * tagIn)
        {
            // make sure lock is acquired and freed up upon return
            iLock->Lock();

            _pMM_Audit_Imp->MM_UnsetFailurePoint(tagIn);

            iLock->Unlock();
        };

        int32 MM_GetRefCount()
        {
            int32 count;
            iLock->Lock();
            count = iRefCount;
            iLock->Unlock();
            return count;
        }

        /**
        *   API to obtain mem lock ptr
        *
        */

        OsclLockBase* GetLock()
        {
            return iLock;
        };

        /*
        * The following are private member functions
        */

    private:
        /**
        *    @returns true if operation succeeds;
        */
        MM_AllocNode* addAllocNode(void *pMem, uint32 sizeIn, OsclMemStatsNode* pStatsNode,
                                   const char *pFileName, uint32 lineNumber)
        {
            return (_pMM_Audit_Imp->addAllocNode(pMem, sizeIn, pStatsNode, pFileName, lineNumber));
        }

        /**
        *    @returns true if operation succeeds;
        */
        OsclMemStatsNode* removeAllocNode(void *pMemBlockIn, uint32& size)
        {
            return (_pMM_Audit_Imp->removeAllocNode(pMemBlockIn, size));
        }

        void removeALLAllocNodes()
        {
            _pMM_Audit_Imp->removeALLAllocNodes();
        }

        /**
        *    @returns true if operation succeeds;
        */
        OsclMemStatsNode* createStatsNode(const char *  tagIn)
        {
            return (_pMM_Audit_Imp->createStatsNode(tagIn));
        }

        /**
        *    @returns true if operation succeeds;
        */
        bool updateStatsNode(OsclMemStatsNode *pCurrStatsNode,
                             const MM_Stats_t& pDelta, bool bAdd)
        {
            return (_pMM_Audit_Imp->updateStatsNode(pCurrStatsNode, pDelta, bAdd));
        }

        /**
        *    @returns true if operation succeeds;
        */
        bool updateStatsNodeInFailure(const char * tagIn)
        {
            return (_pMM_Audit_Imp->updateStatsNodeInFailure(tagIn));
        }

        bool updateStatsNodeInFailure(OsclMemStatsNode * pStatsNode)
        {
            return (_pMM_Audit_Imp->updateStatsNodeInFailure(pStatsNode));
        }

        /**
        *    @returns true if operation succeeds;
        */
        bool pruneSubtree(OsclMemStatsNode *pNode)
        {
            return (_pMM_Audit_Imp->pruneSubtree(pNode));
        }

        bool pruneSubtree(const char * tagIn)
        {
            return (_pMM_Audit_Imp->pruneSubtree(tagIn));
        }

        void retrieveParentTag(char *tag)
        {
            (_pMM_Audit_Imp->retrieveParentTag(tag));
        }

        /**
        *    @returns the length of a immediate parent tag for the input tag
        */
        int32 retrieveParentTagLength(const char *tag, int32 bound)
        {
            return (_pMM_Audit_Imp->retrieveParentTagLength(tag, bound));
        }

        /**
        *    @returns a valid tag; NULL will be converted into root tag
        */
        void makeValidTag(const char * tagIn, MMAuditCharAutoPtr& autoptr)
        {
            (_pMM_Audit_Imp->makeValidTag(tagIn, autoptr));
        }

        /**
        *    @returns the size of the truncated tag; 0 means NO truncation
        */
        uint32 getTagActualSize(const char * tagIn)
        {
            return (_pMM_Audit_Imp->getTagActualSize(tagIn));
        }

        /**
        *    @returns true if operation succeeds;
        */
        bool isSetFailure(const char * tagIn)
        {
            return (_pMM_Audit_Imp->isSetFailure(tagIn));
        }

        bool isSetFailure(OsclMemStatsNode * statsNode)
        {
            return (_pMM_Audit_Imp->isSetFailure(statsNode));
        }

        /**
        *    @returns true if operation succeeds;
        */
        bool validate(void *ptrIn)
        {
            return (_pMM_Audit_Imp->validate(ptrIn));
        }

        /**
        *    @returns true if operation succeeds;
        */
        bool validate_all_heap()
        {
            return (_pMM_Audit_Imp->validate_all_heap());
        }


    private:

        /*
        * The following are private data members
        */

        MM_Audit_Imp *_pMM_Audit_Imp;
        OsclLockBase *iLock;

        /**
        *  Use OSCL singleton version
        */
        _OsclBasicLock iSingletonLock;
        int32 iRefCount;
        friend class OsclMemGlobalAuditObject;

};
#endif

#endif //OSCL_MEM_AUDIT_H_INCLUDED

/*! @} */
