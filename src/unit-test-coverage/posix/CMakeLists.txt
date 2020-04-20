# CMake snippet for building the POSIX layer coverage tests
set(POSIX_MODULE_LIST
    #binsem
    #common
    #console
    #countsem
    #dirs
    #files
    #filesys
    #heap
    #loader
    #mutex
    #network
    #queues
    #shell
    #   symtab
    #tasks
    #timebase
)

set(POSIX_PORTABLE_BLOCK_LIST
    posix-gettime
    posix-io
    posix-files

    #console-bsp
    bsd-select
    #bsd-sockets

    no-loader
    #no-symtab
    #no-network
    #no-sockets
)

# This unit test is allowed to directly include any internal file in
# the respective set under test.
include_directories(${OSAL_SOURCE_DIR}/src/os/${SETNAME}/inc)

# The "ut-stubs" contains additional stubs specific to this sub-module
add_subdirectory(ut-stubs)
add_subdirectory(adaptors)

set(POSIX_COVERAGE_LINK_LIST
        ut-adaptor-posix
        ut_posix_impl_stubs
        ut_osapi_impl_stubs
        ut_osapi_shared_stubs
        ut_osapi_stubs
        ut_libc_stubs
)


# Add all coverage tests in the src dir
foreach(MODNAME ${POSIX_MODULE_LIST})
    add_coverage_testrunner(
        "coverage-${SETNAME}-${MODNAME}"
        "${OSAL_SOURCE_DIR}/src/os/${SETNAME}/src/os-impl-${MODNAME}.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/coveragetest-${MODNAME}.c"
        ${POSIX_COVERAGE_LINK_LIST}
    )
endforeach(MODNAME ${POSIX_MODULE_LIST})

foreach(MODNAME ${POSIX_PORTABLE_BLOCK_LIST})
    add_coverage_testrunner(
        "coverage-${SETNAME}-${MODNAME}"
        "${OSAL_SOURCE_DIR}/src/os/portable/os-impl-${MODNAME}.c"
        "${OSALCOVERAGE_SOURCE_DIR}/portable/src/coveragetest-${MODNAME}.c"
        ${POSIX_COVERAGE_LINK_LIST}
    )
endforeach(MODNAME ${POSIX_PORTABLE_BLOCK_LIST})

