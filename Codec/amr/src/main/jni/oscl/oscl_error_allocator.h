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

//               O S C L _ E R R O R _ A L L O C A T O R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclerror OSCL Error
 *
 * @{
 */


/** \file oscl_error_allocator.h
    \brief Defines a memory allocation class used by the oscl error layer.
*/


#ifndef OSCL_ERROR_ALLOCATOR_H_INCLUDED
#define OSCL_ERROR_ALLOCATOR_H_INCLUDED

// - - Inclusion - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_BASE_MACROS_H_INCLUDED
#include "oscl_base_macros.h"
#endif

#ifndef OSCLCONFIG_ERROR_H_INCLUDED
#include "osclconfig_error.h"
#endif

#ifndef OSCL_ASSERT_H_INCLUDED
#include "oscl_assert.h"
#endif

#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif


//! This class provides static methods to invoke the user defined memory allocation routines
/*!
This class must be instantiated before the static methods are called, else asserts will happen
*/
class OsclErrorAllocator
{
    public:
        //! constructor method
        /*!
        \param allocator - a pointer to the concrete object that provides the allocator/deallocator
        */
        OsclErrorAllocator(Oscl_DefAlloc* allocator)
        {
            iAllocator = allocator;
        }
        //! static method to allocate a block of memory on heap
        /*!
        \param aSize - number of bytes to allocate
        */
        static OsclAny* allocate(uint32 aSize)
        {
            OSCL_ASSERT(iAllocator != NULL);
            return iAllocator->ALLOCATE(aSize);
        }

        //! static method to deallocate a block of memory on heap
        /*!
        \param aPointer - pointer to block of memory to be deallocated
        */
        static OsclAny deallocate(OsclAny* aPointer)
        {
            OSCL_ASSERT(iAllocator != NULL);
            iAllocator->deallocate(aPointer);
        }

        //! placement new operator that allocates memory using the user defined methods
        void* operator new(uint32 size, OsclAny* aPtr)
        {
            OSCL_UNUSED_ARG(size);
            return aPtr;
        }

        //! delete operator that doesn't do anything, user has to deallocate manually
        void operator delete(OsclAny* aPtr, OsclAny* aPtr2)
        {
            OSCL_UNUSED_ARG(aPtr);
            OSCL_UNUSED_ARG(aPtr2);
        }

    private:
        static Oscl_DefAlloc* iAllocator;
};



#endif  //OSCL_ERROR_ALLOCATOR_H_INCLUDED

/*! @} */
