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

//                     O S C L _ F I L E  _ FIND

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclio OSCL IO
 *
 * @{
 */


/*! \file oscl_file_find.h
    \brief The file oscl_file_find.h defines the  class Oscl_FileFind

*/

#ifndef OSCL_FILE_FIND_H_INCLUDED
#define OSCL_FILE_FIND_H_INCLUDED

#ifndef OSCLCONFIG_IO_H_INCLUDED
#include "osclconfig_io.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

#ifndef OSCL_FILE_TYPES_H_INCLUDED
#include "oscl_file_types.h"
#endif

/**
 * Oscl_FileFind class defines the generic way of finding filesystem elements that match a pattern  within a directory
 */

class Oscl_FileFind
{
    public:

        /**
         * Finds first element matching the pattern.
         *
         * @param directory directory to search (utf8).
         * @param pattern wildcard pattern filter (utf8).  passing NULL, results in a universal match.
         * @param buf buffer for returned pathname (utf8).
         * @param buflen size in wide characters of buf.  If buf is not large enough to hold the returned string, NULL is returned, and GetLastError is set to E_BUFFER_TOO_SMALL.
         *
         * @return returns a pointer to buffer supplied, which contains the pathname of the first found element, or NULL otherwise.On a NULL return value, GetLastError() returns a more detailed error.
         */
        OSCL_IMPORT_REF const char *FindFirst(const char *directory, const char *pattern, char *buf, uint32 buflen);
        /**
         * Opens a directory for reading.
         *
         * @param directory directory to search (utf16).
         * @param pattern wildcard pattern filter (utf16).  passing NULL, results in a universal match.
         * @param buf buffer for returned pathname (utf16).
         * @param buflen size in wide characters of buf.  If buf is not large enough to hold the returned string, NULL is returned, and GetLastError is set to E_BUFFER_TOO_SMALL.
         *
         * @return returns a pointer to buffer supplied, which contains the pathname of the first found element, or NULL otherwise.  On a NULL return value, GetLastError() returns a more detailed error.
         */
        OSCL_IMPORT_REF const oscl_wchar *FindFirst(const oscl_wchar *directory, const oscl_wchar *pattern, oscl_wchar *buf, uint32 buflen);
        /**
         * Reads the next element in the directory.
         * Note: the pointer returned by this function is not persistent and
         *   should be stored.  Its scope is limited to the lifetime of the class.
         * @param buf buffer to hold directory name(utf8)
         * @param buflen size in wide characters of buf.  If buf is not large enough to hold the returned string, NULL is returned, and GetLastError is set to E_BUFFER_TOO_SMALL.
         * @return returns a pointer to buffer supplied, which contains the pathname of the next found element, or NULL otherwise.  On a NULL return value, GetLastError() returns a more detailed error.
         */
        OSCL_IMPORT_REF char *FindNext(char *buf, uint32 buflen);
        /**
         * Reads the next element in a directory.
         * Note: the pointer returned by this function is not persistent and
         *   should be stored.  Its scope is limited to the lifetime of the class.
         * @param buf buffer to hold directory name(utf16)
         * @param buflen size in wide characters of buf.  If buf is not large enough to hold the returned string, NULL is returned, and GetLastError is set to E_BUFFER_TOO_SMALL.
         * @return returns a pointer to buffer supplied, which contains the pathname of the next found element, or NULL otherwise.  On a NULL return value, GetLastError() returns a more detailed error.
         */
        OSCL_IMPORT_REF oscl_wchar *FindNext(oscl_wchar *buf, uint32 buflen);
        /**
         * closes the handle to directory.
         * @return none
         */
        OSCL_IMPORT_REF void Close();

        typedef enum
        {
            E_OK = 0,
            E_INVALID_STATE,
            E_INVALID_ARG,
            E_PATH_TOO_LONG,
            E_PATH_NOT_FOUND,
            E_NO_MATCH,
            E_BUFFER_TOO_SMALL,
            E_NOT_IMPLEMENTED,
            E_MEMORY_ERROR,
            E_OTHER
        } error_type;

        typedef enum
        {
            FILE_TYPE = 0, /* file */
            DIR_TYPE,      /* directory */
            INVALID_TYPE   /* no element available */
        } element_type;

        /**
         * Returns the element type for the last element returned
         * @return see enumeration above for more info.
         */
        OSCL_IMPORT_REF element_type GetElementType();


        /**
         * Returns the error code for the last operation.
         * @return see enumeration above for more info.
         */
        OSCL_IMPORT_REF error_type GetLastError();

        /**
         * constructor.
         * @return none
         */
        OSCL_IMPORT_REF Oscl_FileFind();
        /**
         * destructor.  will deallocate open handles if necessary
         *
         * @return none
         */
        OSCL_IMPORT_REF ~Oscl_FileFind();

    private:

        typedef char chartype;
        bool setpathanddelimiter(const chartype* directory);

#if   (OSCL_HAS_GLOB)
        glob_t hFind;
#else
        Oscl_Vector<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> iDirEntVec;
#endif
        uint32 count;
        bool foundfirst;
        error_type lastError;
        element_type type;
        bool appendPathDelimiter;
        chartype* pathname;
        const chartype* delimeter;
        const chartype* nullchar;
};


#endif // OSCL_FILE_FIND_H_INCLUDED

/*! @} */

