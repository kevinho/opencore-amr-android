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

//           O S C L _ E R R O R _ I M P _ F A T A L E R R O R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclerror OSCL Error
 *
 * @{
 */


/** \file oscl_error_imp_fatalerror.h
    \brief Implementation File for Leave using system fatal error.
*/

#ifndef OSCL_ERROR_IMP_FATALERROR_H_INCLUDED
#define OSCL_ERROR_IMP_FATALERROR_H_INCLUDED

// Implementation File for Leave using system fatal error.

//Fatal error implementation for compilers without C++ exceptions.
//This implementation is very limited.  Leave conditions
//just cause fatal program errors.  There is no way to catch or trap
//any Leave.
#ifndef OSCL_ASSERT_H_INCLUDED
#include "oscl_assert.h"
#endif

//Leave just calls a system fatal error.
#define PVError_DoLeave() _OSCL_Abort()

//_PV_TRAP
//_r is leave code, _s is statements.
//this macro isn't really functional since any
//leave will abort the program, but it's needed
//to compile.
#define _PV_TRAP(__r,__s) \
    __r=OsclErrNone;\
    {__s;}

//_PV_TRAP_NO_TLS
//_r is leave code, _s is statements.
//this macro isn't really functional since any
//leave will abort the program, but it's needed
//to compile.
#define _PV_TRAP_NO_TLS(__tr,__r,__s) \
    __r=OsclErrNone;\
    {__s;}



#endif // OSCL_ERROR_IMP_FATALERROR_H_INCLUDED

/*! @} */
