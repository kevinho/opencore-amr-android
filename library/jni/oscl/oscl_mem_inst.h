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

//                     O S C L _ M E M _ I N S T . H

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */

/*! \file oscl_mem_inst.h
    \brief The file defines default memory instrumentation level.
*/


#ifndef OSCL_MEM_INST_H_INCLUDED
#define OSCL_MEM_INST_H_INCLUDED

#ifndef OSCLCONFIG_MEMORY_H_INCLUDED
#include "osclconfig_memory.h"
#endif

/*
** PVMEM_INST_LEVEL controls the instrumentation level for
** memory leak tracking.  Detailed memory leak information (file
** and line # of each allocation) is compiled in or out depending
** on the level.  This information is an aid to debugging memory leaks,
** but compiling it in will increase code size and run-time overhead.
**
** Values are as follows
**   PVMEM_INST_LEVEL 0 -- detailed memory leak reporting information
**      is compiled out.
**   PVMEM_INST_LEVEL 1 -- detailed memory leak reporting information
**      is compiled in.
**
** The value can be defined in the osclconfig_memory.h file for the platform.
** In case PVMEM_INST_LEVEL is not defined for the platform, default
** values are defined here.
*/
#ifndef PVMEM_INST_LEVEL
#if defined(NDEBUG)
/* release mode-- no instrumentation */
#define PVMEM_INST_LEVEL 0
#else
/* debug mode-- full instrumentation */
#define PVMEM_INST_LEVEL 1
#endif
#endif

/*
** When bypassing the Oscl memory manager, instrumentation
** is not needed, so should always be compiled out.
*/
#if(OSCL_BYPASS_MEMMGT)
#undef PVMEM_INST_LEVEL
#define PVMEM_INST_LEVEL 0
#endif


/*! @} */

#endif
