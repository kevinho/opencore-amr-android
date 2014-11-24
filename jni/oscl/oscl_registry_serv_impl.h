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

//               Oscl Registry Serv Impl

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */
/*!
 * \file oscl_registry_serv_impl.h
 * \brief Server-side implementation of OsclRegistry interfaces.
 *
 */


#ifndef OSCL_REGISTRY_SERV_IMPL_H_INCLUDED
#define OSCL_REGISTRY_SERV_IMPL_H_INCLUDED

#include "oscl_base.h"
#include "osclconfig_proc.h"

#include "oscl_registry_types.h"

/*!
** OS-independent declarations.
*/

#include "oscl_string.h"
#include "oscl_vector.h"
#include "oscl_mem.h"

/*!
** Data for each registered component.
*/
class OsclComponentRegistryElement
{
    public:
        OsclComponentRegistryElement(OSCL_String&, OsclComponentFactory);
        OsclComponentRegistryElement(const OsclComponentRegistryElement&);
        OsclComponentRegistryElement& operator=(const OsclComponentRegistryElement& src);
        ~OsclComponentRegistryElement();

        bool Match(OSCL_String& aStr, bool aExact);

        OSCL_String* iId;//use a pointer here instead of a container for efficiency
        //since this element is used in an oscl vector.
        OsclComponentFactory iFactory;
        uint32 iComponentId;
};


/*!
** Registry
*/
class OsclComponentRegistryData
{
    public:
        OsclComponentRegistryElement* Find(OSCL_String&, bool aExact);
        Oscl_Vector<OsclComponentRegistryElement, OsclMemAllocator> iVec;
};

/*!
** Thread-safe singleton registry object.
*/
#include "oscl_mutex.h"
class OsclComponentRegistry
{
    public:
        OsclComponentRegistry();
        ~OsclComponentRegistry();
        int32 Register(uint32& aId, OSCL_String&, OsclComponentFactory);
        int32 Unregister(OSCL_String&);
        int32 Unregister(uint32);
        OsclComponentFactory FindExact(OSCL_String&);
        void FindHierarchical(OSCL_String& , Oscl_Vector<OsclRegistryAccessElement, OsclMemAllocator>&);
        void OpenSession();
        void CloseSession();

        OsclComponentRegistryData iData;
        OsclMutex iMutex;

        uint32 iComponentIdCounter;

        uint32 iNumSessions;
};


#endif //OSCL_REGISTRY_IMPL_H_INCLUDED
/*! @} */


