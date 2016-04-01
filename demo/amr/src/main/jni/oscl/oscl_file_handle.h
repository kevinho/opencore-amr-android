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

//                     O S C L _ F I L E  _ HANDLE

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclio OSCL IO
 *
 * @{
 */


/*! \file oscl_file_handle.h
    \brief The file oscl_file_handle.h defines the  class OsclFileHandle

*/

#ifndef OSCL_FILE_HANDLE_H_INCLUDED
#define OSCL_FILE_HANDLE_H_INCLUDED

#ifndef OSCLCONFIG_IO_H_INCLUDED
#include "osclconfig_io.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif


/**
** TOsclFileHandle is an OS-native file handle type.
*  With a class-based file API such as Symbian, a class ref is used
*  as a file handle.
*  For most ANSI-style file APIs, a file pointer is used as a
*  file handle.
*/
typedef FILE* TOsclFileHandle;

/**
** OsclFileHandle is a container for a handle to a previously-opened file.
*/
class OsclFileHandle
{
    public:
        OsclFileHandle(TOsclFileHandle aHandle)
                : iHandle(aHandle)
        {}
        OsclFileHandle(const OsclFileHandle& aHandle)
                : iHandle(aHandle.iHandle)
        {}
        TOsclFileHandle Handle()const
        {
            return iHandle;
        }
    private:
        TOsclFileHandle iHandle;
        friend class Oscl_File;
};


#endif // OSCL_FILE_HANDLE_H_INCLUDED

/*! @} */

