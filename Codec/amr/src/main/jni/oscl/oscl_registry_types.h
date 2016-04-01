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

//              Oscl Registry Types

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/*!
 * \file oscl_registry_types.h
 * \brief Common types used in Oscl registry interfaces.
 *
 */


#ifndef OSCL_REGISTRY_TYPES_H_INCLUDED
#define OSCL_REGISTRY_TYPES_H_INCLUDED


#ifndef OSCL_TYPES_H_INCLUDED
#include "oscl_types.h"
#endif
#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

/*!
 * OsclComponentFactory is an opaque pointer.
 */
typedef OsclAny* OsclComponentFactory;

/*!
 * A class used to access the registry data
 */
class OsclRegistryAccessElement
{
    public:
        OsclComponentFactory iFactory;
        OSCL_HeapString<OsclMemAllocator> iMimeString;
};



#endif   // OSCL_REGISTRY_TYPES_H_INCLUDED

/*! @} */
