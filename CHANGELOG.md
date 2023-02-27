# Changelog

## Development Build: v6.0.0-rc4+dev199
- Modify unreachable branch in OS_ObjectIdAllocateNew
- See <https://github.com/nasa/osal/pull/1327>

## Development Build: v6.0.0-rc4+dev195
- removed cast that hid bug at compile time
- See <https://github.com/nasa/osal/pull/1367>

## Development Build: v6.0.0-rc4+dev191
- add doxygen for UT_ConfigureGenericStubReturnValue
- See <https://github.com/nasa/osal/pull/1362>

## Development Build: v6.0.0-rc4+dev187
- UT assert stub return values
- See <https://github.com/nasa/osal/pull/1352>

## Development Build: v6.0.0-rc4+dev184
- separate append on volume_name to system_mountpt
- See <https://github.com/nasa/osal/pull/1355>

## Development Build: v6.0.0-rc4+dev179
- Remove obsolete _USING_RTEMS_INCLUDES_
- Support adding default flags at task creation
- See <https://github.com/nasa/osal/pull/1348> and <https://github.com/nasa/osal/pull/1347>

## Development Build: v6.0.0-rc4+dev173
- Update action versions
- add doc-prebuild dependency
- Update incorrect function header comment
- See <https://github.com/nasa/osal/pull/1342>, <https://github.com/nasa/osal/pull/1341>, and <https://github.com/nasa/osal/pull/1321>

## Development Build: v6.0.0-rc4+dev165
- Update CI to use Ubuntu 20.04
- See <https://github.com/nasa/osal/pull/1334>

## Development Build: v6.0.0-rc4+dev161
- Truncate symbol name if > OS_MAX_SYM_LEN
- always provide UT stub library
- cppcheck updates
- See <https://github.com/nasa/osal/pull/1335>, <https://github.com/nasa/osal/pull/1329>, and <https://github.com/nasa/osal/pull/1336>

## Development Build: v6.0.0-rc4+dev151
- add BUGCHECK_VOID macro
- See <https://github.com/nasa/osal/pull/1323>

## Development Build: v6.0.0-rc4+dev147
- adjust timing on condvar tests
- See <https://github.com/nasa/osal/pull/1319>

## Development Build: v6.0.0-rc4+dev143
- locks for condvar objects on rtems/vxworks
- Add time conversion reciprocal functions
- add export targets and package script
- Stop empty function catching CI workflow format checks
- Remove redundant/inconsistent comments (/* end of function */, /* end if */ etc.) and clean up empty lines.
- See <https://github.com/nasa/osal/pull/1311>, <https://github.com/nasa/osal/pull/1310>, <https://github.com/nasa/osal/pull/1313>, <https://github.com/nasa/osal/pull/1306>, and <https://github.com/nasa/osal/pull/1309>

## Development Build: v6.0.0-rc4+dev131
- rtems queue multi-size, mimic posix logic
- cmake script modernization
- Create CHANGELOG.md
- See <https://github.com/nasa/osal/pull/1283>, <https://github.com/nasa/osal/pull/1291>, and <https://github.com/nasa/osal/pull/1298>

## Development Build: v6.0.0-rc4+dev120
- Fix #1300, do not set file permissions on UT assert outputs
- See <https://github.com/nasa/osal/pull/1301>

## Development Build: v6.0.0-rc4+dev116
- Use fstat and fchmod for TOCTOU Bug
- Remove 'return;' from last line of void functions.
- Remove unnecessary parentheses around return values.
- See <https://github.com/nasa/osal/pull/1181>, <https://github.com/nasa/osal/pull/1294>, and <https://github.com/nasa/osal/pull/1292>

## Development Build: v6.0.0-rc4+dev108
- condition variable API for OSAL
- See <https://github.com/nasa/osal/issues/1252>

## Development Build: v6.0.0-rc4+dev103
- Remove stray terminators
- Add RTEMS timebase callback wrapper
- See <https://github.com/nasa/osal/pull/1274> and <https://github.com/nasa/osal/pull/1276>

## Development Build: v6.0.0-rc4+dev97
- Ensure address alignment of TCB for VxWorks 7
- See <https://github.com/nasa/osal/pull/1272>

## Development Build: v6.0.0-rc4+dev93
- Update cmake mimimum required to something more recent (or remove)
- Set OSAL Loader unit test number of modules to OSAL_CONFIG_MAX_MODULES
- See <https://github.com/nasa/osal/pull/1267> and <https://github.com/nasa/osal/pull/1265>

## Development Build: v6.0.0-rc4+dev87
- Add OS_StatusToString API
- See <https://github.com/nasa/cFS/pull/505> 

## Development Build: v6.0.0-rc4+dev83
- Resolve UT uninit vars static analysis warnings
- Remove redundant checks in UT
- Resolve UT uninit vars static analysis warnings
- Add RTEMS 6 Support  
- See https://github.com/nasa/osal/pull/1260 and <https://github.com/nasa/cFS/pull/492>

## Development Build: v6.0.0-rc4+dev74
- Update codeql workflows for reusable update
- add os-impl-no-select.c for RTEMS
- See <https://github.com/nasa/osal/pull/1251> and <https://github.com/nasa/cFS/pull/471>

## Development Build: v6.0.0-rc4+dev66
- Fix UtAssert_StringBufCompare SEGV when -1 for size passed in (on CentOS 7)
- Enable format check workflow on push to all branches
- Update Copyright Headers
- Standardize version.h 
- See <https://github.com/nasa/osal/pull/1239> and <https://github.com/nasa/cFS/445>

## Development Build: v6.0.0-rc4+dev54
- Revert development version identifier to 99 for revision number
- Fix cFS-build CodeQL reusable worfklow
- Implement OS_ModuleGetInfo_Impl for RTEMS
- Doxygen strip path and expose common config
- Refactor doxygen and remove mainpage
- See <https://github.com/nasa/osal/pull/1233> and <https://github.com/nasa/cFS/pull/443>

## Development Build: v6.0.0-rc4+dev42
- Resolve UT uninitialized variable warnings
- Add ut_assert to doxygen and fix warnings
- Protect if OS_FDGetInfo called on socket
- See <https://github.com/nasa/osal/pull/1226> and <https://github.com/nasa/cFS/pull/432>

## Development Build: v6.0.0-rc4+dev32
- Typesafe definition of osal_id_t
- See <https://github.com/nasa/osal/pull/1209> and <https://github.com/nasa/cFS/pull/414>

## Development Build: v6.0.0-rc4+dev29
- Search global and local symbol tables
- Add missing space to UtAssert_STUB_COUNT
- Add UINT8 and UINT16 equivalents for UtAssert_UINT32_ macros
- correct warnings on gcc11
- Set output in OS_stat handler
- Reuse CodeQL, Static Analysis, Format Check
- See <https://github.com/nasa/osal/pull/1212> and <https://github.com/nasa/cFS/pull/410>

## Development Build: v6.0.0-rc4+dev15
- Add Duplicate Check to Local Unit Test
- Rename OS_XXXTime to OS_XXXLocalTime in comments
- Fix vxWorks intLib stub aliasing issue
- Recognize ifdef __cplusplus
- Remove extern in stub prototypes
- Add typedef for OSAL status codes
- Set new build baseline for cFS-Caelum-rc4: v6.0.0-rc4
- See <https://github.com/nasa/osal/pull/1191> and <https://github.com/nasa/cFS/pull/390>

## Development Build: v5.1.0-rc1+dev619
- Enable symbol api test and MIR dump too large
- MIR symbol too long or table to long for osloader test
- Add bsp-specific configuration flag registry 
- Add os-specifc socket flag function 
- See <https://github.com/nasa/osal/pull/1158> and <https://github.com/nasa/cFS/pull/359>

## Development Build: v5.1.0-rc1+dev604
- Add typecast to memchr call
- Regex update in coverage enforcement to match `.0`
- See <https://github.com/nasa/osal/pull/1145> and <https://github.com/nasa/cFS/pull/351>

## Development Build: v5.1.0-rc1+dev598
- Add UTAssert macros that help test bit field storage
- See <https://github.com/nasa/osal/pull/1138> and <https://github.com/nasa/cFS/pull/348>

## Development Build: v5.1.0-rc1+dev594
- Add test case types similar to NA
- See <https://github.com/nasa/osal/pull/1132> and <https://github.com/nasa/cFS/pull/340>

## Development Build: v5.1.0-rc1+dev590
- Add absolute branch coverage check 
- See <https://github.com/nasa/osal/pull/1127> and <https://github.com/nasa/cFS/pull/333>

## Development Build: v5.1.0-rc1+dev586
- Add UtAssert_MIR macro
- Add generic asserts from CFE coverage testing 
- Add osapi-shell-stubs.c to OSAL stub library
- See <https://github.com/nasa/osal/pull/1126> and <https://github.com/nasa/cFS/pull/328>

## Development Build: v5.1.0-rc1+dev578
- Add unit test branch coverage
- See <https://github.com/nasa/osal/pull/1116> and <https://github.com/nasa/cfs/pull/313>

## Development Build: v5.1.0-rc1+dev573
- Add independent OS_rename functional test parameter checks
- See <https://github.com/nasa/osal/pull/1113> and <https://github.com/nasa/cfs/pull/311>

## Development Build: v5.1.0-rc1+dev569
- Skip these "timer reconfig" unit tests on non-POSIX platforms. Add documentation clearly indicating that the API must not be called from a timer context.
- See <https://github.com/nasa/osal/pull/1100> and <https://github.com/nasa/cFS/pull/297>

## Development Build: v5.1.0-rc1+dev564
- Add range to OS_TaskDelay checks
- Return `OS_OBJECT_ID_UNDEFINED` for root task ID on RTEMS
- Increase timeout in network-api-test
- Avoid task delete during UtPrintf
- Increase UT symbol dump size limit
- Do not register RTOS timer for external sync
- Add osal# prefix to Network API group
- Increase timeout in network-api-test
- See <https://github.com/nasa/osal/pull/1098> and <https://github.com/nasa/cfs/pull/287>

## Development Build: v5.1.0-rc1+dev548
- implement missing parameter/retcode test permutations
- See <https://github.com/nasa/osal/pull/1079> and <https://github.com/nasa/cFS/pull/270>

## Development Build: v5.1.0-rc1+dev530
- Implement Coding Standard Rules in CodeQL
- rename doc to docs
- add mutex lock around UtAssert globals
- resolve discrepancies between file API and unit tests
- resolve discrepancies between filesys API and unit tests
- resolve discrepancies between idmap API and unit tests
- resolve discrepancies between select API and unit tests
- resolve discrepancies between socket API and unit tests
- const correct OS_SelectFdIsSet
- Add missing OS_Select param doc
- Add Workflow to build and verify OSAL API Guide
- add detail design template
- Update error codes and documentation
- Increase UT object limit for testing
- See <https://github.com/nasa/osal/pull/1076> and <https://github.com/nasa/cFS/pull/265>

## Development Build: v5.1.0-rc1+dev501
- include doxygen targets locally
- resolve discrepancies between timer API and unit tests
- Ensure correlation between the test cases and documented return values for the OSAL timer API.
- resolve discrepancies between binsem API and unit tests
- add missing clock retcode tests
- resolve discrepancies between common API and unit tests
- resolve discrepancies between countsem API and unit tests
- resolve discrepancies between dir API and unit tests
- resolve discrepancies between module API and unit tests
- resolve discrepancies between mutex API and unit tests
- resolve discrepancies between queue API and unit tests
- resolve discrepancies between task API and unit tests
- resolve discrepancies between timebase API and unit tests
- check misc API return codes
- Rename CodeQL cFE Build and add Duplicate Job
- filter only whole words for keyword match
- update OSAL Config Guide link
- See <https://github.com/nasa/osal/pull/1058> and <https://github.com/nasa/cFS/pull/263>

## Development Build: v5.1.0-rc1+dev458
- Adds Counting Semaphore Test with timeouts
- Removes chance of deadlock by ensuring OS_TaskGetId returns the task ID it knows about, regardless of whether the task is pending cancellation or not. Defers cancellation of the task while the BSP is locked, ensure it reaches the unlock, then restores the previous cancel state.
- Fixes a bogus debug message about unlocking from the wrong task if the task is pending delete.
See <https://github.com/nasa/osal/pull/1050> and <https://github.com/nasa/cFS/pull/260>

## Development Build: v5.1.0-rc1+dev452
- Makes filenames better match terms used in implementation.
- Propagates return code from the caller in low-level implementations when status is not `OS_SUCCESS`.
- The status returned from `OS_rmdir()` should now be correct if the implementation failed.
- Adds details to log file and can be cross referenced back to documentation.
- Updates most of the conditions in the "unit tests" to use the support macros whenever possible.
- The macros display the function being called as well as the return value being checked for and enables that documented return codes are being tested. Removes repetition in the test programs.
- Removes blank lines in test log while preserving extra newlines in the event that the test case was intentionally adding whitespace.
- Adds tests for object id inline functions: `OS_ObjectIdToInteger`, `OS_ObjectIdFromInteger`, `OS_ObjectIdEqual`, `OS_ObjectIdDefined`
- Changes tests to use valid inputs for all `OS_mkdir` calls.
- [docs] Improves config guide documentation issues referenced in #384, #640, #672
- [continuous-integration] Adds new workflow to display CodeQL alerts preview

## Development Build: v5.1.0-rc1+dev434
- Updates the configuration guide to reflect cmake build system and the currently provided set of OS/BSP layers. Removes obsolete items from guide.
- Converts UtAssert API documentation to doxygen and corrects stale and incorrect references.
- Clarifies use of the stack_size parameter, and that it should not be 0.
- Ensures consistency between the return values generated by each OSAL API and the doxygen documentation.
- Fixes missing or incorrect explicitly-returned status code in the coverage tests.
- Corrects one argument name mismatch in QueueCreate where it was prototyped as "data_size" but implemented as "max_size".
- See <https://github.com/nasa/osal/pull/996> and <https://github.com/nasa/cfs/pull/256>

## Development Build: v5.1.0-rc1+dev417
- Fixes infinite loop  in `UtPrintx()`. Adds the data's memory address to output. Note, UtPrintf displays the file/line of the `UtPrintx` function, **not the actual test location**; it is better to call `UT_BSP_DoText` directly.
- Adds `OS_SocketShutdown()` API wrapper around BSD's socket shutdown() API. This allows a data transfer of a stream socket to be gracefully shut down prior to socket closure.
- See osal <https://github.com/nasa/osal/pull/979> and <https://github.com/nasa/cFS/pull/252>

## Development Build: v5.1.0-rc1+dev411
- [docs] Clarifies that zero will be returned on EOF condition in the API documentation for OS_read/write/TimedRead/TimedWrite. In the case of the timed API calls, the `OS_ERR_TIMEOUT` status code will be returned if the timeout expired without the handle becoming readable/writable during that time.
- Addresses shortcomings in the UT Assert hook functions. Namely the assumed return type of int32 which is not always the case.
- Adds the concept of a "handler" function to UT assert to replace hard-coded custom logic in UT assert. A handler is the custom logic that exists between the hook function and the return to the stub caller. The handler is directly responsible for setting all outputs.
- Adds a script to auto-generate stub functions that match this pattern. Given an API header file, the script extracts
the declarations, and generates a source file with stub definitions that rely on a separate handler to deal with the needed outputs.
- Refactors `os-shared-printf.h`) into two parts to improve the compatibility with the script method.
- Updates all existing stubs in OSAL to use the auto-generated stub logic from the script, created directly from the C header. This ensures that stubs will match the FSW implementation.
- [continuous-integration] Adds a local osal-specific makefile to help build unit tests. Adds a new github workflow that runs the unit tests in both the context of the bundle configuration and the local OSAL config. Verifies 100% line coverage.
- Fixes incorrect token use in `OS_SocketAccept`. Enables the `network-api-test` to handle multiple connections that re-use the same acceptor socket between them.
- Promotes the `OS_CONFIG_CONSOLE_ASYNC` option into the shared layer to remove duplicate implementation code and add more coverage testing.
- Adds an osconfig option to allow the user to elect this mode at configuration time.

## Development Build: v5.1.0-rc1+dev393
- Changes parameter names to avoid collisions. Renames `access` as `access_mode` in `osapi-file.h`. Renames `time` as `TimeSp` in `os-impl-posix-gettime.c`.
- Deletes the broken RTEMS `os-impl-shell.c` file so OSAL builds with `OSAL_CONFIG_INCLUDE_SHELL=true`. RTEMS will always report `OS_ERR_NOT_IMPLEMENTED`.
- See <https://github.com/nasa/osal/pull/967> and <https://github.com/nasa/cFS/pull/248>

## Development Build: v5.1.0-rc1+dev387
- Replaces the separate "Initialized" and "Shutdown" flags with a single state flag. Creates a global single source of truth for the OSAL state. This enables users to run tests and OS_API_Init() multiple times without a reboot in the middle to reset the state.
  - Multiple invocations of OS_API_Init() are allowed - subsequent calls can be ignored
  - Deleting of any internal objects that did get created if OS_API_Init() fails (this leaves system in same state as when it started)
  - Allows Re-initialization of OSAL after OS_ApplicationShutdown() - may be relevant when running unit tests several times without rebooting.
- Adds OS_API_Teardown to complement OS_API_Init. This cleans up all OSAL resources ideally leaving the system in a state where `OS_API_Init()` may be invoked again.
- Reworks the shell unit test which was probably not working. Note this requires modifying the osal config to enable shell, otherwise test is skipped.
- See <https://github.com/nasa/osal/pull/956> and <https://github.com/nasa/cFS/pull/242>

## Development Build: v5.1.0-rc1+dev378
- Replaces nonstandard header file block comments and include guards. No behavior changes
- Removes `CLOCK_MONOTONIC` as the osal clock source since PSP no longer needs it. `OS_GetLocalTime()` and `OS_SetLocalTime()` will work as described.
- Replaces `shellName` with a specific `localShellName` that can be polled safely and changes its type to a char of `OS_MAX_API_NAME` length for safety.
- See <https://github.com/nasa/osal/pull/951> and <https://github.com/nasa/cFS/pull/238>

## Development Build: v5.1.0-rc1+dev367
- Removes `SOFTWARE_BIG_BIT_ORDER` and `SOFTWARE_LITTLE_BIT_ORDER` macros from `common_types.h`. These are not needed by OSAL and cannot handle all cases.  Application code with endianness dependency that was relying on these symbols may break. Users should leverage code in cFE: `cfe_endian.h`. See <https://github.com/nasa/cFE/pull/1218> for more details.
- Applies minor code and documentation cleanup: white space, typos, etc.
- Adds test to get full coverage of vxworks in `os-impl-bsd-socket.c` resulting in full line coverage for OSAL
- Adds more descriptive return codes if `OS_SymbolTableDump_Impl` does not do what is expected. Adds a new error `OS_ERR_OUTPUT_TOO_LARGE` if the size limit was insufficient. Return `OS_ERROR` if an empty file was written - this likely indicates some fundamental issue with the VxWorks symbol table. Returns `OS_ERR_NAME_TOO_LONG` if one of the symbol names was too long. Improves unit test to check for/verify these responses.
- Removes the unneeded `OS_TaskRegister()` and all references to it in code, tests, and documentation. No impact to behavior, but does affect API and has dependencies
- Removes unused `-SCRIPT_MODE` flag in cmake
- Remove comparison between `osal_id_t` and `integers` to use the provided comparison function, `OS_ObjectIdDefined()`. System builds and runs again when using a type-safe/non-integer osal_id_t type.
- See <https://github.com/nasa/osal/pull/927>

## Development Build: v5.1.0-rc1+dev350
- Moves copyblock size to a macro and add comments. Defines `OS_CP_BLOCK_SIZE` and adds clear documentation that it could be adjusted for page size, performance, etc.
- Removes while loop
- Replaces all #includes of <os and <OSC_ matches with " to match coding standard.
- Consolidates the duplicated switch in `OS_SocketOpen_Impl`
- Add const to input pointers for `OS_FdSet_ConvertIn_Impl` and `OS_ObjectIdTransactionFinish`
- Removes network prototypes defined in `osapi_sockets.h` that are also in `osapi_network.h`
- Removes `NULL` redefine from `common_types.h`
- Adds `Contributing.md` that points to bundle-level contribution guide
- Reports test cases that "fail" as "not implemented" with new `UtAssert_NA` macro instead of `UtPrintf`
- Calls to `OS_SelectSingle` and `OS_SelectMultiple` will fail if an FD within the set is outside the range of the underlying `FD_SETSIZE` from the C library.
- Fixes calculation used for the relative time interval in the `select()` call. Also adds a UT case that specifically exercises the carryover described. Fixes delay when this carry condition is hit
- Documents algorithm that provides application-controlled timeout on the connection initiation. Also adds a debug statement if the connect fails for a reason other than `EINPROGRESS`. No impact to normal behavior.
- Adds check for `EAGAIN` if the system fails to allocate kernel-internal resources.
- Adds a `CompileTimeAssert` to confirm that the size of the abstract buffer for socket addresses is large enough to store any of the enabled address types thereby removing the need for runtime tests.
- With this change, if `OS_SOCKADDR_MAX_LENis` not large enough for the address type, osal will fail to compile. This enforces that the abstract size is large enough for any/all enabled address types, regardless of what is actually used.
- Adds missing functional test for `OS_ShellOutputToFile`
- Add test for `fcntl()` error return of -1 and report errno. If setting `O_NONBLOCK` fails, then debug message is printed and blocking mode is used and timeouts will not work as a result.
- Improves error codes when attempting to seek on a pipe/socket. Translates the `OS_ERR_OPERATION_NOT_SUPPORTED` error rather than "not implemented". The `ESPIPE` errno means that seeking is not supported on the given file handle.
- Renames `OS_U32ValueWrapper_t` as `OS_VoidPtrValueWrapper_t` to better indicate its purpose. The point is to pass a value through a `void*`. Adds a compile-time assert to check that this is only used to directly pass values which have a size of less than or equal to sizeof(void*).
- Refactors the return statement for `OS_FileSys_FindVirtMountPoint()` so it is easier to read and adds some informational comments.
- Reports an error if calling `timer_gettime` after `timer_settime` fails.
- Returns `OS_ERROR` status to caller after an error on moduleInfoGet()
- Removes an extraneous/unreachable OS_ObjectIdDefined check and its accompanying debug statement. The only way this check could have been reached would be if the normal unlock process was bypassed such that the underlying OS mutex was unlocked but OSAL state still had it owned by a task. This condition never happens at runtime.
- Updates documentation for `OS_MAX_MODULE`
- See <https://github.com/nasa/osal/pull/917>

## Development Build: v5.1.0-rc1+dev297
- Add Testing Tools to the Security Policy
- See <https://github.com/nasa/osal/pull/838>

## Development Build: v5.1.0-rc1+dev293
- Avoids various "possible uninitialized variable" warnings for routines that utilize this API.
- Renames `sockaddr*` structures to `sa*` to deconflict from structure name in `os-impl-bsd-sockets.c`. Adds `OS_NETWORK_SUPPORTS_IPV6` to `os-impl-bsd-sockets.c` compilation. Renames `bsd-select-stubs.c` to `sys-select-stubs.c`. Coverage now includes all currently possible files in VxWorks build
- Resolves CodeQL security warning by restricting permissions on file create.
- Changes comments using "cpp" comment style to "c" style
- Adds _new_ accessor functions APIs to get version strings and return the values of string macros defined in `osapi-version.h`.
  - The "simple" version currently `OS_VERSION` macro - this is the semantic version without any extra detail.  This is returned by `OS_GetVersion()`.
  - The "descriptive" version in `OS_VERSION_STRING` macro - this has extra detail like the most recent official release.  This is returned by `OS_GetVersionDescription()`.
  - The release code name, now returned by `OS_GetVersionDescription()`.
- These accessor functions are the preferred way to get the OSAL version string, from now on users should avoid using the macro definitions as it is evaluated at OSAL library compile time, rather than application compile time, and thus will remain correct in the event that OSAL is relinked without recompiling the application.
Adds `osapi-version.c` to implement these 3 calls and associated coverage test. This allows the version.c file to be auto-generated in the future.
- See <https://github.com/nasa/osal/pull/835>

## Development Build: v5.1.0-rc1+dev280
- Makes tests skip after getting their first not implemented error.
- Updates stub helpers to match the behavior of calling the default implementation stub macro (NULL VA list)
- Removes redundant logic and assignment to fix static analysis warnings
- Truncates at the end of the logic flow for socket name as opposed to possibly 3 different locations. Fixes static analysis warning.
- Renames `timer_id` in unit tests to `local_timer_id` to avoid conflicts
- Removes all deprecated elements
- No behavior change. Renames `UT_Stub_CheckForceFail` to `UT_Stub_CheckDefaultReturnValue`, also only sets `Value` if not `NULL` (can pass in NULL value doesn't matter)
- See <https://github.com/nasa/osal/pull/830>

## Development Build: v5.1.0-rc1+dev262
- Adds test cases for `OS_ObjectIdFinalizeDelete`, `OS_DeleteAllObjects`, and others to get 100% line and function coverage on VxWorks and shared/portable layers.
- Ensures APIs check for `NULL` inputs or have documentation stating that a null value is allowed.
- Adds timeout to static analysis check and adds format check. Removes old .travis.yml and updates badges in readme.
- Adds Code QL analysis on push to main and pull requests (to main)
- Cleans commented-out code in tests to address static analysis warnings
- Initializes local variables to avoid returning uninitialized values from stubs and address static-analysis findings
- Replaces two local return codes defined as `uint32` with `int32` to resolve static-analysis warnings
- Simplifies switch statements based on previous checks. Removes unreachable, dead code to resolve static-analysis warnings
- Terminates  unit test macros variadic lists with `NULL` to address CWE-121 CodeQL warning
- Adds a check to send the semaphore to avoid unreachable code
- Adds a status return to `OS_ConsoleAPI_Init` so debug warnings will get reported correctly on errors.
- Declares `DummyVec` as static to avoid warning and returning stack allocated memory when returning `VecTbl` in `OSC_INUM_TO_IVEC` stub
- Updates types in `os-impl-no-symtab.c` to match latest APIs
- Updates types in `os-impl-no-symtab.c` to match latest APIs
- Fixes missing `NULL` terminations and applies the standard "sizeof" pattern where appropriate. No longer truncates filename in `OS_ModuleInfo`.
- Fixes `%u` conversion in RTEMS so to address build warning
- Create a wrapper around `memchr()` that mimics the non-C99 function `strnlen()` defined in POSIX-2008. Use this instead of `strlen()` whenever the string being checked either originates in or will be copied into a fixed-length array buffer. No behavior changes except if a bug causes strings to be unterminated.
- No behavior change, applies the standard formatting using `clang-format`
- See <https://github.com/nasa/osal/pull/774>

## Development Build: v5.1.0-rc1+dev221
- Fixes `printf` format to correctly build in RTEMS-5.
- **Deprecates `OS_fsBlocksFree()` and `OS_fsBytesFree()`** in favor of `OS_FileSysStatVolume()`.
- Adds `Security.md` with instructions to report vulnerabilities.
- Add `UtDebug` in `OS_printf` stub. Output the `OS_printf` input as a debug message from stub.
- Documentation: Add note on `UtTest_Add` API. Nesting `UtTest_Add` from within an added test fails without error.
- Unit Test: No more garbage characters written to test report log
- Fix typo in `osapi.h` affecting C++ build. No other functional change
- Unit Test: Rename `UT_ClearForceFail` as `UT_ClearDefaultValue`. Update the comments of `UT_SetDefaultReturnValue` to match the more general function.
- Unit Test: Add test teardown failures to the test summary and changed the printout to use the same style as startup failures.
- Unit Test: Removes no longer applicable `UT_CheckForOpenSockets` since the UT framework resets the state for each unit test.
- Changes the file-create operation to read-write permissions to work on RTEMS
- Unit Test: Fixes incorrect assertions in `network-api-test` to correctly check return values.
- Unit Test: Generalizes queue timeout test to also test message queue functionality to validate settings and permissions to work with mqueues.
- Implements `OS_time_t` with a single 64-bit tick counter rather than a split 32 bit seconds + 32 bit microseconds counter.
- Unit Test: Installs the modules used in unit testing and adds removal of post-test, left-over files.
- See <https://github.com/nasa/osal/pulls/767>

## Development Build: v5.1.0-rc1+dev184
- Address issues with OSAL global table management:
  - use iterators whenever possible
  - use an unlock key rather than task ID so OS_TaskExit() doesn't trigger a warning
  - general cleanup of lock/unlock impl and remove redundant logic
  - unlock global tables during create/delete
  - keep threads "attached" in POSIX, so they can be joined when deleted.
- No longer triggers warning with OS_TaskExit() on VxWorks (see #645)
- `OS_TaskDelete()` on POSIX does not return until the task has actually exited (see #642)
- The chmod test is now skipped on VxWorks rather than failing. The `OS_FileChmod_Impl()` function now returns `OS_ERR_NOT_IMPLEMENTED` when run on a file system that does not have permissions, which in turn causes the unit test to be skipped rather than fail.
- Corrects a file handle leak.
-  Add parameter check to `OS_SocketSendTo` and adjust coverage test to validate.
- Replace `OS_fsBytesFree` and `OS_fsBlocksFree` with `OS_FileSysStatVolume`. This new API for getting stats on file system. Uses existing `OS_FileSysStatVolume_Impl` call and exposes it in the public API.
- When cleaning up for shutdown, delete resources that have a task/thread first, followed by other resource types. This helps avoid possible dependencies as running threads might be using the other resources. No detectable external impact; internally, the tasks are deleted first during shutdown, which only has an impact if/when tasks are actively using other OSAL resources.
- The mount/unmount *VxWorks* implementation was not adequately checking for and handling the `FS_BASED` pass -through mapping type - which should be mostly a no-op. Create a mount point directory if it does not already exist when using this mapping type for consistency with POSIX.
- Adds a documentation note to `OS_FileSysAddFixedMap()`: The virtual mount point cannot be empty - so `OS_FileSysAddFixedMap(.., "/", "/")` does not work but `OS_FileSysAddFixedMap(.., "/", "/root")` does work and allows one to open files in the root as `"/root/"` from OSAL applications. Mount-point directories do not need to previously exist when using OS_FileSysAddFixedMap
- store `taskTCB` return in a `void *`, then cast to `OS_impl_task_internal_record_t *` to avoid a strict alignment compiler error
- Removes the non-portable `OS_PACK` and `OS_ALIGNED` macros.
- Uses the POSIX dir implementation on VxWorks 6.9. The only incompatibility is the prototype for `mkdir()`which is missing the second argument; this is worked around with a compatibility macro for VxWorks 6.x builds.
- Translate and convert the VxWorks coverage test cases to the portable dir implementation, which benefits VxWorks7, RTEMS, and POSIX.
- Fixes prototypes so they run on RTEMS by replacing uint32 with size_t
- Adds` OS_CHECK_POINTER` macros to `OS_ConvertToArrayIndex` and `OS_TimeBaseGetFreeRun` so they can handle NULL pointers and return the correct error.
- Adds access functions to convert/extract different units from an OS_time_t value - so that other code in CFE/PSP/Apps can be updated to use the access functions and thereby not break when the internal time definition changes. Replaces the `int32` with `OS_time_t` in the "stat" structure used by the file module. Updates the pointer argument to `OS_SetLocalTime()` to be `const`. Prototype change of `OS_SetLocalTime()` should be backward compatible.
- See <https://github.com/nasa/osal/pulls/750>

## Development Build: v5.1.0-rc1+dev149
- Document UtAssert_Message parameters, also adds "see also" note for helper macros.
- Fix doxygen typo
- Replace `OS_BinSemFlush` with `OS_BinSemGive` to prevent a rare race condition. Change the port numbers to be different from network test for when tests are run in parallel.
- Fix doxygen format errors. Usersguide now builds without warnings.
- Suppress invalid cppcheck warning in `OS_WaitForStateChange`
- Add cppcheck static analysis workflow to osal CI
- See <https://github.com/nasa/osal/pull/744>

## Development Build: v5.1.0-rc1+dev132
- Convert the OSAL Configuration Guide from docx and pdf to a markdown file.
- Test Tasks do not run at 100%. Move all definitions and instantiations out of the core-test header file and reuse the already-existing single task definition.
- Break up `osapi-os-*.h` files into units that correspond to the implementation units. Kept old header file names for compatibility.
- Reworks the POSIX global lock implementation. Does not change the POSIX signal mask when locking/unlocking the global.
  - Fixes a race condition.
  - Adds a condition variable to the global lock structure. improves handling of tasks competing for access to the same object.
  - No longer changing signal masks repeatedly/unexpectedly. May be relevant to some BSP/driver developers.
- Checks return of sysconf for error and reports them. Only sets PageSize on success. If sysconf fails it provides a mechanism to avoid error propagation.
- Uses `errno` instead of status return from `clock_getres` with `strerror` reporting.
- Adds support for VxWorks 7
- See <https://github.com/nasa/osal/pull/690>

## Development Build: v5.1.0-rc1+dev109
- Add support for RTEMS 5.1 in the OSAL and provides defines and necessary ifdefs so RTEMS 4.11 can continue to be supported.
- Adds functional test for OS_chmod
- Refactor the table array access across OSAL. Use a token concept in combination with a macro to obtain the table entry instead of indexing arrays directly. All access is then done through this table pointer. Use the full object ID in the timer call back list. Update the timer sync callback prototype. Pass the entire OSAL ID to the sync function, not just the index. This is technically an API change.
- Replaces condition on forever loops to end on shutdown. Loops now exit on shutdown.
- Removes obsolete printf tests that didn't work
- See <https://github.com/nasa/osal/pull/680>

## Development Build: v5.1.0-rc1+dev91
- Rename `UT_SetForceFail` to `UT_SetDefaultReturnValue` since some functions that retain more than 1 value are not necessarily failing
- Add a 5th timer to TimerTest functional to test the one shot (zero-length time interval) case.
- Ensure all APIs use the proper type. Sizes are now size_t; these will now be 64 bits on a 64 bit platform.
- Fix build issue on VxWorks 6.9 by using the 3 argument form of `open()`. Passing `0` as the mode solves the build issue. This parameter is ignored when not creating a file.
-  The address calculations now use `unsigned long` instead of `long` to ensure that all rounding and base address adjustments behave the same way in the event that the addresses lie in the upper half of memory (i.e. start with a 1 bit) which would put it in the negative range of a long type.
- See <https://github.com/nasa/osal/pull/662>

## Development Build: v5.1.0-rc1+dev75
- Ensure that the handle is not NULL before invoking dlclose(). In particular the handle will be NULL for static modules. Shutdown after CTRL+C occurs normally (no segfault).
- Add a "flags" parameter to OS_ModuleLoad() to indicate the desired symbol visibility:
    - GLOBAL (0, the default, and matches current behavior)
    - LOCAL which hides from other modules and prevents other modules from binding to symbols in this module, thereby ensuring/preserving the ability to unload in the future
  - CFE should use LOCAL flag for apps, and GLOBAL flags for libraries.
- See <https://github.com/nasa/osal/pull/652>

## Development Build: v5.1.0-rc1+dev68

- When `OS_DEBUG` is enabled, this adds a message if mutex give/take actions occur outside the expected sequence. This informs the user (via the debug console) if a lock is taken more than once or if a lock is given by a different task than the one that originally took it:
```
OS_MutSemTake():216:WARNING: Task 65547 taking mutex 327685 while owned by task 65547
```
- Removes all FIXME comments
- Resolves security/filename race issue by opening file and acting on descriptor by adding fstat stub
- Squashed the minor recommended bugs
- UtAssert macros now accept variable string arguments.The `UtAssert_True` wrapper around call is no longer needed to accommodate dynamic string output, thus removing the double assert. UtAssert macros will now be able to offer more information by themselves.
- See <https://github.com/nasa/osal/pull/639>

## Development Build: v5.1.0-rc1+dev60
- Apply standard formatting, whitespace-only changes
- See <https://github.com/nasa/osal/pull/627>

## Development Build: v5.1.0-rc1+dev55
- Deprecate `OS_open` and `OS_creat` to and replaced them with by `OS_OpenCreate`, which implements both functions via flags, and follows the correct OSAL API patterns.
- Change use of uint32 for ID to the correct typedef. Also use ObjectIdFromInteger/ObjectIdToInteger where it is intended to convert these values to integers e.g. for the "name" fields in RTEMS.
- See <https://github.com/nasa/osal/pull/621>

## Development Build: v5.1.0-rc1+dev49
- Adds an event callback mechanism to certain state changes in OSAL. This allows the CFE PSP to be notified at these points, and therefore it can add platform-specific functionality.
- Correct issues involving recent OS_Milli2Ticks change.
- See <https://github.com/nasa/osal/pull/612>

## Development Build: v5.1.0-rc1+dev44
- Removes OS_Tick2Micros and internalize OS_Milli2Ticks.
- Adds ut_assert address equal macro.
- See <https://github.com/nasa/osal/pull/607>

## Development Build: v5.1.0-rc1+dev38
- Sets Revision to 99 for development builds
- See <https://github.com/nasa/osal/pull/600>

## Development Build: v5.1.0-rc1+dev34
- Move this existing function into the public API, as it performs more verification than the OS_ConvertToArrayIndex function.
- The C library type is signed, and this makes the result check work as intended.
- See <https://github.com/nasa/osal/pull/596>


## Development Build: v5.1.0-rc1+dev16
 - In the next major OSAL release, this code will be no longer supported at all. It should be removed early in the cycle to avoid needing to maintain this compatibility code. This code was already conditional on the OSAL_OMIT_DEPRECATED flag and as such the CCB has already tested/verified running the code in this configuration as part of CI scripts. After this change, the build should be equivalent to the result of building with OMIT_DEPRECATED=true.
- See <https://github.com/nasa/osal/pull/582>

## Development Build: v5.1.0-rc1+dev12
- Removes internal functions that are no longer used or defined but whose prototypes and stubs were still present in OS_ObjectIdMap
- Removes repetitive clearing of the global ID and unlocking global table and replaces these with common implementation in the idmap source file. This moves deleting tables to be similar to creating tables and provides
a common location for additional table-deletion-related logic.
- Propagates return code from OS_TaskRegister_Impl(). If this routine fails then return the error to the caller, which also prevents the task from starting.
- See <https://github.com/nasa/osal/pull/576>

## Development Build: v5.1.0-rc1+dev5
- Adds OSAL network APIs missing functional tests as well as tests for OS_TimedRead and OS_TimedWrite
- Allows separate, dynamic registration of test setup and teardown routines which are executed before and after the normal test routine, which can create and delete any global/common test prerequisites.
- Adds FileSysAddFixedMap missing functional API test
- See <https://github.com/nasa/osal/pull/563>


## Development Build: 5.0.0+dev247
- `OS_SocketOpen()` sets `sock_id` and returns a status when successful.
- Changed timer-test to be able to use OS_MAX_TIMERS value on top of the hard-coded NUMBER_OF_TIMERS value. This will allow the test to be functional even if the OS_MAX_TIMERS value is reconfigured.
- Ensures that
  - All stub routines register their arguments in the context, so that the values will be available to hook functions.
  - The argument names used in stubs match the name in the prototype/documentation so the value can be retrieved by name.
- Adds back rounding up to PTHREAD_STACK_MIN and also adds rounding up to a system page size. Keeps check for zero stack at the shared level; attempts to create a task with zero stack will fail. Allows internal helper threads to be created with a default minimum stack size.
- Avoids a possible truncation in snprintf call. No buffer size/truncation warning when building with optimization enabled.
- Added new macros to `osapi-version` to report baseline and build number
- The coverage binaries are now correctly installed for CPU1 and CPU2 as opposed to installed twice to CPU2 but not at all for CPU1.
- Fixes a typo in ut_assert README and clarifies stub documentation.
- See <https://github.com/nasa/osal/pull/529>

## Development Build: 5.0.21
- Command line options in Linux are no longer ignored/dropped.
- No impact to current unit testing which runs UT assert as a standalone app. Add a position independent code (PIC) variant of the ut_assert library, which can be dynamically loaded into other applications rather than running as a standalone OSAL application. This enables loading
UT assert as a CFE library.
- Unit tests pass on RTEMS.
- Resolve inconsistency in how the stack size is treated across different OS implementations. With this change the user-requested size is passed through to the underlying OS without an enforced minimum. An additional sanity check is added at the shared layer to ensure that the stack size is never passed as 0.
- Update Licenses for Apache 2.0
- See <https://github.com/nasa/osal/pull/521>

## Development Build: 5.0.20
-  Add "non-zero" to the out variable description for OS_Create (and related) API's.
- Increases the buffer for context info from 128 to 256 bytes and the total report buffer to 320 bytes.
- Add stub functions for `OS_TaskFindIdBySystemData()`, `OS_FileSysAddFixedMap()`, `OS_TimedRead()`, `OS_TimedWrite()`, and `OS_FileSysAddFixedMap()`
- Added the following wrappers macros around `UtAssert_True` for commonly-used asserts:
  - `UtAssert_INT32_EQ` - check equality as 32 bit signed int
  - `UtAssert_UINT32_EQ` - check equality as 32 bit unsigned int
  - `UtAssert_NOT_NULL` - check pointer not null
  - `UtAssert_NULL` - check pointer is null
  - `UtAssert_NONZERO` - check integer is nonzero
  - `UtAssert_ZERO` - check integer is zero
  - `UtAssert_STUB_COUNT` - check stub count
-  Using `unsigned long` instead of `uintmax_t` to fix support for VxWorks

- See <https://github.com/nasa/osal/pull/511> and <https://github.com/nasa/osal/pull/513>

## Development Build: 5.0.19
- Rename BSPs that can be used on multiple platforms.
`mcp750-vxworks` becomes `generic-vxworks`
`pc-linux` becomes `generic-linux`
- New features only, does not change existing behavior.
UT Hook functions now have the capability to get argument values by name, which is more future proof than assuming a numeric index.
- Add functional test for `OS_TimerAdd`
- Added functional tests for `OS_TimeBase Api` on `OS_TimeBaseCreate`, `OS_TimeBaseSet`, `OS_TimeBaseDelete`, `OS_TimeBaseGetIdByName`, `OS_TimeBaseGetInfo`, `OS_TimeBaseGetFreeRun`
- See <https://github.com/nasa/osal/pull/487> for details

## Development Build: 5.0.18
- Add functional tests for `OS_IdentifyObject`, `OS_ConvertToArrayIndex` and `OS_ForEachObject` functions.
- Fix doxygen warnings
- Unit test cases which use `OS_statfs` and run on an `RTEMS IMFS` volume will be skipped and categorized as "NA" due to `OS_ERR_NOT_IMPLEMENTED` response, rather than a failure.
- The device_name field was using the wrong length, it should be of `OS_FS_DEV_NAME_LEN` Also correct another length check on the local path name.
- For RTEMS, will not shutdown the kernel if test abort occurs.
- Unit tests work on RTEMS without BSP preallocating ramdisks
- If `OSAL_EXT_SOURCE_DIR` cache variable is set, this location will be checked first for a BSP/OS implementation layer.
- Implement `OS_GetResourceName()` and `OS_ForEachObjectOfType()`, which are new functions that allow for additional query capabilities. No impact to current behavior as the FSW does not currently use any of these new APIs.
- A functional test enhancement to `bin-sem-test` which replicates the specific conditions for the observed bug to occur. Deletes the task calling `OS_BinSemTake()` and then attempts to use the semaphore after this.
- Employ a `pthread` "cleanup handler" to handle the situation where a task is canceled during the `pthread_cond_wait()` call. This ensures that the `mutex` is unlocked as part of the cleanup, so other tasks may continue using the semaphore.
- Change all initial `mutex` locking to be a finite "timed" wait rather than an infinite wait. In all cases, the condition variable is only held for brief periods of time and should be readily available. If a task blocks for a long time, this considers the mutex "broken" and aborts, thereby avoiding deadlock. This is a "contingency" fix in that if an exception or signal or other unknown/unhandled async event occurs that leaves the mutex permanently locked.
- Adds the mutex to protect the timer callback `timecb` resource table.
- See <https://github.com/nasa/osal/pull/482>

## Development Build: 5.0.17
-  `OS_QueueCreate()` will return an error code if the depth parameter is larger than the configured `OS_MAX_QUEUE_DEPTH`.
- See <https://github.com/nasa/osal/pull/477>

## Development Build: 5.0.16
- Resized buffers and added explicit termination to string copies. No warnings on GCC9 with strict settings and optimization enabled.
- New API to reverse lookup an OS-provided thread/task identifier back to an OSAL ID. Any use of existing OStask_id field within the task property structure is now deprecated.
- See <https://github.com/nasa/osal/pull/458>

## Development Build: 5.0.15
- Changes the build system.
- No more user-maintained osconfig.h file, this is now replaced by a cmake configuration file.
- Breaks up low-level implementation into small, separate subsystem units, with a separate header file for each one.
- See <https://github.com/nasa/osal/pull/444>

## Development Build: 5.0.14
- Adds library build, functional, and coverage test to CI
- Deprecates `OS_FS_SUCCESS, OS_FS_ERROR , OS_FS_ERR_INVALID_POINTER, OS_FS_ERR_NO_FREE_FDS , OS_FS_ERR_INVALID_FD, and OS_FS_UNIMPLEMENTED` from from `osapi-os-filesys.h`
- Individual directory names now limited to OS_MAX_FILE_NAME
- Fix tautology, local_idx1 is now compared with local_idx2
- Module files are generated when the `osal_loader_UT` test is built and run
- Consistent osal-core-test execution status
- See <https://github.com/nasa/osal/pull/440> for more details

## Development Build: 5.0.13
- Added coverage test to `OS_TimerCreate` for `OS_ERR_NAME_TOO_LONG`.
- Externalize enum for `SelectSingle`, ensures that pointers passed to `SelectFd...()` APIs are not null, ensures that pointer to `SelectSingle` is not null.
- Command to run in shell and output to fill will fail with default (not implemented) setting.
- Builds successfully using the inferred OS when only `OSAL_SYSTEM_BSPTYPE` is set. Generates a warning when `OSAL_SYSTEM_BSPTYPE` and `OSAL_SYSTEM_OSTYPE` are both set but are mismatched.
- See <https://github.com/nasa/osal/pull/433> for more details

## Development Build: 5.0.12
- Use the target_include_directories and target_compile_definitions functions from CMake to manage the build flags per target.
- Build implementation components using a separate CMakeLists.txt file rather than aux_source_directory.
- Provide sufficient framework for combining the OSAL BSP, UT BSP, and the CFE PSP and eliminating the duplication/overlap between these items.
- Minor updates (see <https://github.com/nasa/osal/pull/417>)

## Development Build: 5.0.11
- The more descriptive return value OS_ERR_NAME_NOT_FOUND (instead of OS_FS_ERROR) will now be returned from the following functions (): OS_rmfs, OS_mount, OS_unmount, OS_FS_GetPhysDriveName
- Wraps OS_ShMem* prototype and unit test wrapper additions in OSAL_OMIT_DEPRECATED
- Minor updates (see <https://github.com/nasa/osal/pull/408>)

## Development Build: 5.0.10
- Minor updates (see <https://github.com/nasa/osal/pull/401>)

  - 5.0.9: DEVELOPMENT

- Documentation updates (see <https://github.com/nasa/osal/pull/375>)

## Development Build: 5.0.8
- Minor updates (see <https://github.com/nasa/osal/pull/369>)

## Development Build: 5.0.7
- Fixes memset bug
- Minor updates (see <https://github.com/nasa/osal/pull/361>)

## Development Build: 5.0.6
- Minor updates (see <https://github.com/nasa/osal/pull/355>)

## Development Build: 5.0.5
- Fixed osal_timer_UT test failure case
- Minor updates (see <https://github.com/nasa/osal/pull/350>)

## Development Build: 5.0.4
- Minor updates (see <https://github.com/nasa/osal/pull/334>)

## Development Build: 5.0.3
- Minor updates (see <https://github.com/nasa/osal/pull/292>)

## Development Build: 5.0.2
- Bug fixes and minor updates (see <https://github.com/nasa/osal/pull/281>)

## Development Build: 5.0.1
- Minor updates (see <https://github.com/nasa/osal/pull/264>)

## **_OFFICIAL RELEASE: 5.0.0 - Aquila_**
- Changes are detailed in [cFS repo](https://github.com/nasa/cFS) release 6.7.0 documentation
- Released under the Apache 2.0 license

## **_OFFICIAL RELEASE: 4.2.1a_**
- Released under the [NOSA license](https://github.com/nasa/osal/blob/osal-4.2.1a/LICENSE)
- See [version description document](https://github.com/nasa/osal/blob/osal-4.2.1a/OSAL%204.2.1.0%20Version%20Description%20Document.pdf)
- This is a point release from an internal repository

## Known issues
See all open issues and closed to milestones later than this version.

## Getting Help
For best results, submit issues:questions or issues:help wanted requests at <https://github.com/nasa/cFS>.

Official cFS page: <http://cfs.gsfc.nasa.gov>
