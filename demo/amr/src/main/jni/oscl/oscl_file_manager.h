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

//                     O S C L _ F I L E _ M A N A G E R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclio OSCL IO
 *
 * @{
 */


/*! \file oscl_file_manager.h
    \brief File management class

*/

#ifndef OSCL_FILE_MANAGER_H_INCLUDED
#define OSCL_FILE_MANAGER_H_INCLUDED

#include "oscl_base.h"

class OsclFileManager
{
    public:
        typedef enum
        {
            OSCL_FILE_ATTRIBUTE_READONLY        =       0x00000001,
            /*
            The file or directory is read-only. Applications can read the file but cannot write to it or delete it.
            For a directory, applications cannot delete it.
            */
            OSCL_FILE_ATTRIBUTE_HIDDEN          =       0x00000002,
            /*
            The file or directory is hidden. It is not included in an ordinary directory listing.
            */
            OSCL_FILE_ATTRIBUTE_SYSTEM          =       0x00000004,
            /*
            The file or directory is part of the operating system, or is used exclusively by the operating system.
            */
            OSCL_FILE_ATTRIBUTE_DIRECTORY       =       0x00000010,
            /*
            The handle identifies a directory.
            */
            OSCL_FILE_ATTRIBUTE_ARCHIVE         =       0x00000020,
            /*
            The file or directory is an archive file. Applications use this attribute to mark files for backup or removal.
            */
            OSCL_FILE_ATTRIBUTE_NORMAL            =       0x00000080,
            /*
            The file or directory does not have another attributes set. This attribute is valid only if used alone.
            */
            //OSCL_FILE_ATTRIBUTE_TEMPORARY     =       0x00000100,
            /*
            The file is being used for temporary storage. File systems avoid writing data back to mass storage
            if sufficient cache memory is available, because often the application deletes the temporary file
            shortly after the handle is closed. In that case, the system can entirely avoid writing the data.
            Otherwise, the data is written after the handle is closed.
            */
            //OSCL_FILE_ATTRIBUTE_SPARSE_FILE       =       0x00000200,
            /*
            The file is a sparse file.
            */
            //OSCL_FILE_ATTRIBUTE_REPARSE_POINT =       0x00000400,
            /*
            The file or directory has an associated reparse point.
            */
            //OSCL_FILE_ATTRIBUTE_COMPRESSED        =       0x00000800,
            /*
            The file or directory is compressed.
            For a file, this means that all of the data in the file is compressed.
            For a directory, this means that compression is the default for newly created files and subdirectories.
            */

            //OSCL_FILE_ATTRIBUTE_OFFLINE           =       0x00001000,
            /*
            The data of the file is not immediately available. This attribute indicates that the file data has been
            physically moved to offline storage.This attribute is used by Remote Storage, the hierarchical storage
            management software. Applications should not arbitrarily change this attribute.
            */
            //OSCL_FILE_ATTRIBUTE_ENCRYPTED     =       0x00004000,
            /*
            The file or directory is encrypted. For a file, this means that all data in the file is encrypted.
            For a directory, this means that encryption is the default for newly created files and subdirectories.
            */
            //OSCL_FILE_ATTRIBUTE_VIRTUAL           =       0x00010000
            /*
            A file is a virtual file.
            */

        } OSCL_FILE_ATTRIBUTE_TYPE;

        /**
         * OsclGetFileSize utility function provides the file size.
         * For directory, this value is undefined.
         * creation time
         * @param [in] wide character path; the full path of the file or directory
         * @param [out] file size in bytes
         *
         * @return true if successful, otherwise false.
         */
        OSCL_IMPORT_REF static bool OsclGetFileSize(const oscl_wchar* aFileName, uint64& aFileSize);

        /**
         * OsclGetFileSize utility function provides the file size.
         * For directory, this value is undefined.
         * @param [in] character path; the full path of the file or directory
         * @param [out] file size in bytes.
         *
         * @return true if successful, otherwise false.
         */
        OSCL_IMPORT_REF static bool OsclGetFileSize(const char* aFileName, uint64& aFileSize);

        /**
         * OsclGetFileCreationTime utility function provides the file (or directory)
         * creation time
         * @note On symbian platform, this api returns last modified time.
         * @param [in] wide character path; the full path of the file or directory
         * @param [out] creation time in microseconds
         *
         * @return true if successful, otherwise false.
         */
        OSCL_IMPORT_REF static bool OsclGetFileCreationTime(const oscl_wchar* aFileName, uint64& aFileCreationTime);

        /**
         * OsclGetFileCreationTime utility function provides the file (or directory)
         * creation time
         * @note On symbian platform, this api returns last modified time.
         * @param [in] character path; the full path of the file or directory
         * @param [out] creation time in microseconds.
         *
         * @return true if successful, otherwise false.
         */
        OSCL_IMPORT_REF static bool OsclGetFileCreationTime(const char* aFileName, uint64& aFileCreationTime);

        /**
         * OsclGetFileLastAccessTime utility function provides the file (or directory)
         * last access time, which might be different from last modified time.
         * @note On symbian platform, this api returns last modified time.
         * @param [in] wide character path; the full path of the file or directory
         * @param [out] Last access time in microseconds
         *
         * @return true if successful, otherwise false.
         */

        OSCL_IMPORT_REF static bool OsclGetFileLastAccessTime(const oscl_wchar* aFileName, uint64& aFileLastAccessTime);
        /**
         * OsclGetFileLastAccessTime utility function provides the file (or directory)
         * last access time, which might be different from last modified time.
         * @note On symbian platform, this api returns last modified time.
         * @param [in] character path; the full path of the file or directory
         * @param [out] Last access time in microseconds.
         *
         * @return true if successful, otherwise false.
         */
        OSCL_IMPORT_REF static bool OsclGetFileLastAccessTime(const char* aFileName, uint64& aFileLastAccessTime);

        /**
         * OsclGetFileLastWriteTime utility function provides the file (or directory)
         * last modified time.
         * @param [in] wide character path; the full path of the file or directory
         * @param [out] last modified time in microseconds
         *
         * @return true if successful, otherwise false.
         */
        OSCL_IMPORT_REF static bool OsclGetFileLastWriteTime(const oscl_wchar* aFileName, uint64& aFileLastWriteTime);

        /**
         * OsclGetFileLastWriteTime utility function provides the file (or directory)
         * last modified time.
         * @param [in] character path; the full path of the file or directory
         * @param [out] last modified time in microseconds
         *
         * @return true if successful, otherwise false.
         */
        OSCL_IMPORT_REF static bool OsclGetFileLastWriteTime(const char* aFileName, uint64& aFileLastWriteTime);

        /**
         * OsclGetFileAttributes utility function provides the various attributes
         * of file (or directory) like if it is hidden, read only etc. The uint32 value is
         * to be interpreted as per the enum OSCL_FILE_ATTRIBUTE_TYPE defined in oscl_file_manager.h
         * @param [in] wide character path; the full path of the file or directory
         * @param [out] file attributes.
         *
         * @return true if successful, otherwise false.
         */
        OSCL_IMPORT_REF static bool OsclGetFileAttributes(const oscl_wchar* aFileName, uint32& aFileAttributes);

        /**
         * OsclGetFileAttributes utility function provides the various attributes
         * of file (or directory) like if it is hidden, read only etc. The uint32 value is
         * to be interpreted as per the enum OSCL_FILE_ATTRIBUTE_TYPE defined in oscl_file_manager.h
         * @param [in] character path; the full path of the file or directory
         * @param [out] file attributes.
         *
         * @return true if successful, otherwise false.
         */
        OSCL_IMPORT_REF static bool OsclGetFileAttributes(const char* aFileName, uint32& aFileAttributes);
        /**
        * OsclExtractFilenameFromFullpath utility function provide the FileName From Path of a file.
        * @param [in] character path; the full path of the file or directory
        * @param [out] character FileName :file Name .It is assigned a pointer to file name in path itself.
        *
        * @return void for all condition
        */
        OSCL_IMPORT_REF static void OsclExtractFilenameFromFullpath(const char* aPath, char* &aFileName);
        OSCL_IMPORT_REF static void OsclExtractFilenameFromFullpath(const oscl_wchar* aPath, oscl_wchar* &aFileName);

#endif // OSCL_FILE_MANAGER_H_INCLUDED
        /*! @} */

};
