######################################################################
#
# CMAKE recipe for the OSAL stub library
#
######################################################################

#
# This works in conjunction with the UT Assert library to
# provide "stub" versions of all calls in the OSAL public API.
#

set(OSAL_PUBLIC_API_HEADERS
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-binsem.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-bsp.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-clock.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-common.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-condvar.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-constants.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-countsem.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-dir.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-error.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-file.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-filesys.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-heap.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-idmap.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-macros.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-module.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-mutex.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-network.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-printf.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-queue.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-select.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-shell.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-sockets.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-task.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-timebase.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-timer.h
    ${OSAL_SOURCE_DIR}/src/os/inc/osapi-version.h
)

# The following target rule contains the specific commands required
# to auto-generate the stub implementations from the headers
add_custom_target(generate_osapi_stubs
    COMMAND ${UT_ASSERT_SOURCE_DIR}/scripts/generate_stubs.pl
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${OSAL_PUBLIC_API_HEADERS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM
)


# NOTE: There is no separate public include directory for the stubs.
# By definition, the stubs must implement the same public API that the
# normal OSAL library implements.  Therefore, only the standard OSAL
# header files are used.
add_library(ut_osapi_stubs STATIC
    utstub-helpers.c
    osapi-binsem-stubs.c
    osapi-binsem-handlers.c
    osapi-clock-stubs.c
    osapi-clock-handlers.c
    osapi-common-stubs.c
    osapi-condvar-stubs.c
    osapi-countsem-stubs.c
    osapi-countsem-handlers.c
    osapi-dir-stubs.c
    osapi-dir-handlers.c
    osapi-error-stubs.c
    osapi-error-handlers.c
    osapi-file-stubs.c
    osapi-file-handlers.c
    osapi-filesys-stubs.c
    osapi-filesys-handlers.c
    osapi-heap-stubs.c
    osapi-heap-handlers.c
    osapi-idmap-stubs.c
    osapi-idmap-handlers.c
    osapi-module-stubs.c
    osapi-module-handlers.c
    osapi-mutex-stubs.c
    osapi-mutex-handlers.c
    osapi-network-stubs.c
    osapi-network-handlers.c
    osapi-printf-stubs.c
    osapi-printf-handlers.c
    osapi-queue-stubs.c
    osapi-queue-handlers.c
    osapi-select-stubs.c
    osapi-shell-stubs.c
    osapi-sockets-stubs.c
    osapi-sockets-handlers.c
    osapi-task-stubs.c
    osapi-task-handlers.c
    osapi-timer-stubs.c
    osapi-timer-handlers.c
    osapi-timebase-stubs.c
    osapi-timebase-handlers.c
    osapi-version-stubs.c
    osapi-version-handlers.c
)

# These stubs must always link to UT Assert.
# This also implicitly adds the path to the UT Assert header files.
target_link_libraries(ut_osapi_stubs PUBLIC
    osal_public_api
    ut_assert
)

if (OSAL_INSTALL_LIBRARIES)
    install(
        TARGETS ut_osapi_stubs
        EXPORT nasa-osal-export
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
endif()
