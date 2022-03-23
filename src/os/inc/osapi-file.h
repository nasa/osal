/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * \file
 *
 * Declarations and prototypes for file objects
 */

#ifndef OSAPI_FILE_H
#define OSAPI_FILE_H

#include "osconfig.h"
#include "common_types.h"
#include "osapi-clock.h"

/** @defgroup OSFileAccess OSAL File Access Option Defines
 * @{
 */
#define OS_READ_ONLY  0 /**< Read only file access */
#define OS_WRITE_ONLY 1 /**< Write only file access */
#define OS_READ_WRITE 2 /**< Read write file access */
/**@}*/

/** @defgroup OSFileOffset OSAL Reference Point For Seek Offset Defines
 * @{
 */
#define OS_SEEK_SET 0 /**< Seek offset set */
#define OS_SEEK_CUR 1 /**< Seek offset current */
#define OS_SEEK_END 2 /**< Seek offset end */
/**@}*/

/** @brief OSAL file properties */
typedef struct
{
    char      Path[OS_MAX_PATH_LEN];
    osal_id_t User;
    uint8     IsValid; /* For backward compatibility -- always true if OS_FDGetInfo returned true */
} OS_file_prop_t;

/**
 * @brief File system status
 *
 * @note This used to be directly typedef'ed to the "struct stat" from the C library
 *
 * Some C libraries (glibc in particular) actually define member names to reference into
 * sub-structures, so attempting to reuse a name like "st_mtime" might not work.
 */
typedef struct
{
    uint32    FileModeBits;
    OS_time_t FileTime;
    size_t    FileSize;
} os_fstat_t;

/**
 * @brief File stat mode bits
 *
 * We must also define replacements for the stat structure's mode bits.
 * This is currently just a small subset since the OSAL just presents a very
 * simplified view of the filesystem to the upper layers.  And since not all
 * OS'es are POSIX, the more POSIX-specific bits are not relevant anyway.
 */
enum
{
    OS_FILESTAT_MODE_EXEC  = 0x00001,
    OS_FILESTAT_MODE_WRITE = 0x00002,
    OS_FILESTAT_MODE_READ  = 0x00004,
    OS_FILESTAT_MODE_DIR   = 0x10000
};

/** @brief Access file stat mode bits */
#define OS_FILESTAT_MODE(x) ((x).FileModeBits)
/** @brief File stat is directory logical */
#define OS_FILESTAT_ISDIR(x) ((x).FileModeBits & OS_FILESTAT_MODE_DIR)
/** @brief File stat is executable logical */
#define OS_FILESTAT_EXEC(x) ((x).FileModeBits & OS_FILESTAT_MODE_EXEC)
/** @brief File stat is write enabled logical */
#define OS_FILESTAT_WRITE(x) ((x).FileModeBits & OS_FILESTAT_MODE_WRITE)
/** @brief File stat is read enabled logical */
#define OS_FILESTAT_READ(x) ((x).FileModeBits & OS_FILESTAT_MODE_READ)
/** @brief Access file stat size field */
#define OS_FILESTAT_SIZE(x) ((x).FileSize)
/** @brief Access file stat time field as a whole number of seconds */
#define OS_FILESTAT_TIME(x) (OS_TimeGetTotalSeconds((x).FileTime))

/**
 * @brief Flags that can be used with opening of a file (bitmask)
 */
typedef enum
{
    OS_FILE_FLAG_NONE     = 0x00,
    OS_FILE_FLAG_CREATE   = 0x01,
    OS_FILE_FLAG_TRUNCATE = 0x02,
} OS_file_flag_t;

/*
 * Exported Functions
 */

/** @defgroup OSAPIFile OSAL Standard File APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Open or create a file
 *
 * Implements the same as OS_open/OS_creat but follows the OSAL paradigm
 * of outputting the ID/descriptor separately from the return value, rather
 * than relying on the user to convert it back.
 *
 * @param[out] filedes     The handle ID (OS_OBJECT_ID_UNDEFINED on failure) @nonnull
 * @param[in]  path        File name to create or open @nonnull
 * @param[in]  flags       The file permissions - see @ref OS_file_flag_t
 * @param[in]  access_mode Intended access mode - see @ref OSFileAccess
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if the command was not executed properly
 * @retval #OS_INVALID_POINTER if pointer argument was NULL
 * @retval #OS_ERR_NO_FREE_IDS if all available file handles are in use
 * @retval #OS_FS_ERR_NAME_TOO_LONG if the filename portion of the path exceeds OS_MAX_FILE_NAME
 * @retval #OS_FS_ERR_PATH_INVALID if the path argument is not valid
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the path argument exceeds OS_MAX_PATH_LEN

 */
int32 OS_OpenCreate(osal_id_t *filedes, const char *path, int32 flags, int32 access_mode);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Closes an open file handle
 *
 * This closes regular file handles and any other file-like resource, such as
 * network streams or pipes.
 *
 * @param[in] filedes   The handle ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 * @retval #OS_ERROR if an unexpected/unhandled error occurs @covtest
 */
int32 OS_close(osal_id_t filedes);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Read from a file handle
 *
 * Reads up to nbytes from a file, and puts them into buffer.
 *
 * If the file position is at the end of file (or beyond, if the OS allows) then this
 * function will return 0.
 *
 * @param[in]  filedes  The handle ID to operate on
 * @param[out] buffer   Storage location for file data @nonnull
 * @param[in]  nbytes   Maximum number of bytes to read @nonzero
 *
 * @note All OSAL error codes are negative int32 values.  Failure of this
 * call can be checked by testing if the result is less than 0.
 *
 * @return A non-negative byte count or appropriate error code, see @ref OSReturnCodes
 * @retval #OS_INVALID_POINTER if buffer is a null pointer
 * @retval #OS_ERR_INVALID_SIZE if the passed-in size is not valid
 * @retval #OS_ERROR if OS call failed @covtest
 * @retval #OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 * @retval 0 if at end of file/stream data
 */
int32 OS_read(osal_id_t filedes, void *buffer, size_t nbytes);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Write to a file handle
 *
 * Writes to a file. copies up to a maximum of nbytes of buffer to the file
 * described in filedes
 *
 * @param[in] filedes   The handle ID to operate on
 * @param[in] buffer    Source location for file data @nonnull
 * @param[in] nbytes    Maximum number of bytes to read @nonzero
 *
 * @note All OSAL error codes are negative int32 values.  Failure of this
 * call can be checked by testing if the result is less than 0.
 *
 * @return A non-negative byte count or appropriate error code, see @ref OSReturnCodes
 * @retval #OS_INVALID_POINTER if buffer is NULL
 * @retval #OS_ERR_INVALID_SIZE if the passed-in size is not valid
 * @retval #OS_ERROR if OS call failed @covtest
 * @retval #OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 * @retval 0 if file/stream cannot accept any more data
 */
int32 OS_write(osal_id_t filedes, const void *buffer, size_t nbytes);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief File/Stream input read with a timeout
 *
 * This implements a time-limited read and is primarily intended for use with
 * sockets but may also work with any other stream-like resource that the underlying
 * OS supports, such as pipes or special devices.
 *
 * If data is immediately available on the file/socket, this will return that data
 * along with the actual number of bytes that were immediately available.  It will
 * not block.
 *
 * If the file position is at the end of file or end of stream data (e.g. if the remote
 * end has closed the connection), then this function will immediately return 0 without
 * blocking for the timeout period.
 *
 * If no data is immediately available, but the underlying resource/stream is still
 * connected to a peer, this will wait up to the given timeout for additional
 * data to appear.  If no data appears within the timeout period, then this returns
 * the #OS_ERROR_TIMEOUT status code.  This allows the caller to differentiate
 * an open (but idle) socket connection from a connection which has been closed
 * by the remote peer.
 *
 * In all cases this will return successfully as soon as at least 1 byte of actual
 * data is available.  It will not attempt to read the entire input buffer.
 *
 * If an EOF condition occurs prior to timeout, this function returns zero.
 *
 * @param[in]  filedes   The handle ID to operate on
 * @param[out] buffer    Storage location for file data @nonnull
 * @param[in]  nbytes    Maximum number of bytes to read @nonzero
 * @param[in]  timeout   Maximum time to wait, in milliseconds (OS_PEND = forever)
 *
 * @returns Byte count on success or appropriate error code, see @ref OSReturnCodes
 * @retval #OS_ERROR_TIMEOUT if no data became available during timeout period
 * @retval #OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 * @retval #OS_ERR_INVALID_SIZE if the passed-in size is not valid
 * @retval #OS_INVALID_POINTER if the passed-in buffer is not valid
 * @retval 0 if at end of file/stream data
 */
int32 OS_TimedRead(osal_id_t filedes, void *buffer, size_t nbytes, int32 timeout);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief File/Stream output write with a timeout
 *
 * This implements a time-limited write and is primarily intended for use with
 * sockets but may also work with any other stream-like resource that the underlying
 * OS supports.
 *
 * If output buffer space is immediately available on the file/socket, this will
 * place data into the buffer and return the actual number of bytes that were
 * queued for output.  It will not block.
 *
 * If no output buffer space is immediately available, this will wait up to the
 * given timeout for space to become available.  If no space becomes available within
 * the timeout period, then this returns an error code (not zero).
 *
 * In all cases this will return successfully as soon as at least 1 byte of actual
 * data is output.  It will _not_ attempt to write the entire output buffer.
 *
 * If an EOF condition occurs prior to timeout, this function returns zero.
 *
 * @param[in] filedes   The handle ID to operate on
 * @param[in] buffer    Source location for file data @nonnull
 * @param[in] nbytes    Maximum number of bytes to read @nonzero
 * @param[in] timeout   Maximum time to wait, in milliseconds (OS_PEND = forever)
 *
 * @return A non-negative byte count or appropriate error code, see @ref OSReturnCodes
 * @retval #OS_ERROR_TIMEOUT if no data became available during timeout period
 * @retval #OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 * @retval #OS_ERR_INVALID_SIZE if the passed-in size is not valid
 * @retval #OS_INVALID_POINTER if the passed-in buffer is not valid
 * @retval 0 if file/stream cannot accept any more data
 */
int32 OS_TimedWrite(osal_id_t filedes, const void *buffer, size_t nbytes, int32 timeout);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Changes the permissions of a file
 *
 * @param[in] path        File to change @nonnull
 * @param[in] access_mode Desired access mode - see @ref OSFileAccess
 *
 * @note Some file systems do not implement permissions.  If the underlying OS
 *       does not support this operation, then #OS_ERR_NOT_IMPLEMENTED is returned.
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS @covtest
 * @retval #OS_ERR_NOT_IMPLEMENTED if the filesystem does not support this call
 * @retval #OS_INVALID_POINTER if the path argument is NULL
 */
int32 OS_chmod(const char *path, uint32 access_mode);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain information about a file or directory
 *
 * Returns information about a file or directory in an os_fstat_t structure
 *
 * @param[in]  path      The file to operate on @nonnull
 * @param[out] filestats Buffer to store file information @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if path or filestats is NULL
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the path is too long to be stored locally
 * @retval #OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long to be stored
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_ERROR if the OS call failed
 */
int32 OS_stat(const char *path, os_fstat_t *filestats);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Seeks to the specified position of an open file
 *
 * Sets the read/write pointer to a specific offset in a specific file.
 *
 * @param[in] filedes   The handle ID to operate on
 * @param[in] offset    The file offset to seek to
 * @param[in] whence    The reference point for offset, see @ref OSFileOffset
 *
 * @return Byte offset from the beginning of the file or appropriate error code,
           see @ref OSReturnCodes
 * @retval #OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 * @retval #OS_ERROR if OS call failed @covtest
 */
int32 OS_lseek(osal_id_t filedes, int32 offset, uint32 whence);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Removes a file from the file system
 *
 * Removes a given filename from the drive
 *
 * @note The behavior of this API on an open file is not defined at the OSAL level
 * due to dependencies on the underlying OS which may or may not allow the related
 * operation based on a variety of potential configurations.  For portability,
 * it is recommended that applications ensure the file is closed prior to removal.
 *
 * @param[in]  path      The file to operate on @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if there is no device or the driver returns error
 * @retval #OS_INVALID_POINTER if path is NULL
 * @retval #OS_FS_ERR_PATH_TOO_LONG if path is too long to be stored locally
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_FS_ERR_NAME_TOO_LONG if the name of the file to remove is too long
 */
int32 OS_remove(const char *path);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Renames a file
 *
 * Changes the name of a file, where the source and destination
 * reside on the same file system.
 *
 * @note The behavior of this API on an open file is not defined at the OSAL level
 * due to dependencies on the underlying OS which may or may not allow the related
 * operation based on a variety of potential configurations.  For portability,
 * it is recommended that applications ensure the file is closed prior to removal.
 *
 * @param[in]  old_filename      The original filename @nonnull
 * @param[in]  new_filename      The desired filename @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if the file could not be opened or renamed.
 * @retval #OS_INVALID_POINTER if old or new are NULL
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
 * @retval #OS_FS_ERR_NAME_TOO_LONG if the new name is too long to be stored locally
 */
int32 OS_rename(const char *old_filename, const char *new_filename);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Copies a single file from src to dest
 *
 * @note The behavior of this API on an open file is not defined at the OSAL level
 * due to dependencies on the underlying OS which may or may not allow the related
 * operation based on a variety of potential configurations.  For portability,
 * it is recommended that applications ensure the file is closed prior to removal.
 *
 * @param[in]  src       The source file to operate on @nonnull
 * @param[in]  dest      The destination file @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if the file could not be accessed
 * @retval #OS_INVALID_POINTER if src or dest are NULL
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
 * @retval #OS_FS_ERR_NAME_TOO_LONG if the dest name is too long to be stored locally
 */
int32 OS_cp(const char *src, const char *dest);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Move a single file from src to dest
 *
 * This first attempts to rename the file, which is faster if
 * the source and destination reside on the same file system.
 *
 * If this fails, it falls back to copying the file and removing
 * the original.
 *
 * @note The behavior of this API on an open file is not defined at the OSAL level
 * due to dependencies on the underlying OS which may or may not allow the related
 * operation based on a variety of potential configurations.  For portability,
 * it is recommended that applications ensure the file is closed prior to removal.
 *
 * @param[in]  src       The source file to operate on @nonnull
 * @param[in]  dest      The destination file @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if the file could not be renamed.
 * @retval #OS_INVALID_POINTER if src or dest are NULL
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
 * @retval #OS_FS_ERR_NAME_TOO_LONG if the dest name is too long to be stored locally
 */
int32 OS_mv(const char *src, const char *dest);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain information about an open file
 *
 * Copies the information of the given file descriptor into a structure passed in
 *
 * @param[in]  filedes  The handle ID to operate on
 * @param[out] fd_prop  Storage buffer for file information @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 * @retval #OS_INVALID_POINTER if the fd_prop argument is NULL
 */
int32 OS_FDGetInfo(osal_id_t filedes, OS_file_prop_t *fd_prop);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Checks to see if a file is open
 *
 * This function takes a filename and determines if the file is open. The function
 * will return success if the file is open.
 *
 * @param[in]  Filename      The file to operate on @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS if the file is open
 * @retval #OS_ERROR if the file is not open
 * @retval #OS_INVALID_POINTER if the filename argument is NULL
 */
int32 OS_FileOpenCheck(const char *Filename);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Close all open files
 *
 * Closes All open files that were opened through the OSAL
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR   if one or more file close returned an error @covtest
 */
int32 OS_CloseAllFiles(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Close a file by filename
 *
 * Allows a file to be closed by name.
 * This will only work if the name passed in is the same name used to open
 * the file.
 *
 * @param[in]  Filename      The file to close @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_FS_ERR_PATH_INVALID if the file is not found
 * @retval #OS_ERROR   if the file close returned an error @covtest
 * @retval #OS_INVALID_POINTER if the filename argument is NULL
 */
int32 OS_CloseFileByName(const char *Filename);
/**@}*/

#endif /* OSAPI_FILE_H */
