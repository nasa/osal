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
    UT_ENTRYTYPE_CALLBACK_HOOK,    /**< A custom callback/hook function to be invoked prior to handler */
    UT_ENTRYTYPE_CALLBACK_CONTEXT, /**< Context data for callback/hook function */
    UT_ENTRYTYPE_CALL_ONCE,        /**< Records a "call once" directive */
    UT_ENTRYTYPE_FINAL_HANDLER,    /**< The final handler for the stub */
    UT_ENTRYTYPE_RETURN_BUFFER,    /**< Storage for return value from stub */
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
    void *             Addr;
    UT_HookFunc_t      SimpleHook;
    UT_VaHookFunc_t    VaHook;
    UT_HandlerFunc_t   SimpleHandler;
    UT_VaHandlerFunc_t VaHandler;
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
    if ((StubPtr->EntryType == UT_ENTRYTYPE_DATA_BUFFER || StubPtr->EntryType == UT_ENTRYTYPE_RETURN_BUFFER) &&
        StubPtr->Data.Buff.BasePtr != NULL && (StubPtr->ModeFlags & UT_MODEFLAG_ALLOC_BUF) != 0)
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

bool UT_Stub_CheckDefaultReturnValue(UT_EntryKey_t FuncKey, int32 *Value)
{
    bool                 Result = false;
    UT_StubTableEntry_t *StubPtr;

    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_FORCE_FAIL);
    if (StubPtr != NULL)
    {
        /* For default return value entries, the count will reflect the number of times it was used */
        ++StubPtr->Data.Rc.Count;
        if (Value != NULL)
        {
            *Value = StubPtr->Data.Rc.Value;
        }
        Result = true;
    }

    return (Result);
}

void UT_Stub_RegisterReturnType(UT_EntryKey_t FuncKey, size_t ReturnSize)
{
    UT_StubTableEntry_t *StubPtr;

    if (ReturnSize > 0)
    {
        /* Check for existing buffer and re-use if same size (should be!) */
        StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_RETURN_BUFFER);
        if (StubPtr != NULL)
        {
            if (StubPtr->Data.Buff.TotalSize != ReturnSize)
            {
                UT_ClearStubEntry(StubPtr);
                StubPtr = NULL;
            }
            else
            {
                StubPtr->Data.Buff.Position = 0;
            }
        }

        if (StubPtr == NULL)
        {
            StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_UNUSED);
            if (StubPtr == NULL)
            {
                UtAssert_Abort("Cannot set return buffer - UT_MAX_FUNC_STUBS too low?");
            }
            else
            {
                StubPtr->FuncKey           = FuncKey;
                StubPtr->EntryType         = UT_ENTRYTYPE_RETURN_BUFFER;
                StubPtr->Data.Buff.BasePtr = malloc(ReturnSize);
                if (StubPtr->Data.Buff.BasePtr == NULL)
                {
                    UtAssert_Abort("Cannot allocate data buffer - malloc() failed!");
                }
                else
                {
                    memset(StubPtr->Data.Buff.BasePtr, 0, ReturnSize);
                    StubPtr->ModeFlags |= UT_MODEFLAG_ALLOC_BUF;
                }

                StubPtr->Data.Buff.TotalSize = ReturnSize;
                StubPtr->Data.Buff.Position  = 0;
            }
        }
    }
}

void *UT_Stub_GetReturnValuePtr(UT_EntryKey_t FuncKey, size_t ReturnSize)
{
    UT_StubTableEntry_t *StubPtr;
    void *               ReturnPtr;

    ReturnPtr = NULL;
    StubPtr   = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_RETURN_BUFFER);

    /* Sanity check on the size */
    if (StubPtr != NULL && StubPtr->Data.Buff.TotalSize == ReturnSize)
    {
        ReturnPtr = StubPtr->Data.Buff.BasePtr;
    }
    else
    {
        /* This shouldn't happen, it means the stub tried to use a
         * return buffer that does not exist or does not match size.
         *
         * It is most likely caused by a mismatch/incompatibility between
         * stub and handler.  Aborting now is better than segfaulting later,
         * as the errored call should still be on the stack trace */
        UtAssert_Abort("Return buffer invalid");
    }

    return ReturnPtr;
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
static void UT_DoSetHookFunction(UT_EntryKey_t FuncKey, UT_EntryType_t EntryType, UT_HookFuncPtr_t Value, void *UserObj,
                                 bool IsVarg)
{
    UT_StubTableEntry_t *StubPtr;

    /*
     * First find an existing hook entry for the function.
     * In case one is already set we do not duplicate
     */
    StubPtr = UT_GetStubEntry(FuncKey, EntryType);
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
        StubPtr->EntryType           = EntryType;
        StubPtr->Data.Cb.CallbackArg = UserObj;
        StubPtr->Data.Cb.Ptr         = Value;
        StubPtr->Data.Cb.IsVarg      = IsVarg;
    }
}

void UT_SetHookFunction(UT_EntryKey_t FuncKey, UT_HookFunc_t HookFunc, void *UserObj)
{
    UT_HookFuncPtr_t Value;

    Value.SimpleHook = HookFunc;

    UT_DoSetHookFunction(FuncKey, UT_ENTRYTYPE_CALLBACK_HOOK, Value, UserObj, false);
}

void UT_SetVaHookFunction(UT_EntryKey_t FuncKey, UT_VaHookFunc_t HookFunc, void *UserObj)
{
    UT_HookFuncPtr_t Value;

    Value.VaHook = HookFunc;

    UT_DoSetHookFunction(FuncKey, UT_ENTRYTYPE_CALLBACK_HOOK, Value, UserObj, true);
}

void UT_SetHandlerFunction(UT_EntryKey_t FuncKey, UT_HandlerFunc_t HandlerFunc, void *UserObj)
{
    UT_HookFuncPtr_t Value;

    Value.SimpleHandler = HandlerFunc;

    UT_DoSetHookFunction(FuncKey, UT_ENTRYTYPE_FINAL_HANDLER, Value, UserObj, false);
}

void UT_SetVaHandlerFunction(UT_EntryKey_t FuncKey, UT_VaHandlerFunc_t HandlerFunc, void *UserObj)
{
    UT_HookFuncPtr_t Value;

    Value.VaHandler = HandlerFunc;

    UT_DoSetHookFunction(FuncKey, UT_ENTRYTYPE_FINAL_HANDLER, Value, UserObj, true);
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

bool UT_Stub_GetInt32StatusCode(const UT_StubContext_t *Context, int32 *StatusCodeBuffer)
{
    if (StatusCodeBuffer != NULL)
    {
        *StatusCodeBuffer = Context->Int32StatusCode;
    }

    return Context->Int32StatusIsSet;
}

void UT_Stub_CopyToReturnValue(UT_EntryKey_t FuncKey, const void *BufferPtr, size_t BufferSize)
{
    UT_StubTableEntry_t *StubPtr;

    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_RETURN_BUFFER);
    if (StubPtr != NULL)
    {
        /* If the size does not match, there is a bug */
        if (StubPtr->Data.Buff.TotalSize != BufferSize)
        {
            UtAssert_Abort("Size mismatch in setting return value");
        }
        else
        {
            memcpy(StubPtr->Data.Buff.BasePtr, BufferPtr, BufferSize);
            StubPtr->Data.Buff.Position = BufferSize;
        }
    }
}

/**
 * Default implementation for a stub function that should be useful for most cases.
 * Checks first for a deferred retcode, then for a constant retcode, and a default if neither is present.
 * Prints a debug level status message to show that the function was called.
 */
int32 UT_DefaultStubImplWithArgs(const char *FunctionName, UT_EntryKey_t FuncKey, int32 DefaultRc, va_list ArgList)
{
    const char *         RetcodeString;
    UT_StubTableEntry_t *StubPtr;
    UT_StubTableEntry_t *ContextTblPtr;
    UT_StubContext_t     LocalContext;
    uint32               Counter;
    va_list              ArgListCopy;

    /*
     * In this implementation a context is _always_ needed.
     *
     * First see if the stub has an already-registered context.  Any non-trivial stub
     * should already have registered some arguments and/or a return code buffer.
     *
     * To handle an old-style/incomplete stub function or simple void/void stubs that
     * do not have any arguments, a blank entry can be created on the fly.
     */
    ContextTblPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_CALLBACK_CONTEXT);
    if (ContextTblPtr != NULL)
    {
        LocalContext = ContextTblPtr->Data.Context;

        /* Always clear the context entry -- the next call will have a different one */
        UT_ClearStubEntry(ContextTblPtr);
    }
    else
    {
        memset(&LocalContext, 0, sizeof(LocalContext));
    }

    LocalContext.Int32StatusIsSet = UT_Stub_CheckDeferredRetcode(FuncKey, &LocalContext.Int32StatusCode);
    if (!LocalContext.Int32StatusIsSet)
    {
        LocalContext.Int32StatusIsSet = UT_Stub_CheckDefaultReturnValue(FuncKey, &LocalContext.Int32StatusCode);
    }
    if (!LocalContext.Int32StatusIsSet)
    {
        LocalContext.Int32StatusCode = DefaultRc;
    }

    if (FunctionName != NULL)
    {
        if (!LocalContext.Int32StatusIsSet)
        {
            RetcodeString = "DEFAULT";
        }
        else
        {
            /* Indicate that this invocation got a non-default return code */
            RetcodeString = "*SPECIAL*";
        }

        UtDebug("%s called (%s,%d)", FunctionName, RetcodeString, (int)LocalContext.Int32StatusCode);
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
        StubPtr->Data.Rc.Value = LocalContext.Int32StatusCode;
    }

    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_CALLBACK_HOOK);
    if (StubPtr != NULL)
    {
        if (StubPtr->Data.Cb.IsVarg)
        {
            va_copy(ArgListCopy, ArgList);
            LocalContext.Int32StatusCode = StubPtr->Data.Cb.Ptr.VaHook(
                StubPtr->Data.Cb.CallbackArg, LocalContext.Int32StatusCode, Counter, &LocalContext, ArgListCopy);
            va_end(ArgListCopy);
        }
        else
        {
            LocalContext.Int32StatusCode = StubPtr->Data.Cb.Ptr.SimpleHook(
                StubPtr->Data.Cb.CallbackArg, LocalContext.Int32StatusCode, Counter, &LocalContext);
        }

        LocalContext.Int32StatusIsSet = true;
    }

    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_FINAL_HANDLER);
    if (StubPtr != NULL)
    {
        if (StubPtr->Data.Cb.IsVarg)
        {
            StubPtr->Data.Cb.Ptr.VaHandler(StubPtr->Data.Cb.CallbackArg, FuncKey, &LocalContext, ArgList);
        }
        else
        {
            StubPtr->Data.Cb.Ptr.SimpleHandler(StubPtr->Data.Cb.CallbackArg, FuncKey, &LocalContext);
        }
    }

    /*
     * Handle propagation of return code.
     *
     *   "old style" stubs will translate the int32 return value from this function.
     *     - Return Buffer will always be unset with these stubs
     *     - This should continue to work with no special handling.
     *
     *   "new style" stubs will provide a return value buffer via UT_ENTRYTYPE_RETURN_BUFFER in the context.
     *     - In this pattern the int32 return value of this function is _IGNORED_ by the calling stub.
     *     - The only way to return a value to the caller is via the buffer.
     *     - If the handler did _not_ populate the return value, then attempt to translate the local
     *       Retcode here to the return value buffer.  This eases the transition, as the vast majority
     *       of functions do return an int32, this can be the automatic case.
     */
    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_RETURN_BUFFER);
    if (StubPtr != NULL && StubPtr->Data.Buff.Position == 0)
    {
        /*
         * This means that the stub expects the hook to fill the provided return buffer, but the hook did not
         * actually call UT_Stub_SetReturnValue() to fill that buffer.  To mimic the old behavior where the hook
         * return code was "passed through", attempt to copy the value now.  This works because for the majority
         * of functions, the return type is actually int32, so it is compatible.
         *
         * But if the size is different, then this shouldn't be done, and generate a failure message
         * to indicate the test case or stub/hook needs to be fixed.
         */
        if (StubPtr->Data.Buff.TotalSize == sizeof(LocalContext.Int32StatusCode))
        {
            memcpy(StubPtr->Data.Buff.BasePtr, &LocalContext.Int32StatusCode, sizeof(LocalContext.Int32StatusCode));
        }
        else
        {
            /* cannot copy - generate a "failure" message to indicate the problem */
            UtAssert_Failed("Stub %s w/return size=%lu, value cannot be auto-translated from int32", FunctionName,
                            (unsigned long)StubPtr->Data.Buff.TotalSize);
        }
    }

    return LocalContext.Int32StatusCode;
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

void UT_ExecuteBasicHandler(UT_EntryKey_t FuncKey, const char *FunctionName, UT_HandlerFunc_t DefaultHook)
{
    /* Check if the test case registered a hook, and use the default if not */
    if (UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_FINAL_HANDLER) == NULL && DefaultHook != NULL)
    {
        UT_SetHandlerFunction(FuncKey, DefaultHook, NULL);
    }

    UT_DefaultStubImpl(FunctionName, FuncKey, 0, NULL);
}

void UT_ExecuteVaHandler(UT_EntryKey_t FuncKey, const char *FunctionName, UT_VaHandlerFunc_t DefaultHook,
                         va_list VaList)
{
    /* Check if the test case registered a hook, and use the default if not */
    if (UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_FINAL_HANDLER) == NULL && DefaultHook != NULL)
    {
        UT_SetVaHandlerFunction(FuncKey, DefaultHook, NULL);
    }

    UT_DefaultStubImplWithArgs(FunctionName, FuncKey, 0, VaList);
}
