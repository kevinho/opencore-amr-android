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

//                     O S C L _ F I L E  _ Dir _ utils

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclio OSCL IO
 *
 * @{
 */


/*! \file oscl_file_dir_utils.h
    \brief The file oscl_file_dir_utils.h defines some unix-style directory ops

*/

#ifndef OSCL_FILE_DIR_UTILS_H_INCLUDED
#define OSCL_FILE_DIR_UTILS_H_INCLUDED

#ifndef OSCLCONFIG_IO_H_INCLUDED
#include "osclconfig_io.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif



typedef struct oscl_fsstat
{
    uint64 freebytes;
    uint64 totalbytes;
} OSCL_FSSTAT;

typedef enum
{
    OSCL_FILEMGMT_PERMS_READ = 0x1,
    OSCL_FILEMGMT_PERMS_WRITE = 0x2,
    OSCL_FILEMGMT_PERMS_EXECUTE = 0x4,
} OSCL_FILEMGMT_PERMS;

typedef enum
{
    OSCL_FILEMGMT_MODE_DIR = 0x1
} OSCL_FILEMGMT_MODES;

typedef struct oscl_stat_buf
{
    uint32 mode;
    uint32 perms;
} OSCL_STAT_BUF;

typedef enum
{
    OSCL_FILEMGMT_E_OK = 0,
    OSCL_FILEMGMT_E_PATH_TOO_LONG,
    OSCL_FILEMGMT_E_PATH_NOT_FOUND,
    OSCL_FILEMGMT_E_ALREADY_EXISTS,
    OSCL_FILEMGMT_E_NOT_EMPTY,
    OSCL_FILEMGMT_E_PERMISSION_DENIED,
    OSCL_FILEMGMT_E_NO_MATCH,
    OSCL_FILEMGMT_E_UNKNOWN,
    OSCL_FILEMGMT_E_SYS_SPECIFIC,
    OSCL_FILEMGMT_E_NOT_IMPLEMENTED
} OSCL_FILEMGMT_ERR_TYPE;

/**
 * oscl_getcwd function can be used to determine the full path name of the
 * current directory.
 * @param pointer to wide character buffer to receive the current directory
 * @param size size of buffer in wide characters
 * @return OSCL_FILEMGMT_ERR_TYPE, see enumeration for this type.
 */
OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_getcwd(oscl_wchar *path, uint32 size);


/**
 * oscl_getcwd function can be used to determine the full path name of the
 * current directory.
 * @param pointer to character buffer to receive the current directory
 * @param size size of buffer in characters
 * @return OSCL_FILEMGMT_ERR_TYPE, see enumeration for this type.
 */
OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_getcwd(char *path, uint32 size);



/**
 * oscl_stat function can be used to determine the attributes of a file
 * in addition to whether the file exists or not
 * @param wide character path the full path of the file to stat.
 * @return OSCL_FILEMGMT_ERR_TYPE, see enumeration for this type.
 */
OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_stat(const oscl_wchar *path, OSCL_STAT_BUF *statbuf);


/**
 * oscl_stat function can be used to determine the attributes of a file
 * in addition to whether the file exists or not
 * @param character path the full path of the file to stat.
 * @return OSCL_FILEMGMT_ERR_TYPE, see enumeration for this type.
 */
OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_stat(const char *path, OSCL_STAT_BUF *statbuf);


/**
 * oscl_mkdir function creates a directory in the path given
 * @param wide character path the full path of the directory to create.  if parts
 *   of the path do not exist the function will fail
 * @return OSCL_FILEMGMT_ERR_TYPE, see enumeration for this type.
 */
OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_mkdir(const oscl_wchar *path);

/**
 * oscl_mkdir function creates a directory in the path given
 * @param character path the full path of the directory to create.  if parts
 *   of the path do not exist the function will fail
 * @return OSCL_FILEMGMT_ERR_TYPE, see enumeration for this type.
 */
OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_mkdir(const char *path);

/**
 * oscl_rmdir function removes and empty directory in the path given
 * @param wide character path the full path of the directory to remove.
 * @return OSCL_FILEMGMT_ERR_TYPE, see enumeration for this type.
 */
OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_rmdir(const oscl_wchar *path);

/**
 * oscl_rmdir removes an empty directory in the path given
 * @param character path the full path of the directory to remove.
 * @return OSCL_FILEMGMT_ERR_TYPE, see enumeration for this type.
 */
OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_rmdir(const char *path);

/**
 * oscl_chdir changes the current directory to the path given
 * @param wide character path the full path of the directory to change to.
 * @return OSCL_FILEMGMT_ERR_TYPE, see enumeration for this type.
 */
OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_chdir(const oscl_wchar *path);

/**
 * oscl_chdir changes the current directory to the path given
 * @param character path the full path of the directory to change to.
 * @return OSCL_FILEMGMT_ERR_TYPE, see enumeration for this type.
 */
OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_chdir(const char *path);

/**
 * oscl_rename function renames a file or directory
 * @param wide character path the full path of the file or directory
 *  to rename.
 * @param wide character path the full path the new name for the directory
 *
 * @return OSCL_FILEMGMT_ERR_TYPE, see enumeration for this type.
 */

OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_rename(const oscl_wchar *oldpath, const oscl_wchar *newpath);

/**
 * oscl_rmdir removes an empty directory in the path given
 * @param character path the full path of the directory to remove.
 * @return OSCL_FILEMGMT_ERR_TYPE, see enumeration for this type.
 */

OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_rename(const char *oldpath, const char *newpath);

/**
 * Oscl_StatFS function populates a general structure describing free space available on a filesystem
 * @param stats pointer to structure to hold information
 * @param path located in desired filesystem (utf8)
 * Note: If the OS does not support a particular field in the structure, it is set to -1.
 * @return OSCL_FILEMGMT_ERR_TYPE, see enumeration for this type.
 */
OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_statfs(OSCL_FSSTAT *stats, const char *path);
/**
 * Oscl_StatFS function populates a general structure describing free space available on a filesystem
 * @param stats pointer to structure to hold information
 * @param path located in desired filesystem (utf8)
 * Note: If the OS does not support a particular field in the structure, it is set to -1.
 * @return OSCL_FILEMGMT_ERR_TYPE, see enumeration for this type.
 */
OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_statfs(OSCL_FSSTAT *stats, const oscl_wchar *path);


#endif // OSCL_FILE_DIR_UTILS_H_INCLUDED

/*! @} */

