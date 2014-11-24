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

//               O S C L _ S T R _ P T R _ L E N

//    This file contains C-based data structure definitions for string
//    manipulations.

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/** \file oscl_str_ptr_len.h
    \brief Defines a data structure for string containment/manipulations where the storage for the string is maintained externally.
*/


#ifndef OSCL_STR_PTR_LEN_H_INCLUDED
#define OSCL_STR_PTR_LEN_H_INCLUDED

// - - Inclusion - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_STDSTRING_H_INCLUDED
#include "oscl_stdstring.h"
#endif

//#ifndef OSCL_TYPES_H_INCLUDED
//#include "oscl_types.h"
//#endif

// - - Definitions - - - - - - - - - - - - - - - - - - - - - - - - - - -

// this is the XOR difference between any two ascii letters, 32
const uint8 OSCL_ASCII_CASE_MAGIC_BIT = 0x20;

//! This data structure encapsulates a set of functions used to perform
/*! standard string operations. It should be used for null-terminated constant
    (non-modifiable) strings of char type.
*/
typedef struct StrPtrLen
{
protected:
    const char* ptr;
    int32 len;

    bool isLetter(const char c) const
    {
        return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122));
    }

public:

    StrPtrLen(const char* newPtr)
            : ptr(newPtr)
    {
        len = oscl_strlen(newPtr);
    }

    StrPtrLen(const char* newPtr, uint32 newLen)
            : ptr(newPtr), len(newLen)
    {}

    StrPtrLen()
            : ptr(""), len(0)
    {}

    StrPtrLen(const StrPtrLen& rhs)
            : ptr(rhs.ptr), len(rhs.len)
    {}

    const char* c_str() const
    {
        return ptr;
    }

    int32 length() const
    {
        return len;
    }

    int32 size() const
    {
        return len;
    }

    void setPtrLen(const char* newPtr, uint32 newLen)
    {
        ptr = newPtr;
        len = newLen;
    }

    c_bool isCIEquivalentTo(const StrPtrLen& rhs) const
    {
        if (len != rhs.len)
        {
            return false;
        }
        // since they are the same length just check for the prefix condition
        return isCIPrefixOf(rhs);
    }

    c_bool isCIPrefixOf(const StrPtrLen& rhs) const
    {
        if (len > rhs.len)
        {
            return false;
        }

        for (int32 ii = 0; ii < len; ++ii)
        {
            if (ptr[ii] != rhs.ptr[ii])
            {
                if (!isLetter(ptr[ii]) ||
                        (OSCL_ASCII_CASE_MAGIC_BIT != ((ptr[ii] ^ rhs.ptr[ii]) | OSCL_ASCII_CASE_MAGIC_BIT)))
                {
                    return false;
                }
            }
        }
        return true;
    }

    int32 operator==(const StrPtrLen& rhs) const
    {
        if (len != rhs.len)
        {
            //return (!(len-rhs.len));
            return false;
        }

        return(!oscl_strncmp(ptr, rhs.ptr, rhs.len));
    }

    int32 operator!=(const StrPtrLen& rhs) const
    {
        return !(*this == rhs);
    }

    StrPtrLen &operator=(const StrPtrLen& rhs)
    {
        this->ptr = rhs.ptr;
        this->len = rhs.len;
        return *this;
    }

    StrPtrLen &operator=(const char* rhs)
    {
        this->ptr = rhs;
        this->len = oscl_strlen(rhs);
        return *this;
    }

} StrPtrLen;

//! This data structure encapsulates a set of functions used to perform
/*! standard string operations. It should be used for null-terminated constant
    strings (non-modifiable) of wchar type.
*/
typedef struct WStrPtrLen
{
protected:
    const oscl_wchar* ptr;
    int32 len;

public:

    WStrPtrLen(const oscl_wchar* newPtr)
            : ptr(newPtr)
    {
        len = oscl_strlen(newPtr);
    }

    WStrPtrLen(const oscl_wchar* newPtr, uint32 newLen)
            : ptr(newPtr), len(newLen)
    {}

    WStrPtrLen()
            : ptr(NULL), len(0)
    {}

    WStrPtrLen(const WStrPtrLen& rhs)
            : ptr(rhs.ptr), len(rhs.len)
    {}

    const oscl_wchar* c_str() const
    {
        return ptr;
    }

    int32 length() const
    {
        return len;
    }

    int32 size() const
    {
        return len;
    }

    void setPtrLen(const oscl_wchar* newPtr, uint32 newLen)
    {
        ptr = newPtr;
        len = newLen;
    }

    c_bool isCIEquivalentTo(const WStrPtrLen& rhs) const
    {
        if (len != rhs.len)
        {
            return false;
        }

        for (int32 ii = 0; ii < len; ++ii)
        {
            if (OSCL_ASCII_CASE_MAGIC_BIT != ((ptr[ii] ^ rhs.ptr[ii]) | OSCL_ASCII_CASE_MAGIC_BIT))
            {
                return false;
            }
        }

        return true;
    }

    int32 operator==(const WStrPtrLen& rhs) const
    {
        if (len != rhs.len)
        {
            return (len - rhs.len);
        }
        return(!oscl_strncmp(ptr, rhs.ptr, rhs.len));
    }

    int32 operator!=(const WStrPtrLen& rhs) const
    {
        return !(*this == rhs);
    }

    WStrPtrLen& operator=(const WStrPtrLen& rhs)
    {
        this->ptr = rhs.ptr;
        this->len = rhs.len;
        return *this;
    }

    WStrPtrLen& operator=(const oscl_wchar* rhs)
    {
        this->ptr = rhs;
        this->len = oscl_strlen(rhs);
        return *this;
    }

} WStrPtrLen;

//! same as StrPtrLen, but includes checksum field and method to speed up querying
typedef struct StrCSumPtrLen : public StrPtrLen
{
public:
    typedef int16 CheckSumType;

protected:
    CheckSumType checkSum;

public:

    void setPtrLen(const char* newPtr, uint32 newLen)
    {
        StrPtrLen::setPtrLen(newPtr, newLen);
        setCheckSum();
    }

    CheckSumType getCheckSum() const
    {
        return checkSum;
    }

    OSCL_IMPORT_REF void setCheckSum();

    StrCSumPtrLen()
            : checkSum(0)
    {}

    StrCSumPtrLen(const char* newPtr)
            : StrPtrLen(newPtr)
    {
        setCheckSum();
    }

    StrCSumPtrLen(const char* newPtr, uint32 newLen)
            : StrPtrLen(newPtr, newLen)
    {
        setCheckSum();
    }

    StrCSumPtrLen(const StrCSumPtrLen& rhs)
            : StrPtrLen(rhs), checkSum(rhs.checkSum)
    {}

    StrCSumPtrLen(const StrPtrLen & rhs)
            : StrPtrLen(rhs)
    {
        setCheckSum();
    }


    c_bool isCIEquivalentTo(const StrCSumPtrLen& rhs) const
    {
        if (getCheckSum() != rhs.getCheckSum())
        {
            return false;
        }

        return static_cast<const StrPtrLen&>(*this).isCIEquivalentTo(
                   static_cast<const StrPtrLen&>(rhs));
    }

    c_bool operator==(const StrCSumPtrLen& rhs) const
    {
        if (getCheckSum() != rhs.getCheckSum())
        {
            return false;
        }

        return (static_cast<const StrPtrLen&>(*this)
                ==  static_cast<const StrPtrLen&>(rhs));
    }

    c_bool operator!=(const StrCSumPtrLen& rhs) const
    {
        return !(*this == rhs);
    }

    StrCSumPtrLen& operator=(const StrCSumPtrLen& rhs)
    {
        StrPtrLen::operator=(rhs);
        this->checkSum = rhs.checkSum;
        return *this;
    }

    StrCSumPtrLen& operator=(const StrPtrLen& rhs)
    {
        StrPtrLen::operator=(rhs);
        setCheckSum();
        return *this;
    }

    StrCSumPtrLen& operator=(const char* rhs)
    {
        StrPtrLen::operator=(rhs);
        setCheckSum();
        return *this;
    }

} StrCSumPtrLen;

// Set character code to UNICODE
typedef WStrPtrLen OSCL_TStrPtrLen;

#endif // OSCL_STR_PTR_LEN_H_INCLUDED

/*! @} */
