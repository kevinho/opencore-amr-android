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

//               O S C L _ E R R O R _ I M P

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclerror OSCL Error
 *
 * @{
 */


/** \file oscl_error_imp.h
    \brief Internal error implementation support
*/

#ifndef OSCL_ERROR_IMP_H_INCLUDED
#define OSCL_ERROR_IMP_H_INCLUDED

#ifndef OSCLCONFIG_ERROR_H_INCLUDED
#include "osclconfig_error.h"
#endif

/** Internal leave/trap implementation.
*/

//This selects the type of implementation
//based on OSCL capabilities.
#define PVERROR_IMP_JUMPS


/*
#define PVERROR_IMP_CPP_EXCEPTIONS
#undef PVERROR_IMP_JUMPS
#undef PVERROR_IMP_FATAL_ERROR
*/

/**
//include the internal leave/trap implementation macros.
*/
#if defined( PVERROR_IMP_JUMPS)
#include "oscl_error_imp_jumps.h"
#elif defined (PVERROR_IMP_CPP_EXCEPTIONS)
#include "oscl_error_imp_cppexceptions.h"
#elif defined(PVERROR_IMP_FATAL_ERROR)
#include "oscl_error_imp_fatalerror.h"
#else
#error No leave implementation!
#endif




#endif

/*! @} */

