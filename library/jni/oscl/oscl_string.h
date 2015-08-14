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

//               O S C L_ S T R I N G   C L A S S

//    This file contains a standardized set of string containers that
//    can be used in place of character arrays.

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/*!
 * \file oscl_string.h
 * \brief Provides a standardized set of string containers that
 *    can be used in place of character arrays.
 *
 */


#ifndef OSCL_STRING_H_INCLUDED
#define OSCL_STRING_H_INCLUDED


#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

/** Conversion operations for OSCL_String classes
*/
enum TOSCL_StringOp
{
    //Compress from wide-character to single-wide without any conversion.
    //This operation is only appropriate for ASCII strings.
    EOSCL_StringOp_CompressASCII
    //Compress from wide-character to single-side with UTF-16 to UTF-8 conversion
    , EOSCL_StringOp_UTF16ToUTF8
};

/** Conversion operations for OSCL_wString classes
*/
enum TOSCL_wStringOp
{
    //Expand from single-wide to wide-character without any conversion.
    //This operation is only appropriate for ASCII strings.
    EOSCL_wStringOp_ExpandASCII
    //Expand from single-wide to wide-character with UTF-8 to UTF-16 conversion.
    , EOSCL_wStringOp_UTF8ToUTF16
};

/**
    A common base class for string classes with
    "char" character format
*/
class OSCL_IMPORT_REF OSCL_String : public HeapBase

{
    public:
        typedef char chartype;

        /**
            This function returns the string size not including
            the null-terminator.
        */
        virtual uint32 get_size() const = 0;

        /**
            This function returns the maximum available storage size,
            not including null terminator.  The maximum size may be
            larger than the current string size.
        */
        virtual uint32 get_maxsize() const = 0;

        /**
            This function returns the C-style string for read access.
        */
        virtual const chartype* get_cstr() const = 0;

        /**
            This function returns true if the string is writable.
        */
        virtual bool is_writable() const;

        /**
            This function returns the C-style string for write access.
            If the string is not writable it returns NULL.
        */
        virtual chartype* get_str() const = 0;

        /** Assignment operator
        */
        OSCL_String& operator=(const OSCL_String& src);

        /** Assignment operator
            @param: null-terminated string
        */
        OSCL_String& operator=(const chartype* cstr);

        /** Append operator.
            This operator appends the input string to this object.
            The string may be truncated to fit available storage.
        */
        OSCL_String& operator+=(const OSCL_String& src);

        /** Append operator.
            This operator appends the input string to this object.
            The string may be truncated to fit available storage.
            @param: null-terminated string
        */
        OSCL_String& operator+=(const chartype* cstr);

        /** Append operator.
            This operator appends the input character to this object.
            The string may be truncated to fit available storage.
        */
        OSCL_String& operator+=(const chartype c);

        /** Comparison operators
        */
        bool operator== (const OSCL_String& src) const;
        bool operator!= (const OSCL_String& src) const;
        bool operator< (const OSCL_String& src) const;
        bool operator<= (const OSCL_String& src) const;
        bool operator> (const OSCL_String& src) const;
        bool operator>= (const OSCL_String& src) const;

        /** Comparison operator
            @param: null-terminated string
        */
        bool operator== (const chartype* cstr) const;

        /**
            This is subscript notation to access a character
            at the given position.
            If the index is outside the current size range then the
            function leaves.
        */
        chartype operator[](uint32 index) const;

        /**
            This function returns the character at the given position.
            If the index is outside the current size range then the
            function leaves.
        */
        virtual chartype read(uint32 index)const;

        /**
            This function allocates a temp storage for performing one of
            the following operations based on TOSCL_StringOp
            - compress src string from oscl_wchar to utf8.
            - convert src string from oscl_wchar to utf8.
            call parent set_rep() to copy resulting string.
            @param src: reference input string
            @param len: length of string to operate on
            @param op: type operation mentioned above
            @param aAlloc: optional, memory allocator if available
            @return length of compressed or converted string exclude terminated '\0'.
        */
        virtual uint32 setrep_to_char(const oscl_wchar* src, uint32 len,
                                      TOSCL_StringOp op, Oscl_DefAlloc* aAlloc);
        /**
            This function performs a hash operation on the string.
            If the string is not writable, the function leaves.
        */
        virtual int8 hash() const;

        /**
            This function stores a character at the specified position.
            If the string is not writable, the function leaves.
            If the index is outside the current size range then the
            function leaves.
        */
        virtual void write(uint32 index, chartype c);

        /**
            This function replaces characters at the specified offset
            within the current string.
            If the string is not writable, the function leaves.
            The characters may be truncted to fit the current storage.
            @param offset: the offset into the existing string buffer
            @param length: number of characters to copy.
            @param ptr: character buffer, not necessarily null-terminated.
        */
        virtual void write(uint32 offset, uint32 length, const chartype* buf);

#ifdef T_ARM
//ADS 1.2 compiler doesn't interpret "protected" correctly for templates.
//so the constructor/destructor need to be public.
    public:
#else
    protected:
#endif
        OSCL_String();
        virtual ~OSCL_String();

    protected:
        /** Each representation class must implement these pure virtuals.
        */

        /** Set string representation to input
            null-terminated string.
        */
        virtual void set_rep(const chartype* cstr) = 0;

        /** Append the input null-terminated string to the current
            string.  The string may be truncated to fit the available
            storage.
        */
        virtual void append_rep(const chartype* cstr) = 0;

        /** Set string representation to input string.
        */
        virtual void set_rep(const OSCL_String& src) = 0;

        /** Append the input string to the current string.
            The string may be truncated to fit the available storage.
        */
        virtual void append_rep(const OSCL_String& src) = 0;

        /** Update the length of the string.  This function
            will only be called when the string is writable.
        */
        virtual void set_len(uint32 len) = 0;
};

/**
    A common base class for string classes with
    wide character (oscl_wchar) format.
    OSCL_wString and OSCL_String are identical except
    for the character format.
    For descriptions, see OSCL_String.
*/
class OSCL_IMPORT_REF OSCL_wString
{
    public:
        typedef oscl_wchar chartype;

        virtual uint32 get_size() const = 0;

        virtual uint32 get_maxsize() const = 0;

        virtual const chartype* get_cstr() const = 0;

        virtual bool is_writable() const;

        virtual chartype* get_str() const = 0;

        OSCL_wString& operator=(const OSCL_wString& src);
        OSCL_wString& operator=(const chartype* cstr);

        OSCL_wString& operator+=(const OSCL_wString& src);
        OSCL_wString& operator+=(const chartype* cstr);
        OSCL_wString& operator+=(const chartype c);

        bool operator== (const OSCL_wString& src) const;
        bool operator!= (const OSCL_wString& src) const;
        bool operator< (const OSCL_wString& src) const;
        bool operator<= (const OSCL_wString& src) const;
        bool operator> (const OSCL_wString& src) const;
        bool operator>= (const OSCL_wString& src) const;
        bool operator== (const chartype* cstr) const;

        chartype operator[](uint32 index) const;

        virtual chartype read(uint32 index)const;

        virtual uint32 setrep_to_wide_char(const char* src, uint32 len,
                                           TOSCL_wStringOp op, Oscl_DefAlloc* aAlloc);

        virtual int8 hash() const;

        virtual void write(uint32 index, chartype c);
        virtual void write(uint32 offset, uint32 length, const chartype* buf);

#ifdef T_ARM
//ADS 1.2 compiler doesn't interpret "protected" correctly for templates.
//so the constructor/destructor need to be public.
    public:
#else
    protected:
#endif
        OSCL_wString();
        virtual ~OSCL_wString();

    protected:
        virtual void set_rep(const chartype* cstr) = 0;
        virtual void append_rep(const chartype* cstr) = 0;

        virtual void set_rep(const OSCL_wString& src) = 0;
        virtual void append_rep(const OSCL_wString& src) = 0;

        virtual void set_len(uint32 len) = 0;
};

#endif   // OSCL_STRING_H_INCLUDED

/*! @} */
