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

//                     O S C L _ V E C T O R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 * Additional osclbase comment
 * @{
 */

/*! \file oscl_vector.h
    \brief The file oscl_vector.h defines the template class Oscl_Vector which has a very similar API as the STL Vector class (it basically provides a subset of the STL functionality).  Memory allocation is abstracted through the use of an allocator template parameter.
*/

#ifndef OSCL_VECTOR_H_INCLUDED
#define OSCL_VECTOR_H_INCLUDED

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

#ifndef OSCL_DEF_ALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif

/**
 * Oscl_Vector_Base is a non-templatized base class for Oscl_Vector.
 * The purpose of this base class is to avoid large inline routines
 * in the Oscl_Vector implementation.
 * This class is not intended for direct instantiation except by
 * Oscl_Vector.
 */
class Oscl_Vector_Base
{
    public:
        /**
         * Returns the size of the vector in units of number of elements.
         */
        uint32 size() const
        {
            return numelems;
        }

        /**
         * Returns the allocated memory of the vector in units of number of elements.
         */
        uint32 capacity() const
        {
            return bufsize;
        }

        /**
         * True if the vector's size is 0.
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

        //for use in default constructor.  vtable isn't available in c'tor so this is a subroutine.
        OSCL_IMPORT_REF void construct(Oscl_Opaque_Type_Alloc* aType);

        //for use in constructor with pre-allocation for "n" elements.
        OSCL_IMPORT_REF void construct(Oscl_Opaque_Type_Alloc* aType, uint32 n) ;

        //for use in copy constructor.
        OSCL_IMPORT_REF void construct(Oscl_Opaque_Type_Alloc* aType, const Oscl_Vector_Base& x);

        /**
         * The destructor.
         */
        virtual ~Oscl_Vector_Base()
        {}

        /**
         * Inserts a new element at the end.
         * Inserting an element invalidates all iterators if memory reallocation occurs
         * as a result of the insertion.
         * @param x pointer to the new element
         */
        OSCL_IMPORT_REF void push_back(const OsclAny* x) ;

        /**
         * Removes the last element.
         */
        OSCL_IMPORT_REF void pop_back() ;

        /**
         * Inserts a new element at the front.
         * Inserting an element invalidates all iterators if memory reallocation occurs
         * as a result of the insertion.
         * @param x pointer to new element
         */
        OSCL_IMPORT_REF void push_front(const OsclAny* x) ;

        /**
         * Inserts a new element at a specific position.
         * @param pos iterator at insert position.
         * @param x pointer to new element
         */
        OSCL_IMPORT_REF OsclAny* insert(OsclAny* pos, const OsclAny* x);

        /**
          * Erases the element pointed to by iterator pos.
          * Erasing an element invalidates all iterators pointing to elements
          * following the deletion point.
          * @param pos iterator at erase position
          */
        OSCL_IMPORT_REF OsclAny* erase(OsclAny* pos) ;

        /**
         * Erases elements in range [first, last).
         * Erasing an element invalidates all iterators pointing to elements
         * following the deletion point.
         * @param first starting position
         * @param last ending position, this position is not erased
         */
        OSCL_IMPORT_REF OsclAny* erase(OsclAny* first, OsclAny* last) ;

        //copy vector x into this vector
        OSCL_IMPORT_REF void assign_vector(const Oscl_Vector_Base& x);

        OSCL_IMPORT_REF void destroy();

        uint32 numelems; //number of T items in buffer.
        uint32 bufsize; //available bufsize as number of T-sized items.
        OsclAny* elems; //buffer
        uint32 sizeof_T;

        friend class OsclPriorityQueueBase;

    private:

        /**
         * return a T* incremented by n.
         */
        OsclAny* increment_T(OsclAny* p_T, int32 n) const
        {
            return (OsclAny*)((int32)p_T + n*sizeof_T);
        }

        /**
         * Returns an iterator pointing to the beginning of the vector
         */
        OsclAny* begin() const
        {
            return elems;
        }

        /**
         * Returns an iterator pointing to the end of the vector.
         */
        OsclAny* end() const
        {
            return increment_T(elems, numelems);
        }

        OsclAny* move(OsclAny* first, OsclAny* last, OsclAny* result) ;

        OsclAny* copy(OsclAny* first, OsclAny* last, OsclAny* result) ;

        OsclAny* uninitialized_copy(OsclAny* first, OsclAny* last, OsclAny* result) ;

        //destroy a range of T*
        void destroy(OsclAny* first, OsclAny* last) ;

        Oscl_Opaque_Type_Alloc* pOpaqueType;
};

/**
 * Oscl_Vector Class.  A subset of STL::Vector methods.
 * Oscl_Vector supports random access to elements, constant time insertion
 * and removal of elements at the end of the vector, and linear time
 * insertion and removal of elements at the beginning or middle of the
 * vector.  The number of elements in a vector can vary dynamically, and
 * memory management is performed automatically.
 */
template<class T, class Alloc>
class Oscl_Vector
        : public Oscl_Vector_Base
        , public Oscl_Opaque_Type_Alloc
{

    public:
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* iterator;
        typedef const T* const_iterator;

        /**
         * Creates an empty vector.
         */
        Oscl_Vector() : Oscl_Vector_Base(), Oscl_Opaque_Type_Alloc()
        {
            sizeof_T = sizeof(T);
            Oscl_Vector_Base::construct(this);
        }

        /**
         * Creates an empty vector with capacity n.
         * @param n creates a vector with n elements.  The main reason for specifying n
         * is efficiency.  If you know the capacity to which your vector must grow, then
         * it is more efficient to allocate the vector all at once rather than rely on
         * the automatic reallocation scheme.  This also helps cotrol the invalidation
         * of iterators.
         */
        Oscl_Vector(uint32 n) : Oscl_Vector_Base(), Oscl_Opaque_Type_Alloc()
        {
            sizeof_T = sizeof(T);
            Oscl_Vector_Base::construct(this, n);
        }

        /**
         * Copy Constructor.
         * @param x vector class to copy.
         */
        Oscl_Vector(const Oscl_Vector<T, Alloc>& x) : Oscl_Vector_Base(), Oscl_Opaque_Type_Alloc()
        {
            sizeof_T = sizeof(T);
            Oscl_Vector_Base::construct(this, x);
        }

        /**
         * The destructor.
         */
        virtual ~Oscl_Vector()
        {
            Oscl_Vector_Base::destroy();
        }

        /**
         * The assignment operator
         */
        Oscl_Vector<T, Alloc>& operator=(const Oscl_Vector<T, Alloc>& x)
        {
            if (&x != this)
                Oscl_Vector_Base::assign_vector(x);
            return *this;
        }

        /**
         * Inserts a new element at the end.
         * Inserting an element invalidates all iterators if memory reallocation occurs
         * as a result of the insertion.
         * @param x new element
         */
        void push_back(const T& x)
        {
            Oscl_Vector_Base::push_back(&x);
        }

        /**
         * Inserts a new element at the front.
         * Inserting an element invalidates all iterators if memory reallocation occurs
         * as a result of the insertion.
         * @param x new element
         */
        void push_front(const T& x)
        {
            Oscl_Vector_Base::push_front(&x);
        }

        /**
         * Inserts a new element before the one at pos.
         * @param pos position at which to insert the new element.
         * @param x new element
         */
        iterator insert(iterator pos, const T& x)
        {
            return (iterator)Oscl_Vector_Base::insert(pos, &x);
        }

        /**
         * Returns the n'th element.
         * @param n element position to return
         */
        T& operator[](uint32 n)
        {
            return (*(begin() + n));
        }

        /**
         * Returns the n'th element.
         * @param n element position to return
         */
        const T& operator[](uint32 n) const
        {
            return (*(begin() + n));
        }

        /**
         * Returns the first element.
         */
        T& front()
        {
            return *begin();
        }

        /**
         * Returns the first element.
         */
        const T& front() const
        {
            return *begin();
        }

        /**
         * Returns the last element.
         */
        T& back()
        {
            return (*(end() - 1));
        }

        /**
         * Returns the last element.
         */
        const T& back() const
        {
            return (*(end() - 1));
        }

        /**
         * Removes the last element.
         */
        void pop_back()
        {
            OSCL_ASSERT(numelems);
            numelems--;
            destroy(end());
        }

        /**
         * Removes all elements.
         */
        void clear()
        {
            Oscl_Vector_Base::erase(begin(), end());
        }

        /**
         * Destroy -- this is like an explicit destructor call.
         */
        void destroy()
        {
            Oscl_Vector_Base::destroy();
        }

        /**
         * Returns an iterator pointing to the beginning of the vector.
         */
        iterator begin() const
        {
            return (pointer)elems;
        }

        /**
         * Returns an iterator pointing to the end of the vector..
         */
        iterator end() const
        {
            return (pointer)elems + numelems;
        }

        /**
         * Erases the element pointed to by iterator pos.
         * Erasing an element invalidates all iterators pointing to elements
         * following the deletion point.
         * @param pos iterator at erase position
         */
        iterator erase(iterator pos)
        {
            return (iterator)Oscl_Vector_Base::erase(pos);
        }

        /**
         * Erases elements in range [first, last).
         * Erasing an element invalidates all iterators pointing to elements
         * following the deletion point.
         * @param first starting position
         * @param last ending position, this position is not erased
         */
        iterator erase(iterator first, iterator last)
        {
            return (iterator)Oscl_Vector_Base::erase(first, last);
        }

    private:

        Alloc defAlloc;

        //from Oscl_Opaque_Type_Alloc
        OsclAny* allocate(const uint32 size)
        {
            //prevent zero-size allocations since some allocators don't handle this
            return (size) ? defAlloc.ALLOCATE(size) : NULL;
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
            //note: must use "pointer" instead of "T*" here to avoid ADS 1.2 compile error.
            ((pointer)first)->~T();
        }

};


/*! @} */


#endif





