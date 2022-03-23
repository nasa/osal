/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-dir header
 */

#include "os-shared-dir.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_DirClose_Impl()
 * ----------------------------------------------------
 */
int32 OS_DirClose_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_DirClose_Impl, int32);

    UT_GenStub_AddParam(OS_DirClose_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_DirClose_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_DirClose_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_DirCreate_Impl()
 * ----------------------------------------------------
 */
int32 OS_DirCreate_Impl(const char *local_path, uint32 access)
{
    UT_GenStub_SetupReturnBuffer(OS_DirCreate_Impl, int32);

    UT_GenStub_AddParam(OS_DirCreate_Impl, const char *, local_path);
    UT_GenStub_AddParam(OS_DirCreate_Impl, uint32, access);

    UT_GenStub_Execute(OS_DirCreate_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_DirCreate_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_DirOpen_Impl()
 * ----------------------------------------------------
 */
int32 OS_DirOpen_Impl(const OS_object_token_t *token, const char *local_path)
{
    UT_GenStub_SetupReturnBuffer(OS_DirOpen_Impl, int32);

    UT_GenStub_AddParam(OS_DirOpen_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_DirOpen_Impl, const char *, local_path);

    UT_GenStub_Execute(OS_DirOpen_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_DirOpen_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_DirRead_Impl()
 * ----------------------------------------------------
 */
int32 OS_DirRead_Impl(const OS_object_token_t *token, os_dirent_t *dirent)
{
    UT_GenStub_SetupReturnBuffer(OS_DirRead_Impl, int32);

    UT_GenStub_AddParam(OS_DirRead_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_DirRead_Impl, os_dirent_t *, dirent);

    UT_GenStub_Execute(OS_DirRead_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_DirRead_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_DirRemove_Impl()
 * ----------------------------------------------------
 */
int32 OS_DirRemove_Impl(const char *local_path)
{
    UT_GenStub_SetupReturnBuffer(OS_DirRemove_Impl, int32);

    UT_GenStub_AddParam(OS_DirRemove_Impl, const char *, local_path);

    UT_GenStub_Execute(OS_DirRemove_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_DirRemove_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_DirRewind_Impl()
 * ----------------------------------------------------
 */
int32 OS_DirRewind_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_DirRewind_Impl, int32);

    UT_GenStub_AddParam(OS_DirRewind_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_DirRewind_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_DirRewind_Impl, int32);
}
