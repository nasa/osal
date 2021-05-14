#
# CMake build snippet for OSAL tests
#

# Indicates that this should output a ctest script
enable_testing()

# Each test module is stored within its own subdir
file(GLOB OSAL_TESTS *-test)

foreach(OSTEST ${OSAL_TESTS})
  get_filename_component(TESTNAME ${OSTEST} NAME)
  set(TESTFILES)
  aux_source_directory(${OSTEST} TESTFILES)
  add_osal_ut_exe(${TESTNAME} ${TESTFILES})
endforeach(OSTEST ${OSAL_TESTS})
