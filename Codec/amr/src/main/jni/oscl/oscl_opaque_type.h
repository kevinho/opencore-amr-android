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

//                     O S C L _ O P A Q U E _ T Y P E

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 * Additional osclbase comment
 * @{
 */

/*! \file oscl_opaque_type.h
    \brief The file oscl_opaque_type.h defines pure virtual classes for working
       with opaque types.
*/

#ifndef OSCL_OPAQUE_TYPE_H_INCLUDED
#define OSCL_OPAQUE_TYPE_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif



/**
 * This class combines opaque type operations with memory allocation operations.
 */
class Oscl_Opaque_Type_Alloc
{
    public:
        /**
         * Construct element at p using element at init_val as the initial value.
         * Both pointers must be non-NULL.
         */
        virtual void construct(OsclAny* p, const OsclAny* init_val) = 0;

        /**
         * Destroy element at p.
         */
        virtual void destroy(OsclAny* p) = 0;

        /**
         * Allocate "size" bytes
         */
        virtual OsclAny* allocate(const uint32 size) = 0;

        /**
         * Deallocate memory previously allocated with "allocate"
         */
        virtual void deallocate(OsclAny* p) = 0;
};

/**
 * Opaque type operations with swap & comparisons.
 */
class Oscl_Opaque_Type_Compare
{
    public:
        /**
         * Swap element at "a" with element at "b".
         * Both pointers must be non-NULL.
         */
        virtual void swap(OsclAny* a, const OsclAny* b) = 0;

        /**
         * Return a<b.
         */
        virtual int compare_LT(OsclAny* a, OsclAny* b) const = 0;

        /**
         * Return a==b.
         */
        virtual int compare_EQ(const OsclAny* a, const OsclAny* b) const = 0;

};

/**
 * This class combines opaque type operations with memory allocation operations
 * and linked list support
 */
class Oscl_Opaque_Type_Alloc_LL
{
    public:
        /**
         * Construct element at p using element at init_val as the initial value.
         * Both pointers must be non-NULL.
         */
        virtual void construct(OsclAny* p, const OsclAny* init_val) = 0;

        /**
         * Destroy element at p.
         */
        virtual void destroy(OsclAny* p) = 0;

        /**
         * Allocate "size" bytes
         */
        virtual OsclAny* allocate(const uint32 size) = 0;

        /**
         * Deallocate memory previously allocated with "allocate"
         */
        virtual void deallocate(OsclAny* p) = 0;

        /**
         * Get next element in linked list.
         */
        virtual OsclAny* get_next(const OsclAny* elem)const = 0;

        /**
         * Set next element in linked list.
         */
        virtual void set_next(OsclAny* elem, const OsclAny* nextelem) = 0;

        /**
         * Get data
         */
        virtual void get_data(OsclAny*elem, OsclAny*data_val) = 0;

        /**
         * Compare data.
         */
        virtual bool compare_data(const OsclAny*elem, const OsclAny*data_val)const = 0;
};

/*! @} */


#endif





