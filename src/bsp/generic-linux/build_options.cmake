##########################################################################
#
# Build options for "generic-linux" BSP
#
##########################################################################

# C flags that should be used when (re-) compiling code for unit testing.
# Note: --coverage is just a shortcut for "-ftest-coverage" and "-fprofile-arcs"
# This also does not work well when cross compiling since paths to the _compile_ dir
# are baked into the executables, so they will not be there when copied to the target
# Note - although GCC understands the same flags for compile and link here, this may
# not be true on all platforms so the compile and link flags are specified separately.
if (NOT CMAKE_CROSSCOMPILING AND NOT OSAL_OMIT_DEPRECATED)
  # The variables here (UT_COVERAGE_COMPILE_FLAGS/LINK_FLAGS) should be phased out, prefer
  # to use the interface libraries (ut_coverage_compile/link) instead, which are more flexible.
  set(UT_COVERAGE_COMPILE_FLAGS -pg --coverage)
  set(UT_COVERAGE_LINK_FLAGS    -pg --coverage)
endif()
