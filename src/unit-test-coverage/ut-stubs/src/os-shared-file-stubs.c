/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-file header
 */

#include "os-shared-file.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileIteratorClose()
 * ----------------------------------------------------
 */
int32 OS_FileIteratorClose(osal_id_t filedes, void *arg)
{
    UT_GenStub_SetupReturnBuffer(OS_FileIteratorClose, int32);

    UT_GenStub_AddParam(OS_FileIteratorClose, osal_id_t, filedes);
    UT_GenStub_AddParam(OS_FileIteratorClose, void *, arg);

    UT_GenStub_Execute(OS_FileIteratorClose, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileIteratorClose, int32);
}
