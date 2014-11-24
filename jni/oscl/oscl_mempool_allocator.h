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

//               O S C L _ M E M P O O L _ A L L O C A T O R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclerror OSCL Error
 *
 * @{
 */


/*! \file oscl_mempool_allocator.h
    \brief This file contains the definition of memory pool allocator for leave/trap
*/

/***************************************************************************************
File Name       : oscl_mempool_allocator.h
Description     : File containing definition of class OsclMemPoolAllocator that provides
                  methods for creating, deleting memory pool.
Coding History  :
                  Achint Kaur           April 11, 2006      Initial Draft
***************************************************************************************/
#ifndef OSCL_MEMPOOL_ALLOCATOR_H_INCLUDED
#define OSCL_MEMPOOL_ALLOCATOR_H_INCLUDED

#ifndef OSCL_DEFALLOC_H_INCLUDED
// For custom allocator Oscl_DefAlloc object
#include "oscl_defalloc.h"
#endif

class OsclMemPoolAllocator
{
    public:
        // Constructor
        OsclMemPoolAllocator(Oscl_DefAlloc* gen_alloc = NULL);

        // Virtual destructor
        virtual ~OsclMemPoolAllocator();

        // Create memory pool
        OsclAny* CreateMemPool(const uint32 aNumChunk = 2, const uint32 aChunkSize = 4);

        // Delete memory pool
        void DestroyMemPool();

        // Memory alignment
        uint oscl_mem_aligned_size(uint size);

    private:
        // Custom allocator for memory
        Oscl_DefAlloc* iCustomAllocator;

        // Base address for memory pool
        OsclAny* iBaseAddress;

};

#endif
