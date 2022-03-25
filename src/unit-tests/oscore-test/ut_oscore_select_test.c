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

/*================================================================================*
** File:  ut_oscore_select_test.c
** Owner: Chris Knight
** Date:  March 2020
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_select_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

#define UT_SELECT_FN "/cf/select_test.tmp"

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

static osal_id_t selecttest_fd;
static osal_id_t invalid_fd;

/*--------------------------------------------------------------------------------*
** External function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

char *fsAddrPtr = NULL;
void  UT_os_select_setup_file(void)
{
    UT_SETUP(OS_mkfs(fsAddrPtr, "/ramdev3", "RAM3", OSAL_SIZE_C(512), OSAL_BLOCKCOUNT_C(20)));
    UT_SETUP(OS_mount("/ramdev3", "/drive3"));
    UT_SETUP(OS_OpenCreate(&selecttest_fd, "/drive3/select_test.txt", OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE,
                           OS_READ_WRITE));

    /* create a bad ID by flipping the bits of a good ID */
    invalid_fd = OS_ObjectIdFromInteger(OS_ObjectIdToInteger(selecttest_fd) ^ 0xFFFFFFFF);
}

void UT_os_select_teardown_file(void)
{
    OS_close(selecttest_fd);
    OS_remove("/drive3/select_test.txt");
    OS_unmount("/drive3");
    OS_rmfs("/ramdev3");
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_SelectFdZero, OS_SelectFdAdd, OS_SelectFdClear, OS_SelectFdIsSet
** Purpose: Configure file descriptor set for select
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_select_fd_test(void)
{
    OS_FdSet FdSet;

    if (!UT_NOMINAL_OR_NOTIMPL(OS_SelectFdZero(&FdSet)))
    {
        return;
    }

    UT_RETVAL(OS_SelectFdZero(NULL), OS_INVALID_POINTER);
    UT_RETVAL(OS_SelectFdAdd(NULL, selecttest_fd), OS_INVALID_POINTER);
    UT_RETVAL(OS_SelectFdClear(NULL, selecttest_fd), OS_INVALID_POINTER);
    UT_RETVAL(OS_SelectFdIsSet(NULL, selecttest_fd), false);

    UT_RETVAL(OS_SelectFdAdd(&FdSet, invalid_fd), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_SelectFdAdd(&FdSet, OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_SelectFdClear(&FdSet, invalid_fd), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_SelectFdClear(&FdSet, OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_SelectFdIsSet(&FdSet, invalid_fd), false);
    UT_RETVAL(OS_SelectFdIsSet(&FdSet, OS_OBJECT_ID_UNDEFINED), false);

    UT_NOMINAL(OS_SelectFdZero(&FdSet));
    UT_NOMINAL(OS_SelectFdAdd(&FdSet, selecttest_fd));
    UT_RETVAL(OS_SelectFdIsSet(&FdSet, selecttest_fd), true);

    UT_NOMINAL(OS_SelectFdZero(&FdSet));
    UT_NOMINAL(OS_SelectFdAdd(&FdSet, selecttest_fd));
    UT_NOMINAL(OS_SelectFdClear(&FdSet, selecttest_fd));
    UT_RETVAL(OS_SelectFdIsSet(&FdSet, selecttest_fd), false);
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_SelectSingle(uint32 objid, uint32 *StateFlags, int32 msecs);
** Purpose: Select on a single file descriptor
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_select_single_test(void)
{
    uint32 StateFlags;

    UT_RETVAL(OS_SelectSingle(selecttest_fd, NULL, 0), OS_INVALID_POINTER);

    StateFlags = OS_STREAM_STATE_WRITABLE;
    if (!UT_NOMINAL_OR_NOTIMPL(OS_SelectSingle(selecttest_fd, &StateFlags, 0)))
    {
        return;
    }

    UtAssert_True((StateFlags & OS_STREAM_STATE_WRITABLE) != 0, "StateFlags (0x%x) & OS_STREAM_STATE_WRITABLE",
                  (unsigned int)StateFlags);

    StateFlags = OS_STREAM_STATE_READABLE;
    UT_NOMINAL(OS_SelectSingle(selecttest_fd, &StateFlags, 1));

    UtAssert_True((StateFlags & OS_STREAM_STATE_READABLE) != 0, "StateFlags (0x%x) & OS_STREAM_STATE_READABLE",
                  (unsigned int)StateFlags);

    UT_RETVAL(OS_SelectSingle(invalid_fd, &StateFlags, 0), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_SelectSingle(OS_OBJECT_ID_UNDEFINED, &StateFlags, 0), OS_ERR_INVALID_ID);
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_SelectMultiple(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs)
** Purpose: Select on a multiple file descriptors
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_select_multi_test(void)
{
    OS_FdSet ReadSet, WriteSet;

    UT_SETUP(OS_SelectFdZero(&WriteSet));
    UT_SETUP(OS_SelectFdAdd(&WriteSet, selecttest_fd));
    if (!UT_NOMINAL_OR_NOTIMPL(OS_SelectMultiple(NULL, &WriteSet, 1)))
    {
        return;
    }

    UtAssert_True(OS_SelectFdIsSet(&WriteSet, selecttest_fd), "OS_SelectFdIsSet(&WriteSet, selecttest_fd)");

    UT_SETUP(OS_SelectFdZero(&ReadSet));
    UT_SETUP(OS_SelectFdAdd(&ReadSet, selecttest_fd));
    UT_NOMINAL(OS_SelectMultiple(&ReadSet, NULL, 1));

    UtAssert_True(OS_SelectFdIsSet(&ReadSet, selecttest_fd), "!OS_SelectFdIsSet(&ReadSet, selecttest_fd)");

    /* empty set */
    UT_RETVAL(OS_SelectMultiple(NULL, NULL, 1), OS_ERR_INVALID_ID);
}

/*================================================================================*
** End of File: ut_oscore_queue_test.c
**================================================================================*/
