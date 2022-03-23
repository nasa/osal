/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-filesys header
 */

#include "os-shared-filesys.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSysFilterFree()
 * ----------------------------------------------------
 */
bool OS_FileSysFilterFree(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSysFilterFree, bool);

    UT_GenStub_AddParam(OS_FileSysFilterFree, void *, ref);
    UT_GenStub_AddParam(OS_FileSysFilterFree, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_FileSysFilterFree, const OS_common_record_t *, obj);

    UT_GenStub_Execute(OS_FileSysFilterFree, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileSysFilterFree, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSys_FindVirtMountPoint()
 * ----------------------------------------------------
 */
bool OS_FileSys_FindVirtMountPoint(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSys_FindVirtMountPoint, bool);

    UT_GenStub_AddParam(OS_FileSys_FindVirtMountPoint, void *, ref);
    UT_GenStub_AddParam(OS_FileSys_FindVirtMountPoint, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_FileSys_FindVirtMountPoint, const OS_common_record_t *, obj);

    UT_GenStub_Execute(OS_FileSys_FindVirtMountPoint, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileSys_FindVirtMountPoint, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSys_Initialize()
 * ----------------------------------------------------
 */
int32 OS_FileSys_Initialize(char *address, const char *fsdevname, const char *fsvolname, size_t blocksize,
                            osal_blockcount_t numblocks, bool should_format)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSys_Initialize, int32);

    UT_GenStub_AddParam(OS_FileSys_Initialize, char *, address);
    UT_GenStub_AddParam(OS_FileSys_Initialize, const char *, fsdevname);
    UT_GenStub_AddParam(OS_FileSys_Initialize, const char *, fsvolname);
    UT_GenStub_AddParam(OS_FileSys_Initialize, size_t, blocksize);
    UT_GenStub_AddParam(OS_FileSys_Initialize, osal_blockcount_t, numblocks);
    UT_GenStub_AddParam(OS_FileSys_Initialize, bool, should_format);

    UT_GenStub_Execute(OS_FileSys_Initialize, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileSys_Initialize, int32);
}
