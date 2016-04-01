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

//                     O S C L _ F I L E  _ TYPES

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclio OSCL IO
 *
 * @{
 */


/*! \file oscl_file_types.h
    \brief The file oscl_file_types.h defines some constants and types
    for file I/O implementations.  Anything that needs to be shared
    across implementation modules can go here.

*/

#ifndef OSCL_FILE_TYPES_H_INCLUDED
#define OSCL_FILE_TYPES_H_INCLUDED


#define OSCL_IO_FILENAME_MAXLEN 512
#define OSCL_IO_EXTENSION_MAXLEN 512

#define OSCL_FILE_WCHAR_PATH_DELIMITER _STRLIT("/")
#define OSCL_FILE_CHAR_PATH_DELIMITER _STRLIT_CHAR("/")

//a class for passing configuration options from Oscl_File to OsclNativeFile
class OsclNativeFileParams
{
    public:
        OsclNativeFileParams(uint32 mode = 0, uint32 bufsize = 0, uint32 asyncsize = 0)
                : iNativeAccessMode(mode)
                , iNativeBufferSize(bufsize)
                , iAsyncReadBufferSize(asyncsize)
        {}
        uint32 iNativeAccessMode;
        uint32 iNativeBufferSize;
        uint32 iAsyncReadBufferSize;
};


#endif // OSCL_FILE_TYPES_H_INCLUDED

/*! @} */

