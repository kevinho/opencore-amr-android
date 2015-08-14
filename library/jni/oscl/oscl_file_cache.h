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

//                     O S C L _ F I L E  _ C A C H E

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclio OSCL IO
 *
 * @{
 */


/*! \file oscl_file_cache.h
    \brief The file oscl_file_cache.h defines the  class OsclFileCache

*/

#ifndef OSCL_FILE_CACHE_H_INCLUDED
#define OSCL_FILE_CACHE_H_INCLUDED

#ifndef OSCLCONFIG_IO_H_INCLUDED
#include "osclconfig_io.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#include "oscl_file_io.h"

class OsclFileCache;

class OsclFileCacheBuffer
{
    public:
        OsclFileCacheBuffer(): isFixed(false)
                , capacity(0)
                , usableSize(0)
                , pBuffer(NULL)
                , filePosition(0)
                , currentPos(0)
                , endPos(0)
                , updateStart(0)
                , updateEnd(0)
        {}

        OsclFileCache* iContainer;

        //Movable or fixed cache?
        bool isFixed;

        //Capacity of the cache buffer
        uint32 capacity;

        //Usable size of the cache buffer (may be l.t. its capacity)
        uint32 usableSize;

        //constant pointer to cache buffer
        uint8*    pBuffer;

        //the native file position corresponding to the start of the
        //cache
        TOsclFileOffset  filePosition;

        //current working position (virtual file pointer) in the cache.
        //units: 0-based byte offset from beginning of cache
        uint32    currentPos;

        //end of valid data in the cache.
        //units: 0-based byte offset from beginning of cache
        uint32    endPos;

        //variables to track the range of data in the cache that
        //has been updated by write operations, but has not yet
        //been written to disk.
        //units: 0-based byte offset from beginning of cache
        uint32    updateStart;
        uint32    updateEnd;

        int32 SetPosition(TOsclFileOffset  pos);
        int32 PrepRead();
        int32 PrepWrite();
        int32 WriteUpdatesToFile();
        int32 FillFromFile(uint32, uint32);

        bool IsUpdated()
        {
            return updateEnd > updateStart;
        }

        //return true if this cache contains the input position.
        bool Contains(TOsclFileOffset  pos)
        {
            return (filePosition <= pos && pos < filePosition + (TOsclFileOffset)usableSize);
        }
        //return true if this cache preceeds the input position.
        bool Preceeds(TOsclFileOffset  pos)
        {
            return (filePosition + (TOsclFileOffset)usableSize <= pos);
        }
};

class Oscl_File;

class OsclFileCache : public HeapBase
{
    public:
        OsclFileCache(Oscl_File& aContainer);
        ~OsclFileCache();

        int32 Open(uint32 mode, uint32 cache_size);

        void Close();

        uint32 Read(void* outputBuffer, uint32 size, uint32 numelements);

        uint32 Write(const void* inputBuffer, uint32 size, uint32 numelements);

        TOsclFileOffset  FileSize()
        {
            return _fileSize;
        }

        int32 Seek(TOsclFileOffset  offset, Oscl_File::seek_type origin);

        TOsclFileOffset  Tell()
        {
            return (_curCache) ? (_curCache->filePosition + _curCache->currentPos) : 0;
        }

        int32 Flush();

        int32 EndOfFile()
        {
            return (Tell() == FileSize()) ? 1 : 0;
        }

    private:
        friend class OsclFileCacheBuffer;
        Oscl_File& iContainer;

        //file mode from the Open call.
        uint32 _mode;

    public:
        OsclFileCacheBuffer _movableCache;
        Oscl_Vector<OsclFileCacheBuffer, OsclMemAllocator> _fixedCaches;
        OSCL_IMPORT_REF OsclFileCacheBuffer* AddFixedCache(const Oscl_File::OsclFixedCacheParam&);

    private:
        OsclFileCacheBuffer* _curCache;
        int32 SetCachePosition(TOsclFileOffset);
        int32 UpdateFixedCaches();

        //Current file size.  This is a virtual file size and
        //may not match the native file size when there is
        //cached data.
        TOsclFileOffset     _fileSize;

        //Current true native file position.
        TOsclFileOffset  _nativePosition;

        PVLogger* iLogger;
};


#endif // OSCL_FILE_CACHE_H_INCLUDED

/*! @} */

