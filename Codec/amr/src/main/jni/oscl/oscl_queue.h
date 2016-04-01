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

//                     O S C L _ Q U E U E
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 * Additional osclbase comment
 * @{
 */

/*! \file oscl_queue.h
    \brief The file oscl_queue.h defines the template class Oscl_Queue.  It is
    similar to the STL::queue class, with some differences:
     - less complete
     - based on array rather than a deque
     - some interfaces modeled on oscl_vector, for ease of transition
    Memory allocation is abstracted through the use of an allocator template parameter.
*/

#ifndef OSCL_QUEUE_H_INCLUDED
#define OSCL_QUEUE_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_MEM_BASIC_FUNCTIONS_H_INCLUDED
#include "oscl_mem_basic_functions.h"
#endif

#ifndef OSCL_ASSERT_H_INCLUDED
#include "oscl_assert.h"
#endif

#ifndef OSCL_OPAQUE_TYPE_H_INCLUDED
#include "oscl_opaque_type.h"
#endif

/**
 * Oscl_Queue_Base is a non-templatized base class for Oscl_Queue.
 * The purpose of this base class is to avoid large inline routines
 * in the Oscl_Queue implementation.
 * This class is not intended for direct instantiation except by
 * Oscl_Queue.
 */
class Oscl_Queue_Base
{
    public:

        /**
         * Returns the size of the queue.
         */
        uint32 size() const
        {
            return numelems;
        }

        /**
         * Returns the allocated memory of the queue.
         */
        uint32 capacity() const
        {
            return bufsize;
        }

        /**
         * True if there are no elements in the queue
         */
        bool empty() const
        {
            return numelems == 0;
        }

        /**
         * Reallocates memory if necessary to a capacity of n elements.
         * The main reason for reserve is efficiency.  If you know the capacity to
         * which your vector must grow, then it is more efficient to allocate the
         * vector all at once rather than rely on the automatic reallocation scheme.
         * This also helps cotrol the invalidation of iterators.
         * @param n size of vector
         */
        OSCL_IMPORT_REF void reserve(uint32 n) ;

    protected:

        //for use in default constructor.  vtable is needed so this is a subroutine.
        OSCL_IMPORT_REF void construct(Oscl_Opaque_Type_Alloc* aType);

        //for use in constructor with pre-allocation for "n" elements.
        OSCL_IMPORT_REF void construct(Oscl_Opaque_Type_Alloc* aType, uint32 n) ;

        /**
         * The destructor.
         */
        virtual ~Oscl_Queue_Base()
        {}

        /**
         * Like an explicit destructor call.
         */
        OSCL_IMPORT_REF void destroy();

        /**
         * Inserts a new element at the end.
         *  Queue will be grown if necessary.  If allocation fails, an OSCL_LEAVE will occur
         * @param x new element
         */
        OSCL_IMPORT_REF void push(const OsclAny* x) ;

        /**
         * Removes the first element
         */
        OSCL_IMPORT_REF void pop() ;

        /**
         * Removes all elements.
         */
        OSCL_IMPORT_REF void clear() ;

        uint32 numelems;  // number of valid entries in queue
        uint32 bufsize;   // size of elems
        OsclAny* elems;   // array holding the elements
        uint32 sizeof_T;

        uint32 ifront;    // front of queue: removal point
        uint32 irear;     // just before back of queue: increment=>insertion point

    private:

        /**
         * return a T* incremented by n.
         */
        OsclAny* increment_T(OsclAny* p_T, int32 n) const
        {
            return (OsclAny*)((int32)p_T + n*sizeof_T);
        }

        /**
         * Returns the first element.
         */
        OsclAny* front()
        {
            return increment_T(elems, ifront);
        }

        Oscl_Opaque_Type_Alloc* pOpaqueType;
};

/**
 * Oscl_Queue Class.  A subset of STL::Queue methods.
 * Oscl_Queue supports constant time insertion (at the end) and removal of
 * elements at the front of the queue.  It does not support
 * insertion or removal of elements at the other ends or middle of the
 * queue, nor random access to elements.
 *  * No iteration capability is [currently] supplied.
 *  * No assignment or copy  capability is [currently] supplied.
 *  The number of elements in a queue can vary dynamically, and
 * memory management is performed automatically.
 */

template<class T, class Alloc>
class Oscl_Queue
        : public Oscl_Queue_Base
        , public Oscl_Opaque_Type_Alloc
{

    public:
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef uint32 size_type;

        /**
         * Creates an empty queue.
         */
        Oscl_Queue() : Oscl_Queue_Base(), Oscl_Opaque_Type_Alloc()
        {
            sizeof_T = sizeof(T);
            Oscl_Queue_Base::construct(this);
        }

        /**
         * Creates an empty queue with capacity n.
         * @param n creates a queue with n elements.  The main reason for specifying n
         * is efficiency.  If you know the capacity to which your queue must grow, then
         * it is more efficient to allocate the queue all at once rather than rely on
         * the automatic reallocation scheme.
         */
        Oscl_Queue(uint32 n) : Oscl_Queue_Base(), Oscl_Opaque_Type_Alloc()
        {
            sizeof_T = sizeof(T);
            Oscl_Queue_Base::construct(this, n);
        }

        /**
         * The destructor.
         */
        virtual ~Oscl_Queue()
        {
            Oscl_Queue_Base::destroy();
        }

        /**
         * Inserts a new element at the end.
         *  Queue will be grown if necessary.  If allocation fails, an OSCL_LEAVE will occur
         * @param x new element
         */
        void push(const T& x)
        {
            Oscl_Queue_Base::push(&x);
        }

        /**
         * Returns the first element.
         */
        reference front()
        {
            OSCL_ASSERT(! empty());
            return (reference)((pointer)elems)[ifront];
        }

        /**
         * Returns the first element (const)
         */
        const_reference front() const
        {
            OSCL_ASSERT(! empty());
            return (const_reference)((pointer)elems)[ifront];
        }


        /**
         * Removes the first element
         */
        void pop()
        {
            Oscl_Queue_Base::pop();
        }

        /**
         * Returns the last element:  "back"
         *  (generally not too useful, but some debugging aids might
         *   want to find out what was just added)
         */
        reference back()
        {
            OSCL_ASSERT(! empty());
            return (reference)((pointer)elems)[irear];
        }

        /**
         * Returns the last element (const)
         */
        const_reference back() const
        {
            OSCL_ASSERT(! empty());
            return (const_reference)((pointer)elems)[irear];
        }

        /**
         * Removes all elements.
         */
        void clear()
        {
            Oscl_Queue_Base::clear();
        }

        /**
         * Return various iterators
         *  : not provided for queues.  Paradigm is FIFO
         */

        /**
         * Erases the element pointed to by iterator pos.
         *  : not provided for queues.  Paradigm is QUEUE.
         */

    private:
        Alloc defAlloc;

        //from Oscl_Opaque_Type_Alloc
        OsclAny* allocate(const uint32 size)
        {
            return defAlloc.allocate(size);
        }

        //from Oscl_Opaque_Type_Alloc
        void deallocate(OsclAny* p)
        {
            defAlloc.deallocate(p);
        }

        //from Oscl_Opaque_Type_Alloc
        void construct(OsclAny* p, const OsclAny* x)
        {
            OSCL_ASSERT(x);
            new(p) value_type(*((T*)x));
        }

        //from Oscl_Opaque_Type_Alloc
        void destroy(OsclAny* first)
        {
            OSCL_ASSERT(first);
            OSCL_UNUSED_ARG(first);
            //must use "pointer" instead of "T*" here to avoid ADS 1.2 compile error.
            ((pointer)first)->~T();
        }


        /***************************
         * things we don't believe are needed
         * -- private definitions to block implicit ones --
         */

        /**
         * The assignment operator --
         *  - private, to block use of implicit assignment
         *  - not actually implemented, don't believe it's needed
         */
        Oscl_Queue<T, Alloc>& operator=(const Oscl_Queue<T, Alloc>& x)
        {
            // Do we need to copy a queue?  Why...?
            //  .. unless there's a need, let's not bother with the
            //  complexity here..
            // (need something here, we don't want implicit assignment either
            OSCL_UNUSED_ARG(x);
            OSCL_ASSERT(false);
            return *this;
        }

        /**
         * Copy Constructor.
         * @param x queue class to copy, also for value passage as argument
         * (but: why do we want to copy a queue??)
         */
        Oscl_Queue(const Oscl_Queue<T, Alloc>& x)
                : Oscl_Queue_Base(sizeof(T), this, this)
        {
            OSCL_UNUSED_ARG(x);
            OSCL_ASSERT(false);
        }
};  // end class oscl_queue

/*! @} */


#endif


