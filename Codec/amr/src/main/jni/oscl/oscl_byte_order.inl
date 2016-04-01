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

//               B Y T E O R D E R   U T I L S

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \file byte_order_utils.inl This file defines functions providing byte ordering utility.
*/

//! Convert little endian to host format.
/*!
   This function takes a buffer of data which is assumed to be in little endian order and
   rearranges it to the native order of the machine running the code. If the machine is a
   little endian machine, nothing is done.
   \param data A pointer to the input/output buffer
   \param size The number of bytes in the buffer.
*/

#if !(OSCL_BYTE_ORDER_LITTLE_ENDIAN) && !(OSCL_BYTE_ORDER_BIG_ENDIAN)
#error must define either OSCL_BYTE_ORDER_LITTLE_ENDIAN or OSCL_BYTE_ORDER_BIG_ENDIAN
#elif (OSCL_BYTE_ORDER_LITTLE_ENDIAN) && (OSCL_BYTE_ORDER_BIG_ENDIAN)
#error must only define one of OSCL_BYTE_ORDER_LITTLE_ENDIAN, OSCL_BYTE_ORDER_BIG_ENDIAN
#endif

//! Swap the order of bytes.
/*!
   This function takes a buffer of data and reverses the order of the bytes.
   \param data A pointer to the input/output buffer
   \param size The number of bytes in the buffer.
*/
OSCL_INLINE void swap_byte_order(char *data, unsigned int size)
{
    char tmp;
    char *ptr1, *ptr2;

    ptr1 = data;
    ptr2 = data + size - 1;

    while (ptr1 < ptr2)
    {
        tmp  = *ptr1;
        *ptr1++ = *ptr2;
        *ptr2-- = tmp;
    }

}




OSCL_INLINE void little_endian_to_host(char *data, uint32 size)
{
#if (OSCL_BYTE_ORDER_BIG_ENDIAN)
    swap_byte_order(data, size);
#else
    OSCL_UNUSED_ARG(data); // to remove warning 'unreferenced parameter
    OSCL_UNUSED_ARG(size); // to remove warning 'unreferenced parameter
    //  data=data; size=size;
#endif
}



//! Convert host to little endian format.
/*!
   This function takes a buffer of data which is assumed to be in the host's native order and
   rearranges it to the little endian format. If the machine is a little endian machine, nothing is done.
   \param data A pointer to the input/output buffer
   \param size The number of bytes in the buffer.
*/
OSCL_INLINE void host_to_little_endian(char *data, unsigned int size)
{
#if (OSCL_BYTE_ORDER_BIG_ENDIAN)
    swap_byte_order(data, size);
#else
    OSCL_UNUSED_ARG(data); // to remove warning 'unreferenced parameter
    OSCL_UNUSED_ARG(size); // to remove warning 'unreferenced parameter
#endif
}

//! Convert big endian to host format.
/*!
   This function takes a buffer of data which is assumed to be in big endian order and
   rearranges it to the native order of the machine running the code. If the machine is a
   big endian machine, nothing is done.
   \param data A pointer to the input/output buffer
   \param size The number of bytes in the buffer.
*/
OSCL_INLINE void big_endian_to_host(char *data, unsigned int size)
{
#if (OSCL_BYTE_ORDER_LITTLE_ENDIAN)
    swap_byte_order(data, size);
#else
    OSCL_UNUSED_ARG(data); // to remove warning 'unreferenced parameter
    OSCL_UNUSED_ARG(size); // to remove warning 'unreferenced parameter
#endif

}


//! Convert host to big endian format.
/*!
   This function takes a buffer of data which is assumed to be in native host order and
   rearranges it to big endian format. If the machine is a big endian machine, nothing is done.
   \param data A pointer to the input/output buffer
   \param size The number of bytes in the buffer.
*/
OSCL_INLINE void host_to_big_endian(char *data, unsigned int size)
{
#if (OSCL_BYTE_ORDER_LITTLE_ENDIAN)
    swap_byte_order(data, size);
#else
    OSCL_UNUSED_ARG(data); // to remove warning 'unreferenced parameter
    OSCL_UNUSED_ARG(size); // to remove warning 'unreferenced parameter
#endif


}


