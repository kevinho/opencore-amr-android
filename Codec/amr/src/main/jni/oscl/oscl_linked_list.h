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

//                     O S C L _ L I N K E D _ L I S T

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */


/*! \file oscl_linked_list.h
    \brief The file oscl_linked_list.h defines the template class Oscl_Linked_List which has a very similar API as the STL Vector class (it basically provides a subset of the STL functionality).  Memory allocation is abstracted through the use of an allocator template parameter.
*/

#ifndef OSCL_LINKED_LIST_H_INCLUDED
#define OSCL_LINKED_LIST_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif
#ifndef OSCL_OPAQUE_TYPE_H_INCLUDED
#include "oscl_opaque_type.h"
#endif
#ifndef OSCL_ASSERT_H_INCLUDED
#include "oscl_assert.h"
#endif


/**
 * oscl_linked_list supports random access to elements, constant time insertion
 * and removal of elements from the list, and linear time
 * insertion and removal of elements at the beginning or middle of the
 * list.  The number of elements in a list can vary dynamically, and
 * memory management is performed using new and delete operators.
 */

template <class LLClass, class Alloc> class Oscl_Linked_List;
/**
 * Linked List Element Class
 */
template <class LLClass> class LinkedListElement
{

    public:
        LinkedListElement(LLClass in_data)
        {
            data = in_data;
            next = NULL;
        };
        //  ~LinkedListElement() {};

//  friend class Oscl_Linked_List<LLClass>;
        LinkedListElement<LLClass>* next;
        LLClass data;

    private:
};

/**
 * Oscl Linked List Base Class.  A non-templated base class is used to avoid
 * large inline functions in the Oscl_Linked_List implementation.
 */
class Oscl_Linked_List_Base
{
    protected:
        virtual ~Oscl_Linked_List_Base()
        {}

        OSCL_IMPORT_REF void construct(Oscl_Opaque_Type_Alloc_LL* op);

        OSCL_IMPORT_REF void destroy();

        /**
         * Return the first element of list in passed parameter,
         * @param ele return the value of first element of list in this parameter
         * @return 32-bit interger,If first element found, it returns 1
         * otherwise it returns 0
         */
        OSCL_IMPORT_REF int32 get_first(OsclAny* ele);

        /**
         * Return the next element of list in passed parameter,
         * @param ele return the value of next element of list in this parameter
         * @return 32-bit interger ,if next element is found in list,it returns 1
         * otherwise it returns 0
         */
        OSCL_IMPORT_REF int32 get_next(OsclAny* ele);

        /**
         * Debug routine: Checks the list for elements.
         * @return 32-bit integer, if node count is equal to number of node added
         * to the list then returns 1 otherwise returns 0.
         */
        OSCL_IMPORT_REF int32 check_list();

        /**
         * Adds new element to the list.if list is already there then it adds element at end of list otherwise
         * it create the list and add the element as first element of list.
         * @param new_element the element to be add in the list.
         * @return 32-bit integer on the success returns 1.
         */
        OSCL_IMPORT_REF int32 add_element(const OsclAny* new_element);

        /**
         * Adds new element at the start of the list.if list is already
         * exist then it adds element at start of list otherwise
         * it create the list and add the element as first element of list.
         * @param new_element the element to be add in the list.
         * @return 32-bit integer on the success returns 1.
         */
        OSCL_IMPORT_REF int32 add_to_front(const OsclAny* new_element);

        /**
        * Inserts new element in the list.  If the index is past the end of
        * the list
        * it creates the list and add the element as first element of list.
        * @param new_element the element to be add in the list.
        * @return 32-bit integer on the success returns 1.
        */
        OSCL_IMPORT_REF int32 insert_element(const OsclAny* new_element, int index);

        /**
         * Search and returs the element in the list for passed index.
         * @param index, element The index is the count for the node.
         * @return 32-bit integer on success returns 1 otherwise returns 0.
         */
        OSCL_IMPORT_REF int32 get_element(int32 index, OsclAny* element);

        /**
         * Removes the element from the list.
         * @param data_to_remove
         * @return 32-bit integer on if  element fount in the list returns 1 otherwise returns 0.
         */
        OSCL_IMPORT_REF int32 remove_element(const OsclAny* data_to_remove);

        /**
         * Returns the index for requested element.
         * @param data the element for which index to be return.
         * @return 32-bit integer if data is found in the list it returns index
         * otherwise it returns -1.
         */
        OSCL_IMPORT_REF int32 get_index(const OsclAny* data);

        /**
         * Removes the element for requested index.
         * @param index_to_remove
         * @return on success return 1 otherwise return 0.
         */
        OSCL_IMPORT_REF int32 remove_element(const int32 index_to_remove);

        /**
         * Moves the element to end of the list
         * @param data_to_move
         * @return On success returns 1 otherwise returns 0.
         */
        OSCL_IMPORT_REF int32 move_to_end(const OsclAny* data_to_move);

        /**
         * Moves the element to front of the list
         * @param data_to_move
         * @return On success returns 1 otherwise returns 0.
         */
        OSCL_IMPORT_REF int32 move_to_front(const OsclAny* data_to_move);

        OsclAny *head;
        OsclAny *tail;
        OsclAny *iterator;
        int32 num_elements;
        uint32 sizeof_T;

    private:
        Oscl_Opaque_Type_Alloc_LL* pOpaqueType;
};

/**
 * Oscl Linked List Class
 */
template <class LLClass, class Alloc> class Oscl_Linked_List
        : public Oscl_Linked_List_Base
        , public Oscl_Opaque_Type_Alloc_LL
{

    public:
        /**
         * Initialized the protected variables of list.
         */
        Oscl_Linked_List(): Oscl_Linked_List_Base(), Oscl_Opaque_Type_Alloc_LL()
        {
            sizeof_T = sizeof(LinkedListElement<LLClass>);
            Oscl_Linked_List_Base::construct(this);
        }

        /**
         * The destructor.
         */
        ~Oscl_Linked_List()
        {
            Oscl_Linked_List_Base::destroy();
        }

        void clear()
        {
            Oscl_Linked_List_Base::destroy();
        }

        int32 dequeue_element(LLClass & element)
        {
            get_element(0, element);
            return remove_element((int32) 0);
        }
        // get_first() and get_next() together provide iterator function

        /**
         * Return the first element of list in passed parameter,
         * @param ele return the value of first element of list in this parameter
         * @return 32-bit interger,If first element found, it returns 1
         * otherwise it returns 0
         */
        int32 get_first(LLClass & ele)
        {
            return Oscl_Linked_List_Base::get_first(&ele);
        }

        /**
         * Return the next element of list in passed parameter,
         * @param ele return the value of next element of list in this parameter
         * @return 32-bit interger ,if next element is found in list,it returns 1
         * otherwise it returns 0
         */
        int32 get_next(LLClass & ele)
        {
            return Oscl_Linked_List_Base::get_next(&ele);
        }

        /**
         * Debug routine: Checks the list for elements.
         * @return 32-bit integer, if node count is equal to number of node added
         * to the list then returns 1 otherwise returns 0.
         */
        int32 check_list()
        {
            return Oscl_Linked_List_Base::check_list();
        }

        /**
         * Get number of elements in the list.
         * @return 32-bit integer, number of elements in list.
         */
        int32 get_num_elements()
        {
            return num_elements;
        }

        /**
         * Adds new element to the list.if list is already there then it adds element at end of list otherwise
         * it create the list and add the element as first element of list.
         * @param new_element the element to be add in the list.
         * @return 32-bit integer on the success returns 1.
         */
        int32 add_element(LLClass& new_element)
        {
            return Oscl_Linked_List_Base::add_element(&new_element);
        }

        /**
         * Adds new element at the start of the list.if list is already
         * exist then it adds element at start of list otherwise
         * it create the list and add the element as first element of list.
         * @param new_element the element to be add in the list.
         * @return 32-bit integer on the success returns 1.
         */
        int32 add_to_front(const LLClass& new_element)
        {
            return Oscl_Linked_List_Base::add_to_front(&new_element);
        }
        /**
         * Inserts new element in the list.  If the index is past the end of
         * the list
         * it creates the list and add the element as first element of list.
         * @param new_element the element to be add in the list.
         * @return 32-bit integer on the success returns 1.
         */
        int32 insert_element(const LLClass& new_element, int index)
        {
            return Oscl_Linked_List_Base::insert_element(&new_element, index);
        }

        /**
         * Search and returs the element in the list for passed index.
         * @param index, element The index is the count for the node.
         * @return 32-bit integer on success returns 1 otherwise returns 0.
         */
        int32 get_element(int32 index, LLClass& element)
        {
            return Oscl_Linked_List_Base::get_element(index, &element);
        }

        /**
         * Removes the element from the list.
         * @param data_to_remove
         * @return 32-bit integer on if  element fount in the list returns 1 otherwise returns 0.
         */
        int32 remove_element(const LLClass& data_to_remove)
        {
            return Oscl_Linked_List_Base::remove_element(&data_to_remove);
        }

        /**
         * Returns the index for requested element.
         * @param data the element for which index to be return.
         * @return 32-bit integer if data is found in the list it returns index
         * otherwise it returns -1.
         */
        int32 get_index(const LLClass& data)
        {
            return Oscl_Linked_List_Base::get_index(&data);
        }

        /**
         * Removes the element for requested index.
         * @param index_to_remove
         * @return on success return 1 otherwise return 0.
         */
        int32 remove_element(const int32 index_to_remove)
        {
            return Oscl_Linked_List_Base::remove_element(index_to_remove);
        }

        /**
         * Moves the element to end of the list
         * @param data_to_move
         * @return On success returns 1 otherwise returns 0.
         */
        int32 move_to_end(const LLClass& data_to_move)
        {
            return Oscl_Linked_List_Base::move_to_end(&data_to_move);
        }

        /**
         * Moves the element to front of the list
         * @param data_to_move
         * @return On success returns 1 otherwise returns 0.
         */
        int32 move_to_front(const LLClass& data_to_move)
        {
            return Oscl_Linked_List_Base::move_to_front(&data_to_move);
        }


    private:

        //from Oscl_Opaque_Type_Alloc_LL
        void construct(OsclAny* p, const OsclAny* init_val)
        {
            OSCL_ASSERT(init_val);
            OSCL_ASSERT(p);
            new(p) LinkedListElement<LLClass>(*(LLClass*)init_val);
        }

        //this typedef is needed to avoid compile errors ADS 1.2 compiler.
        typedef LinkedListElement<LLClass>* p_elem_type;

        //from Oscl_Opaque_Type_Alloc_LL
        void destroy(OsclAny* p)
        {
            OSCL_ASSERT(p);
            ((p_elem_type)p)->~LinkedListElement<LLClass>();
        }

        //from Oscl_Opaque_Type_Alloc_LL
        OsclAny* allocate(const uint32 size)
        {
            return alloc.ALLOCATE(size);
        }

        //from Oscl_Opaque_Type_Alloc_LL
        void deallocate(OsclAny* p)
        {
            alloc.deallocate(p);
        }

        //from Oscl_Opaque_Type_Alloc_LL
        OsclAny* get_next(const OsclAny* elem)const
        {
            return ((LinkedListElement<LLClass>*)elem)->next;
        }

        //from Oscl_Opaque_Type_Alloc_LL
        void set_next(OsclAny* elem, const OsclAny* nextelem)
        {
            OSCL_ASSERT(elem);
            ((LinkedListElement<LLClass>*)elem)->next = (LinkedListElement<LLClass>*)nextelem;
        }

        //from Oscl_Opaque_Type_Alloc_LL
        void get_data(OsclAny*elem, OsclAny*data_val)
        {
            OSCL_ASSERT(elem);
            OSCL_ASSERT(data_val);
            *((LLClass*)data_val) = ((LinkedListElement<LLClass>*)elem)->data ;
        }

        //from Oscl_Opaque_Type_Alloc_LL
        bool compare_data(const OsclAny*elem, const OsclAny*data_val)const
        {
            OSCL_ASSERT(elem);
            OSCL_ASSERT(data_val);
            return ((LinkedListElement<LLClass>*)elem)->data == *((LLClass*)data_val);
        }

        Alloc alloc;

};

/**
 * Oscl_MTLinked_List is a multi-threaded version of
 * the LinkedList.  It has mutex protection to
 * allow access by different threads.
 */
template <class LLClass, class Alloc, class TheLock> class Oscl_MTLinked_List
{

    public:
        /**
         * Constructor for Oscl_MTLinked_List
         */
        Oscl_MTLinked_List() {};

        /**
         * Destructor for Oscl_MTLinked_List
         */
        ~Oscl_MTLinked_List() {};

        int32 dequeue_element(LLClass & element)
        {
            int32 status;
            TheLock Mylock;
            Mylock.Lock();
            status = the_list.dequeue_element(element);
            Mylock.Unlock();
            return status;
        }

        /**
         * Adds new element to the Multi Threaded Linked list.if list
         * is already there then it adds element at end of list otherwise
         * it create the list and add the element as first element of list.
         * @param new_element the element to be add in the list.
         * @return 32-bit integer on the success returns 1.
         */
        int32 add_element(LLClass& new_element)
        {
            int32 status;
            TheLock Mylock;
            Mylock.Lock();
            status = the_list.add_element(new_element);
            Mylock.Unlock();
            return status;
        }

        /**
         * Adds new element at the start of the Multi Threaded Linked list.
         * if list is already exist then it adds element at start of list otherwise
         * it create the list and add the element as first element of list.
         * @param new_element the element to be add in the list.
         * @return 32-bit integer on the success returns 1.
         */
        int32 add_to_front(LLClass& new_element)
        {
            int32 status;
            TheLock Mylock;
            Mylock.Lock();
            status = the_list.add_to_front(new_element);
            Mylock.Unlock();
            return status;
        }

        /**
         * Search and returs the element in the Multi Treaded Linked List
         * for passed index.
         * @param index, element The index is the count for the node.
         * @return 32-bit integer on success returns 1 otherwise returns 0.
         */
        uint32 get_element(int32 index, LLClass& element)
        {

            int32 status;
            TheLock Mylock;
            Mylock.Lock();
            status = the_list.get_element(index, element);
            Mylock.Unlock();
            return status;
        }

        /**
         * Removes the element from the list.
         * @param data_to_remove
         * @return 32-bit integer on if  element fount in the list returns 1 otherwise returns 0.
         */
        int32 remove_element(const LLClass& data_to_remove)
        {
            int32 status;
            TheLock Mylock;
            Mylock.Lock();
            status = the_list.remove_element(data_to_remove);
            Mylock.Unlock();
            return status;
        }

        /**
         * Returns the index for requested element.
         * @param data the element for which index to be return.
         * @return 32-bit integer if data is found in the list it returns index
         * otherwise it returns -1.
         */
        int32 get_index(const LLClass& data)
        {
            int32 status;
            TheLock Mylock;
            Mylock.Lock();
            status = the_list.get_index(data);
            Mylock.Unlock();
            return status;
        }

        /**
         * Removes the element for requested index.
         * @param index_to_remove
         * @return on success return 1 otherwise return 0.
         */
        int32 remove_element(const int32 index_to_remove)
        {
            int32 status;
            TheLock Mylock;
            Mylock.Lock();
            status = the_list.remove_element(index_to_remove);
            Mylock.Unlock();
            return status;
        }

        /**
         * Moves the element to end of the list
         * @param data_to_move
         * @return On success returns 1 otherwise returns 0.
         */
        int32 move_to_end(const LLClass& data_to_move)
        {
            int32 status;
            TheLock Mylock;
            Mylock.Lock();
            status = the_list.move_to_end(data_to_move);
            Mylock.Unlock();
            return status;
        }

        /**
         * Moves the element to front of the list
         * @param data_to_move
         * @return On success returns 1 otherwise returns 0.
         */
        int32 move_to_front(const LLClass& data_to_move)
        {
            int32 status;
            TheLock Mylock;
            Mylock.Lock();
            status = the_list.move_to_front(data_to_move);
            Mylock.Unlock();
            return status;
        }

    protected:
        Oscl_Linked_List<LLClass, Alloc> the_list;
//    PVMutex mutex;

};


/*! @} */


#endif  // __LINKED_LIST_H

