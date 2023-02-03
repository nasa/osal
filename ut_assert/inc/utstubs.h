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

#ifndef UTSTUBS_H
#define UTSTUBS_H

#include <stdarg.h>
#include "common_types.h"

/**
 * Using a generic memory address as a key into table -
 * this should allow the function name (with a cast) to be used as the key,
 * but allows a fancier hash algorithm if needed.
 * Note - in pedantic mode using a "void *" here triggers a warning
 * if used with a function address, but no warning is generated if using
 * an integer memory address type.
 */
typedef cpuaddr UT_EntryKey_t;

/**
 * Type for generic integer value return codes
 *
 * By using the C99 "ptrdiff_t" type, this should be large enough to also
 * store pointer values on the target system, in addition to all normal
 * integer values.
 */
typedef ptrdiff_t UT_IntReturn_t;

/**
 * Macro to obtain a UT_EntryKey_t value from any function name
 */
#define UT_KEY(Func) ((UT_EntryKey_t)&Func)

/**
 * Maximum size of a callback hook context list
 *
 * This is the maximum number of function arguments that can be passed to a hook
 * Note that OS_TaskCreate() has (possibly) the highest parameter count in OSAL with 7 parameters
 */
#define UT_STUBCONTEXT_MAXSIZE 8

/**
 * Identifies the type of value stored in the ArgPtr field of a UT_StubContext_t object
 */
typedef enum
{
    UT_STUBCONTEXT_ARG_TYPE_UNSPECIFIED = 0,
    UT_STUBCONTEXT_ARG_TYPE_DIRECT,  /**< Indicates "ArgPtr" is a direct copy of the actual parameter value */
    UT_STUBCONTEXT_ARG_TYPE_INDIRECT /**< Indicates "ArgPtr" is a pointer to the argument value on the stack */
} UT_StubContext_Arg_Type_t;

/**
 * Identifies different genres of return values.  This serves as a hint to determine how to adapt
 * or convert a return value if the stub has a different return size.
 */
typedef enum UT_ValueGenre
{
    UT_ValueGenre_OPAQUE  = 0, /**< The nature of the value is opaque, reference is stored directly (NOT copied!) */
    UT_ValueGenre_INTEGER = 1, /**< The value is an integer and may be converted to integers of other sizes */
    UT_ValueGenre_FLOAT   = 2, /**< The value is a floating point and may be converted to floats of other sizes */
    UT_ValueGenre_POINTER = 3  /**< The value is a pointer and should only be used to fulfill a pointer return */
} UT_ValueGenre_t;

/**
 * Complete Metadata associated with a context argument
 */
typedef struct
{
    UT_StubContext_Arg_Type_t Type;
    const char *              Name;
    size_t                    Size;
} UT_StubArgMetaData_t;

/**
 * Structure to hold context data for callback hooks
 */
typedef struct
{
    int32                Int32StatusCode;
    bool                 Int32StatusIsSet;
    uint32               ArgCount;
    const void *         ArgPtr[UT_STUBCONTEXT_MAXSIZE];
    UT_StubArgMetaData_t Meta[UT_STUBCONTEXT_MAXSIZE];
} UT_StubContext_t;

/**
 * Function pointer for user-specified hook callbacks
 *
 * Hook functions provide a user-specified supplement to the handler
 * function.  They are invoked prior to the actual stub handler, and can
 * perform any additional test-specific logic necessary.
 *
 * \sa UT_HandlerFunc_t
 */
typedef int32 (*UT_HookFunc_t)(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context);

/**
 * Function pointer for user-specified variable-argument callbacks
 *
 * \copydoc UT_HookFunc_t
 */
typedef int32 (*UT_VaHookFunc_t)(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context,
                                 va_list va);

/**
 * Function pointer for user-specified stub handlers
 *
 * Handler functions are the final logic associated with a stub
 *
 * Handlers must perform the role of generating the actual
 * return code to be sent to the caller, and must also output
 * any required output parameters.
 *
 * \note Stubs should no longer have any "assumed" logic.  Any
 *       additional outputs or return code translation is now
 *       dealt with only in the handler function.
 *
 * \param[inout] UserObj  Opaque object from original registration
 * \param[in]    FuncKey  UT key of function currently being handled
 * \param[in]    Context  Context information for current handler
 */
typedef void (*UT_HandlerFunc_t)(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context);

/**
 * Function pointer for user-specified variable-argument stub handlers
 *
 * \copydoc UT_HandlerFunc_t
 * \param[in]    va  Variable argument list from initial stub call
 */
typedef void (*UT_VaHandlerFunc_t)(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context, va_list va);

/**************************************************************
 * Functions for use within test code implementation
 **************************************************************/

/**
 * Reset the Unit test framework for a given function, or all functions
 *
 * Any return code or data buffer records for the given function key
 * are cleared.
 *
 * \param FuncKey The stub function to reset.  If zero, all functions
 *      are reset.
 */
void UT_ResetState(UT_EntryKey_t FuncKey);

/**
 * Add a deferred return code entry for the given stub function
 *
 * A deferred ("count down") return code for the stub function will be
 * installed.  The specific implementation depends on the stub function,
 * but typically it will return its default code until it is called "Count"
 * times, where it will return the given Retcode, then return to
 * its default return code again.
 *
 * Multiple deferred entries for a single function are allowed.  These
 * will be done in the order they were added.
 *
 * It is recommended to call UT_ResetState() at the beginning of the test case
 * to ensure that any old entries for the stub functions are cleared out.
 *
 * \param FuncKey The stub function to add the return code to.
 * \param Count   The number of times after which the Retcode should be triggered
 * \param Retcode The signed integer value to return after Count calls.
 */
void UT_SetDeferredRetcode(UT_EntryKey_t FuncKey, int32 Count, UT_IntReturn_t Retcode);

/**
 * Add a type-agnostic return code entry for the given stub function
 *
 * This allocates a return value entry in the UT Assert state table and associates it with the
 * specified stub function.  The DeferCount parameter determines how many times the stub must
 * be invoked before the return value is used.  If this is passed in as 0, the value will be used
 * for all stub invocations, and will be retained until the state is reset.  For nonzero defer counts,
 * the value acts as a decrement counter, and will be used once the counter reaches 0.  In these cases,
 * the value will be discarded/forgotten once it has been used by the stub.
 *
 * The handling of the value depends on the ValueGenre:
 *
 *   #UT_ValueGenre_OPAQUE -  The object will be used directly as the return value from the stub, and no
 *                            conversion of any type will be attempted.
 *   #UT_ValueGenre_INTEGER - The object is an integer, and thus may be converted to numbers of other sizes/types
 *                            using integer value semantics.
 *   #UT_ValueGenre_FLOAT   - The object is a floating point, and thus may be converted to numbers of other
 *                            sizes/types using floating point value semantics.
 *   #UT_ValueGenre_POINTER - The object is a pointer, no conversions will be attempted, and the size must be
 *                            equal to sizeof(void*)
 *
 * \note for OPAQUE values, the passed-in pointer value is held directly, and will be dereferenced at the
 * time when the called stub returns the value.  Notably, the content is NOT cached in the UtAssert internal
 * storage structures, so the caller must ensure that the pointed-to object remains valid and does not go
 * out of scope for the remainder of the test case, or until UT_ResetState is called.  Conversely, for INTEGER,
 * FLOAT, or POINTER value genres, the content will be copied into the internal storage structures, so in
 * these cases, the pointed-to value may be immediately reused or freed by the caller.
 *
 * \param FuncKey    The stub function to add the return code to.
 * \param ValuePtr   Pointer to the value to return
 * \param ValueSize  Size of the object referred to by ValuePtr
 * \param ValueGenre Genre of the object referred to by ValuePtr
 * \param DeferCount Number of times the stub needs to be called until this value is used
 * \param TypeName   Data type as an ASCII string, for possible type matching (may be set from a preprocessor macro)
 */
void UT_ConfigureGenericStubReturnValue(UT_EntryKey_t FuncKey, const void *ValuePtr, size_t ValueSize,
                                        UT_ValueGenre_t ValueGenre, int32 DeferCount, const char *TypeName);

/**
 * Add a data buffer for a given stub function
 *
 * Some stub functions (i.e. read/write calls) may need to work with actual
 * test data supplied by the test function.  This specifies a data buffer
 * that the read/write call copy to or from.  Its specific usage is dependent
 * on the implementation of the stub function itself.
 *
 * Multiple buffer entries for a single function are allowed.  These
 * will be used in the order they were added.
 *
 * It is recommended to call UT_ResetState(0) at the beginning of the test case
 * to ensure that any old entries for the stub functions are cleared out.
 *
 * \param FuncKey The stub function to add the data buffer to.
 * \param DataBuffer Pointer to data buffer that should be associated with the stub function
 * \param BufferSize Size of data buffer
 * \param AllocateCopy If true then malloc() will be used to allocate a copy of the buffer
 *      and the supplied DataBuffer will be copied into it via memcpy() before returning.  The
 *      buffer will be freed after the test code reads it or the test state is reset.  If this
 *      is false then the DataBuffer pointer is used directly, and must remain valid for the duration
 *      of the current test case.
 */
void UT_SetDataBuffer(UT_EntryKey_t FuncKey, void *DataBuffer, size_t BufferSize, bool AllocateCopy);

/**
 * Gets the data buffer for a given stub function
 *
 * This is the complement to UT_SetDataBuffer() and it can be useful in stub functions
 * where the buffer needs to be checked but not necessarily consumed.
 *
 * In case there is no associated buffer the pointer will be set NULL and the size will be
 * set to 0.
 *
 * \param FuncKey The stub function to reference.
 * \param DataBuffer Set to Pointer to data buffer that is associated with the stub function (output)
 * \param MaxSize Set to Maximum Size of data buffer (output)
 * \param Position Set to current position in data buffer (output)
 */
void UT_GetDataBuffer(UT_EntryKey_t FuncKey, void **DataBuffer, size_t *MaxSize, size_t *Position);

/**
 * Set the default return value for the given stub function.
 * User needs to use UT_ClearDefaultReturnValue to clear the value.
 *
 * \param FuncKey The stub function to add the return code to.
 * \param Value Arbitrary signed integer return value (may or may not be used by the stub)
 */
void UT_SetDefaultReturnValue(UT_EntryKey_t FuncKey, UT_IntReturn_t Value);

/**
 * Disable the default return for the given stub function
 *
 * This undoes the action of UT_SetDefaultReturnValue()
 *
 * \param FuncKey The stub function entry to clear.
 */
void UT_ClearDefaultReturnValue(UT_EntryKey_t FuncKey);

/**
 * Set a Hook function for a particular call
 *
 * This triggers a callback to a user-defined function when the stub is invoked.
 * The callback may optionally pass back context data, depending on the stub
 * implementation.
 *
 * In UT assert, hook functions supplement the handler functions, and will
 * be called prior to the handler to add additional logic.  This facility should
 * be used in cases where the default handling logic is acceptable, but needs some
 * extensions or additional work performed at the time the stub is invoked, such
 * as setting a state variable in the parent test program.
 *
 *
 * \param FuncKey  The stub function to add the hook to.
 * \param HookFunc User defined hook function.  Set NULL to delete/clear an entry.
 * \param UserObj  Arbitrary user data object to pass to the hook function
 */
void UT_SetHookFunction(UT_EntryKey_t FuncKey, UT_HookFunc_t HookFunc, void *UserObj);

/**
 * Set a Handler function for a particular call
 *
 * This allows the user to completely replace the handler for a given function.
 *
 * The handler assumes all responsibility for the final behavior of the stub,
 * including translating return values, and any outputs/side effects the stub
 * function should have.  The default handler for the stub is NOT used.
 *
 * \param FuncKey  The stub function to add the hook to.
 * \param HandlerFunc User defined hook function.  Set NULL to delete/clear an entry.
 * \param UserObj  Arbitrary user data object to pass to the hook function
 */
void UT_SetHandlerFunction(UT_EntryKey_t FuncKey, UT_HandlerFunc_t HandlerFunc, void *UserObj);

/**
 * Set a variable-argument Hook function for a particular call
 *
 * Identical to UT_SetHookFunction() except that it includes a va_list argument
 * such that it can be used with a variable-argument stub function.
 *
 * This can be important for printf-like stubs where correctness might depend on the
 * contents of the arguments but the types of arguments aren't known.
 *
 * However, some systems have limited support for va_list, so this might not be
 * available on those systems.  Tests should use the generic (non-va) hook function
 * unless the arguments are truly necessary.
 *
 * \sa UT_SetHookFunction
 *
 * \param FuncKey  The stub function to add the hook to.
 * \param HookFunc User defined hook function.  Set NULL to delete/clear an entry.
 * \param UserObj  Arbitrary user data object to pass to the hook function
 */
void UT_SetVaHookFunction(UT_EntryKey_t FuncKey, UT_VaHookFunc_t HookFunc, void *UserObj);

/**
 * Set a variable-argument Handler function for a particular call
 *
 * This allows the user to completely replace the handler for a given function.
 *
 * The handler assumes all responsibility for the final behavior of the stub,
 * including translating return values, and any outputs/side effects the stub
 * function should have.  The default handler for the stub is NOT used.
 *
 * This is identical to UT_SetHandlerFunction() but the function includes a va_list
 * argument to allow use with variadic functions.
 *
 * \param FuncKey  The stub function to add the hook to.
 * \param HandlerFunc User defined hook function.  Set NULL to delete/clear an entry.
 * \param UserObj  Arbitrary user data object to pass to the hook function
 */
void UT_SetVaHandlerFunction(UT_EntryKey_t FuncKey, UT_VaHandlerFunc_t HandlerFunc, void *UserObj);

/**
 * Get a count for the number of times a stub was invoked, at its most recent return value
 *
 * Test cases may check this to determine if a particular stub was called during the test.
 *
 * \param FuncKey The stub function to check.
 * \param Retcode Buffer to store the most recent return code from the stub
 * \param Count   Buffer to store the number of times the stub was invoked
 */
bool UT_GetStubRetcodeAndCount(UT_EntryKey_t FuncKey, int32 *Retcode, int32 *Count);

/**
 * Get a count for the number of times a stub was invoked
 *
 * Test cases may check this to determine if a particular stub was called during the test.
 *
 * \param FuncKey The stub function to check.
 * \return The number of times the stub was invoked
 */
uint32 UT_GetStubCount(UT_EntryKey_t FuncKey);

/**************************************************************
 * Functions for use within stub function implementation
 **************************************************************/

/**
 * Registers a function to be called once per test case
 *
 * This can be used for initialization purposes to preserve state
 * across stub calls.  The first time this is encountered after a
 * reset, the function will be called.  For all subsequent encounters,
 * it will be ignored.
 *
 * \param Func      A function to call once per test case.
 */
void UT_Stub_CallOnce(void (*Func)(void));

/**
 * Copies data from a test-supplied buffer to the local buffer
 *
 * If a UT_SetDataBuffer() option is in place for the given function this
 * will return nonzero and increment the internal usage counter.
 *
 * \param FuncKey The stub function to entry to use.
 * \param LocalBuffer The local (destination) buffer
 * \param MaxSize The maximum size of data to copy
 * \returns The actual size of data copied.  If no data buffer is
 *      supplied by the test harness this will return 0.
 */
size_t UT_Stub_CopyToLocal(UT_EntryKey_t FuncKey, void *LocalBuffer, size_t MaxSize);

/**
 * Copies data from a local buffer to the test-supplied buffer
 *
 * If a UT_SetDataBuffer() option is in place for the given function this
 * will return nonzero and increment the internal usage counter.
 *
 * \param FuncKey The stub function to entry to use.
 * \param LocalBuffer The local (source) buffer
 * \param MaxSize The maximum size of data to copy
 * \returns The actual size of data copied.  If no data buffer is
 *      supplied by the test harness this will return 0.
 */
size_t UT_Stub_CopyFromLocal(UT_EntryKey_t FuncKey, const void *LocalBuffer, size_t MaxSize);

/**
 * Registers a single context element for the hook callback
 *
 * Stubs may pass up to UT_STUBCONTEXT_MAXSIZE arguments to a user-defined
 * hook function.  These arguments are opaque to the stub function and generally
 * passed as "void *" pointers to the actual stack values.  The user code must
 * then cast them to the right type again.
 *
 * This is now implemented as a macro which calls UT_Stub_RegisterContextWithMetaData
 * to associate the name of the argument as well as the pointer.
 *
 * \param FuncKey   The stub function to entry to use.
 * \param Parameter Arbitrary parameter to pass.
 */
#define UT_Stub_RegisterContext(FuncKey, Parameter) \
    UT_Stub_RegisterContextWithMetaData(FuncKey, #Parameter, UT_STUBCONTEXT_ARG_TYPE_UNSPECIFIED, Parameter, 0)

/**
 * Registers a single value argument into the context for the hook callback
 *
 * A pointer to the stack value is actually stored into the context,
 * which can be dereferenced in the hook.
 */
#define UT_Stub_RegisterContextGenericArg(FuncKey, Parameter)                                              \
    UT_Stub_RegisterContextWithMetaData(FuncKey, #Parameter, UT_STUBCONTEXT_ARG_TYPE_INDIRECT, &Parameter, \
                                        sizeof(Parameter))

/**
 * Set a stub return value from the handler function
 *
 * This sets/copies the specified value to the buffer that will be
 * returned from the original stub call back to the caller.  This
 * provides the actual return value and it will override/replace
 * any assumed or default return value.
 *
 * The passed-in buffer should be a pointer to the same data type
 * that the stub returns.  Any type mismatch is considered an error.
 *
 * The handler function must call this routine for any stub which
 * returns a data type other than int32.
 *
 * @note If there is no handler function or the handler does not call
 * this routine to set a return value, the implementation will use the
 * "Int32StatusCode" value as a return if the size matches sizeof(int32).
 *
 * \param FuncKey    The stub function associated with the buffer
 * \param BufferPtr  Pointer to the local return value
 * \param BufferSize Size of the local return value
 */
void UT_Stub_SetReturnValue(UT_EntryKey_t FuncKey, const void *BufferPtr, size_t BufferSize);

/**
 * Creates a buffer to hold the return value for the stub
 *
 * \param FuncKey    The stub function associated with the buffer
 * \param ReturnSize Size of the return value
 * \param TypeName   Expected return value type, as a string
 */
void UT_Stub_RegisterReturnType(UT_EntryKey_t FuncKey, size_t ReturnSize, const char *TypeName);

/**
 * Obtains direct pointer to buffer for stub return value
 *
 * This is a helper routine and not intended to be invoked directly
 * by handler/hook routines.  Use UT_Stub_CopyToReturnValue() to
 * set the return value from a local variable.
 *
 * \sa UT_Stub_CopyToReturnValue()
 *
 * \param FuncKey    The stub function associated with the buffer
 * \param ReturnSize Size of the return value
 * \param TypeName   Expected return value type, as a string
 */
void *UT_Stub_GetReturnValuePtr(UT_EntryKey_t FuncKey, size_t ReturnSize, const char *TypeName);

/**
 * Exports a value from a hook/handler and stages it to be returned to the caller.
 *
 * This is the preferred alternative to using "int32" type returns, since not all
 * functions return int32 status codes.  This method works with any return type.
 *
 * \param FuncKey    The stub function associated with the buffer
 * \param ReturnPtr  Pointer to return value data
 * \param ReturnSize Size of the return value data
 */
void UT_Stub_CopyToReturnValue(UT_EntryKey_t FuncKey, const void *ReturnPtr, size_t ReturnSize);

/**
 * Macro to simplify use of the UT_Stub_CopyToReturnValue() in a hook/handler context.
 * This returns the value stored in the specified variable
 *
 * \param FuncKey     The stub function identifier
 * \param ReturnValue The value to return
 */
#define UT_Stub_SetReturnValue(FuncKey, ReturnValue) \
    UT_Stub_CopyToReturnValue(FuncKey, &ReturnValue, sizeof(ReturnValue))

/**
 * Checks if the int32 status code is set, and optionally gets its value
 *
 * The UT framework is optimized to handle functions that return an int32 status
 * code, as this is a very common pattern in CFE, OSAL, and related applications.
 *
 * In the context of a hook or handler function, this retrieves the value of an
 * int32 status code configured by the test case, either via the "deferred" or
 * the fixed/forced status code APIs, or generated by a previous hook function.
 *
 * This returns whether or not a status code was explicitly set by the test case.
 * If no status code was set, the value will be set to a default (0).
 *
 * \param[in]  Context           The context object that was passed to the hook
 * \param[out] StatusCodeBuffer  If non-NULL, this will be set to the actual status code value
 *                 If status code was not unset, this will be set to the default code value
 *
 * \returns true if code was explicitly set, false if it is not set (and has default value)
 *
 * \note The return value allows a hook/handler function to differentiate between a default
 * status code value that was explicitly set vs. implicitly set.  That is, if the test case
 * explicitly sets a status code of "0", this will return "true", even if the default value
 * is also "0".
 */
bool UT_Stub_GetInt32StatusCode(const UT_StubContext_t *Context, int32 *StatusCodeBuffer);

/**
 * Registers a single context element for the hook callback
 *
 * Stubs may pass up to UT_STUBCONTEXT_MAXSIZE arguments to a user-defined
 * hook function.  These arguments are opaque to the stub function and generally
 * passed as "void *" pointers to the actual stack values.  The user code must
 * then cast them to the right type again.
 *
 * \param FuncKey   The stub function to entry to use.
 * \param Name      Argument name to associate with the pointer
 * \param ParamType The type of parameter (direct, indirect, or unknown)
 * \param ParamPtr  Pointer to argument data
 * \param ParamSize The size of the object pointed to, or zero if not known
 */
void UT_Stub_RegisterContextWithMetaData(UT_EntryKey_t FuncKey, const char *Name, UT_StubContext_Arg_Type_t ParamType,
                                         const void *ParamPtr, size_t ParamSize);

/**
 * Retrieve a context argument value by name
 *
 * This returns a pointer to a buffer containing the value, rather than the
 * value itself, even if the argument was registered originally as a direct value.
 *
 * If the name is not found, this logs a UT assert failure message, as it
 * indicates a mismatch between the hook and stub functions with respect to argument
 * names and (possibly) types that needs to be corrected.  If possible, a buffer
 * containing all zeros may be used as a substitute.
 *
 * This does not return NULL, such that the returned value can always be dereferenced.
 *
 * \param ContextPtr       The context structure containing arguments
 * \param Name             Argument name to find
 * \param ExpectedTypeSize The size of the expected object type
 *
 * \returns Pointer to buffer containing the value.
 */
const void *UT_Hook_GetArgPtr(const UT_StubContext_t *ContextPtr, const char *Name, size_t ExpectedTypeSize);

/**
 * Macro which retrieves a value argument by name.
 *
 * This is a convenience method to easily use UT_Hook_GetArgPtr() to get the value
 * associated with an argument as the correct/expected type.
 *
 */
#define UT_Hook_GetArgValueByName(ContextPtr, Name, Type) \
    (*(Type const *)UT_Hook_GetArgPtr(ContextPtr, Name, sizeof(Type)))

/**
 * Default implementation for a stub function that takes a va_list of arguments.
 *
 * Checks first for a deferred retcode, then for a constant retcode, and a default (0) if neither is present.
 * Optionally also prints a debug level status message to show that the function was called.
 *
 * If a hook function is registered, then it will be called with the stub context
 *
 * Finally, the handler function will be invoked, if one is associated with the stub.  The handler
 * assumes final responsibility for any output parameters or return values.
 *
 * \param FunctionName  The printable name of the actual function called, for the debug message.  If
 *    NULL then no debug message will be generated.
 * \param FuncKey       The Key to look up in the table
 * \param DefaultRc     Default return code
 * \param ArgList       Argument list
 */
int32 UT_DefaultStubImplWithArgs(const char *FunctionName, UT_EntryKey_t FuncKey, int32 DefaultRc, va_list ArgList);

/**
 * Handles a stub call for a variadic function
 *
 * Implements the complete sequence of a stub for a "bare" stub function (i.e. a stub that has no
 * built-in/assumed logic).  This is the preferred stub architecture as it allows a complete override
 * of any and all output behavior / side effects by the test program.
 *
 * The DefaultHandler will be invoked unless the parent program has registered a replacement handler.
 *
 * \note The "generate_stubs.pl" tool can be used to auto-generate bare stub functions that are
 *       compatible with this pattern.
 *
 * \sa UT_DefaultStubImplWithArgs()
 *
 * \param FuncKey        The key of the stub being executed
 * \param FunctionName   The printable name of the actual function called, for the debug message.
 * \param DefaultHandler The default handler
 * \param VaList         Argument list
 */
void UT_ExecuteVaHandler(UT_EntryKey_t FuncKey, const char *FunctionName, UT_VaHandlerFunc_t DefaultHandler,
                         va_list VaList);

/**
 * Default implementation for a stub function that should be useful for most cases.
 *
 * This is a wrapper around UT_DefaultStubImplWithArgs() with a variable argument prototype.
 *
 * \param FunctionName  The printable name of the actual function called, for the debug message.  If
 *    NULL then no debug message will be generated.
 * \param FuncKey       The Key to look up in the table
 * \param DefaultRc     The default code to be returned when no code is registered in the UT state
 */
int32 UT_DefaultStubImpl(const char *FunctionName, UT_EntryKey_t FuncKey, int32 DefaultRc, ...);

/**
 * Handles a stub call for a normal (non-variadic) function
 *
 * Implements the complete sequence of a stub for a "bare" stub function (i.e. a stub that has no
 * built-in/assumed logic).  This is the preferred stub architecture as it allows a complete override
 * of any and all output behavior / side effects by the test program.
 *
 * The DefaultHandler will be invoked unless the parent program has registered a replacement handler.
 *
 * \note The "generate_stubs.pl" tool can be used to auto-generate bare stub functions that are
 *       compatible with this pattern.
 *
 * \sa UT_DefaultStubImplWithArgs()
 *
 * \param FuncKey        The key of the stub being executed
 * \param FunctionName   The printable name of the actual function called, for the debug message.
 * \param DefaultHandler The default handler
 */
void UT_ExecuteBasicHandler(UT_EntryKey_t FuncKey, const char *FunctionName, UT_HandlerFunc_t DefaultHandler);

/**
 * Macro to simplify usage of the UT_DefaultStubImpl() function
 *
 * The UT_DEFAULT_IMPL can be used as part of a larger implementation
 * where the default items of deferred return codes / force fails are
 * checked first, then additional functionality is added.
 *
 * This version should be used on stubs that take no arguments
 * and are expected to return 0 in the nominal case
 *
 * NOTE - Adding a NULL to the va list is only done for the
 *        two macros that do not have a va list passed in by the
 *        caller and is NOT a general pattern. Hooks that handle
 *        va lists should utilize the UT_KEY to process
 *        va lists correctly based on the implementation (no
 *        general pattern should be assumed).
 */
#define UT_DEFAULT_IMPL(FuncName) UT_DefaultStubImpl(#FuncName, UT_KEY(FuncName), 0, NULL)

/**
 * Macro to simplify usage of the UT_DefaultStubImpl() function
 *
 * The UT_DEFAULT_IMPL_RC can be used as part of a larger implementation
 * where the default items of deferred return codes / force fails are
 * checked first, then additional functionality is added.
 *
 * This version should be used on stubs that take no arguments
 * and are expected to return nonzero in the nominal case
 *
 * NOTE - Adding a NULL to the va list is only done for the
 *        two macros that do not have a va list passed in by the
 *        caller and is NOT a general pattern. Hooks that handle
 *        va lists should utilize the UT_KEY to process
 *        va lists correctly based on the implementation (no
 *        general pattern should be assumed).
 */
#define UT_DEFAULT_IMPL_RC(FuncName, Rc) UT_DefaultStubImpl(#FuncName, UT_KEY(FuncName), Rc, NULL)

/**
 * Macro to simplify usage of the UT_DefaultStubImpl() function
 *
 * The UT_DEFAULT_IMPL_ARGS can be used as part of a larger implementation
 * where the default items of deferred return codes / force fails are
 * checked first, then additional functionality is added.
 *
 * This version should be used on stubs that do take arguments
 * and are expected to return 0 in the nominal case
 */
#define UT_DEFAULT_IMPL_ARGS(FuncName, ...) UT_DefaultStubImpl(#FuncName, UT_KEY(FuncName), 0, __VA_ARGS__)

/**
 * Macro to simplify usage of the UT_DefaultStubImpl() function
 *
 * The UT_DEFAULT_IMPL_RC_ARGS can be used as part of a larger implementation
 * where the default items of deferred return codes / force fails are
 * checked first, then additional functionality is added.
 */
#define UT_DEFAULT_IMPL_RC_ARGS(FuncName, Rc, ...) UT_DefaultStubImpl(#FuncName, UT_KEY(FuncName), Rc, __VA_ARGS__)

/**
 * Macro to simplify usage of the UT_DefaultStubImplWithArgs() function
 *
 * This function accepts a list of arguments as a va_list
 */
#define UT_DEFAULT_IMPL_VARARGS(FuncName, va) UT_DefaultStubImplWithArgs(#FuncName, UT_KEY(FuncName), 0, va)

/**
 * Macro to simplify usage of the UT_DefaultStubImplWithArgs() function
 *
 * This function accepts a list of arguments as a va_list and
 * a nonzero default return code
 */
#define UT_DEFAULT_IMPL_RC_VARARGS(FuncName, Rc, va) UT_DefaultStubImplWithArgs(#FuncName, UT_KEY(FuncName), Rc, va)

/**
 * Macro to simplify usage of the UT_DefaultStubImpl() function
 *
 * The UT_DEFAULT_STUB can be used as a complete stub implementation
 * when only deferred return codes / force fails are in use.
 */
#define UT_DEFAULT_STUB(FuncName, Args)   \
    int32 FuncName Args                   \
    {                                     \
        return UT_DEFAULT_IMPL(FuncName); \
    }

#endif /* UTSTUBS_H */
