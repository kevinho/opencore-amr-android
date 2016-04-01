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

//                           O S C L _ B A S E _ A L L O C

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */


/*! \file oscl_base_alloc.h
    \brief A basic allocator that does not rely on other modules
*/


#ifndef OSCL_BASE_ALLOC_H_INCLUDED
#define OSCL_BASE_ALLOC_H_INCLUDED

#include "osclconfig.h"
#include "oscl_defalloc.h"
#include "osclconfig_memory.h"

/** A basic allocator that does not rely on other modules.
 * There is no memory auditing or exception generation.
 *
 * Note: this allocator is for internal use by Oscl code only.
 * Higher level code should use OsclMemAllocator defined in
 * "oscl_mem.h".
 */
class _OsclBasicAllocator: public Oscl_DefAlloc
{
    public:
        OsclAny* allocate(const uint32 size)
        {
            return malloc(size);
        }

        void deallocate(OsclAny *p)
        {
            free(p);
        }

        virtual ~_OsclBasicAllocator() {}
};

/*! @} */

#endif  // OSCL_BASE_H_INCLUDED
