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
// Class OsclDoubleList
template <class T>
OSCL_INLINE OsclDoubleList<T>::OsclDoubleList()
{}
template <class T>
OSCL_INLINE OsclDoubleList<T>::OsclDoubleList(int32 anOffset)
        : OsclDoubleListBase(anOffset)
{}
template <class T>
OSCL_INLINE void OsclDoubleList<T>::InsertHead(T &aRef)
{
    OsclDoubleListBase::InsertHead(&aRef);
}
template <class T>
OSCL_INLINE void OsclDoubleList<T>::InsertTail(T &aRef)
{
    OsclDoubleListBase::InsertTail(&aRef);
}
template <class T>
OSCL_INLINE bool OsclDoubleList<T>::IsHead(const T *aPtr) const
{
    return(OsclPtrAdd(aPtr, iOffset) == (T *)&iHead);
}
template <class T>
OSCL_INLINE bool OsclDoubleList<T>::IsTail(const T *aPtr) const
{
    return(OsclPtrAdd(aPtr, iOffset) == (T *)iHead.iPrev);
}
template <class T>
OSCL_INLINE T *OsclDoubleList<T>::Head() const
{
    OSCL_ASSERT(!IsEmpty());
    return(OsclPtrSub((T *)iHead.iNext, iOffset));
}
template <class T>
OSCL_INLINE T *OsclDoubleList<T>::Tail() const
{
    OSCL_ASSERT(!IsEmpty());
    return(OsclPtrSub((T *)iHead.iPrev, iOffset));
}

// Class OsclPriorityList
template <class T>
OSCL_INLINE OsclPriorityList<T>::OsclPriorityList()
{}
template <class T>
OSCL_INLINE OsclPriorityList<T>::OsclPriorityList(int32 anOffset)
        : OsclDoubleListBase(anOffset)
{}
template <class T>
OSCL_INLINE void OsclPriorityList<T>::Insert(T &aRef)
{
    OsclDoubleListBase::Insert(&aRef);
}
template <class T>
OSCL_INLINE bool OsclPriorityList<T>::IsHead(const T *aPtr) const
{
    return(OsclPtrAdd(aPtr, iOffset) == (T *)&iHead);
}
template <class T>
OSCL_INLINE bool OsclPriorityList<T>::IsTail(const T *aPtr) const
{
    return(OsclPtrAdd(aPtr, iOffset) == (T *)iHead.iPrev);
}
template <class T>
OSCL_INLINE T *OsclPriorityList<T>::Head() const
{
    OSCL_ASSERT(!IsEmpty());
    return(OsclPtrSub((T *)iHead.iNext, iOffset));
}
template <class T>
OSCL_INLINE T *OsclPriorityList<T>::Tail() const
{
    OSCL_ASSERT(!IsEmpty());
    return(PtrSub((T *)iHead.iPrev, iOffset));
}






