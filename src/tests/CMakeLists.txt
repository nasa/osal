#
# CMake build snippet for OSAL tests
#

# Indicates that this should output a ctest script
enable_testing()

# Each test module is stored within its own subdir
file(GLOB OSAL_TESTS *-test)

# The mutex robustness test deletes a task that holds a mutex and expects the
# next take to succeed; without robust mutex support it would block forever
if (NOT OSAL_SYSTEM_OSTYPE STREQUAL "posix" OR NOT OSAL_CONFIG_POSIX_ROBUST_MUTEX)
    list(FILTER OSAL_TESTS EXCLUDE REGEX "mutex-robust-test$")
endif ()

# if using the address sanitizer tool, this can affect some aspects
# of the test cases - in particular where things appear in memory - 
# and this can sometimes cause a test to fail, depending on the context.
if (ENABLE_ASAN)
    add_compile_definitions(OSAL_UT_ASAN_ENABLED)
endif (ENABLE_ASAN)

foreach(OSTEST ${OSAL_TESTS})
  get_filename_component(TESTNAME ${OSTEST} NAME)
  set(TESTFILES)
  aux_source_directory(${OSTEST} TESTFILES)
  add_osal_ut_exe(${TESTNAME} ${TESTFILES})
endforeach(OSTEST ${OSAL_TESTS})
