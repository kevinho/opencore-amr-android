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

//         O S C L _ S H A R E D _ P T R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */


/*! \file oscl_shared_ptr.h
    \brief This file defines a template class OsclSharedPtr which is a "smart pointer" to the parameterized type.
*/


#ifndef OSCL_SHARED_PTR_H_INCLUDED
#define OSCL_SHARED_PTR_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_REFCOUNTER_H_INCLUDED
#include "oscl_refcounter.h"
#endif

#define OSCL_DISABLE_WARNING_RETURN_TYPE_NOT_UDT
#include "osclconfig_compiler_warnings.h"

//! A parameterized smart pointer class.
template <class TheClass>
class OsclSharedPtr
{
    public:
        //! Constructor
        OsclSharedPtr() :
                mpRep(NULL), refcnt(NULL) {}

        //! Constructor
        /*!
          \param inClassPtr A pointer to an instance of the parameterized type that the new OsclSharedPtr will wrap.
        */
        OsclSharedPtr(TheClass* inClassPtr, OsclRefCounter* in_refcnt) :
                mpRep(inClassPtr), refcnt(in_refcnt) {};


        //! Copy constructor
        OsclSharedPtr(const OsclSharedPtr& inSharedPtr) :
                mpRep(inSharedPtr.mpRep), refcnt(inSharedPtr.refcnt)
        {
            if (refcnt)
            {
                refcnt->addRef();
            }
        }


        //! Destructor.
        virtual ~OsclSharedPtr()
        {
            if (refcnt != NULL)
            {
                refcnt->removeRef();
            }
        } // end destructor


        //! The dereferencing operator returns a pointer to the parameterized type and can be used to access member
        //! elements of TheClass.
        TheClass* operator->()
        {
            return mpRep;
        }

        //! The indirection operator returns a reference to an object of the parameterized type.
        TheClass& operator*()
        {
            return *mpRep;
        }

        //! Casting operator
        operator TheClass*()
        {
            return mpRep;
        }

        //! Use this function to get a pointer to the wrapped object.
        TheClass* GetRep()
        {
            return mpRep;
        }

        //! Get the refcount pointer.  This should primarily be used for conversion operations
        OsclRefCounter* GetRefCounter()
        {
            return refcnt;
        }

        //! Get a count of how many references to the object exist.
        int get_count()
        {
            return (refcnt == NULL) ? 0 : refcnt->getCount();
        }

        //! Use this function to bind an existing OsclSharedPtr to a already-wrapped object.
        void Bind(const OsclSharedPtr& inHandle);

        //! Use this function to bind an existing OsclSharedPtr to a new (unwrapped) object.
        void Bind(TheClass* ptr, OsclRefCounter* in_refcnt);

        //! Use this function of unbind an existing OsclSharedPtr.
        void Unbind()
        {
            Bind(NULL, NULL);
        };

        //! Assignment operator.
        OsclSharedPtr& operator=(const OsclSharedPtr& inSharedPtr)
        {
            Bind(inSharedPtr);
            return *this;
        }

        //! Test for equality to see if two PVHandles wrap the same object.
        bool operator==(const OsclSharedPtr& b) const;

    private:

        TheClass* mpRep;
        OsclRefCounter* refcnt;

};


template <class TheClass> inline bool OsclSharedPtr<TheClass>::operator==(const OsclSharedPtr<TheClass>& b) const
{
    if ((this->mpRep == b.mpRep) &&
            (this->refcnt == b.refcnt))
    {
        return true;
    }
    return false;
}


template <class TheClass> inline void OsclSharedPtr<TheClass>::Bind(const OsclSharedPtr& inSharedPtr)
{
    if (mpRep == inSharedPtr.mpRep) return;

    if (refcnt != NULL)
    {
        refcnt->removeRef();
    }

    refcnt = inSharedPtr.refcnt;
    mpRep = inSharedPtr.mpRep;

    if (refcnt != NULL)
    {
        refcnt->addRef();
    }

}

template <class TheClass> inline void OsclSharedPtr<TheClass>::Bind(TheClass* ptr,
        OsclRefCounter* in_refcnt)
{
    if (refcnt != NULL)
    {
        refcnt->removeRef();
    }

    mpRep = ptr;
    refcnt = in_refcnt;

}

#endif  // OSCL_SHARED_PTR_H_INCLUDED
