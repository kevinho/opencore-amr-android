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

//                     O S C L _ E R R N O

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclerror OSCL Error
 *
 * @{
 */


/** \file oscl_errno.h
    \brief Defines functions to access additional information on errors where supported through an errno or similar service.
*/

//! oscl_errno.h contains functions to access the global errno

#ifndef OSCL_ERRNO_H_INCLUDED
#define OSCL_ERRNO_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCLCONFIG_ERROR_H_INCLUDED
#include "osclconfig_error.h"
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


/*!

*/
//! This function determines if a particular system saves the error number that occurs on a system call
/*!
    \return This method returns false on systems that do not save the
    error number that occurs on a system call in a global variable.
    Returns true for systems that do save the error number
*/
OSCL_IMPORT_REF bool OSCL_IsErrnoSupported();

//! This function returns the value of the system's global error number variable
/*!
    \return Returns 0 for system's that do not have this functionality
    The value of the error number variable does not change until the user
    calls SetLastError or if another system call occurs that changes the value
    Supported Platforms: Win32/wince, Unix
    Unsupported Platforms : Symbian
*/
OSCL_IMPORT_REF int OSCL_GetLastError();

//! This function sets the last error code for the system
/*!
    \param newVal This value represents the new value for the global error number
    This method can be used to reset the error number after having retrieved it
    using GetLastError.
    Supported Platforms: Win32/wince, Unix
    Unsupported Platforms : Symbian
*/
OSCL_IMPORT_REF bool OSCL_SetLastError(int newVal);

//! This function maps an error number to an error-message string.
/*!
    \param errnum This value represents the error number to map
    \return This method returns a pointer to a string containing the
    system error-message. It returns NULL for systems that do not have
    this functionality
    Supported Platforms: Win32/wince, Unix
    Unsupported Platforms : Symbian
*/
OSCL_IMPORT_REF char *OSCL_StrError(int errnum);

#if (!OSCL_DISABLE_INLINES)
#include "oscl_errno.inl"
#endif


#endif // INCLUDED_OSCL_ERRNO_H

/*! @} */
