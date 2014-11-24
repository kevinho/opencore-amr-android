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

//               O S C L   B Y T E O R D E R   U T I L S

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */


/*! \file oscl_byte_order.h
  \brief This file defines functions providing byte ordering utility (e.g., switching between big and little endian orders).
*/


#ifndef OSCL_BYTE_ORDER_H_INCLUDED
#define OSCL_BYTE_ORDER_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif


//! Convert little endian to host format.
/*!
   This function takes a buffer of data which is assumed to be in little endian order and
   rearranges it to the native order of the machine running the code. If the machine is a
   little endian machine, nothing is done.
   \param data A pointer to the input/output buffer
   \param size The number of bytes in the buffer.
*/
void little_endian_to_host(char *data, uint32 size);

//! Convert host to little endian format.
/*!
   This function takes a buffer of data which is assumed to be in the host's native order and
   rearranges it to the little endian format. If the machine is a little endian machine, nothing is done.
   \param data A pointer to the input/output buffer
   \param size The number of bytes in the buffer.
*/
void host_to_little_endian(char *data, unsigned int size);

//! Convert big endian to host format.
/*!
   This function takes a buffer of data which is assumed to be in big endian order and
   rearranges it to the native order of the machine running the code. If the machine is a
   big endian machine, nothing is done.
   \param data A pointer to the input/output buffer
   \param size The number of bytes in the buffer.
*/
void big_endian_to_host(char *data, unsigned int size);

//! Convert host to big endian format.
/*!
   This function takes a buffer of data which is assumed to be in native host order and
   rearranges it to big endian format. If the machine is a big endian machine, nothing is done.
   \param data A pointer to the input/output buffer
   \param size The number of bytes in the buffer.
*/
void host_to_big_endian(char *data, unsigned int size);

#if (!OSCL_DISABLE_INLINES)
#include "oscl_byte_order.inl"
#endif

/*! @} */

#endif
