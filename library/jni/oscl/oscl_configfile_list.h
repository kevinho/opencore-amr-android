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
#ifndef OSCL_CONFIGFILE_LIST_H_INCLUDED
#define OSCL_CONFIGFILE_LIST_H_INCLUDED

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

#ifndef OSCL_LIBRARY_COMMON_H_INCLUDED
#include "oscl_library_common.h"
#endif

struct OsclUuid;
class PVLogger;

/**
* OsclConfigFileList contains the list of .cfg files found from a given directory
**/
class OsclConfigFileList
{
    public:
        enum SortType
        {
            ESortByName = 0,
            ENoSort
        };

        /**
         * Default object Constructor function
         **/
        OSCL_IMPORT_REF OsclConfigFileList();

        /**
         * Object destructor function
         **/
        OSCL_IMPORT_REF ~OsclConfigFileList();

        /**
         * Populate the list for the given directory.
         *
         * @param aConfigFileDir - directory path of the .cfg files; path can trail with or without delimiter (path prefix, e.g. '\')
         * @param aSortType - Define type type of sort, if any, for the config file list. Default is not sorted
         *
         * @returns OsclLibStatus about whether the config file vector got populated or not
         **/
        OSCL_IMPORT_REF OsclLibStatus Populate(const OSCL_String& aConfigFileDir,
                                               OsclConfigFileList::SortType aSortType = ENoSort);

        /**
        * Get the number of config file in the list.
        *
        * @returns Number of config file in list.
        */
        OSCL_IMPORT_REF uint32 Size();

        /**
         * Returns the n'th element.
         * @param n element position to return
         * @returns the reference to the config file name stored in the iCfgList vector
         */
        OSCL_IMPORT_REF const OSCL_String& GetConfigfileAt(uint32 n);


    private:
        Oscl_Vector<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> iCfgList;
        PVLogger* ipLogger;
#if OSCL_LIBRARY_PERF_LOGGING
        PVLogger* iDiagnosticsLogger;
        uint32 iCfgFileNum;
#endif

        /**
         * Sort iCfgList by name
         */
        void Sort(void);
};

/**
* OsclLibraryUtil is mainly used by test app to setup library search path
**/
class OsclLibraryUtil
{
    public:
        /**
         * static api sets, appends, or unset aDir to system PATH environment as the lib search folder
         *
         * @param aEnvName - environment name to be set or unset
         * @param aEnvValue - environment value of the aEnvName to be set or unset
         *
         * @returns OsclLibStatus
         **/
        OSCL_IMPORT_REF static OsclLibStatus SetupLibraryEnv(const char* aEnvName, const char* aEnvValue);
};

#endif //OSCL_LIBRARY_LIST_H_INCLUDED

