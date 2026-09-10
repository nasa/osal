# POSIX object-table synchronization tests, using the existing pthread stubs.
add_coverage_testrunner(
    coverage-posix-idmap
    ${OSAL_SOURCE_DIR}/src/os/posix/src/os-impl-idmap.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/coveragetest-idmap.c
    ut_osapi_shared_stubs
    ut_osapi_stubs
    ut_libc_stubs
)
target_include_directories(utobj_coverage-posix-idmap PRIVATE
    ${OSAL_SOURCE_DIR}/src/os/posix/inc
)
