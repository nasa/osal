######################################################################
#
# CMAKE build recipe for POSIX OSAL implementation
#
######################################################################

# This CMake script generates targets specific to the POSIX implementation

# The basic set of files which are always built
set(POSIX_BASE_SRCLIST
    ../posix/src/os-impl-binsem.c
    ../posix/src/os-impl-common.c
    ../posix/src/os-impl-console.c
    ../posix/src/os-impl-condvar.c
    ../posix/src/os-impl-countsem.c
    ../posix/src/os-impl-dirs.c
    ../posix/src/os-impl-errors.c
    ../posix/src/os-impl-heap.c
    ../posix/src/os-impl-idmap.c
    ../posix/src/os-impl-mutex.c
    ../posix/src/os-impl-queues.c
    ../posix/src/os-impl-tasks.c
    ../posix/src/os-impl-timebase.c
    src/os-impl-filesys.c     # VxWorks RTP specific implementation
    src/os-impl-files.c       # VxWorks RTP specific implementation
)

# Use portable blocks for basic I/O
set(POSIX_IMPL_SRCLIST
    ../portable/os-impl-posix-gettime.c
    ../portable/os-impl-console-bsp.c
    ../portable/os-impl-bsd-select.c
    ../portable/os-impl-posix-io.c
    ../portable/os-impl-posix-files.c
    ../portable/os-impl-posix-dirs.c
    ../portable/os-impl-no-file-allocate.c
)

if (OSAL_CONFIG_INCLUDE_SHELL)
    list(APPEND POSIX_IMPL_SRCLIST
       ../posix/src/os-impl-shell.c
    )
else ()
    list(APPEND POSIX_IMPL_SRCLIST
       ../portable/os-impl-no-shell.c
    )
endif ()

if (OSAL_CONFIG_RWLOCK)
    list(APPEND POSIX_IMPL_SRCLIST
       ../posix/src/os-impl-rwlock.c
    )
else ()
    list(APPEND POSIX_IMPL_SRCLIST
       ../portable/os-impl-no-rwlock.c
    )
endif ()

# If some form of module loading is configured,
# then build the module loader
if (OSAL_CONFIG_INCLUDE_DYNAMIC_LOADER)
    list(APPEND POSIX_IMPL_SRCLIST
        ../posix/src/os-impl-loader.c
        ../portable/os-impl-posix-dl-loader.c
        ../portable/os-impl-posix-dl-symtab.c
    )
else ()
    list(APPEND POSIX_IMPL_SRCLIST
        ../posix/src/os-impl-no-module.c
        ../portable/os-impl-no-loader.c
        ../portable/os-impl-no-symtab.c
    )
endif ()

# VxWorks does not support gethostid
list(APPEND POSIX_IMPL_SRCLIST
        ../portable/os-impl-no-network-gethostid.c
)

if (OSAL_CONFIG_INCLUDE_NETWORK)
    list(APPEND POSIX_IMPL_SRCLIST
        ../portable/os-impl-bsd-sockets.c   # Use BSD socket layer implementation
        ../portable/os-impl-bsd-sockets-no-dns.c   # Use functions that do not require DNS
        ../portable/os-impl-posix-network-gethostname.c # Use POSIX-defined hostname implementation
    )
else()
    list(APPEND POSIX_IMPL_SRCLIST
        ../portable/os-impl-no-network-gethostname.c    # non-implemented versions of gethostname API
        ../portable/os-impl-no-sockets.c    # non-implemented versions of all socket APIs
    )
endif ()

# Defines an OBJECT target named "osal_posix_impl" with selected source files
add_library(osal_vxworks-rtp_impl OBJECT
    ${POSIX_BASE_SRCLIST}
    ${POSIX_IMPL_SRCLIST}
)

target_include_directories(osal_vxworks-rtp_impl PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../posix/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_compile_definitions(osal_public_api INTERFACE
    _POSIX_OS_
)
