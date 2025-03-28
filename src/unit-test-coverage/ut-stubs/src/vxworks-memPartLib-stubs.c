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
 * Auto-Generated stub implementations for functions defined in OCS_memPartLib header
 */

#include "OCS_memPartLib.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OCS_memAddToPool()
 * ----------------------------------------------------
 */
void OCS_memAddToPool(char *pPool, unsigned int poolSize)
{
    UT_GenStub_AddParam(OCS_memAddToPool, char *, pPool);
    UT_GenStub_AddParam(OCS_memAddToPool, unsigned int, poolSize);

    UT_GenStub_Execute(OCS_memAddToPool, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OCS_memPartAddToPool()
 * ----------------------------------------------------
 */
OCS_STATUS OCS_memPartAddToPool(OCS_PART_ID partId, char *pPool, unsigned int poolSize)
{
    UT_GenStub_SetupReturnBuffer(OCS_memPartAddToPool, OCS_STATUS);

    UT_GenStub_AddParam(OCS_memPartAddToPool, OCS_PART_ID, partId);
    UT_GenStub_AddParam(OCS_memPartAddToPool, char *, pPool);
    UT_GenStub_AddParam(OCS_memPartAddToPool, unsigned int, poolSize);

    UT_GenStub_Execute(OCS_memPartAddToPool, Basic, NULL);

    return UT_GenStub_GetReturnValue(OCS_memPartAddToPool, OCS_STATUS);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OCS_memPartAlignedAlloc()
 * ----------------------------------------------------
 */
void *OCS_memPartAlignedAlloc(OCS_PART_ID partId, unsigned int nBytes, unsigned int alignment)
{
    UT_GenStub_SetupReturnBuffer(OCS_memPartAlignedAlloc, void *);

    UT_GenStub_AddParam(OCS_memPartAlignedAlloc, OCS_PART_ID, partId);
    UT_GenStub_AddParam(OCS_memPartAlignedAlloc, unsigned int, nBytes);
    UT_GenStub_AddParam(OCS_memPartAlignedAlloc, unsigned int, alignment);

    UT_GenStub_Execute(OCS_memPartAlignedAlloc, Basic, NULL);

    return UT_GenStub_GetReturnValue(OCS_memPartAlignedAlloc, void *);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OCS_memPartAlloc()
 * ----------------------------------------------------
 */
void *OCS_memPartAlloc(OCS_PART_ID partId, unsigned int nBytes)
{
    UT_GenStub_SetupReturnBuffer(OCS_memPartAlloc, void *);

    UT_GenStub_AddParam(OCS_memPartAlloc, OCS_PART_ID, partId);
    UT_GenStub_AddParam(OCS_memPartAlloc, unsigned int, nBytes);

    UT_GenStub_Execute(OCS_memPartAlloc, Basic, NULL);

    return UT_GenStub_GetReturnValue(OCS_memPartAlloc, void *);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OCS_memPartCreate()
 * ----------------------------------------------------
 */
OCS_PART_ID OCS_memPartCreate(char *pPool, unsigned int poolSize)
{
    UT_GenStub_SetupReturnBuffer(OCS_memPartCreate, OCS_PART_ID);

    UT_GenStub_AddParam(OCS_memPartCreate, char *, pPool);
    UT_GenStub_AddParam(OCS_memPartCreate, unsigned int, poolSize);

    UT_GenStub_Execute(OCS_memPartCreate, Basic, NULL);

    return UT_GenStub_GetReturnValue(OCS_memPartCreate, OCS_PART_ID);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OCS_memPartFree()
 * ----------------------------------------------------
 */
OCS_STATUS OCS_memPartFree(OCS_PART_ID partId, char *pBlock)
{
    UT_GenStub_SetupReturnBuffer(OCS_memPartFree, OCS_STATUS);

    UT_GenStub_AddParam(OCS_memPartFree, OCS_PART_ID, partId);
    UT_GenStub_AddParam(OCS_memPartFree, char *, pBlock);

    UT_GenStub_Execute(OCS_memPartFree, Basic, NULL);

    return UT_GenStub_GetReturnValue(OCS_memPartFree, OCS_STATUS);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OCS_memPartInfoGet()
 * ----------------------------------------------------
 */
OCS_STATUS OCS_memPartInfoGet(OCS_PART_ID partId, OCS_MEM_PART_STATS *ppartStats)
{
    UT_GenStub_SetupReturnBuffer(OCS_memPartInfoGet, OCS_STATUS);

    UT_GenStub_AddParam(OCS_memPartInfoGet, OCS_PART_ID, partId);
    UT_GenStub_AddParam(OCS_memPartInfoGet, OCS_MEM_PART_STATS *, ppartStats);

    UT_GenStub_Execute(OCS_memPartInfoGet, Basic, NULL);

    return UT_GenStub_GetReturnValue(OCS_memPartInfoGet, OCS_STATUS);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OCS_memPartShow()
 * ----------------------------------------------------
 */
OCS_STATUS OCS_memPartShow(OCS_PART_ID partId, int type)
{
    UT_GenStub_SetupReturnBuffer(OCS_memPartShow, OCS_STATUS);

    UT_GenStub_AddParam(OCS_memPartShow, OCS_PART_ID, partId);
    UT_GenStub_AddParam(OCS_memPartShow, int, type);

    UT_GenStub_Execute(OCS_memPartShow, Basic, NULL);

    return UT_GenStub_GetReturnValue(OCS_memPartShow, OCS_STATUS);
}
