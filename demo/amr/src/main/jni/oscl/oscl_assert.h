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

//                 O S C L _ A S S E R T

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */

/*! \file oscl_assert.h
 * \brief The file oscl_assert.h provides an OSCL_ASSERT macro to document
 assumptions and test them during development.
*/


#ifndef OSCL_ASSERT_H_INCLUDED
#define OSCL_ASSERT_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif


//! This function terminates the current process abnormally
/*!
*/
OSCL_COND_IMPORT_REF void _OSCL_Abort();

//! OSCL_ASSERT macro evaluates an expression and when the result is false, prints a diagnostic message and aborts the program.
/*!
    \param expr is the expression to be evaluated
    \param filename is the name of the current source file
    \param line_number is the line number in the current source file
*/
#if (!defined(NDEBUG) || (OSCL_ASSERT_ALWAYS))
OSCL_IMPORT_REF void OSCL_Assert(const char *expr, const char *filename, int line_number);

#define OSCL_ASSERT(_expr) \
    ((_expr)?((void)0):OSCL_Assert(# _expr,__FILE__,__LINE__))

#else
#define OSCL_ASSERT(_expr) ((void)0)
#endif

#if (!OSCL_DISABLE_INLINES)
#include "oscl_assert.inl"
#endif

/*! @} */

#endif // OSCL_ASSERT_H_INCLUDED
