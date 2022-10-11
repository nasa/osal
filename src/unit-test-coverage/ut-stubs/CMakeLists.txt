# ----------------------------------------
#    Stub libraries for coverage testing
# ----------------------------------------

# This provides suitable "stub" implementations of every
# function call used internally by the various OSAL modules
# for which there is not a stub already defined.
#
# (Note this is not the public OSAL API - that is stubbed separately)
#
# This includes:
# - Stub versions of C library calls defined per ANSI C89/C99
#     i.e. memset, strcmp, etc - these should be relevant for all
#     supported operating systems as they are standard C
# - Stub versions of internal "shared" OSAL implementation functions
#     i.e. everything declared in the internal API.  These are needed by
#     any coverage test referencing on the shared/ng OSAL layer.
#

add_library(ut_osapi_stub_headers INTERFACE)
target_include_directories(ut_osapi_stub_headers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(ut_osapi_stub_headers INTERFACE 
    ut_assert
)

# The "ut_libc_stubs" target provides stub versions of C library calls.
# They are prefixed with "OCS_" and target code must be recompiled to
# call the OCS_ version of the syscall instead of the regular syscall.
# This is because in some circumstances for these calls the stub actually
# needs to invoke the real version or else weird things happen.
# This library includes stubs from all supported operating systems.  This
# is generally OK as we do not use any actual OS system headers
#
# These files are generally organized to match whatever header file
# defines the function. For instance, POSIX defines the "mqueue.h"
# header file which in turn provides mq_open, mq_close, etc.  So
# the OCS_mq_open, OCS_mq_close declarations are in overrides/mqueue.h, and
# the respective implementation is in posix-mqueue-stubs.c.
#
# This keeps things relatively organized, and by keeping the source files
# relatively small and targeted like this the linker should only pull in
# the OCS functions that are actually used.
#
add_library(ut_libc_stubs STATIC EXCLUDE_FROM_ALL
    src/arpa-inet-stubs.c
    src/libc-ctype-stubs.c
    src/libc-stdio-stubs.c
    src/libc-stdlib-stubs.c
    src/libc-string-stubs.c
    src/netinet-in-stubs.c
    src/posix-dirent-stubs.c
    src/posix-dlfcn-stubs.c
    src/posix-errno-stubs.c
    src/posix-fcntl-stubs.c
    src/posix-ioctl-stubs.c
    src/posix-mqueue-stubs.c
    src/posix-pthread-stubs.c
    src/posix-sched-stubs.c
    src/posix-semaphore-stubs.c
    src/posix-signal-stubs.c
    src/posix-stat-stubs.c
    src/posix-time-stubs.c
    src/posix-unistd-stubs.c
    src/sys-socket-stubs.c
    src/sys-select-stubs.c
    src/vxworks-ataDrv-stubs.c
    src/vxworks-dosFsLib-stubs.c
    src/vxworks-errnoLib-stubs.c
    src/vxworks-hostLib-stubs.c
    src/vxworks-intLib-stubs.c
    src/vxworks-loadLib-stubs.c
    src/vxworks-memPartLib-stubs.c
    src/vxworks-moduleLib-stubs.c
    src/vxworks-msgQLib-stubs.c
    src/vxworks-ramDrv-stubs.c
    src/vxworks-semLib-stubs.c
    src/vxworks-shellLib-stubs.c
    src/vxworks-symLib-stubs.c
    src/vxworks-sysLib-stubs.c
    src/vxworks-taskLib-stubs.c
    src/vxworks-taskVarLib-stubs.c
    src/vxworks-xbdBlkDev-stubs.c
)

target_link_libraries(ut_libc_stubs PUBLIC
    ut_osapi_stub_headers
)

# About the generated UT stub sources
#
# The interface between "shared" (upper) and "impl" (lower) layers presents a few
# complexities for stubs, because the os/shared/inc headers define prototypes for
# functions that are both referenced from upper and implemented in lower level, as
# well as some functions that are referenced from lower and implemented in upper.
#
# it is important _NOT_ to mix these into the same stub units, even if they are
# declared in the same header file, or else link-time errors are likely to occur.
#
# However, this is eased to some degree because of the naming conventions.  All
# functions that are intended to be implemented in the lower layer have an "_Impl"
# suffix on their name.
#
# The stubs are generated in three sets
#   impl-stubs : for all "Impl" routines, normally implemented in lower layer.  No handlers.
#   init-stubs : for all "Init" routines, normally implemented in upper layer.  No handlers.
#   stubs : for everything else, normally implemented in upper layer.  Has handlers.
#
set(OSAL_SHARED_IMPL_HEADERS
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-binsem.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-clock.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-common.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-console.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-condvar.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-countsem.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-dir.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-errors.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-file.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-filesys.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-globaldefs.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-heap.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-idmap.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-module.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-mutex.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-network.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-printf.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-queue.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-select.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-shell.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-sockets.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-task.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-timebase.h
    ${OSAL_SOURCE_DIR}/src/os/shared/inc/os-shared-time.h
)

# The following target rule contains the specific commands required
# to auto-generate the stub implementations from the headers
add_custom_target(generate_osal_coverage_stubs
    COMMAND ${UT_ASSERT_SOURCE_DIR}/scripts/generate_stubs.pl
        --filter=/_Impl$/
        --stub-suffix=impl-stubs
        --handler-suffix=impl-handlers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${OSAL_SHARED_IMPL_HEADERS}
    COMMAND ${UT_ASSERT_SOURCE_DIR}/scripts/generate_stubs.pl
        --filter=/_Init$/
        --stub-suffix=init-stubs
        --handler-suffix=init-handlers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${OSAL_SHARED_IMPL_HEADERS}
    COMMAND ${UT_ASSERT_SOURCE_DIR}/scripts/generate_stubs.pl
        --filter=!/_Init$|_Impl$/
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${OSAL_SHARED_IMPL_HEADERS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM
)

# The "ut_osapi_impl_stubs" provides stub functions for the
# implementation-specific function calls that are declared under os/shared/inc
# header directory.  These are the functions that all end in an "_Impl" suffix,
# and are not public API calls.   This is only compiled if used.
add_library(ut_osapi_impl_stubs STATIC EXCLUDE_FROM_ALL
    src/os-shared-binsem-impl-stubs.c
    src/os-shared-clock-impl-stubs.c
    src/os-shared-common-impl-stubs.c
    src/os-shared-condvar-impl-stubs.c
    src/os-shared-condvar-init-stubs.c
    src/os-shared-console-impl-stubs.c
    src/os-shared-countsem-impl-stubs.c
    src/os-shared-dir-impl-stubs.c
    src/os-shared-file-impl-handlers.c
    src/os-shared-file-impl-stubs.c
    src/os-shared-filesys-impl-handlers.c
    src/os-shared-filesys-impl-stubs.c
    src/os-shared-heap-impl-stubs.c
    src/os-shared-idmap-impl-stubs.c
    src/os-shared-module-impl-stubs.c
    src/os-shared-mutex-impl-stubs.c
    src/os-shared-network-impl-handlers.c
    src/os-shared-network-impl-stubs.c
    src/os-shared-printf-impl-stubs.c
    src/os-shared-queue-impl-stubs.c
    src/os-shared-select-impl-stubs.c
    src/os-shared-shell-impl-stubs.c
    src/os-shared-sockets-impl-handlers.c
    src/os-shared-sockets-impl-stubs.c
    src/os-shared-task-impl-stubs.c
    src/os-shared-timebase-impl-stubs.c
)

target_link_libraries(ut_osapi_impl_stubs PUBLIC
    ut_osapi_stub_headers
)

# The "ut_osapi_init_stubs" provides stub functions for the
# initialization function calls that are declared under os/shared/inc
# header directory.  These are the functions that all end in an "_Init" suffix,
# and are not public API calls.   This is only compiled if used.
add_library(ut_osapi_init_stubs STATIC EXCLUDE_FROM_ALL
    src/os-shared-binsem-init-stubs.c
    src/os-shared-common-init-stubs.c
    src/os-shared-console-init-stubs.c
    src/os-shared-countsem-init-stubs.c
    src/os-shared-dir-init-stubs.c
    src/os-shared-file-init-stubs.c
    src/os-shared-filesys-init-stubs.c
    src/os-shared-module-init-stubs.c
    src/os-shared-mutex-init-stubs.c
    src/os-shared-network-init-stubs.c
    src/os-shared-queue-init-stubs.c
    src/os-shared-sockets-init-stubs.c
    src/os-shared-task-init-stubs.c
    src/os-shared-timebase-init-stubs.c
    src/os-shared-time-init-stubs.c
)

target_link_libraries(ut_osapi_init_stubs PUBLIC
    ut_osapi_stub_headers
)

# The "ut_osapi_shared_stubs" provides stub functions for the
# all other function calls that are declared under os/shared/inc
# header directory.  These are the non-public functions that do
# NOT end in either _Init or _Impl suffix, and are normally
# implemented in the shared (upper) layer.
# Unlike the others, these stubs have default handler/hook functions.
add_library(ut_osapi_shared_stubs STATIC EXCLUDE_FROM_ALL
    src/os-shared-common-stubs.c
    src/os-shared-file-stubs.c
    src/os-shared-filesys-stubs.c
    src/os-shared-globaldefs-stubs.c
    src/os-shared-idmap-handlers.c
    src/os-shared-idmap-stubs.c
    src/os-shared-module-stubs.c
    src/os-shared-sockets-stubs.c
    src/os-shared-task-stubs.c
    src/os-shared-timebase-stubs.c
)

target_link_libraries(ut_osapi_shared_stubs PUBLIC
    ut_osapi_stub_headers
)

# The "ut_osapi_table_stubs" provides stub objects for shared
# table objects used by the implementation layer.  These
# are not public.   This is only compiled if used.
add_library(ut_osapi_table_stubs STATIC EXCLUDE_FROM_ALL
    src/osapi-shared-binsem-table-stubs.c
    src/osapi-shared-console-table-stubs.c
    src/osapi-shared-common-stubs.c
    src/osapi-shared-countsem-table-stubs.c
    src/osapi-shared-error-impl-table-stubs.c
    src/osapi-shared-dir-table-stubs.c
    src/osapi-shared-filesys-table-stubs.c
    src/osapi-shared-idmap-table-stubs.c
    src/osapi-shared-module-table-stubs.c
    src/osapi-shared-mutex-table-stubs.c
    src/osapi-shared-queue-table-stubs.c
    src/osapi-shared-stream-table-stubs.c
    src/osapi-shared-task-table-stubs.c
    src/osapi-shared-timebase-table-stubs.c
    src/osapi-shared-timecb-table-stubs.c
)

target_link_libraries(ut_osapi_table_stubs PUBLIC
    ut_osapi_stub_headers
)

add_library(ut_bsp_impl_stubs STATIC EXCLUDE_FROM_ALL
    src/bsp-console-impl-stubs.c
)

target_link_libraries(ut_bsp_impl_stubs PUBLIC
    ut_osapi_stub_headers
)

