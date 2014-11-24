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

/*! \addtogroup osclproc OSCL Proc
 *
 * @{
 */

/** \file oscl_double_list.h
    \brief Internal use types for scheduler
*/
#ifndef OSCL_DOUBLE_LIST_H_INCLUDED
#define OSCL_DOUBLE_LIST_H_INCLUDED

#ifndef OSCLCONFIG_PROC_H_INCLUDED
#include "osclconfig_proc.h"
#endif


#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_ASSERT_H_INCLUDED
#include "oscl_assert.h"
#endif


//OsclDoubleList and OsclPriorityList are used in the internal scheduler implementation.
//All the IMPORT_C was removed so they're not available as
//public APIs.

template <class T, class S>
inline T* OsclPtrAdd(T* aPtr, S aVal)
{
    return((T*)(((uint8*)aPtr) + aVal));
}

template <class T, class S>
inline T* OsclPtrSub(T* aPtr, S aVal)
{
    return((T*)(((uint8*)aPtr) - aVal));
}

class OsclDoubleLink
{
    public:
        OsclDoubleLink() : iNext(NULL) {}
        void Remove();
        void InsertAfter(OsclDoubleLink* aLink);
        void InsertBefore(OsclDoubleLink* aLink);
    public:
        OsclDoubleLink* iNext;
        OsclDoubleLink* iPrev;
};


class OsclReadyQ;
class OsclPriorityLink : public OsclDoubleLink
{
    public:
        int32 iPriority;
};

class OsclDoubleListBase
{
    public:
        bool IsEmpty() const;
        void SetOffset(int32 anOffset);
        void Reset();
        OsclDoubleLink* getHead()
        {
            return &iHead;
        }
        int32 getOffset()
        {
            return iOffset;
        }
    protected:
        OsclDoubleListBase();
        OsclDoubleListBase(int32 anOffset);
        void InsertHead(OsclAny* aPtr);
        void InsertTail(OsclAny* aPtr);
        void Insert(OsclAny* aPtr);
    protected:
        OsclDoubleLink iHead;
        int32 iOffset;
    private:
        OsclDoubleListBase(const OsclDoubleListBase& aList);
        OsclDoubleListBase& operator=(const OsclDoubleListBase& aList);
};



template <class T>
class OsclDoubleList : public OsclDoubleListBase
{
    public:
        OSCL_INLINE OsclDoubleList();
        OSCL_INLINE OsclDoubleList(int32 anOffset);
        OSCL_INLINE void InsertHead(T& aRef);
        OSCL_INLINE void InsertTail(T& aRef);
        OSCL_INLINE bool IsHead(const T* aPtr) const;
        OSCL_INLINE bool IsTail(const T* aPtr) const;
        OSCL_INLINE T* Head() const;
        OSCL_INLINE T* Tail() const;
};

template <class T>
class OsclPriorityList : public OsclDoubleListBase
{
    public:
        OSCL_INLINE OsclPriorityList();
        OSCL_INLINE OsclPriorityList(int32 anOffset);
        OSCL_INLINE void Insert(T& aRef);
        OSCL_INLINE bool IsHead(const T* aPtr) const;
        OSCL_INLINE bool IsTail(const T* aPtr) const;
        OSCL_INLINE T* Head() const;
        OSCL_INLINE T* Tail() const;
};

//
class OsclDoubleListBase;

template <class T>
class OsclDoubleRunner
{
    public:
        OsclDoubleRunner(OsclDoubleListBase& aQue)
        {
            //save the queue information.
            iOffset = aQue.getOffset();
            iHead = aQue.getHead();
            iNext = NULL;
        }

        void Set(T& aLink)
        {
            iNext = (OsclDoubleLink*)OsclPtrAdd(aLink, iOffset);
        }

        //This was inline but ADS 1.2 compiler gets a link error from it...
        operator T*()
        {
            if (iNext)
                return ((T *)OsclPtrSub(iNext, iOffset));
            return NULL;
        }

        T* operator++(int)
        {
            //get current.
            OsclAny* p = NULL;
            if (iNext)
                p = ((OsclAny *)OsclPtrSub(iNext, iOffset));

            //advance.
            if (iNext)
                iNext = iNext->iNext;
            //return current.
            return ((T *)p);
        }

        T* operator--(int);

    public:
        void SetToHead()
        {
            iNext = iHead->iNext;
        }
        void SetToTail()
        {
            iNext = iHead->iPrev;
        }
    protected:
        int32 iOffset;
        OsclDoubleLink* iHead;
        OsclDoubleLink* iNext;
};




//#if !(OSCL_DISABLE_INLINES)
#include "oscl_double_list.inl"
//#endif



//Some handy macros
#define QUE_ITER_BEGIN(_type,_qname)\
    if (!_qname.IsEmpty())\
    {\
        OsclDoubleRunner <_type> iter(_qname);\
        _type *item;\
        for (iter.SetToHead(); ;iter++)\
        {\
            item=iter;\
 
#define QUE_ITER_END(_qname)\
            if (_qname.IsTail(item))\
                break;\
        }\
    }

#endif

/*! @} */
