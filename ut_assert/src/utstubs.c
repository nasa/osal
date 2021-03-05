/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file utstubs.c
 *
 *  Created on: Feb 25, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Types and definitions for the generic Unit Test Stub framework implemented
 * as part of OSAL.
 *
 * Provides a means to implement stub functions WITHOUT needing any specific
 * headers or knowledge about the real implementations of those functions.
 *
 * This is a major key difference between this implementation and the other stub
 * implementations that have existed before this, as all other hook/stub implementations
 * ultimately use the prototypes or are somehow tied to the functions they are stubbing out.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "utstubs.h"
#include "utbsp.h"

#define UT_MAX_FUNC_STUBS     200
#define UT_APPNAME_MAX_LEN    80
#define UT_SUBSYS_MAX_LEN     5
#define UT_MODEFLAG_ALLOC_BUF 0x1U

typedef enum
{
    UT_ENTRYTYPE_UNUSED,           /**< Unused/available table entries */
    UT_ENTRYTYPE_COUNTER,          /**< Records a usage count plus most recent return code */
    UT_ENTRYTYPE_FORCE_FAIL,       /**< Always return a designated code from stub */
    UT_ENTRYTYPE_DEFERRED_RC,      /**< Return a designated code from stub after "N" calls */
    UT_ENTRYTYPE_DATA_BUFFER,      /**< Storage for data buffers to simulate read/write or queue ops */
    UT_ENTRYTYPE_CALLBACK_HOOK,    /**< Pointer to a custom callback/hook function */
    UT_ENTRYTYPE_CALLBACK_CONTEXT, /**< Context data for callback/hook function */
    UT_ENTRYTYPE_CALL_ONCE,        /**< Records a "call once" directive */
    UT_ENTRYTYPE_OVERRIDE_STUB,    /**< Records when stub after hook call behavior should not be used */
} UT_EntryType_t;

typedef struct
{
    uint32 Count;
    int32  Value;
} UT_RetcodeEntry_t;

typedef struct
{
    size_t Position;
    size_t TotalSize;
    uint8 *BasePtr;
} UT_BufferEntry_t;

typedef union
{
    void *          Addr;
    UT_HookFunc_t   Simple;
    UT_VaHookFunc_t Va;
} UT_HookFuncPtr_t;

typedef struct
{
    UT_HookFuncPtr_t Ptr;
    void *           CallbackArg;
    bool             IsVarg;
} UT_CallbackEntry_t;

typedef union
{
    UT_RetcodeEntry_t  Rc;
    UT_BufferEntry_t   Buff;
    UT_CallbackEntry_t Cb;
    UT_StubContext_t   Context;
} UT_EntryData_t;

/*
 * Definition of internal stub table structure
 */
typedef struct
{
    UT_EntryType_t EntryType;
    UT_EntryKey_t  FuncKey;
    uint32         ModeFlags;
    UT_EntryData_t Data;
} UT_StubTableEntry_t;

static UT_StubTableEntry_t UT_StubTable[UT_MAX_FUNC_STUBS] = {{0}};
static uint32              UT_MaxStubSearchLen             = 0;

/**
 * Helper function to clear an entry in the stub table.
 * This will call "free()" on any dynamically allocated buffers within the entry,
 * then zero out the entire block so it can be re-used.
 */
static void UT_ClearStubEntry(UT_StubTableEntry_t *StubPtr)
{
    /* Be sure to call free() on any malloc'ed buffers before clearing */
    if (StubPtr->EntryType == UT_ENTRYTYPE_DATA_BUFFER && StubPtr->Data.Buff.BasePtr != NULL &&
        (StubPtr->ModeFlags & UT_MODEFLAG_ALLOC_BUF) != 0)
    {
        free(StubPtr->Data.Buff.BasePtr);
    }
    memset(StubPtr, 0, sizeof(*StubPtr));
}

/*
** Functions
*/

static UT_StubTableEntry_t *UT_GetStubEntry(UT_EntryKey_t FuncKey, UT_EntryType_t TestMode)
{
    UT_StubTableEntry_t *StubPtr   = NULL;
    uint32               Idx       = FuncKey % (UT_MAX_FUNC_STUBS - 1); /* hash the key to determine the start point */
    uint32               SearchLen = 0;
    uint32               SearchLimit;
    UT_EntryKey_t        SearchKey;

    /* If searching for an unused entry, look through the entire table.
     * Otherwise bound the search */
    if (TestMode == UT_ENTRYTYPE_UNUSED)
    {
        SearchLimit = UT_MAX_FUNC_STUBS;
        SearchKey   = 0;
    }
    else
    {
        SearchLimit = UT_MaxStubSearchLen;
        SearchKey   = FuncKey;
    }

    while (1)
    {
        if (SearchLen >= SearchLimit)
        {
            StubPtr = NULL;
            break;
        }
        ++SearchLen;
        StubPtr = &UT_StubTable[Idx];
        if (StubPtr->EntryType == TestMode && StubPtr->FuncKey == SearchKey)
        {
            break;
        }
        ++Idx;
        if (Idx >= UT_MAX_FUNC_STUBS)
        {
            Idx = 0;
        }
    }

    /*
     * Keep track of the longest search length since the last reset.
     * This serves as the upper bound for future searches.
     */
    if (SearchLen > UT_MaxStubSearchLen)
    {
        UT_MaxStubSearchLen = SearchLen;
    }

    return (StubPtr);
}

static void UT_DoSetOverride(UT_EntryKey_t FuncKey)
{
    UT_StubTableEntry_t *StubPtr;

    /* check if there is already an entry */
    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_OVERRIDE_STUB);

    /* If NULL, then this is the first set, set a UT_ENTRYTYPE_OVERRIDE_STUB */
    if (StubPtr == NULL)
    {
        StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_UNUSED);

        StubPtr->FuncKey     = FuncKey;
        StubPtr->EntryType   = UT_ENTRYTYPE_OVERRIDE_STUB;
    }

}

void UT_ResetState(UT_EntryKey_t FuncKey)
{
    UT_StubTableEntry_t *StubPtr;
    int32                i;

    StubPtr = UT_StubTable;
    for (i = 0; i < UT_MAX_FUNC_STUBS; ++i)
    {
        if (FuncKey == 0 || StubPtr->FuncKey == FuncKey)
        {
            UT_ClearStubEntry(StubPtr);
        }
        ++StubPtr;
    }
    if (FuncKey == 0)
    {
        UT_MaxStubSearchLen = 0;
    }
}

void UT_Stub_CallOnce(void (*Func)(void))
{
    UT_StubTableEntry_t *StubPtr;
    UT_StubTableEntry_t *OnceEnt;
    UT_EntryKey_t        FuncKey;
    int32                i;

    if (Func == NULL)
    {
        return;
    }

    FuncKey = (UT_EntryKey_t)Func;
    OnceEnt = NULL;
    StubPtr = UT_StubTable;
    for (i = 0; i < UT_MAX_FUNC_STUBS; ++i)
    {
        if (StubPtr->EntryType == UT_ENTRYTYPE_UNUSED && OnceEnt == NULL)
        {
            OnceEnt = StubPtr;
        }
        else if (StubPtr->EntryType == UT_ENTRYTYPE_CALL_ONCE && StubPtr->FuncKey == FuncKey)
        {
            OnceEnt = StubPtr;
            break;
        }
        ++StubPtr;
    }

    if (OnceEnt == NULL)
    {
        /* should never happen -- UT_MAX_FUNC_STUBS needs increase if it does */
        UtAssert_Abort("Cannot do CallOnce - UT_MAX_FUNC_STUBS too low?");
    }
    else if (OnceEnt->EntryType == UT_ENTRYTYPE_UNUSED)
    {
        OnceEnt->EntryType = UT_ENTRYTYPE_CALL_ONCE;
        OnceEnt->FuncKey   = FuncKey;

        Func();
    }
}

void UT_SetDeferredRetcode(UT_EntryKey_t FuncKey, int32 Count, int32 Retcode)
{
    UT_StubTableEntry_t *StubPtr;

    if (Count > 0)
    {
        StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_UNUSED);

        if (StubPtr == NULL)
        {
            UtAssert_Abort("Cannot set retcode - UT_MAX_FUNC_STUBS too low?");
        }
        else
        {
            StubPtr->FuncKey       = FuncKey;
            StubPtr->EntryType     = UT_ENTRYTYPE_DEFERRED_RC;
            StubPtr->Data.Rc.Count = Count;
            StubPtr->Data.Rc.Value = Retcode;
        }
    }
}

void UT_ClearDeferredRetcode(UT_EntryKey_t FuncKey)
{
    UT_StubTableEntry_t *StubPtr;

    while (true)
    {
        StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_DEFERRED_RC);
        if (StubPtr == NULL)
        {
            break;
        }
        UT_ClearStubEntry(StubPtr);
    }
}

bool UT_Stub_CheckDeferredRetcode(UT_EntryKey_t FuncKey, int32 *Retcode)
{
    bool                 Result = false;
    UT_StubTableEntry_t *StubPtr;

    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_DEFERRED_RC);
    if (StubPtr != NULL)
    {
        if (StubPtr->Data.Rc.Count > 0)
        {
            --StubPtr->Data.Rc.Count;
        }
        if (StubPtr->Data.Rc.Count == 0)
        {
            Result   = true;
            *Retcode = StubPtr->Data.Rc.Value;

            /* Once the count has reached zero, void the entry */
            UT_ClearStubEntry(StubPtr);
        }
    }

    return (Result);
}

void UT_SetDefaultReturnValue(UT_EntryKey_t FuncKey, int32 Value)
{
    UT_StubTableEntry_t *Rc;

    /*
     * First find an existing default return value entry for the function.
     * In case one is already set we do not duplicate (unlike deferred codes)
     */
    Rc = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_FORCE_FAIL);
    if (Rc == NULL)
    {
        /* Creating default return value entry - repeat search and grab any unused slot */
        Rc = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_UNUSED);
    }

    if (Rc != NULL)
    {
        Rc->FuncKey       = FuncKey;
        Rc->EntryType     = UT_ENTRYTYPE_FORCE_FAIL;
        Rc->Data.Rc.Value = Value;
    }
    else
    {
        UtAssert_Abort("Cannot set retcode - UT_MAX_FUNC_STUBS too low?");
    }
}

void UT_ClearDefaultReturnValue(UT_EntryKey_t FuncKey)
{
    UT_StubTableEntry_t *StubPtr;

    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_FORCE_FAIL);
    if (StubPtr != NULL)
    {
        UT_ClearStubEntry(StubPtr);
    }
}

#ifndef OSAL_OMIT_DEPRECATED
void UT_SetForceFail(UT_EntryKey_t FuncKey, int32 Value)
{
    UT_SetDefaultReturnValue(FuncKey, Value);
}

void UT_ClearForceFail(UT_EntryKey_t FuncKey)
{
    UT_ClearDefaultReturnValue(FuncKey);
}
#endif

bool UT_GetStubRetcodeAndCount(UT_EntryKey_t FuncKey, int32 *Retcode, int32 *Count)
{
    UT_StubTableEntry_t *StubPtr;
    bool                 Result;

    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_COUNTER);
    if (StubPtr != NULL)
    {
        *Count   = StubPtr->Data.Rc.Count;
        *Retcode = StubPtr->Data.Rc.Value;
        Result   = true;
    }
    else
    {
        Result = false;
    }

    return Result;
}

uint32 UT_GetStubCount(UT_EntryKey_t FuncKey)
{
    UT_StubTableEntry_t *StubPtr;
    uint32               Count;

    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_COUNTER);
    if (StubPtr != NULL)
    {
        Count = StubPtr->Data.Rc.Count;
    }
    else
    {
        Count = 0;
    }

    return Count;
}

bool UT_Stub_CheckForceFail(UT_EntryKey_t FuncKey, int32 *Value)
{
    bool                 Result = false;
    UT_StubTableEntry_t *StubPtr;

    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_FORCE_FAIL);
    if (StubPtr != NULL)
    {
        /* For "force fail" entries, the count will reflect the number of times it was used */
        ++StubPtr->Data.Rc.Count;
        *Value = StubPtr->Data.Rc.Value;
        Result = true;
    }

    return (Result);
}

void UT_SetDataBuffer(UT_EntryKey_t FuncKey, void *DataBuffer, size_t BufferSize, bool AllocateCopy)
{
    UT_StubTableEntry_t *StubPtr;

    if (DataBuffer != NULL && BufferSize > 0)
    {
        StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_UNUSED);

        if (StubPtr == NULL)
        {
            UtAssert_Abort("Cannot set data buffer - UT_MAX_FUNC_STUBS too low?");
        }
        else
        {
            StubPtr->FuncKey   = FuncKey;
            StubPtr->EntryType = UT_ENTRYTYPE_DATA_BUFFER;
            if (AllocateCopy)
            {
                StubPtr->Data.Buff.BasePtr = malloc(BufferSize);
                if (StubPtr->Data.Buff.BasePtr == NULL)
                {
                    UtAssert_Abort("Cannot allocate data buffer - malloc() failed!");
                }
                else
                {
                    memcpy(StubPtr->Data.Buff.BasePtr, DataBuffer, BufferSize);
                    StubPtr->ModeFlags |= UT_MODEFLAG_ALLOC_BUF;
                }
            }
            else
            {
                /* Use buffer directly */
                StubPtr->Data.Buff.BasePtr = DataBuffer;
            }
            StubPtr->Data.Buff.TotalSize = BufferSize;
            StubPtr->Data.Buff.Position  = 0;
        }
    }
}

void UT_GetDataBuffer(UT_EntryKey_t FuncKey, void **DataBuffer, size_t *MaxSize, size_t *Position)
{
    UT_StubTableEntry_t *StubPtr;
    void *               ResultDataBuffer;
    size_t               ResultMaxSize;
    size_t               ResultPosition;

    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_DATA_BUFFER);

    if (StubPtr == NULL)
    {
        ResultDataBuffer = NULL;
        ResultMaxSize    = 0;
        ResultPosition   = 0;
    }
    else
    {
        ResultDataBuffer = StubPtr->Data.Buff.BasePtr;
        ResultMaxSize    = StubPtr->Data.Buff.TotalSize;
        ResultPosition   = StubPtr->Data.Buff.Position;
    }

    if (DataBuffer != NULL)
    {
        *DataBuffer = ResultDataBuffer;
    }
    if (MaxSize != NULL)
    {
        *MaxSize = ResultMaxSize;
    }
    if (Position != NULL)
    {
        *Position = ResultPosition;
    }
}

size_t UT_Stub_CopyToLocal(UT_EntryKey_t FuncKey, void *LocalBuffer, size_t MaxSize)
{
    size_t               ActualCopy;
    UT_StubTableEntry_t *StubPtr;

    ActualCopy = 0;
    StubPtr    = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_DATA_BUFFER);

    if (StubPtr != NULL)
    {
        ActualCopy = StubPtr->Data.Buff.TotalSize - StubPtr->Data.Buff.Position;
        if (MaxSize < ActualCopy)
        {
            ActualCopy = MaxSize;
        }

        memcpy(LocalBuffer, &StubPtr->Data.Buff.BasePtr[StubPtr->Data.Buff.Position], ActualCopy);
        StubPtr->Data.Buff.Position += ActualCopy;

        /*
         * Once the buffer is completely copied then drop this entry.
         * The test harness will either refill it or there might be
         * another entry in the table.
         */
        if (StubPtr->Data.Buff.Position >= StubPtr->Data.Buff.TotalSize)
        {
            UT_ClearStubEntry(StubPtr);
        }
    }

    return ActualCopy;
}

size_t UT_Stub_CopyFromLocal(UT_EntryKey_t FuncKey, const void *LocalBuffer, size_t MaxSize)
{
    size_t               ActualCopy;
    UT_StubTableEntry_t *StubPtr;

    ActualCopy = 0;
    StubPtr    = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_DATA_BUFFER);

    if (StubPtr != NULL)
    {
        ActualCopy = StubPtr->Data.Buff.TotalSize - StubPtr->Data.Buff.Position;
        if (MaxSize < ActualCopy)
        {
            ActualCopy = MaxSize;
        }

        memcpy(&StubPtr->Data.Buff.BasePtr[StubPtr->Data.Buff.Position], LocalBuffer, ActualCopy);
        StubPtr->Data.Buff.Position += ActualCopy;

        /*
         * Once the buffer is completely copied then drop this entry.
         * The test harness will either refill it or there might be
         * another entry in the table.
         */
        if (StubPtr->Data.Buff.Position >= StubPtr->Data.Buff.TotalSize)
        {
            UT_ClearStubEntry(StubPtr);
        }
    }

    return ActualCopy;
}

/*
 * Helper function used by UT_SetHookFunction() and UT_SetVaHookFunction()
 */
static void UT_DoSetHookFunction(UT_EntryKey_t FuncKey, UT_HookFuncPtr_t Value, void *UserObj, bool IsVarg)
{
    UT_StubTableEntry_t *StubPtr;

    /*
     * First find an existing hook entry for the function.
     * In case one is already set we do not duplicate
     */
    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_CALLBACK_HOOK);
    if (StubPtr == NULL && Value.Addr != NULL)
    {
        /* Creating force fail entry - repeat search and grab any unused slot */
        StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_UNUSED);
        if (StubPtr == NULL)
        {
            UtAssert_Abort("Cannot set hook function - UT_MAX_FUNC_STUBS too low?");
        }
    }

    if (Value.Addr == NULL && StubPtr != NULL)
    {
        /* Caller wants to delete the entry */
        UT_ClearStubEntry(StubPtr);
    }
    else if (StubPtr != NULL && Value.Addr != NULL)
    {
        /* Caller wants to set the entry */
        StubPtr->FuncKey             = FuncKey;
        StubPtr->EntryType           = UT_ENTRYTYPE_CALLBACK_HOOK;
        StubPtr->Data.Cb.CallbackArg = UserObj;
        StubPtr->Data.Cb.Ptr         = Value;
        StubPtr->Data.Cb.IsVarg      = IsVarg;
    }
}

void UT_SetHookFunction(UT_EntryKey_t FuncKey, UT_HookFunc_t HookFunc, void *UserObj)
{
    UT_HookFuncPtr_t Value;

    Value.Simple = HookFunc;

    UT_DoSetHookFunction(FuncKey, Value, UserObj, false);
}

void UT_SetHookOverrideStubFunction(UT_EntryKey_t FuncKey, UT_HookFunc_t HookFunc, void *UserObj)
{    
    UT_DoSetOverride(FuncKey);

    UT_SetHookFunction(FuncKey, HookFunc, UserObj);
}

void UT_SetVaHookFunction(UT_EntryKey_t FuncKey, UT_VaHookFunc_t HookFunc, void *UserObj)
{
    UT_HookFuncPtr_t Value;

    Value.Va = HookFunc;

    UT_DoSetHookFunction(FuncKey, Value, UserObj, true);
}

const void *UT_Hook_GetArgPtr(const UT_StubContext_t *ContextPtr, const char *Name, size_t ExpectedTypeSize)
{
    uint32                      i;
    const void *                Result;
    const UT_StubArgMetaData_t *MetaPtr;

    static const union
    {
        unsigned long AsInt;
        void *        AsPtr;
        double        AsFloat;
    } ARG_DEFAULT_ZERO_VALUE = {0};

    Result = NULL;
    for (i = 0; i < ContextPtr->ArgCount; ++i)
    {
        MetaPtr = &ContextPtr->Meta[i];
        if (MetaPtr->Name != NULL)
        {
            if (strcmp(MetaPtr->Name, Name) == 0 && (MetaPtr->Size == 0 || MetaPtr->Size == ExpectedTypeSize))
            {
                if (MetaPtr->Type == UT_STUBCONTEXT_ARG_TYPE_DIRECT)
                {
                    Result = &ContextPtr->ArgPtr[i];
                }
                else if (MetaPtr->Type == UT_STUBCONTEXT_ARG_TYPE_INDIRECT)
                {
                    Result = ContextPtr->ArgPtr[i];
                }
                break;
            }
        }
    }

    /*
     * If no suitable result pointer was found, this means a mismatch
     * between the stub and test case, such as a change in argument/parameter names.
     * This is an error that should be corrected, so report it as a failure.
     */
    if (Result == NULL)
    {
        UtAssert_Failed("Requested parameter %s of size %lu which was not provided by the stub", Name,
                        (unsigned long)ExpectedTypeSize);

        if (ExpectedTypeSize <= sizeof(ARG_DEFAULT_ZERO_VALUE))
        {
            Result = &ARG_DEFAULT_ZERO_VALUE;
        }
        else
        {
            /*
             * As the caller will likely dereference the returned pointer, should
             * never return NULL.  Just abort here.
             */
            UtAssert_Abort("No value for parameter");
        }
    }

    return Result;
}

void UT_Stub_RegisterContextWithMetaData(UT_EntryKey_t FuncKey, const char *Name, UT_StubContext_Arg_Type_t ParamType,
                                         const void *ParamPtr, size_t ParamSize)
{
    UT_StubTableEntry_t * StubPtr;
    UT_StubArgMetaData_t *MetaPtr;

    /*
     * First find an existing context entry for the function.
     * In case one is already set we do not duplicate
     */
    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_CALLBACK_CONTEXT);
    if (StubPtr == NULL)
    {
        /* Creating force fail entry - repeat search and grab any unused slot */
        StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_UNUSED);
    }

    if (StubPtr == NULL)
    {
        UtAssert_Abort("Cannot set context - UT_MAX_FUNC_STUBS too low?");
    }
    else
    {
        StubPtr->FuncKey   = FuncKey;
        StubPtr->EntryType = UT_ENTRYTYPE_CALLBACK_CONTEXT;
        if (StubPtr->Data.Context.ArgCount < UT_STUBCONTEXT_MAXSIZE)
        {
            StubPtr->Data.Context.ArgPtr[StubPtr->Data.Context.ArgCount] = ParamPtr;

            MetaPtr       = &StubPtr->Data.Context.Meta[StubPtr->Data.Context.ArgCount];
            MetaPtr->Size = ParamSize;
            MetaPtr->Type = ParamType;

            /*
             * If name was specified, then trim any leading address operator (&)
             * and/or whitespace, keeping only the actual name part.
             */
            if (Name != NULL)
            {
                /*
                 * If the _address_ of the stack variable was actually passed in,
                 * the mark this as indirect (i.e. hook must dereference ArgPtr
                 * to get actual parameter value).  Otherwise assume it as direct.
                 */
                MetaPtr->Name = Name;
                while (*MetaPtr->Name != 0)
                {
                    if (*MetaPtr->Name == '&')
                    {
                        /* this means its a pointer to the value, not the value itself */
                        if (MetaPtr->Type == UT_STUBCONTEXT_ARG_TYPE_UNSPECIFIED)
                        {
                            MetaPtr->Type = UT_STUBCONTEXT_ARG_TYPE_INDIRECT;
                        }
                    }
                    else if (*MetaPtr->Name != ' ')
                    {
                        /* stop at non-whitespace */
                        break;
                    }
                    ++MetaPtr->Name;
                }

                if (MetaPtr->Type == UT_STUBCONTEXT_ARG_TYPE_UNSPECIFIED)
                {
                    MetaPtr->Type = UT_STUBCONTEXT_ARG_TYPE_DIRECT;
                }
            }
            ++StubPtr->Data.Context.ArgCount;
        }
    }
}

/**
 * Default implementation for a stub function that should be useful for most cases.
 * Checks first for a deferred retcode, then for a constant retcode, and a default if neither is present.
 * Prints a debug level status message to show that the function was called.
 */
int32 UT_DefaultStubImplWithArgs(const char *FunctionName, UT_EntryKey_t FuncKey, int32 DefaultRc, va_list va)
{
    int32                   Retcode;
    const char *            RetcodeString;
    UT_StubTableEntry_t *   StubPtr;
    UT_StubTableEntry_t *   ContextTblPtr;
    const UT_StubContext_t *ContextPtr;
    uint32                  Counter;

    if (!UT_Stub_CheckDeferredRetcode(FuncKey, &Retcode))
    {
        if (!UT_Stub_CheckForceFail(FuncKey, &Retcode))
        {
            Retcode = DefaultRc;
        }
    }

    if (FunctionName != NULL)
    {
        if (Retcode == DefaultRc)
        {
            RetcodeString = "DEFAULT";
        }
        else
        {
            /* Indicate that this invocation got a non-default return code */
            RetcodeString = "*SPECIAL*";
        }

        UtDebug("%s called (%s,%d)", FunctionName, RetcodeString, (int)Retcode);
    }

    Counter = 0;
    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_COUNTER);
    if (StubPtr == NULL)
    {
        /* Creating counter entry - repeat search and grab any unused slot */
        StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_UNUSED);
    }

    if (StubPtr == NULL)
    {
        UtAssert_Abort("Cannot set counter - UT_MAX_FUNC_STUBS too low?");
    }
    else
    {
        StubPtr->EntryType = UT_ENTRYTYPE_COUNTER;
        StubPtr->FuncKey   = FuncKey;
        Counter            = StubPtr->Data.Rc.Count;
        ++StubPtr->Data.Rc.Count;
        StubPtr->Data.Rc.Value = Retcode;
    }

    /* Handle a user-requested callback hook.
     * First see if the stub has a registered context.
     */
    ContextTblPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_CALLBACK_CONTEXT);
    if (ContextTblPtr == NULL)
    {
        ContextPtr = NULL;
    }
    else
    {
        ContextPtr = &ContextTblPtr->Data.Context;
    }

    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_CALLBACK_HOOK);
    if (StubPtr != NULL)
    {
        if (StubPtr->Data.Cb.IsVarg)
        {
            Retcode = (*StubPtr->Data.Cb.Ptr.Va)(StubPtr->Data.Cb.CallbackArg, Retcode, Counter, ContextPtr, va);
        }
        else
        {
            Retcode = (*StubPtr->Data.Cb.Ptr.Simple)(StubPtr->Data.Cb.CallbackArg, Retcode, Counter, ContextPtr);
        }
    }

    /* Always clear the context entry -- the next call will have a different one */
    if (ContextTblPtr != NULL)
    {
        UT_ClearStubEntry(ContextTblPtr);
    }

    return Retcode;
}

/**
 * Default implementation for a stub function that passes through args.
 * Calls the UT_DefaultStubImplWithArgs function with the supplied args
 */
int32 UT_DefaultStubImpl(const char *FunctionName, UT_EntryKey_t FuncKey, int32 DefaultRc, ...)
{
    int32   Retcode;
    va_list va;

    va_start(va, DefaultRc);
    Retcode = UT_DefaultStubImplWithArgs(FunctionName, FuncKey, DefaultRc, va);
    va_end(va);

    return Retcode;
}

bool UT_StubIsOverridden(UT_EntryKey_t FuncKey)
{
    UT_StubTableEntry_t  *StubPtr;

    /* get override status */
    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_OVERRIDE_STUB);

    /* StubPtr will be NULL when override is not set */
    return (StubPtr != NULL);
}
