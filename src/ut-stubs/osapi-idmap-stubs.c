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
 * @file
 *
 * Auto-Generated stub implementations for functions defined in osapi-idmap header
 */

#include "osapi-idmap.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_ConvertToArrayIndex(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_ForEachObject(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_ForEachObjectOfType(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_GetResourceName(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_IdentifyObject(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_ObjectIdToArrayIndex(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ConvertToArrayIndex()
 * ----------------------------------------------------
 */
int32 OS_ConvertToArrayIndex(osal_id_t object_id, osal_index_t *ArrayIndex)
{
    UT_GenStub_SetupReturnBuffer(OS_ConvertToArrayIndex, int32);

    UT_GenStub_AddParam(OS_ConvertToArrayIndex, osal_id_t, object_id);
    UT_GenStub_AddParam(OS_ConvertToArrayIndex, osal_index_t *, ArrayIndex);

    UT_GenStub_Execute(OS_ConvertToArrayIndex, Basic, UT_DefaultHandler_OS_ConvertToArrayIndex);

    return UT_GenStub_GetReturnValue(OS_ConvertToArrayIndex, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ForEachObject()
 * ----------------------------------------------------
 */
void OS_ForEachObject(osal_id_t creator_id, OS_ArgCallback_t callback_ptr, void *callback_arg)
{
    UT_GenStub_AddParam(OS_ForEachObject, osal_id_t, creator_id);
    UT_GenStub_AddParam(OS_ForEachObject, OS_ArgCallback_t, callback_ptr);
    UT_GenStub_AddParam(OS_ForEachObject, void *, callback_arg);

    UT_GenStub_Execute(OS_ForEachObject, Basic, UT_DefaultHandler_OS_ForEachObject);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ForEachObjectOfType()
 * ----------------------------------------------------
 */
void OS_ForEachObjectOfType(osal_objtype_t objtype, osal_id_t creator_id, OS_ArgCallback_t callback_ptr,
                            void *callback_arg)
{
    UT_GenStub_AddParam(OS_ForEachObjectOfType, osal_objtype_t, objtype);
    UT_GenStub_AddParam(OS_ForEachObjectOfType, osal_id_t, creator_id);
    UT_GenStub_AddParam(OS_ForEachObjectOfType, OS_ArgCallback_t, callback_ptr);
    UT_GenStub_AddParam(OS_ForEachObjectOfType, void *, callback_arg);

    UT_GenStub_Execute(OS_ForEachObjectOfType, Basic, UT_DefaultHandler_OS_ForEachObjectOfType);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GetResourceName()
 * ----------------------------------------------------
 */
int32 OS_GetResourceName(osal_id_t object_id, char *buffer, size_t buffer_size)
{
    UT_GenStub_SetupReturnBuffer(OS_GetResourceName, int32);

    UT_GenStub_AddParam(OS_GetResourceName, osal_id_t, object_id);
    UT_GenStub_AddParam(OS_GetResourceName, char *, buffer);
    UT_GenStub_AddParam(OS_GetResourceName, size_t, buffer_size);

    UT_GenStub_Execute(OS_GetResourceName, Basic, UT_DefaultHandler_OS_GetResourceName);

    return UT_GenStub_GetReturnValue(OS_GetResourceName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_IdentifyObject()
 * ----------------------------------------------------
 */
osal_objtype_t OS_IdentifyObject(osal_id_t object_id)
{
    UT_GenStub_SetupReturnBuffer(OS_IdentifyObject, osal_objtype_t);

    UT_GenStub_AddParam(OS_IdentifyObject, osal_id_t, object_id);

    UT_GenStub_Execute(OS_IdentifyObject, Basic, UT_DefaultHandler_OS_IdentifyObject);

    return UT_GenStub_GetReturnValue(OS_IdentifyObject, osal_objtype_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ObjectIdToArrayIndex()
 * ----------------------------------------------------
 */
int32 OS_ObjectIdToArrayIndex(osal_objtype_t idtype, osal_id_t object_id, osal_index_t *ArrayIndex)
{
    UT_GenStub_SetupReturnBuffer(OS_ObjectIdToArrayIndex, int32);

    UT_GenStub_AddParam(OS_ObjectIdToArrayIndex, osal_objtype_t, idtype);
    UT_GenStub_AddParam(OS_ObjectIdToArrayIndex, osal_id_t, object_id);
    UT_GenStub_AddParam(OS_ObjectIdToArrayIndex, osal_index_t *, ArrayIndex);

    UT_GenStub_Execute(OS_ObjectIdToArrayIndex, Basic, UT_DefaultHandler_OS_ObjectIdToArrayIndex);

    return UT_GenStub_GetReturnValue(OS_ObjectIdToArrayIndex, int32);
}
