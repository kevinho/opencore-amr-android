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


#ifndef OSCL_REGISTRY_SERV_IMPL_GLOBAL_H_INCLUDED
#define OSCL_REGISTRY_SERV_IMPL_GLOBAL_H_INCLUDED

#include "osclconfig_proc.h"
#include "oscl_base.h"
#if (OSCL_HAS_SINGLETON_SUPPORT)

#include "oscl_registry_serv_impl.h"
#include "oscl_registry_types.h"
#include "oscl_vector.h"
#include "oscl_mem.h"

//global-variable-based implementation.
//this handles both the registration client and the access client
//interfaces.
class OsclRegistryServImpl
{
    protected:
        OsclRegistryServImpl();
        virtual ~OsclRegistryServImpl();

        int32 Connect();
        void Close();

        //for registration client
        int32 Register(OSCL_String& aComponentID, OsclComponentFactory aFactory);
        int32 UnRegister(OSCL_String& aComponentID);

        //for access client.
        OsclComponentFactory GetFactory(OSCL_String& aComponent);
        void GetFactories(OSCL_String& aRegistry, Oscl_Vector<OsclRegistryAccessElement, OsclMemAllocator>& aVec);

        friend class OsclRegistryClient;
        friend class OsclRegistryAccessClient;
    private:
        bool IsOpen() const
        {
            return iIsOpen;
        }
        bool iIsOpen;
        //server data.
        OsclComponentRegistry* GetOsclComponentRegistry();
        //session data.
        Oscl_Vector<uint32, OsclMemAllocator> iIdVec;

};

#endif //oscl config

#endif //OSCL_REGISTRY_IMPL_GLOBAL_H_INCLUDED
/*! @} */


