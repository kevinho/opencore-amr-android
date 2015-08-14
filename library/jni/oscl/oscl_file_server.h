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

//                     O S C L _ F I L E  _ SERVER

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclio OSCL IO
 *
 * @{
 */


/*! \file oscl_file_server.h
    \brief The file oscl_file_server.h defines the class Oscl_FileServer.
    This is the porting layer for file server implementations.
*/

#ifndef OSCL_FILE_SERVER_H_INCLUDED
#define OSCL_FILE_SERVER_H_INCLUDED

#ifndef OSCLCONFIG_IO_H_INCLUDED
#include "osclconfig_io.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

class Oscl_File;

class Oscl_FileServer
{
    public:
        /**
         * Constructor
         */
        OSCL_IMPORT_REF Oscl_FileServer();

        /**
         * Destructor
         */
        OSCL_IMPORT_REF ~Oscl_FileServer();

        /**
         * Connects the server.  This must be called before a file
         * server can be used.
         *
         * @return returns 0 on success and a non-zero value otherwise
         */
        OSCL_IMPORT_REF int32 Connect(bool aShareSession = false);

        /**
         * Closes a file server.
         *
         * @return returns 0 on success and a non-zero value otherwise
         */
        OSCL_IMPORT_REF int32 Close();

        /**
         * Deletes a file from the filesystem
         * * @param filename name of the file to delete (Utf8)
         *
         * @return returns 0 if successful, and a non-zero value otherwise.
         */
        OSCL_IMPORT_REF int32 Oscl_DeleteFile(const char *filename);

        /**
         * Deletes a file from the filesystem
         *
         * @param filename name of the file to delete (Unicode)
         *
         * @return returns 0 if successful, and a non-zero value otherwise.
         */
        OSCL_IMPORT_REF int32 Oscl_DeleteFile(const oscl_wchar *filename);


    protected:
        friend class Oscl_File;
        friend class OsclNativeFile;

        //file server objects (if appropriate)
};



#endif // OSCL_FILE_SERVER_H_INCLUDED

/*! @} */

