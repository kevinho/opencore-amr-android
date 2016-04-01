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

//                     O S C L _ F I L E  _ I O

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclio OSCL IO
 *
 * @{
 */


/*! \file oscl_file_io.h
    \brief The file oscl_file_io.h defines the class Oscl_File.  This is the
    public API to the basic file I/O operations.
*/



#ifndef OSCL_FILE_IO_H_INCLUDED
#define OSCL_FILE_IO_H_INCLUDED

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

/**
 * Oscl_File_IO class defines the generic way of creating all the file object for all the platforms.
 * The class defines all the input output files operations like open, close, read, write, seek, tell
 * that will be common over various Operating Systems we are supporting.
 * It stores an file pointer that is used to access the file, a cache reference depending whether
 * cache is supported by the OS
 */
class PVLogger;
class OsclFileCache;
class OsclFileCacheBuffer;
class Oscl_FileServer;
class OsclFileHandle;
class OsclNativeFile;
class OsclFileStats;
class OsclNativeFileParams;
class OsclAsyncFile;
#define TOsclFileOffsetInt32 int32

class Oscl_File : public HeapBase
{
    public:

        typedef enum
        {
            /**
             * Beginning of file
             */
            SEEKSET,
            /**
             * Current position of file pointer
             */
            SEEKCUR,
            /**
             * End of file
             */
            SEEKEND
        } seek_type;

        typedef enum
        {
            /**
             * Opens a file for reading.  The file must exist.
             */
            MODE_READ =      0x0001,
            /**
             * Opens the file for reading and writing.  If the file exists,
             * its contents will be overwritten unless APPEND mode is
             * specified.  If the file does not exist, it will be created.
             */
            MODE_READWRITE = 0x0002,
            /**
             * Specifies all write operations to occur at the end of the
             * file.  The file pointer can be moved with the Seek command,
             * but will always be moved to the end of the file for write
             * commands.
             */
            MODE_APPEND =    0x0004,
            /**
             * Opens the file in 'binary' mode.  This is the default.
             */
            MODE_BINARY =    0x0008,
            /**
             * Opens the file in 'text' mode.  The default mode is 'binary'.
             */
            MODE_TEXT =      0x0010,
            /**
            * Open a file for reading and writing. The file must exist.
            * The default mode is 'binary'.
            */
            MODE_READ_PLUS =  0x0020

        } mode_type;

        /**
         * Constructor
         */
        OSCL_IMPORT_REF Oscl_File();

        /**
         * Deprecated Constructor, present for back-compatibility.
         * @param aCacheSize: sets native buffer size, and when pv cache is enabled,
         *    also sets pv cache size.
         */
        OSCL_IMPORT_REF Oscl_File(uint32 aCacheSize);
        /**
         * Deprecated Constructor, present for back-compatibility.
         * @param aCacheSize: sets native buffer size, and when pv cache is enabled,
         *    also sets pv cache size.
         * @param aFileHandle: open file handle.
         */
        OSCL_IMPORT_REF Oscl_File(uint32 aCacheSize, OsclFileHandle* aFileHandle);

        /**
         * Destructor
         */
        OSCL_IMPORT_REF ~Oscl_File();

        /**
         * SetPVCacheSize configures the read/write cache.
         *
         *   This should be called before opening the file.  If used when
         *   the file is open, the option will not take effect until the
         *   next Open.
         *
         * @param aSize: cache size in bytes.  Zero disables the cache.
         */
        OSCL_IMPORT_REF void SetPVCacheSize(uint32 aSize);

        /**
        * Parameters for defining a fixed cache
        */
        class OsclFixedCacheParam
        {
            public:
                /*
                ** File position where the fixed cache is located.
                */
                TOsclFileOffset iFilePosition;
                /*
                ** Fixed cache size in bytes.
                */
                uint32 iSize;

                bool Contains(TOsclFileOffset pos) const
                {
                    return (iFilePosition <= pos && pos < iFilePosition + (TOsclFileOffset)iSize);
                }
        };

        /**
         * AddFixedCache adds a fixed cache.  The fixed cache will
         *    be used on the next opportunity.
         *    The fixed cache must not overlap with any other fixed cache.
         *
         * @param aParam: Cache location and size.
         */
        void AddFixedCache(const OsclFixedCacheParam& aParam)
        {
            iAddFixedCache.push_back(aParam);
        }

        /**
         * RemoveFixedCache removes a fixed cache.
         *
         * @param aPos: Cache location and size.
         */
        void RemoveFixedCache(const TOsclFileOffset &aPos)
        {
            iRemoveFixedCache.push_back(aPos);
        }

        /**
        * For defining a cache observer.  Cache observer
        * can implement customized cache schemes by replacing the
        * SetCachePosition routine.
        */
        class OsclCacheObserver
        {
            public:
                virtual OsclFileCacheBuffer* ChooseCurCache(OsclFileCache& aContext, TOsclFileOffset aPos) = 0;
        };
        void SetCacheObserver(OsclCacheObserver* aObs)
        {
            iCacheObserver = aObs;
        }

        /**
         * SetNativeAccessMode allows switching between different native file access
         *  modes, when available.
         *
         *  Note: for For Symbian, use the TSymbianAccessMode values to choose the mode.
         *   If multiple access modes are not available on the platform, this call will
         *   have no effect.
         *
         * @param aMode: access mode.
         */
        OSCL_IMPORT_REF void SetNativeAccessMode(uint32 aMode);

        /**
         *  Defines mode options for SetNativeAccessMode on Symbian.
         */
        enum TSymbianAccessMode
        {
            ESymbianAccessMode_Rfile = 0
            , ESymbianAccessMode_RfileBuf = 1
        };

        /**
         * SetNativeBufferSize configures the native file buffering feature,
         *   when available.
         *
         *   This should be called before opening the file.  If used when
         *   the file is open, the option will not take effect until the
         *   next Open.
         *
         *   Note: For Symbian, this sets the RFileBuf cache size.
         *     If native buffing is not available on the platform, this call
         *     will have no effect.
         *
         * @param aSize: native buffer size in bytes.  Zero disables the feature.
         */
        OSCL_IMPORT_REF void SetNativeBufferSize(int32 aSize);

        /**
         * SetAsyncReadBufferSize configures the asynchronous background
         *   read function.  May not be available on all platforms.
         *
         *   This should be called before opening the file.  If used when
         *   the file is open, the option will not take effect until the
         *   next Open.
         *
         *   Note: if asynchronous read is not available on the platform,
         *     this call will have no effect.
         *
         * @param aSize: buffer size in bytes.  Zero disables the feature.
         */
        OSCL_IMPORT_REF void SetAsyncReadBufferSize(uint32 aSize);

        /**
         * SetFileHandle adds an open file handle to the Oscl_File object.
         *   The Oscl_File object will use that handle to access the file.
         *
         *   This call is not available when the Oscl_File object is already
         *   open.
         *
         *   Note: This feature is used in Symbian with the MMF framework.
         *   The MMF framework provides an open RFile handle to access content.
         *   When using RFileBuf access mode with an RFile handle, the RFileBuf
         *   will be attached to the open RFile handle.
         *
         *   To use the external file handle, the caller starts with a native file handle to an open file.  The caller must
         *   wrap the native file handle in an OsclFileHandle object, pass the OsclFileHandle pointer to SetFileHandle,
         *   call Oscl_File::Open, then proceed to use the Oscl_File object, finally calling Oscl_File::Close.
         *   In this usage mode, Oscl_File::Open and Oscl_File::Close do not actually call native file open and close.
         *   It is assumed that the caller will close the original native file handle after usage is complete.
         *
         * @param aHandle: container for an open file handle.
         * @return returns 0 if successful, non-zero if error.
         *
         */
        OSCL_IMPORT_REF int32 SetFileHandle(OsclFileHandle* aHandle);

        /**
         * Opens a file.
         *
         * Note: when an external file handle is used, Open will attach to the file
         *  handle and initialize cacheing features, but will not do a native file open.
         *
         *
         * @param filename name of file to open (Utf8)
         * @param mode     combination of open mode flags
         * @param fileserv fileserver to use
         *
         * @return returns 0 if successful and a non-zero value otherwise
         */
        OSCL_IMPORT_REF int32 Open(const char *filename, uint32 mode, Oscl_FileServer& fileserv);

        /**
         * Opens a file.
         *
         * Note: when an external file handle is used, Open will attach to the file
         *  handle and initialize cacheing features, but will not do a native file open.
         *
         * @param filename name of file to open (Unicode)
         * @param mode     combination of open mode flags
         * @param fileserv fileserver to use
         *
         * @return returns 0 if successful and a non-zero value otherwise
         */
        OSCL_IMPORT_REF int32 Open(const oscl_wchar *filename, uint32 mode, Oscl_FileServer& fileserv);

        /**
         * The File Read operation
         * Reads from the file into the buffer a maximum of 'numelements'
         * of size 'size'.
         *
         * @param buffer pointer to buffer of type void
         * @param size   element size in bytes
         * @param numelements
         *               max number of elements to read
         *
         * @return returns the number of full elements actually read, which
         *         may be less than count if an error occurs or if the end
         *         of the file is encountered before reaching count. Use the
         *         CheckEndOfFile or GetError function to distinguish a read
         *         error from an end-of-file condition.
         */
        OSCL_IMPORT_REF uint32 Read(OsclAny *buffer,
                                    uint32 size,
                                    uint32 numelements);

        /**
         * The File Write operation
         * Writes from the buffer 'numelements' objects of size 'size'
         *
         * @param buffer pointer to buffer of type void
         * @param size   element size in bytes
         * @param numelements
         *               number of elements to write
         *
         * @return The number of elements written
         */
        OSCL_IMPORT_REF uint32 Write(const OsclAny *buffer,
                                     uint32 size,
                                     uint32 numelements);


        /**
         * The File Seek operation
         * Sets the position for file pointer
         *
         * @param offset offset from the specified origin.
         * @param origin starting point
         *
         * @return returns 0 on success, and a non-zero value otherwise
         */
        OSCL_IMPORT_REF int32 Seek(TOsclFileOffset offset,
                                   seek_type origin);

        /**
         * The File Tell operation
         * Returns the current file position for file specified by fp
         */
        OSCL_IMPORT_REF TOsclFileOffset Tell();


        /**
         * The File Close operation
         * Closes the file after flushing any remaining data in the
         * buffers.
         *
         * Note: If the file object was opened with an external file handle,
         *  then Close will simply flush the file.  The file will remain open.
         *
         * @return returns 0 if successful, and a non-zero value otherwise
         */
        OSCL_IMPORT_REF int32 Close();


        /**
         * The File Flush operation
         * On an output stream OSCL_FileFlush causes any buffered
         * but unwritten data to be written to the file.
         *
         * @return returns 0 if successful, and a non-zero value otherwise
         */
        OSCL_IMPORT_REF int32 Flush();

        /**
        * The File SetSize operation
        * If the file has been opened for writing
        * This sets the size of the file.  The file pointer position
        * is unchanged unless the pointers position is greated than
        * the new filesize.
        *
        * @return returns 0 if successful, and a non-zero value otherwise
        */
        OSCL_IMPORT_REF int32 SetSize(uint32 size);

        /**
         * The File EOF(end of file) operation
         * returns a nonzero value after the first read operation
         * that attempts to read past the end of the file
         *
         * @return
         */
        OSCL_IMPORT_REF int32 EndOfFile();


        /**
         * The File Error operation
         * If no error has occurred on stream, returns 0. Otherwise,
         * it returns a nonzero value
         *
         * @return
         */
        OSCL_IMPORT_REF int32 GetError();

        /**
         * Retrieve the file handle.
         *
         * @return file handle
         */
        OsclFileHandle* Handle()
        {
            return iOpenFileHandle;
        }

        /**
         * Get the file size in bytes.
         *
         * @return - The size of the file, or -1 on error.
         */
        OSCL_IMPORT_REF TOsclFileOffset Size();

        /**
         * SetLoggingEnable configures the PVLogger output for this file.
         *   This will enable full logging of each API entry and
         *   exit using the logger object "Oscl_File", plus full logging
         *   of native operation entry & exit using logger object "OsclNativeFile".
         *
         * @param aEnable: true to enable, false to disable logging.
         */
        OSCL_IMPORT_REF void SetLoggingEnable(bool aEnable);

        /**
         * SetSummaryStatsLoggingEnable configures the PVLogger output for this file.
         *   This will enable summary statistics logging only, using the logger
         *   object "OsclFileStats".
         *
         * @param aEnable: true to enable, false to disable stats logging.
         */
        OSCL_IMPORT_REF void SetSummaryStatsLoggingEnable(bool aEnable);


    private:
        friend class OsclFileCache;
        friend class OsclFileCacheBuffer;
        friend class asyncfilereadwrite_test;
        friend class largeasyncfilereadwrite_test;
        friend class asyncfilereadcancel_test;

        void Construct();
        void OldCacheDefaults();
        void OldCacheSelect(uint32);

        //state info
        bool iIsOpen;

        //For Logging
        PVLogger* iLogger;
        PVLogger* iStatsLogger;
        PVLogger* iNativeLogger;
        PVLogger* iAsyncLogger;
        OsclFileStats* iFileStats;
        void CreateFileStats();

        //External file handle.
        OsclFileHandle* iOpenFileHandle;

        //For PV File Cache
        uint32 iPVCacheSize;
        OsclFileCache* iFileCache;
        Oscl_Vector<OsclFixedCacheParam, OsclMemAllocator> iAddFixedCache;
        Oscl_Vector<TOsclFileOffset, OsclMemAllocator> iRemoveFixedCache;
        OsclCacheObserver* iCacheObserver;

        int32 OpenFileCacheOrAsyncBuffer(const char *filename
                                         , const oscl_wchar* wfilename
                                         , uint32 mode
                                         , const OsclNativeFileParams& params
                                         , Oscl_FileServer& fileserv) ;

        //For native file I/O.
        OsclNativeFile* iNativeFile;
        int32 iNativeBufferSize;
        uint32 iNativeAccessMode;

        //For async read feature
        uint32 iAsyncReadBufferSize;
        OsclAsyncFile* iAsyncFile;

        // These are wrappers to call the Native OS File IO calls with
        // stats and logging.

        int32  CallNativeOpen(const OsclFileHandle&, uint32 mode
                              , const OsclNativeFileParams& params
                              , Oscl_FileServer& fileserv);
        int32  CallNativeOpen(const oscl_wchar *filename, uint32 mode
                              , const OsclNativeFileParams& params
                              , Oscl_FileServer& fileserv);
        int32  CallNativeOpen(const char *filename, uint32 mode
                              , const OsclNativeFileParams& params
                              , Oscl_FileServer& fileserv);
        uint32 CallNativeRead(OsclAny *buffer, uint32 size, uint32 numelements);
        uint32 CallNativeWrite(const OsclAny *buffer, uint32 size, uint32 numelements);
        int32  CallNativeSeek(TOsclFileOffset offset, Oscl_File::seek_type origin);
        int32  CallNativeSetSize(uint32 size);
        TOsclFileOffset  CallNativeTell();
        int32  CallNativeFlush();
        int32  CallNativeEndOfFile();
        TOsclFileOffset CallNativeSize();
        int32 CallNativeClose();
        uint32 CallNativeMode();
        int32 CallNativeGetError();
        OSCL_IMPORT_REF uint32 GetAsyncFileNumOfRun();
        OSCL_IMPORT_REF uint32 GetAsyncFileNumOfRunError();

};


//Include all public file apis for convenience here.
#include "oscl_file_server.h"
#include "oscl_file_find.h"
#include "oscl_file_dir_utils.h"
#include "oscl_file_handle.h"

#endif // OSCL_FILE_IO_H_INCLUDED

/*! @} */

