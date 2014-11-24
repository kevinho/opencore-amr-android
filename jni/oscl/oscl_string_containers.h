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

//               O S C L_ S T R I N G _ C O N T A I N E R S
//
//    This file contains a standardized set of string containers that
//    can be used in place of character arrays.

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/*!
 * \file oscl_string_containers.h
 * \brief Provides a standardized set of string containers that
 *    can be used in place of character arrays.
 *
 */


#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#define OSCL_STRING_CONTAINERS_H_INCLUDED


#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif

#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif

#ifndef OSCL_REFCOUNTER_H_INCLUDED
#include "oscl_refcounter.h"
#endif

#ifndef OSCL_ERROR_H_INCLUDED
#include "oscl_error.h"
#endif

#ifndef OSCL_STRING_REP_H_INCLUDED
#include "oscl_string_rep.h"
#endif

#ifndef OSCL_STDSTRING_H_INCLUDED
#include "oscl_stdstring.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

/**
    OSCL_HeapString is a simple string class, compatible with
    regular character array strings.

    The string array is variable length, is allocated from the heap,
    and is modifiable.
    A copy-on-write mechanism is used to minimize unnecessary copying when
    multiple instances of a string are created for reading.
    Allocated memory is automatically freed by the class destructor when
    the last string referencing the memory is destroyed.

    The class HAS NO thread synchronization built-in, so it is NOT MT-SAFE.
    External locks should be used if the class is to be shared across threads.

    @param Alloc: memory allocator, derived from Oscl_DefAlloc.
*/
template <class Alloc>
class OSCL_HeapString : public OSCL_String
{
    public:
        typedef OSCL_String::chartype chartype;

        typedef TOSCL_StringOp optype;
        typedef OSCL_wString::chartype other_chartype;

        /**
            The default constructor creates an empty string.
        */
        OSCL_HeapString();

        /**
            Creates a heap string that contains a copy of the input
            string.
            @param src: input string.
        */
        OSCL_HeapString(const OSCL_HeapString& src);
        OSCL_HeapString(const OSCL_String& src);

        /**
            Creates a heap string that contains a copy of the input
            string.
            @param cp: null-terminated string.
        */
        OSCL_HeapString(const chartype* cstr);

        /**
            Creates a heap string that contains a copy of the input
            string or character array.
            @param src: character array, not necessarily null-terminated.
            @param length: number of characters to copy.
        */
        OSCL_HeapString(const chartype* buf, uint32 length);

        ~OSCL_HeapString();

        /** Pure virtuals from OSCL_String
        */
        uint32 get_size() const;
        uint32 get_maxsize() const;
        const chartype* get_cstr() const;
        chartype* get_str() const;

        /** Assignment operators
        */
        OSCL_HeapString& operator=(const OSCL_HeapString& src);
        OSCL_HeapString& operator=(const OSCL_String& src);
        OSCL_HeapString& operator=(const chartype* cstr);

        /**
            Set the contents of this string to a new string or
            character array.
            @param buf: string or character array.
            @param length: number of characters to copy.
        */
        void set(const chartype* buf, uint32 length);

        /**
            Set the contents of this string to a new string,
            with conversion operation.

            @param buf: NULL-terminated wide string.
            @param op: conversion operation to apply
        */
        void set(const other_chartype* buf, optype op);

        /**
            Set the contents of this string to a new string or
            character array, with conversion operation.

            @param buf: string or character array.
            @param length: number of characters to copy.
            @param op: conversion operation to apply
        */
        void set(const other_chartype* buf, uint32 length, optype op);

    private:
        CHeapRep *iRep;
        Alloc iAlloc;
        void create();
        void set_rep(const chartype*, uint32);
        void append_rep(const chartype*, uint32);

        /** Pure virtuals from OSCL_String.
        */
        void set_rep(const chartype* cstr);
        void append_rep(const chartype* cstr);
        void set_rep(const OSCL_String& src);
        void append_rep(const OSCL_String& src);
        void set_len(uint32 len);
        friend class OSCL_String;
};

/**
    OSCL_wHeapString is identical to OSCL_HeapString
    except that it uses wide-character format.
    For descriptions, see OSCL_HeapString.
*/
template <class Alloc>
class OSCL_wHeapString : public OSCL_wString
{
    public:
        typedef OSCL_wString::chartype chartype;
        typedef TOSCL_wStringOp optype;
        typedef OSCL_String::chartype other_chartype;

        OSCL_wHeapString();

        OSCL_wHeapString(const OSCL_wHeapString& src);
        OSCL_wHeapString(const OSCL_wString& src);

        OSCL_wHeapString(const chartype* cstr);
        OSCL_wHeapString(const chartype* buf, uint32 length);

        ~OSCL_wHeapString();

        uint32 get_size() const;
        uint32 get_maxsize() const;
        const chartype* get_cstr() const;
        chartype* get_str() const;

        OSCL_wHeapString& operator=(const OSCL_wHeapString& src);
        OSCL_wHeapString& operator=(const OSCL_wString& src);
        OSCL_wHeapString& operator=(const chartype* cstr);

        void set(const chartype* buf, uint32 length);

        void set(const other_chartype* buf, optype op);
        void set(const other_chartype* buf, uint32 length, optype op);

    private:
        CHeapRep *iRep;
        Alloc iAlloc;
        void create();
        void set_rep(const chartype*, uint32);
        void append_rep(const chartype*, uint32);

        void set_rep(const chartype* cstr);
        void append_rep(const chartype* cstr);
        void set_rep(const OSCL_wString& src);
        void append_rep(const OSCL_wString& src);
        void set_len(uint32 len);
        friend class OSCL_wString;
};

/**
    OSCL_HeapStringA is a simple string class, compatible with
    regular character array strings.  It is similar to
    OSCL_HeapString, except that the allocator is passed at
    run-time instead of compile-time.
    The allocator pointer is passed in the constructor, and
    may be a reference-counted object.
    If the allocator is not a reference-counted object then it must
    persist over the lifetime of all OSCL_HeapStringA objects that use it.
    If no allocator is provided, then an OsclMemAllocator
    will be used.

    The string array is variable length, is allocated from the heap,
    and is modifiable.
    A copy-on-write mechanism is used to minimize unnecessary copying when
    multiple instances of a string are created for reading.
    Allocated memory is automatically freed by the class destructor when
    the last string referencing the memory is destroyed.

    The class HAS NO thread synchronization built-in, so it is NOT MT-SAFE.
    External locks should be used if the class is to be shared across threads.

*/
class OSCL_HeapStringA : public OSCL_String
{
    public:
        typedef OSCL_String::chartype chartype;
        typedef TOSCL_StringOp optype;
        typedef OSCL_wString::chartype other_chartype;

        /**
            The default constructor creates an empty string.
            @param: (optional) allocator or reference-counted allocator.
            @param: (optional) reference counter associated with allocator object.
            If no allocator is provided, this this object will use
            an OsclMemAllocator.
        */
        OSCL_IMPORT_REF OSCL_HeapStringA();
        OSCL_IMPORT_REF OSCL_HeapStringA(Oscl_DefAlloc *alloc, OsclRefCounter *ref = NULL);

        /**
            Creates a heap string that contains a copy of the input
            string.
            @param src: input string.
            @param: (optional) allocator or reference-counted allocator.
            @param: (optional) reference counter associated with allocator object.
            If no allocator is provided, this this object will use
            an OsclMemAllocator.
        */
        OSCL_IMPORT_REF OSCL_HeapStringA(const OSCL_HeapStringA& src);
        OSCL_IMPORT_REF OSCL_HeapStringA(const OSCL_HeapStringA& src, Oscl_DefAlloc *alloc, OsclRefCounter *ref = NULL);
        OSCL_IMPORT_REF OSCL_HeapStringA(const OSCL_String& src, Oscl_DefAlloc *alloc = NULL, OsclRefCounter *ref = NULL);

        /**
            Creates a heap string that contains a copy of the input
            string.
            @param cp: null-terminated string.
            @param: (optional) allocator or reference-counted allocator.
            @param: (optional) reference counter associated with allocator object.
            If no allocator is provided, this this object will use
            an OsclMemAllocator.
        */
        OSCL_IMPORT_REF OSCL_HeapStringA(const chartype* cstr, Oscl_DefAlloc *alloc = NULL, OsclRefCounter *ref = NULL);

        /**
            Creates a heap string that contains a copy of the input
            string or character array.
            @param src: character array, not necessarily null-terminated.
            @param length: number of characters to copy.
            @param: (optional) allocator or reference-counted allocator.
            @param: (optional) reference counter associated with allocator object.
            If no allocator is provided, this this object will use
            an OsclMemAllocator.
        */
        OSCL_IMPORT_REF OSCL_HeapStringA(const chartype* buf, uint32 length, Oscl_DefAlloc *alloc = NULL, OsclRefCounter *ref = NULL);

        OSCL_IMPORT_REF ~OSCL_HeapStringA();

        /** Pure virtuals from OSCL_String
        */
        OSCL_IMPORT_REF uint32 get_size() const;
        OSCL_IMPORT_REF uint32 get_maxsize() const;
        OSCL_IMPORT_REF const chartype* get_cstr() const;
        OSCL_IMPORT_REF chartype* get_str() const;

        /** Assignment operators
        */
        OSCL_IMPORT_REF OSCL_HeapStringA& operator=(const OSCL_HeapStringA& src);
        OSCL_IMPORT_REF OSCL_HeapStringA& operator=(const OSCL_String& src);
        OSCL_IMPORT_REF OSCL_HeapStringA& operator=(const chartype* cstr);

        /**
            Set the contents of this string to a new string or
            character array.
            @param buf: string or character array.
            @param length: number of characters to copy.
        */
        OSCL_IMPORT_REF void set(const chartype* buf, uint32 length);

        /**
            Set the contents of this string to a new string,
            with conversion operation.

            @param buf: NULL-terminated wide string.
            @param op: conversion operation to apply
        */
        OSCL_IMPORT_REF void set(const other_chartype* buf, optype op);

        /**
            Set the contents of this string to a new string or
            character array, with conversion operation.

            @param buf: string or character array.
            @param length: number of characters to copy.
            @param op: conversion operation to apply
        */
        OSCL_IMPORT_REF void set(const other_chartype* buf, uint32 length, optype op);

    private:
        CHeapRep *iRep;
        OsclRefCounter *iAllocRef;
        Oscl_DefAlloc *iAlloc;
        OsclMemAllocator iDefAlloc;
        void create(Oscl_DefAlloc *, OsclRefCounter*);
        void set_rep(const chartype*, uint32);
        void append_rep(const chartype*, uint32);

        /** Pure virtuals from OSCL_String.
        */
        void set_rep(const chartype* cstr);
        void append_rep(const chartype* cstr);
        void set_rep(const OSCL_String& src);
        void append_rep(const OSCL_String& src);
        void set_len(uint32 len);
        friend class OSCL_String;
};

/**
    OSCL_wHeapStringA is identical to OSCL_HeapStringA
    except that it uses wide-character format.
    For descriptions, see OSCL_HeapStringA.
*/
class OSCL_wHeapStringA : public OSCL_wString
{
    public:
        typedef OSCL_wString::chartype chartype;
        typedef TOSCL_wStringOp optype;
        typedef OSCL_String::chartype other_chartype;

        OSCL_IMPORT_REF OSCL_wHeapStringA();
        OSCL_IMPORT_REF OSCL_wHeapStringA(Oscl_DefAlloc *alloc, OsclRefCounter *ref = NULL);

        OSCL_IMPORT_REF OSCL_wHeapStringA(const OSCL_wHeapStringA& src);
        OSCL_IMPORT_REF OSCL_wHeapStringA(const OSCL_wHeapStringA& src, Oscl_DefAlloc *alloc, OsclRefCounter *ref = NULL);
        OSCL_IMPORT_REF OSCL_wHeapStringA(const OSCL_wString& src, Oscl_DefAlloc *alloc = NULL, OsclRefCounter *ref = NULL);

        OSCL_IMPORT_REF OSCL_wHeapStringA(const chartype* cstr, Oscl_DefAlloc *alloc = NULL, OsclRefCounter *ref = NULL);

        OSCL_IMPORT_REF OSCL_wHeapStringA(const chartype* buf, uint32 length, Oscl_DefAlloc *alloc = NULL, OsclRefCounter *ref = NULL);

        OSCL_IMPORT_REF ~OSCL_wHeapStringA();

        OSCL_IMPORT_REF uint32 get_size() const;
        OSCL_IMPORT_REF uint32 get_maxsize() const;
        OSCL_IMPORT_REF const chartype* get_cstr() const;
        OSCL_IMPORT_REF chartype* get_str() const;

        OSCL_IMPORT_REF OSCL_wHeapStringA& operator=(const OSCL_wHeapStringA& src);
        OSCL_IMPORT_REF OSCL_wHeapStringA& operator=(const OSCL_wString& src);
        OSCL_IMPORT_REF OSCL_wHeapStringA& operator=(const chartype* cstr);

        OSCL_IMPORT_REF void set(const chartype* buf, uint32 length);

        OSCL_IMPORT_REF void set(const other_chartype* buf, optype op);
        OSCL_IMPORT_REF void set(const other_chartype* buf, uint32 length, optype op);

    private:
        CHeapRep *iRep;
        OsclRefCounter *iAllocRef;
        Oscl_DefAlloc *iAlloc;
        OsclMemAllocator iDefAlloc;
        void create(Oscl_DefAlloc *, OsclRefCounter*);
        void set_rep(const chartype*, uint32);
        void append_rep(const chartype*, uint32);

        void set_rep(const chartype* cstr);
        void append_rep(const chartype* cstr);
        void set_rep(const OSCL_wString& src);
        void append_rep(const OSCL_wString& src);
        void set_len(uint32 len);
        friend class OSCL_wString;
};


/**
    OSCL_StackString is a simple string class, compatible with
    regular character array strings.

    The string array is fixed length, is allocated from the stack,
    and is modifiable.  Operations that update the string will automatically
    truncate it to fit the fixed size storage.
    This is recommended for use for short strings (<255).
    Use OSCL_HeapString for very large strings to avoid stack overflow.

    @param C: type of character.
    @param MaxBufSize: maximum string length not including null terminator.
*/
template <uint32 MaxBufSize>
class OSCL_StackString : public OSCL_String
{
    public:
        typedef OSCL_String::chartype chartype;
        typedef TOSCL_StringOp optype;
        typedef OSCL_wString::chartype other_chartype;

        /** Creates an OSCL_StackString initialized with an empty string.
        */
        OSCL_StackString();

        /** Creates an OSCL_StackString with a copy of the input string.
            The string may be truncated to fit the available storage.
            @param src: input string.
        */
        OSCL_StackString(const OSCL_StackString& src);
        OSCL_StackString(const OSCL_String& src);

        /** Creates an OSCL_StackString with a copy of the input string.
            The string may be truncated to fit the available storage.
            @param cp: a null-terminated string.
        */
        OSCL_StackString(const chartype* cstr);

        /** Creates an OSCL_StackString with a copy of the input string.
            The string may be truncated to fit the available storage.
            @param src: a character array, not necessarily null-terminated.
            @param length: the number of characters to copy.
        */
        OSCL_StackString(const chartype* buf, uint32 length);

        ~OSCL_StackString();

        /** Pure virtuals from OSCL_String
        */
        uint32 get_size() const;
        uint32 get_maxsize() const;
        const chartype* get_cstr() const;
        chartype* get_str() const;

        /** Assignment operators
        */
        OSCL_StackString& operator=(const OSCL_StackString& src);
        OSCL_StackString& operator=(const OSCL_String& src);
        OSCL_StackString& operator=(const chartype* cstr);

        /**
            Set the contents of this string to a new string or
            character array.
            @param buf: string or character array.
            @param length: number of characters to copy.
        */
        void set(const chartype* buf, uint32 length);

        /**
            Set the contents of this string to a new string,
            with conversion operation.

            @param buf: NULL-terminated wide string.
            @param op: conversion operation to apply
        */
        void set(const other_chartype* buf, optype op);

        /**
            Set the contents of this string to a new string or
            character array, with conversion operation.

            @param buf: string or character array.
            @param length: number of characters to copy.
            @param op: conversion operation to apply
        */
        void set(const other_chartype* buf, uint32 length, optype op);

    private:
        CStackRep rep;
        char buffer[MaxBufSize+1];//fixed string buffer.
        void create();

        /** Pure virtuals from OSCL_String
        */
        void set_rep(const chartype* cstr);
        void append_rep(const chartype* cstr);
        void set_rep(const OSCL_String& src);
        void append_rep(const OSCL_String& src);
        void set_len(uint32 len);
        friend class OSCL_String;
};

/**
    OSCL_wStackString is identical to OSCL_StackString
    except that it uses wide-character format.
    For descriptions, see OSCL_StackString.
*/
template <uint32 MaxBufSize>
class OSCL_wStackString : public OSCL_wString
{
    public:
        typedef OSCL_wString::chartype chartype;
        typedef TOSCL_wStringOp optype;
        typedef OSCL_String::chartype other_chartype;

        OSCL_wStackString();

        OSCL_wStackString(const OSCL_wStackString& src);
        OSCL_wStackString(const OSCL_wString& src);

        OSCL_wStackString(const chartype* cstr);
        OSCL_wStackString(const chartype* buf, uint32 length);

        ~OSCL_wStackString();

        uint32 get_size() const;
        uint32 get_maxsize() const;
        const chartype* get_cstr() const;
        chartype* get_str() const;

        OSCL_wStackString& operator=(const OSCL_wStackString& src);
        OSCL_wStackString& operator=(const OSCL_wString& src);
        OSCL_wStackString& operator=(const chartype* cstr);

        void set(const chartype* buf, uint32 length);

        void set(const other_chartype* buf, optype op);
        void set(const other_chartype* buf, uint32 length, optype op);

    private:
        CStackRep rep;
        chartype buffer[MaxBufSize+1];//fixed string buffer.
        void create();

        void set_rep(const chartype* cstr);
        void append_rep(const chartype* cstr);
        void set_rep(const OSCL_wString& src);
        void append_rep(const OSCL_wString& src);
        void set_len(uint32 len);
        friend class OSCL_wString;
};

/**
    OSCL_FastString is a simple string class, compatible with
    regular character array strings.

    This class does not allocate internal memory for the string but acts as
    a container for a user-defined buffer. This means no copying of the string
    is done and provides a faster way of manipulating strings.
    Depending on initialization, this container provides either read-only
    or read-write access to the string.

    Implementation assumes the input string is null-terminated.

    @param C: type of character.
*/
class OSCL_FastString : public OSCL_String
{
    public:
        typedef OSCL_String::chartype chartype;
        typedef TOSCL_StringOp optype;
        typedef OSCL_wString::chartype other_chartype;

        /**
            Default constructor.
        */
        OSCL_IMPORT_REF OSCL_FastString();

        /**
            Creates a fast string that contains a copy of the input
            string.
            The string inherits the writable-ness of the source string.
            @param src: input string.
        */
        OSCL_IMPORT_REF OSCL_FastString(const OSCL_FastString& src);

        /**
            Create the string and initialize it to contain the input string.
            The string is not writable.
            @param: null-terminated string.
        */
        OSCL_IMPORT_REF OSCL_FastString(const chartype* cstr);

        /**
            Create the string and initialize it to contain the input string.
            The string is writable.
            @param cp: null-terminated string.
            @param maxlen: maximum size of storage at cp, not incl null terminator.
            If input string is not null-terminated, the function leaves.
        */
        OSCL_IMPORT_REF OSCL_FastString(chartype* buf, uint32 maxlen);

        OSCL_IMPORT_REF ~OSCL_FastString();

        /** Pure virtuals from OSCL_String
        */
        OSCL_IMPORT_REF uint32 get_size() const;
        OSCL_IMPORT_REF uint32 get_maxsize() const;
        OSCL_IMPORT_REF const chartype* get_cstr() const;
        OSCL_IMPORT_REF chartype* get_str() const;

        /** Assignment operators
        */
        OSCL_IMPORT_REF OSCL_FastString& operator=(const OSCL_FastString& src);
        OSCL_IMPORT_REF OSCL_FastString& operator=(const chartype* cstr);

        /**
            This function can be used to reassign the string to a new
            writable string.
            If input string is not null-terminated, the function leaves.
        */
        OSCL_IMPORT_REF void set(chartype* cstr, uint32 maxlen);

        /**
            Set the contents of this string to a new string or
            character array, with conversion operation.

            @param buf: string or character array.
            @param numofbyte: number of bytes available in the buffer.
               There must be enough space available for the converted string including its NULL terminator.
               The converted string may be smaller or larger than the original string.
            @param op: conversion operation to apply
            If numofbyte is not large enough for conversion, the function leaves.
            If input string is not null-terminated, the function leaves.
        */
        OSCL_IMPORT_REF void set(const other_chartype* buf, uint32 numofbyte, optype op);

        /**
            This function can be used to refresh the string size in case the
            contents of the string buffer have been modified since the
            container was created.
        */
        OSCL_IMPORT_REF void set_length();

    private:
        CFastRep rep;

        /** Pure virtuals from OSCL_String
        */
        void set_rep(const chartype* cstr);
        void append_rep(const chartype* cstr);
        void set_rep(const OSCL_String& src);
        void append_rep(const OSCL_String& src);
        void set_len(uint32 len);
        friend class OSCL_String;
};

/**
    OSCL_wFastString is identical to OSCL_FastString
    except that it uses wide-character format.
    For descriptions, see OSCL_FastString.
*/
class OSCL_wFastString : public OSCL_wString
{
    public:
        typedef OSCL_wString::chartype chartype;
        typedef TOSCL_wStringOp optype;
        typedef OSCL_String::chartype other_chartype;

        OSCL_IMPORT_REF OSCL_wFastString();

        OSCL_IMPORT_REF OSCL_wFastString(const OSCL_wFastString& src);

        OSCL_IMPORT_REF OSCL_wFastString(const chartype* cstr);
        OSCL_IMPORT_REF OSCL_wFastString(chartype* buf, uint32 maxlen);

        OSCL_IMPORT_REF ~OSCL_wFastString();

        OSCL_IMPORT_REF uint32 get_size() const;
        OSCL_IMPORT_REF uint32 get_maxsize() const;
        OSCL_IMPORT_REF const chartype* get_cstr() const;
        OSCL_IMPORT_REF chartype* get_str() const;

        OSCL_IMPORT_REF OSCL_wFastString& operator=(const OSCL_wFastString& src);
        OSCL_IMPORT_REF OSCL_wFastString& operator=(const chartype* cstr);

        OSCL_IMPORT_REF void set(chartype* cstr, uint32 maxlen);

        OSCL_IMPORT_REF void set(const other_chartype* buf, uint32 numofbyte, optype op);

        OSCL_IMPORT_REF void set_length();

    private:
        CFastRep rep;

        void set_rep(const chartype* cstr);
        void append_rep(const chartype* cstr);
        void set_rep(const OSCL_wString& src);
        void append_rep(const OSCL_wString& src);
        void set_len(uint32 len);
        friend class OSCL_wString;
};


// **************************************************************
//                   OSCL_HeapString<Alloc> Implementation
//                   OSCL_wHeapString<Alloc> Implementation
// **************************************************************

template<class Alloc>
void OSCL_HeapString<Alloc>::set_rep(const chartype* cp, uint32 len)
//set heap rep to new string.
{
    CHeapRep::set_rep(iRep, iAlloc, cp, len);
}

template<class Alloc>
void OSCL_wHeapString<Alloc>::set_rep(const chartype* cp, uint32 len)
//set heap rep to new string.
{
    CHeapRep::set_rep(iRep, iAlloc, cp, len);
}

// **************************************************************
template<class Alloc>
void OSCL_HeapString<Alloc>::append_rep(const chartype* cp, uint32 len)
//set heap rep to current string plus new string.
{
    CHeapRep::append_rep(iRep, iAlloc, cp, len);
}

template<class Alloc>
void OSCL_wHeapString<Alloc>::append_rep(const chartype* cp, uint32 len)
//set heap rep to current string plus new string.
{
    CHeapRep::append_rep(iRep, iAlloc, cp, len);
}

// **************************************************************
template<class Alloc>
void OSCL_HeapString<Alloc>::set_rep(const chartype* cp)
{
    set_rep(cp, (cp) ? oscl_strlen(cp) : 0);
}

template<class Alloc>
void OSCL_wHeapString<Alloc>::set_rep(const chartype* cp)
{
    set_rep(cp, (cp) ? oscl_strlen(cp) : 0);
}


// **************************************************************
template<class Alloc>
void OSCL_HeapString<Alloc>::append_rep(const chartype* cp)
{
    append_rep(cp, (cp) ? oscl_strlen(cp) : 0);
}

template<class Alloc>
void OSCL_wHeapString<Alloc>::append_rep(const chartype* cp)
{
    append_rep(cp, (cp) ? oscl_strlen(cp) : 0);
}


// **************************************************************
template<class Alloc>
void OSCL_HeapString<Alloc>::set_rep(const OSCL_String& src)
{
    set_rep(src.get_cstr(), src.get_size());
}

template<class Alloc>
void OSCL_wHeapString<Alloc>::set_rep(const OSCL_wString& src)
{
    set_rep(src.get_cstr(), src.get_size());
}


// **************************************************************
template<class Alloc>
void OSCL_HeapString<Alloc>::append_rep(const OSCL_String& src)
{
    append_rep(src.get_cstr(), src.get_size());
}

template<class Alloc>
void OSCL_wHeapString<Alloc>::append_rep(const OSCL_wString& src)
{
    append_rep(src.get_cstr(), src.get_size());
}


// **************************************************************
template<class Alloc>
uint32 OSCL_HeapString<Alloc>::get_size() const
{
    if (iRep)
        return iRep->size;
    return 0;
}

template<class Alloc>
uint32 OSCL_wHeapString<Alloc>::get_size() const
{
    if (iRep)
        return iRep->size;
    return 0;
}

// **************************************************************
template<class Alloc>
void OSCL_HeapString<Alloc>::set_len(uint32 len)
{
    iRep->size = len;
}

template<class Alloc>
void OSCL_wHeapString<Alloc>::set_len(uint32 len)
{
    iRep->size = len;
}


// **************************************************************
template<class Alloc>
uint32 OSCL_HeapString<Alloc>::get_maxsize() const
{
    if (iRep)
        return iRep->maxsize;
    return 0;
}

template<class Alloc>
uint32 OSCL_wHeapString<Alloc>::get_maxsize() const
{
    if (iRep)
        return iRep->maxsize;
    return 0;
}


// **************************************************************
template<class Alloc>
const typename OSCL_HeapString<Alloc>::chartype* OSCL_HeapString<Alloc>::get_cstr() const
{
    if (iRep)
        return (chartype*)iRep->buffer;
    return NULL;
}

template<class Alloc>
const typename OSCL_wHeapString<Alloc>::chartype* OSCL_wHeapString<Alloc>::get_cstr() const
{
    if (iRep)
        return (chartype*)iRep->buffer;
    return NULL;
}


// **************************************************************
template<class Alloc>
typename OSCL_HeapString<Alloc>::chartype* OSCL_HeapString<Alloc>::get_str() const
{
    if (iRep)
        return (chartype*)iRep->buffer;
    return NULL;
}

template<class Alloc>
typename OSCL_wHeapString<Alloc>::chartype* OSCL_wHeapString<Alloc>::get_str() const
{
    if (iRep)
        return (chartype*)iRep->buffer;
    return NULL;
}


// **************************************************************
template<class Alloc>
void OSCL_HeapString<Alloc>::create()
{
    iRep = NULL;
}

template<class Alloc>
void OSCL_wHeapString<Alloc>::create()
{
    iRep = NULL;
}


// **************************************************************
template<class Alloc>
OSCL_HeapString<Alloc>::OSCL_HeapString()
{
    create();
    set_rep(NULL);
}

template<class Alloc>
OSCL_wHeapString<Alloc>::OSCL_wHeapString()
{
    create();
    set_rep(NULL);
}


// **************************************************************
template<class Alloc>
OSCL_HeapString<Alloc>::OSCL_HeapString(const chartype* cp)
{
    create();
    set_rep(cp);
}

template<class Alloc>
OSCL_wHeapString<Alloc>::OSCL_wHeapString(const chartype* cp)
{
    create();
    set_rep(cp);
}


// **************************************************************
template<class Alloc>
void OSCL_HeapString<Alloc>::set(const chartype* cp, uint32 length)
{
    set_rep(cp, length);
    //just in case input string is shorter than 'length'
    iRep->size = oscl_strlen(get_cstr());
}

template<class Alloc>
void OSCL_wHeapString<Alloc>::set(const chartype* cp, uint32 length)
{
    set_rep(cp, length);
    //just in case input string is shorter than 'length'
    iRep->size = oscl_strlen(get_cstr());
}


// **************************************************************
template<class Alloc>
void OSCL_HeapString<Alloc>::set(const other_chartype* buf, optype op)
{
    iRep->size = setrep_to_char(buf, oscl_strlen(buf), op, &iAlloc);
}

template<class Alloc>
void OSCL_wHeapString<Alloc>::set(const other_chartype* buf, optype op)
{
    iRep->size = setrep_to_wide_char(buf, oscl_strlen(buf), op, &iAlloc);
}

// **************************************************************
template<class Alloc>
void OSCL_HeapString<Alloc>::set(const other_chartype* buf, uint32 length, optype op)
{
    iRep->size = setrep_to_char(buf, length, op, &iAlloc);
}

template<class Alloc>
void OSCL_wHeapString<Alloc>::set(const other_chartype* buf, uint32 length, optype op)
{
    iRep->size = setrep_to_wide_char(buf, length, op, &iAlloc);
}

// **************************************************************
template<class Alloc>
OSCL_HeapString<Alloc>::OSCL_HeapString(const chartype* cp, uint32 length)
{
    create();
    set(cp, length);
}

template<class Alloc>
OSCL_wHeapString<Alloc>::OSCL_wHeapString(const chartype* cp, uint32 length)
{
    create();
    set(cp, length);
}

// **************************************************************
template<class Alloc>
OSCL_HeapString<Alloc>::OSCL_HeapString(const OSCL_HeapString<Alloc>& src) : OSCL_String(src)
{
    create();
    if (src.iRep)
        CHeapRep::assign(iRep, src.iRep, iAlloc);
    else
        set_rep(src);
}

template<class Alloc>
OSCL_wHeapString<Alloc>::OSCL_wHeapString(const OSCL_wHeapString<Alloc>& src)  : OSCL_wString(src)
{
    create();
    if (src.iRep)
        CHeapRep::assign(iRep, src.iRep, iAlloc);
    else
        set_rep(src);
}

// **************************************************************
template<class Alloc>
OSCL_HeapString<Alloc>::OSCL_HeapString(const OSCL_String& src)
{
    create();
    set_rep(src);
}

template<class Alloc>
OSCL_wHeapString<Alloc>::OSCL_wHeapString(const OSCL_wString& src)
{
    create();
    set_rep(src);
}

// **************************************************************
template<class Alloc>
OSCL_HeapString<Alloc>::~OSCL_HeapString()
{
    if (iRep)
        iRep->remove_ref(iAlloc);
}

template<class Alloc>
OSCL_wHeapString<Alloc>::~OSCL_wHeapString()
{
    if (iRep)
        iRep->remove_ref(iAlloc);
}


// **************************************************************
template<class Alloc>
OSCL_HeapString<Alloc>& OSCL_HeapString<Alloc>::operator=(const OSCL_HeapString<Alloc>& src)
{
    if (src.iRep)
        CHeapRep::assign(iRep, src.iRep, iAlloc);
    else
        set_rep(src);
    return (*this);
}

template<class Alloc>
OSCL_wHeapString<Alloc>& OSCL_wHeapString<Alloc>::operator=(const OSCL_wHeapString<Alloc>& src)
{
    if (src.iRep)
        CHeapRep::assign(iRep, src.iRep, iAlloc);
    else
        set_rep(src);
    return (*this);
}

// **************************************************************
template<class Alloc>
OSCL_HeapString<Alloc>& OSCL_HeapString<Alloc>::operator=(const OSCL_String & src)
{
    set_rep(src);
    return (*this);
}

template<class Alloc>
OSCL_wHeapString<Alloc>& OSCL_wHeapString<Alloc>::operator=(const OSCL_wString & src)
{
    set_rep(src);
    return (*this);
}

// **************************************************************
template<class Alloc>
OSCL_HeapString<Alloc>& OSCL_HeapString<Alloc>::operator=(const chartype * cp)
{
    set_rep(cp);
    return (*this);
}

template<class Alloc>
OSCL_wHeapString<Alloc>& OSCL_wHeapString<Alloc>::operator=(const chartype * cp)
{
    set_rep(cp);
    return (*this);
}

// **************************************************************
//                 OSCL_StackString<MaxBufSize> Implementation
//                 OSCL_wStackString<MaxBufSize> Implementation
// **************************************************************
template<uint32 MaxBufSize>
void OSCL_StackString<MaxBufSize>::set_rep(const chartype* cp)
{
    rep.set(cp, (cp) ? oscl_strlen(cp) : 0);
}

template<uint32 MaxBufSize>
void OSCL_wStackString<MaxBufSize>::set_rep(const chartype* cp)
{
    rep.set(cp, (cp) ? oscl_strlen(cp) : 0);
}


// **************************************************************
template<uint32 MaxBufSize>
void OSCL_StackString<MaxBufSize>::append_rep(const chartype* cp)
{
    rep.append(cp, (cp) ? oscl_strlen(cp) : 0);
}

template<uint32 MaxBufSize>
void OSCL_wStackString<MaxBufSize>::append_rep(const chartype* cp)
{
    rep.append(cp, (cp) ? oscl_strlen(cp) : 0);
}

// **************************************************************
template<uint32 MaxBufSize>
void OSCL_StackString<MaxBufSize>::set_rep(const OSCL_String& src)
{
    rep.set(src.get_cstr(), src.get_size());
}

template<uint32 MaxBufSize>
void OSCL_wStackString<MaxBufSize>::set_rep(const OSCL_wString& src)
{
    rep.set(src.get_cstr(), src.get_size());
}

// **************************************************************
template<uint32 MaxBufSize>
void OSCL_StackString<MaxBufSize>::append_rep(const OSCL_String& src)
{
    rep.append(src.get_cstr(), src.get_size());
}

template<uint32 MaxBufSize>
void OSCL_wStackString<MaxBufSize>::append_rep(const OSCL_wString& src)
{
    rep.append(src.get_cstr(), src.get_size());
}

// **************************************************************
template<uint32 MaxBufSize>
void OSCL_StackString<MaxBufSize>::set_len(uint32 len)
{
    rep.size = len;
}

template<uint32 MaxBufSize>
void OSCL_wStackString<MaxBufSize>::set_len(uint32 len)
{
    rep.size = len;
}

// **************************************************************
template<uint32 MaxBufSize>
uint32 OSCL_StackString<MaxBufSize>::get_size() const
{
    return rep.size;
}

template<uint32 MaxBufSize>
uint32 OSCL_wStackString<MaxBufSize>::get_size() const
{
    return rep.size;
}

// **************************************************************
template<uint32 MaxBufSize>
uint32 OSCL_StackString<MaxBufSize>::get_maxsize() const
{
    return rep.maxsize;
}

template<uint32 MaxBufSize>
uint32 OSCL_wStackString<MaxBufSize>::get_maxsize() const
{
    return rep.maxsize;
}

// **************************************************************
template<uint32 MaxBufSize>
const typename OSCL_StackString<MaxBufSize>::chartype* OSCL_StackString<MaxBufSize>::get_cstr() const
{
    return (chartype*)rep.buffer;
}

template<uint32 MaxBufSize>
const typename OSCL_wStackString<MaxBufSize>::chartype* OSCL_wStackString<MaxBufSize>::get_cstr() const
{
    return (chartype*)rep.buffer;
}

// **************************************************************
template<uint32 MaxBufSize>
typename OSCL_StackString<MaxBufSize>::chartype* OSCL_StackString<MaxBufSize>::get_str() const
{
    return (chartype*)rep.buffer;
}

template<uint32 MaxBufSize>
typename OSCL_wStackString<MaxBufSize>::chartype* OSCL_wStackString<MaxBufSize>::get_str() const
{
    return (chartype*)rep.buffer;
}

// **************************************************************
template<uint32 MaxBufSize>
void OSCL_StackString<MaxBufSize>::create()
{
    rep.buffer = &buffer[0];
    rep.maxsize = MaxBufSize;
}

template<uint32 MaxBufSize>
void OSCL_wStackString<MaxBufSize>::create()
{
    rep.buffer = &buffer[0];
    rep.maxsize = MaxBufSize;
}

// **************************************************************
template<uint32 MaxBufSize>
OSCL_StackString<MaxBufSize>::OSCL_StackString()
{
    create();
    set_rep(NULL);
}

template<uint32 MaxBufSize>
OSCL_wStackString<MaxBufSize>::OSCL_wStackString()
{
    create();
    set_rep(NULL);
}

// **************************************************************
template<uint32 MaxBufSize>
OSCL_StackString<MaxBufSize>::OSCL_StackString(const chartype* cp)
{
    create();
    set_rep(cp);
}

template<uint32 MaxBufSize>
OSCL_wStackString<MaxBufSize>::OSCL_wStackString(const chartype* cp)
{
    create();
    set_rep(cp);
}

// **************************************************************
template<uint32 MaxBufSize>
void OSCL_StackString<MaxBufSize>::set(const chartype* cp, uint32 length)
{
    rep.set(cp, length);
    //just in case input string is shorter than 'length';
    rep.size = oscl_strlen(get_cstr());
}

template<uint32 MaxBufSize>
void OSCL_wStackString<MaxBufSize>::set(const chartype* cp, uint32 length)
{
    rep.set(cp, length);
    //just in case input string is shorter than 'length';
    rep.size = oscl_strlen(get_cstr());
}

// **************************************************************
template<uint32 MaxBufSize>
void OSCL_StackString<MaxBufSize>::set(const other_chartype* buf, optype op)
{
    rep.size = setrep_to_char(buf, oscl_strlen(buf), op, NULL);
}

template<uint32 MaxBufSize>
void OSCL_wStackString<MaxBufSize>::set(const other_chartype* buf, optype op)
{
    rep.size = setrep_to_wide_char(buf, oscl_strlen(buf), op, NULL);
}

// **************************************************************
template<uint32 MaxBufSize>
void OSCL_StackString<MaxBufSize>::set(const other_chartype* buf, uint32 length, optype op)
{
    rep.size = setrep_to_char(buf, length, op, NULL);
}

template<uint32 MaxBufSize>
void OSCL_wStackString<MaxBufSize>::set(const other_chartype* buf, uint32 length, optype op)
{
    rep.size = setrep_to_wide_char(buf, length, op, NULL);
}

// **************************************************************
template<uint32 MaxBufSize>
OSCL_StackString<MaxBufSize>::OSCL_StackString(const chartype* cp, uint32 length)
{
    create();
    set(cp, length);
}

template<uint32 MaxBufSize>
OSCL_wStackString<MaxBufSize>::OSCL_wStackString(const chartype* cp, uint32 length)
{
    create();
    set(cp, length);
}

// **************************************************************
template<uint32 MaxBufSize>
OSCL_StackString<MaxBufSize>::OSCL_StackString(const OSCL_StackString<MaxBufSize>& src) : OSCL_String(src)
{
    create();
    set_rep(src);
}

template<uint32 MaxBufSize>
OSCL_wStackString<MaxBufSize>::OSCL_wStackString(const OSCL_wStackString<MaxBufSize>& src)
{
    create();
    set_rep(src);
}

// **************************************************************
template<uint32 MaxBufSize>
OSCL_StackString<MaxBufSize>::OSCL_StackString(const OSCL_String& src)
{
    create();
    set_rep(src);
}

template<uint32 MaxBufSize>
OSCL_wStackString<MaxBufSize>::OSCL_wStackString(const OSCL_wString& src)
{
    create();
    set_rep(src);
}

// **************************************************************
template<uint32 MaxBufSize>
OSCL_StackString<MaxBufSize>::~OSCL_StackString()
{}

template<uint32 MaxBufSize>
OSCL_wStackString<MaxBufSize>::~OSCL_wStackString()
{}

// **************************************************************
template<uint32 MaxBufSize>
OSCL_StackString<MaxBufSize>& OSCL_StackString<MaxBufSize>::operator=(const OSCL_StackString<MaxBufSize>& src)
{
    set_rep(src);
    return (*this);
}

template<uint32 MaxBufSize>
OSCL_wStackString<MaxBufSize>& OSCL_wStackString<MaxBufSize>::operator=(const OSCL_wStackString<MaxBufSize>& src)
{
    set_rep(src);
    return (*this);
}

// **************************************************************
template<uint32 MaxBufSize>
OSCL_StackString<MaxBufSize>& OSCL_StackString<MaxBufSize>::operator=(const OSCL_String & src)
{
    set_rep(src);
    return (*this);
}

template<uint32 MaxBufSize>
OSCL_wStackString<MaxBufSize>& OSCL_wStackString<MaxBufSize>::operator=(const OSCL_wString & src)
{
    set_rep(src);
    return (*this);
}

// **************************************************************
template<uint32 MaxBufSize>
OSCL_StackString<MaxBufSize>& OSCL_StackString<MaxBufSize>::operator=(const chartype * cp)
{
    set_rep(cp);
    return (*this);
}

template<uint32 MaxBufSize>
OSCL_wStackString<MaxBufSize>& OSCL_wStackString<MaxBufSize>::operator=(const chartype * cp)
{
    set_rep(cp);
    return (*this);
}

#endif   // OSCL_STRING_H_INCLUDED

/*! @} */
