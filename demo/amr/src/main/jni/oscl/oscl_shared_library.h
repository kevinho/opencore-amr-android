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
#ifndef OSCL_SHARED_LIBRARY_H_INCLUDED
#define OSCL_SHARED_LIBRARY_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

#ifndef OSCL_SHARED_LIB_INTERFACE_H_INCLUDED
#include "oscl_shared_lib_interface.h"
#endif

#ifndef OSCL_LIBRARY_COMMON_H_INCLUDED
#include "oscl_library_common.h"
#endif

struct OsclUuid;
class PVLogger;

/**
 * OsclSharedLibrary gives access to shared libraries in the user specified location
 **/
class OsclSharedLibrary
{
    public:

        /**
         * Default object Constructor function
         **/

        OSCL_IMPORT_REF OsclSharedLibrary();

        /**
        * Constructor that accepts a path to the library to load. The library
        * isn't loaded at this point. It is only loaded after a call to LoadLib.
        *
        * @param aPath path name of a specific library to load
        */
        OSCL_IMPORT_REF OsclSharedLibrary(const OSCL_String& aPath);

        /**
         * Object destructor function
         **/
        OSCL_IMPORT_REF ~OsclSharedLibrary();

        /**
        * Attempts to load the library specified by the pathname.
        *
        * @param aPath path name of a specific library to load
        * @returns Returns some status information about whether the library loaded.
        */
        OSCL_IMPORT_REF OsclLibStatus LoadLib(const OSCL_String& aPath);

        /**
        * Attempts to load the library specified by the stored pathname (as set
        * in the constructor or SetLibPath).
        *
        * @returns Returns some status information about whether the library loaded.
        */
        OSCL_IMPORT_REF OsclLibStatus LoadLib();

        /**
        * Sets iLibPath to the pathname provided
        *
        * @param aPath path name of a specific library to load
        */
        OSCL_IMPORT_REF void SetLibPath(const OSCL_String& aPath);

        /**
         * Query for the instance of a particular interface based on the request interface ID
         *
         * @param aInterfaceId ID
         *
         * @param aInterfacePtr - output parameter filled in with the requested interface
         *                        pointer or NULL if the interface pointer is not supported.
         * @returns OsclLibStatus information about query for the interface
         **/
        OSCL_IMPORT_REF OsclLibStatus QueryInterface(const OsclUuid& aInterfaceId,
                OsclAny*& aInterfacePtr);

        /**
         * Close the library. The library will be closed only when there are no live references
         * @returns success or fail in case of closing the library
         **/
        OSCL_IMPORT_REF OsclLibStatus Close();

        /**
         * Increment the reference count for tracking number of live references of node
         * by the shared lib instance.
         **/
        OSCL_IMPORT_REF void AddRef();

        /**
         * Decrement the reference count when done with live reference node
         * by the shared lib instance.
         **/
        OSCL_IMPORT_REF void RemoveRef();

    private:
        OsclLibStatus loadlibrary(const OSCL_String& alib);

        PVLogger* ipLogger;
        int32 iRefCount;
        OsclSharedLibraryInterface *pSharedLibInterface;
#if OSCL_LIBRARY_PERF_LOGGING
        PVLogger* iDiagnosticsLogger;
        uint32 iLibCount;
        uint32 iLibLoadTime;
        uint32 delta;
#endif
        void* ipHandle;
        OSCL_HeapString<OsclMemAllocator> iLibPath;
        bool iLoaded;
};

#include "oscl_vector.h"

/*
** OsclSharedLibraryList is a handy class for locating and using libraries that support
** a given interface.
*/
class OsclSharedLibraryList
{
    public:
        OSCL_IMPORT_REF OsclSharedLibraryList();
        OSCL_IMPORT_REF ~OsclSharedLibraryList();

        /*
        ** Give a config file directory, locate all the libraries that support the given
        ** interface ID.  For each library, create a shared library object for it and add
        ** it to the list.
        */
        OSCL_IMPORT_REF void Populate(const OSCL_String& aPath, const OsclUuid& aInterfaceId);

        /*
        ** Return the size of the list
        */
        uint32 Size()
        {
            return iList.size();
        }

        /*
        ** Query interface for the given library.  This will load the library if needed.
        */
        OSCL_IMPORT_REF OsclLibStatus QueryInterfaceAt(uint32 aIndex, OsclAny*& aInterfacePtr);

        /*
        ** Close all open libraries and clear the list.
        */
        OSCL_IMPORT_REF void CloseAll();

    private:
        Oscl_Vector<OsclSharedLibrary*, OsclMemAllocator> iList;
        OsclUuid iInterfaceId;
        PVLogger* iLogger;
};

#endif //OSCL_SHARED_LIBRARY_H_INCLUDED

