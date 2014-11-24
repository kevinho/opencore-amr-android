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

//                     O S C L _ R A N D

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/*!
 * \file oscl_rand.h
 * \brief Provides pseudo-random number generation.
 *
 */

#ifndef OSCL_RAND_H_INCLUDED
#define OSCL_RAND_H_INCLUDED

#ifndef OSCLCONFIG_UTIL_H_INCLUDED
#include "osclconfig_util.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_MEM_BASIC_FUNCTIONS_H
#include "oscl_mem_basic_functions.h"
#endif

class OsclRand
{
    public:
        OSCL_COND_IMPORT_REF void Seed(int32 seed);
        OSCL_COND_IMPORT_REF int32 Rand();
    private:
};

#if (!OSCL_DISABLE_INLINES)
#include "oscl_rand.inl"
#endif


#endif

/*! @} */
