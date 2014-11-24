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

//               O S C L_ S N P R I N T F

//    This is a portable implementation of snprintf() for use on all
//    platforms supported by oscl.

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/*!
 * \file oscl_snprintf.h
 * \brief Provides a portable implementation of snprintf
 *
 */


#ifndef OSCL_SNPRINTF_H_INCLUDED
#define OSCL_SNPRINTF_H_INCLUDED

// - - Inclusion - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCLCONFIG_UTIL_H_INCLUDED
#include "osclconfig_util.h"
#endif

OSCL_IMPORT_REF int32 oscl_snprintf(char *str, uint32 count, const char *fmt, /*args*/ ...);
OSCL_IMPORT_REF int32 oscl_snprintf(oscl_wchar *str, uint32 count, const oscl_wchar *fmt, /*args*/ ...);
OSCL_IMPORT_REF int32 oscl_vsnprintf(char *str, uint32 count, const char *fmt, va_list args);
OSCL_IMPORT_REF int32 oscl_vsnprintf(oscl_wchar *str, uint32 count, const oscl_wchar *fmt, va_list args);
#endif

/*! @} */
