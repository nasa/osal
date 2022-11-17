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
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-impl-symtab.h"
#include "os-shared-module.h"

#include <stdio.h>
#include <string.h> /* memset() */
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <errnoLib.h>
#include <sysLib.h>
#include <moduleLib.h>
#include <symLib.h>
#include <loadLib.h>
#include <unldLib.h>

typedef struct
{
    char    SymbolName[OS_MAX_SYM_LEN];
    cpuaddr SymbolAddress;
} SymbolRecord_t;

/* A global for storing the state in a SymbolDump call */
SymbolDumpState_t OS_VxWorks_SymbolDumpState;

/* the system symbol table */
extern SYMTAB_ID sysSymTbl;

/****************************************************************************************
                                SYMBOL TABLE API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GenericSymbolLookup_Impl(SYMTAB_ID SymTab, cpuaddr *SymbolAddress, const char *SymbolName)
{
    STATUS      vxStatus;
    SYMBOL_DESC SymDesc;

    /*
    ** Check parameters
    */
    if ((SymbolAddress == NULL) || (SymbolName == NULL))
    {
        return OS_INVALID_POINTER;
    }

    /*
    ** Lookup the entry point
    **
    ** VxWorks 6.9 has deprecated the "symFindByName" API and it is replaced
    ** with a "symFind" API instead.
    */

    memset(&SymDesc, 0, sizeof(SYMBOL_DESC));
    SymDesc.mask = SYM_FIND_BY_NAME;
    SymDesc.name = (char *)SymbolName;

    vxStatus       = symFind(SymTab, &SymDesc);
    *SymbolAddress = (cpuaddr)SymDesc.value;

    if (vxStatus == ERROR)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolLookup_Impl(cpuaddr *SymbolAddress, const char *SymbolName)
{
    return OS_GenericSymbolLookup_Impl(sysSymTbl, SymbolAddress, SymbolName);
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleSymbolLookup_Impl(const OS_object_token_t *token, cpuaddr *SymbolAddress, const char *SymbolName)
{
    /*
     * NOTE: this is currently exactly the same as OS_SymbolLookup_Impl().
     *
     * Ideally this should get a SYMTAB_ID from the MODULE_ID and search only
     * for the symbols provided by that module - but it is not clear if vxWorks
     * offers this capability.
     */
    return OS_GenericSymbolLookup_Impl(sysSymTbl, SymbolAddress, SymbolName);
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Function called by vxWorks to iterate the vxworks symbol table
 *
 * Parameters:
 *           name - The symbol name
 *           val  - The symbol address value
 *           type - The vxWorks symbol type ( not used )
 *           max_size - The maximum size of the file that is written to.
 *           group - The vxWorks symbol group ( not used )
 *
 *  Returns: true to tell vxWorks to continue to iterate the symbol table
 *           false to tell vxWorks to stop iterating the symbol table
 *
 *           The address of the symbol will be stored in the pointer that is passed in.
 *
 *-----------------------------------------------------------------*/
BOOL OS_SymTableIterator_Impl(char *name, SYM_VALUE val, SYM_TYPE type, _Vx_usr_arg_t arg, SYM_GROUP group)
{
    SymbolRecord_t     symRecord;
    size_t             NextSize;
    int                status;
    SymbolDumpState_t *state;

    /*
     * Rather than passing the state pointer through the generic "int" arg,
     * use a global.  This is OK because dumps are serialized externally.
     */
    state = &OS_VxWorks_SymbolDumpState;

    /*
    ** Copy symbol name
    */
    strncpy(symRecord.SymbolName, name, sizeof(symRecord.SymbolName) - 1);
    symRecord.SymbolName[sizeof(symRecord.SymbolName) - 1] = '\0';

    /*
    ** Check to see if the max length of each symbol name has been reached
    */
    if (memchr(name, 0, OS_MAX_SYM_LEN) == NULL)
    {
        symRecord.SymbolName[sizeof(symRecord.SymbolName) - 2] = '*';
        OS_DEBUG("%s(): symbol name too long\n", __func__);
        state->StatusCode = OS_ERR_NAME_TOO_LONG;
    }

    /*
    ** Check to see if the maximum size of the file has been reached
    */
    NextSize = state->CurrSize + sizeof(symRecord);
    if (NextSize > state->Sizelimit)
    {
        /*
        ** We exceeded the maximum size, so tell vxWorks to stop
        ** However this is not considered an error, just a stop condition.
        */
        OS_DEBUG("%s(): symbol table size exceeded\n", __func__);
        state->StatusCode = OS_ERR_OUTPUT_TOO_LARGE;
        return false;
    }

    /*
    ** Save symbol address
    */
    symRecord.SymbolAddress = (cpuaddr)val;

    /*
    ** Write entry in file
    */
    status = write(state->fd, (char *)&symRecord, sizeof(symRecord));
    /* There is a problem if not all bytes were written OR if we get an error
     * value, < 0. */
    if (status < (int)sizeof(symRecord))
    {
        state->StatusCode = OS_ERROR;
        return false;
    }

    state->CurrSize = NextSize;

    /*
    ** It's OK to continue
    */
    return true;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolTableDump_Impl(const char *filename, size_t size_limit)
{
    SymbolDumpState_t *state;

    /*
     * Rather than passing the state pointer through the generic "int" arg,
     * use a global.  This is OK because dumps are serialized externally.
     */
    state = &OS_VxWorks_SymbolDumpState;

    memset(state, 0, sizeof(*state));
    state->Sizelimit = size_limit;

    /*
    ** Open file
    */
    state->fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (state->fd < 0)
    {
        OS_DEBUG("open(%s): error: %s\n", filename, strerror(errno));
        state->StatusCode = OS_ERROR;
    }
    else
    {
        /*
        ** Iterate the symbol table
        */
        (void)symEach(sysSymTbl, OS_SymTableIterator_Impl, 0);

        close(state->fd);
    }

    /*
     * If output size was zero this means a failure of the symEach call,
     * in that it didn't iterate over anything at all.
     */
    if (state->StatusCode == OS_SUCCESS && state->CurrSize == 0)
    {
        OS_DEBUG("%s(): No symbols found!\n", __func__);
        state->StatusCode = OS_ERROR;
    }

    return state->StatusCode;
}
