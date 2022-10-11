######################################################################
#
# CMAKE build recipe for LINUX Board Support Package (BSP)
#
######################################################################

# This basic implementation library should be generic enough to use
# on any Linux-based processor board, as well as a standard development PC.
add_library(osal_generic-linux_impl OBJECT
	src/bsp_start.c
	src/bsp_console.c
)

# OSAL needs conformance to at least POSIX.1c (aka POSIX 1995) - this includes all the
# real-time support and threading extensions.
#
# When compiling against glibc, using "_XOPEN_SOURCE=600" enables the X/Open 6 standard.
# XPG6 includes all necessary XPG5, POSIX.1c features as well as SUSv2/UNIX98 extensions.
# This OSAL implementation uses clock_nanosleep(), mq_timedreceive(), and
# mq_timedsend() which are enhancements added in the XPG6 standard.
#
# See http://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html
# for a more detailed description of the feature test macros and available values
target_compile_definitions(osal_public_api INTERFACE
    _XOPEN_SOURCE=600
)

# Linux system libraries required for the final link of applications using OSAL
target_link_libraries(osal_public_api INTERFACE
    pthread dl rt
)

# This BSP only works with "posix" OS layer.
# Confirming this reduces risk of accidental misconfiguration
set_property(TARGET osal_generic-linux_impl PROPERTY OSAL_EXPECTED_OSTYPE "posix")

# Configure the ut_coverage_compile and ut_coverage_link for enabling coverage
# testing on this platform.
if (NOT CMAKE_CROSSCOMPILING AND ENABLE_UNIT_TESTS)
  # Support for other compilers/coverage tools could be added here.
  # for now only the GNU "gcov" will be enabled
  if (CMAKE_C_COMPILER_ID STREQUAL GNU)
    target_compile_options(ut_coverage_compile INTERFACE -pg -ftest-coverage -fprofile-arcs)
    target_link_libraries(ut_coverage_link INTERFACE gcov)
  endif()
endif()
