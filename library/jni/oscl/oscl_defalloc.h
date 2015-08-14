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

//                     O S C L _ D E F A L L O C

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */

/*! \file oscl_defalloc.h
    \brief The file defines simple default memory allocator classes.  These allocators are used by the Oscl_Vector and Oscl_Map class, etc.
*/


#ifndef OSCL_DEFALLOC_H_INCLUDED
#define OSCL_DEFALLOC_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#define OSCL_DISABLE_WARNING_TRUNCATE_DEBUG_MESSAGE
#include "osclconfig_compiler_warnings.h"

#ifndef OSCL_MEM_INST_H_INCLUDED
#include "oscl_mem_inst.h"
#endif

//A macro for using the Oscl_Alloc or Oscl_DefAlloc call with file name and
//line number inputs to aid memory auditing.
#if(PVMEM_INST_LEVEL>0)
#define ALLOCATE(n) allocate_fl(n,__FILE__,__LINE__)
#else
#define ALLOCATE(n) allocate(n)
#endif

//A macro for using the Oscl_TAlloc call with file name and line number inputs
//to aid memory auditing.
#if(PVMEM_INST_LEVEL>0)
#define ALLOC_AND_CONSTRUCT(n) alloc_and_construct_fl(n,__FILE__,__LINE__)
#else
#define ALLOC_AND_CONSTRUCT(n) alloc_and_construct(n)
#endif

// This macro is defined is osclconfig_compiler_warnings.h
// This GCC #pragma turns off compiler warning for the rest of this header file
// This needs to be done because with the GCC 4.1 toolchain, many compiler warnings
// are generated because Oscl_Alloc and Oscl_Dealloc have virtual functions, but
// no virtual destructor.
// An attempt has been made to add the virtual destructors, however, it resulted
// in run time crashes indicative of double freeing of memory.
// This is a temporary fix, until the crashes are resolved.
//
#ifdef OSCL_DISABLE_GCC_WARNING_SYSTEM_HEADER
#pragma GCC system_header
#endif

class Oscl_Alloc
{
    public:
        virtual OsclAny* allocate(const uint32 size) = 0;

        //Allocator with file name and line number inputs to aid memory auditing.
        //This call should be used in cases where the allocation will invoke
        //the Oscl memory manager.
        //A default implementation is provided for use with allocators that don't
        //invoke Oscl memory manager.
        virtual OsclAny* allocate_fl(const uint32 size, const char * file_name, const int line_num)
        {
            OSCL_UNUSED_ARG(file_name);
            OSCL_UNUSED_ARG(line_num);
            return allocate(size);
        }
};

class Oscl_Dealloc
{
    public:
        virtual void deallocate(OsclAny* p) = 0;
};


class Oscl_DefAlloc : public Oscl_Alloc, public Oscl_Dealloc
{
    public:
        virtual OsclAny* allocate(const uint32 size) = 0;

        //Allocator with file name and line number inputs to aid memory auditing.
        //This call should be used in cases where the allocation will invoke
        //the Oscl memory manager.
        //A default implementation is provided for use with allocators that don't
        //invoke Oscl memory manager.
        virtual OsclAny* allocate_fl(const uint32 size, const char * file_name, const int line_num)
        {
            OSCL_UNUSED_ARG(file_name);
            OSCL_UNUSED_ARG(line_num);
            return allocate(size);
        }
        virtual void deallocate(OsclAny* p) = 0;
};


class OsclDestructDealloc
{
    public:
        virtual void destruct_and_dealloc(OsclAny* ptr) = 0;
};

class OsclAllocDestructDealloc
        : public OsclDestructDealloc, public Oscl_DefAlloc
{

    public:
        virtual ~OsclAllocDestructDealloc() {};
};

template<class T, class Alloc>
class Oscl_TAlloc : public OsclDestructDealloc
{
    public:
        typedef T           value_type;
        typedef T           * pointer;
        typedef const T     * const_pointer;
        typedef uint32      size_type;
        typedef T&          reference;
        typedef const T&    const_reference;

        virtual ~Oscl_TAlloc() {};

        //this is the preferred call-- with file and line number recorded by
        //the caller.  It can be invoked with the ALLOCATE macro.
        pointer allocate_fl(uint32 size , const char * file_name, const int line_num)
        {
            OsclAny* tmp = alloc.allocate_fl(size * sizeof(value_type), file_name, line_num);
            return OSCL_STATIC_CAST(pointer, tmp);
        }

        pointer allocate(uint32 size)
        {
            OsclAny* tmp = alloc.allocate_fl(size * sizeof(value_type), NULL, 0);
            return OSCL_STATIC_CAST(pointer, tmp);
        }

        //this is the preferred call-- with file and line number recorded by
        //the caller.  It can be invoked by the ALLOC_AND_CONSTRUCT macro.
        pointer alloc_and_construct_fl(const_reference val, const char * file_name, const int line_num)
        {
            OsclAny* tmp = alloc.allocate_fl(sizeof(value_type), file_name, line_num);
            construct(OSCL_STATIC_CAST(pointer, tmp), val);
            return OSCL_STATIC_CAST(pointer, tmp);
        }

        pointer alloc_and_construct(const_reference val)
        {
            //note: recording file & line # here is not useful-- the caller
            //should provide them.  Just pass zero to aid debugging.
            OsclAny* tmp = alloc.allocate_fl(sizeof(value_type), NULL, 0);
            construct(OSCL_STATIC_CAST(pointer, tmp), val);
            return OSCL_STATIC_CAST(pointer, tmp);
        }

        void deallocate(OsclAny* p)
        {
            alloc.deallocate(p);
        }

        void deallocate(OsclAny* p, size_type n)
        {
            OSCL_UNUSED_ARG(n);
            alloc.deallocate(p);
        }

        void destruct_and_dealloc(OsclAny* p)
        {
            destroy(OSCL_STATIC_CAST(pointer, p));
            deallocate(p);
        }

        pointer address(reference r)
        {
            return &r;
        }
        const_pointer address(const_reference r) const
        {
            return &r;
        }

        void construct(pointer p, const_reference val)
        {
            new(p) T(val);
        }
        void destroy(pointer p)
        {
            OSCL_UNUSED_ARG(p);
            p->~T();
        }

        template <class U, class V>
        struct rebind
        {
            typedef Oscl_TAlloc<U, V> other;
        };

    private:
        Alloc alloc;
};


/*! @} */

#endif
