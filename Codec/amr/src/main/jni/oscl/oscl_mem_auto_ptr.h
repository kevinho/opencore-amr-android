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
/* -*- c++ -*- */
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//           O S C L _ M E M _ A U T O _ P T R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclmemory OSCL Memory
 *
 * @{
 */


/**
 *  @file oscl_mem_auto_ptr.h
 *  @brief This file defines the oscl_mem_auto_ptr template class. This class is
 *         used to avoid any potential memory leaks that may arise while returning
 *         from methods in case of error.
 *
 */

#ifndef OSCL_MEM_AUTO_PTR_H
#define OSCL_MEM_AUTO_PTR_H

#ifndef OSCLCONFIG_MEMORY_H_INCLUDED
#include "osclconfig_memory.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#define OSCL_DISABLE_WARNING_TRUNCATE_DEBUG_MESSAGE
#define OSCL_DISABLE_WARNING_RETURN_TYPE_NOT_UDT
#include "osclconfig_compiler_warnings.h"
/**
 * @brief The oscl_auto_ptr class is a template class that defines a pointer
 *   like object intended to be assigned an address obtanined (directly or
 *   or indirectly) by new. When the oscl_auto_ptr expires, its destructor
 *   uses delete to free the memory.
 *
 * The purpose of this class is to provide a way to prevent accidental memory
 * leaks in a class or a method, due to "not remembering to delete" variables
 * allocated on the heap. Thus if you assign an address returned by new to an
 * oscl_auto_ptr object, you don't have to remember to free the memory later,
 * it will be freed automatically when the object goes out of scope.
 * The oscl_auto_ptr is an example of a smart pointer, an object that acts like
 * a pointer, but with additional features. The class is defined so that it acts
 * like a regular pointer in most respects
 *
 */

template < class T, class _Allocator = Oscl_TAlloc<T, OsclMemAllocator> > class OSCLMemAutoPtr
{
    private:
        T* _Ptr;


    public:
        bool _Ownership;


        /**
        * @brief Default constructor
        * Initializes the pointer and takes ownership.
        */
        explicit OSCLMemAutoPtr(T* inPtr = 0) : _Ptr(inPtr), _Ownership(inPtr != 0) {};

        /**
        * @brief Copy constructor
        *
        * Initializes the pointer and takes ownership from another oscl_auto_ptr.
        * Note that the other class does NOT own the pointer any longer, and
        * hence it is NOT its responsibility to free it.
        */
        OSCLMemAutoPtr(const OSCLMemAutoPtr<T>& _Y): _Ownership(_Y._Ownership),
                _Ptr(_Y.release()) {};


        /**
        * @brief Assignment operator from an another oscl_auto_ptr
        *
        * @param _Y The value parameter should be another oscl_auto_ptr
        * @returns Returns a reference to this oscl_auto_ptr instance with
        *   pointer initialized.
        * @pre The input class should be non-null and should point to
        *   a valid pointer.
        *
        * This assignment operator initializes the class to the contents
        * of the oscl_auto_ptr given as the input parameter. The ownership
        * of the pointer is transferred.
        */
        OSCLMemAutoPtr<T, _Allocator>& operator=(const OSCLMemAutoPtr<T, _Allocator>& _Y)
        {
            if (this != &_Y)
            {
                if (_Ptr != _Y.get())
                {
                    if (_Ownership)
                    {
                        _Allocator alloc;
                        alloc.destroy(_Ptr);
                        alloc.deallocate(_Ptr, 1);
                    }
                    _Ownership = _Y._Ownership;
                }
                else if (_Y._Ownership)
                {
                    _Ownership = true;
                }
                _Ptr = _Y.release();
            }
            return (*this);
        }

        /**
        * @brief Destructor
        *
        * The pointer is deleted in case this class still has ownership
        */
        ~OSCLMemAutoPtr()
        {
            if (_Ownership)
            {
                _Allocator alloc;
                alloc.destroy(_Ptr);
                alloc.deallocate(_Ptr, 1);
            }
        }

        /**
        * @brief The indirection operator (*) accesses a value indirectly,
        * through a pointer
        *
        * This operator ensures that the OSCLMemAutoPtr can be used like the
        * regular pointer that it was initialized with.
        */
        T& operator*() const
        {
            return (*get());
        }

        /**
        * @brief The indirection operator (->) accesses a value indirectly,
        * through a pointer
        *
        * This operator ensures that the OSCLMemAutoPtr can be used like the
        * regular pointer that it was initialized with.
        */
        T *operator->() const
        {
            return (get());
        }


        /**
        * @brief The takeOwnership function assigns the value with ownership.
        *
        *
        */
        void takeOwnership(T* ptr)
        {
            if (_Ptr != ptr)
            {
                if (_Ownership)
                {
                    _Allocator alloc;
                    alloc.destroy(_Ptr);
                    alloc.deallocate(_Ptr, 1);
                }
                _Ptr = ptr;
            }

            if (_Ptr)
            {
                _Ownership = true;
            }
        }

        static void deallocate(T* ptr)
        {
            _Allocator alloc;
            alloc.destroy(ptr);
            alloc.deallocate(ptr, 1);
        }

        void allocate(oscl_memsize_t size)
        {
            _Allocator alloc;
            if (_Ptr && _Ownership)
            {
                alloc.destroy(_Ptr);
                alloc.deallocate(_Ptr, 1);
            }
            _Ptr = alloc.ALLOCATE(size);
            _Ownership = true;
        }

        /**
          * @brief The takeOwnership function assigns the value with ownership.
          *
          *
          */
        void setWithoutOwnership(T* ptr)
        {
            if (_Ptr != ptr)
            {
                if (_Ownership)
                {
                    _Allocator alloc;
                    alloc.destroy(_Ptr);
                    alloc.deallocate(_Ptr, 1);
                }
                _Ptr = ptr;
            }

            _Ownership = false;
        }

        /**
        * @brief get() method returns the pointer, currently owned by the class.
        *
        */
        T *get() const
        {
            return (_Ptr);
        }

        /**
        * @brief release() method releases ownership of the pointer, currently owned
        * by the class. It returns the pointer as well.
        *
        */
        T *release() const
        {
            ((OSCLMemAutoPtr<T> *)this)->_Ownership = false;
            return (_Ptr);
        }

};

#endif //OSCL_MEM_AUTO_PTR_H

/*! @} */
