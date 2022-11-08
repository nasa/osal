######################################################################
#
# CMAKE build recipe for Operating System Abstraction Layer (OSAL)
#
######################################################################
#
# This defines the following target(s):
#
# osal_public_api : Interface library that refers to the OSAL public API only.
#                   This may be used for applications that need to use the
#                   OSAL-provided types (such as common-types.h) but do not
#                   make any OSAL API calls.
#
#         osal   : The main library containing the OSAL binary code.  This is
#                  the normal library that applications should link to.
#                  This is built based off the OSAL_SYSTEM_OSTYPE selection
#
#     osal_bsp   : The board support library containing the system-
#                  specific entry point function (e.g. main) and the
#                  file system volume table for the target board.
#                  This is based off the OSAL_SYSTEM_BSPTYPE selection
#
#    ut_assert   : The unit test support library.  This implements
#                  an application entry point (OS_Application_Startup)
#                  that contains a unit test subsystem.  This uses
#                  the OSAL BSP to provide system-specific entry point.
#                  Linking with this library also links with osal_bsp,
#                  but not necessarily the osal library itself.
#
# ut_coverage_compile : an interface target that contains the
#                       compiler options/definitions to enable coverage
#                       instrumentation in the generated objects.  It should
#                       be specified on files compiled for coverage analysis.
#
# ut_coverage_link    : an interface target that contains options/definitions
#                       and any link libraries to enable coverage instrumentation in
#                       the final executable.   It should be specified on coverage
#                       test executable targets.
#
#
# The following options and variables will customize the build:
#
# OSAL_SYSTEM_BSPTYPE: Should be set by the caller to indicate the type of
#                  BSP to build.  This is a minimal platform-specific shim
#                  layer to aid with startup and shutdown.  This implies a
#                  value for OSTYPE (see below).
#
# OSAL_SYSTEM_OSTYPE: The selected OS abstraction type (posix, vxworks, rtems).
#                  Normally this setting is not needed to be configured as it is
#                  inferred from the BSP type.
#
# OSAL_INSTALL_LIBRARIES : Boolean, enables "install" of targets listed above such
#                  that libraries and public API header files are copied into the system
#                  location specified by CMAKE_INSTALL_PREFIX.  This set of headers
#                  and link libraries can then be used to compile other applications
#                  separately from OSAL.  Default is "ON" when OSAL is being compiled
#                  standalone (i.e. cmake called directly on the OSAL source dir).
#                  Default is "OFF" when OSAL is included via 'add_subdirectory' in
#                  a parent application project such as CFE/CFS.
#
# ENABLE_UNIT_TESTS : Boolean, enables build of the unit tests (coverage and functional)
#
# OSAL_OMIT_DEPRECATED : Boolean, Compile without deprecated or obsolete features for
#                  forward compatibility testing.  Any features marked as deprecated
#                  will be disabled, allowing developers to test software based
#                  only on currently-supported software features and patterns.
#
# OSAL_EXT_SOURCE_DIR : External source directory to check for BSP/OS implementation
#
# Additionally the following target is defined if ENABLE_UNIT_TESTS
# is set TRUE:
#
# ut_osapi_stubs : Stub library correlating to the OSAL public API
#                  This is for unit testing OSAL-based applications
#                  It operates in conjunction with the ut_assert library.
#
# For backward compatibility with previous versions, this also exports the following
# variables.  Code which is depending on these values should migrate to using the
# interfaces of the provide ut_coverage_compile/link targets instead.
#
#  UT_COVERAGE_COMPILE_FLAGS : Compiler flags that must be used to
#                  instrument code for coverage testing
#  UT_COVERAGE_LINK_FLAGS : Linker flags that must be used to
#                  instrument code for coverage testing
#
# The ENABLE_UNIT_TESTS option also builds a set of test applications from
# that demonstrate the usage and validate the runtime behavior of various
# OSAL resources.
#
######################################################################
cmake_minimum_required(VERSION 3.5)

# Set the policy dictating use of target_link_libraries across directories
# Either OLD or NEW should work here but setting it to something avoids a
# warning when using newer versions of the tool.
if (CMAKE_VERSION VERSION_GREATER 3.13)
    cmake_policy(SET CMP0079 NEW)
endif()

project(OSAL C)

# define a custom property to track relationship between BSP and OS
# this should be set on BSP "impl" targets to indicate the correct OS impl to go with it
define_property(TARGET PROPERTY OSAL_EXPECTED_OSTYPE
    BRIEF_DOCS
        "The expected OS type for an OSAL BSP implementation"
    FULL_DOCS
        "This property is used to indicate the OS implementation layer that is intended to be paired with the BSP implementation"
)

# If this build is being performed as a subdirectory within a larger project,
# then the static libraries and header files will _not_ be installed by default,
# as in those cases one would typically just use the "osal" target directly.
# This behavior can be explicitly controlled via the OSAL_INSTALL_LIBRARIES option.
get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if (HAS_PARENT)
    set(OSAL_DEFAULT_INSTALL_LIBRARIES OFF)
else()
    set(OSAL_DEFAULT_INSTALL_LIBRARIES ON)
endif()

option(OSAL_INSTALL_LIBRARIES "Whether or not to install static libraries and headers" ${OSAL_DEFAULT_INSTALL_LIBRARIES})
option(OSAL_OMIT_DEPRECATED   "Compile without deprecated or obsolete features for backward compatibility testing" OFF)

if (OSAL_INSTALL_LIBRARIES)
    include(CMakePackageConfigHelpers)
endif()

# The "OSAL_EXT_SOURCE_DIR" cache variable may be set to a path
# on the host containing extra OS/BSP implementations which are not
# part of the open source release.
# CAUTION: The API between the OSAL and the low level implementation and/or BSP
# is not stabilized, and may change with every OSAL release.  No attempt is made
# to provide backward compatibility with external sources.
set(OSAL_EXT_SOURCE_DIR "$ENV{OSAL_EXT_SOURCE_DIR}"
    CACHE PATH "External source directory to check for additional OS/BSP implementations")

# Read the default compile-time configuration, and update with
# any mission/project specific options in the OSAL_CONFIGURATION_FILE
include("${OSAL_SOURCE_DIR}/default_config.cmake")

# The user-specified file is optional, but in case the value is defined but the
# file does not exist, this should be treated as an error.
foreach(CONFIG ${OSAL_CONFIGURATION_FILE})
    include(${CONFIG})
endforeach(CONFIG OSAL_CONFIGURATION_FILE)

# Use the supplied configuration to generate the osconfig.h file
# which can be referenced by the code.  This will be stored in the top level
# "inc" directory of the binary output directory
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/inc")
configure_file(
    "${OSAL_SOURCE_DIR}/osconfig.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/osconfig.gen"
    @ONLY
)

# Only copy the osconfig.h into place if different from the existing file
# This avoids unnecessarily rebuilding all code in case cmake was re-run
# and but generated the same file.
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_BINARY_DIR}/osconfig.gen"
    "${CMAKE_CURRENT_BINARY_DIR}/inc/osconfig.h"
)

# The "osal_public_api" is an interface target that will always be defined by this script,
# and refers to the complete set of public API header files for OSAL
add_library(osal_public_api INTERFACE)

# The initial set of directories that define the OSAL API
# This is used to initialize the interface include directory property of external targets
target_include_directories(osal_public_api INTERFACE
    $<BUILD_INTERFACE:${OSAL_SOURCE_DIR}/src/os/inc>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/inc>
    $<INSTALL_INTERFACE:include/osal>
)
if (OSAL_OMIT_DEPRECATED)
    target_compile_definitions(osal_public_api INTERFACE OSAL_OMIT_DEPRECATED)
endif (OSAL_OMIT_DEPRECATED)


# OSAL_SYSTEM_BSPTYPE indicate which of the BSP packages
# to build.  If not defined then no OSAL implementation will be built, however
# the api interface target (osal_public_api) is still usable.  Therefore
# this is not considered a fatal error for that reason.  If called with no BSP type,
# then only the osal_public_api interface target will be defined, and the script ends here.
if (NOT DEFINED OSAL_SYSTEM_BSPTYPE)
  message(STATUS "OSAL_SYSTEM_BSPTYPE not defined, no OSAL implementation will be compiled")
  return()
endif ()

# The initial set of directories that define the OSAL API
# This is used to initialize the interface include directory property of external targets
set(OSAL_API_INCLUDE_DIRECTORIES
    "${OSAL_SOURCE_DIR}/src/os/inc"
    "${CMAKE_CURRENT_BINARY_DIR}/inc"
)
include_directories(${OSAL_API_INCLUDE_DIRECTORIES})

# In case the OSAL_USER_C_FLAGS was specified, use them
add_definitions(${OSAL_USER_C_FLAGS})

#
# Build UT assert -
# the basic ut_assert library target is always defined regardless of "ENABLE_UNIT_TESTS",
# This is because the library is usable with functional tests, not just unit (coverage) tests.
# This is done early, so that other targets may reference UT_ASSERT_SOURCE_DIR if needed
# Note that UT assert is only usable with an OSAL BSP implementation, thus this target
# cannot be compiled unless a specific BSP is selected.
add_subdirectory(ut_assert)

# The "ut_osapi_stubs" library contains stub functions of the OSAL API calls, used for
# testing _other_ application code that uses OSAL.  Like UT Assert, this is also
# provided independently of the local ENABLE_UNIT_TESTS switch, because it mainly affects
# testing of other components.  That is, even if OSAL itself is not being tested in this
# build, a dependent application may still be built with testing enabled.
add_subdirectory(src/ut-stubs ut-stubs)

#
# Step 1:
# Build the BSP layer
#

if (OSAL_EXT_SOURCE_DIR AND IS_DIRECTORY "${OSAL_EXT_SOURCE_DIR}/${OSAL_SYSTEM_BSPTYPE}")
    set(OSAL_BSP_SOURCE_DIR "${OSAL_EXT_SOURCE_DIR}/${OSAL_SYSTEM_BSPTYPE}")
elseif(IS_DIRECTORY "${OSAL_SOURCE_DIR}/src/bsp/${OSAL_SYSTEM_BSPTYPE}")
    set(OSAL_BSP_SOURCE_DIR "${OSAL_SOURCE_DIR}/src/bsp/${OSAL_SYSTEM_BSPTYPE}")
else()
  # It is an error if the indicated BSPTYPE does not correspond to a subdirectory
  # If this is not caught here then a more obfuscated error will occur later.
  message(FATAL_ERROR "Error: No source directory found for \"${OSAL_SYSTEM_BSPTYPE}\" BSP type")
endif()

message(STATUS "BSP Selection: ${OSAL_SYSTEM_BSPTYPE} at ${OSAL_BSP_SOURCE_DIR}")


# The BSP library is a separate target from OSAL and can be used
# independently of the OSAL library and/or in combination with
# UT assert and the OSAL stub library for unit testing.
#
# The Implementation-Specific BSP subdirectory should define
# an OBJECT target named "osal_${OSAL_SYSTEM_BSPTYPE}_impl"
add_subdirectory(${OSAL_BSP_SOURCE_DIR} ${OSAL_SYSTEM_BSPTYPE}_impl)

target_compile_definitions(osal_${OSAL_SYSTEM_BSPTYPE}_impl PRIVATE
    $<TARGET_PROPERTY:osal_public_api,INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(osal_${OSAL_SYSTEM_BSPTYPE}_impl PRIVATE
    $<TARGET_PROPERTY:osal_public_api,INTERFACE_COMPILE_OPTIONS>
)
target_include_directories(osal_${OSAL_SYSTEM_BSPTYPE}_impl PRIVATE
    ${OSAL_SOURCE_DIR}/src/bsp/shared/inc
)

# Confirm that the selected OS is compatible with the selected BSP.
get_target_property(OSAL_EXPECTED_OSTYPE osal_${OSAL_SYSTEM_BSPTYPE}_impl OSAL_EXPECTED_OSTYPE)
if (OSAL_EXPECTED_OSTYPE)
    if (NOT DEFINED OSAL_SYSTEM_OSTYPE)
        # In the event that OSAL_SYSTEM_OSTYPE was not specified at all,
        # implicitly assume the expected OSTYPE.
        set(OSAL_SYSTEM_OSTYPE ${OSAL_EXPECTED_OSTYPE})
    elseif(NOT OSAL_SYSTEM_OSTYPE STREQUAL OSAL_EXPECTED_OSTYPE)
        # Generate a warning about the OSTYPE not being expected.
        # Not calling this a fatal error because it could possibly be intended during development
        message(WARNING "Mismatched BSP/OS: ${OSAL_SYSTEM_BSPTYPE} implies ${OSAL_EXPECTED_OSTYPE}, but ${OSAL_SYSTEM_OSTYPE} is configured")
    endif(NOT DEFINED OSAL_SYSTEM_OSTYPE)
endif (OSAL_EXPECTED_OSTYPE)

set(BSP_SRCLIST
    src/bsp/shared/src/osapi-bsp.c
    src/bsp/shared/src/bsp_default_app_run.c
    src/bsp/shared/src/bsp_default_app_startup.c
    src/bsp/shared/src/bsp_default_symtab.c
    src/bsp/shared/src/bsp_default_resourcecfg.c
)

# Define the external "osal_bsp" static library target
add_library(osal_bsp STATIC
    ${BSP_SRCLIST}
    $<TARGET_OBJECTS:osal_${OSAL_SYSTEM_BSPTYPE}_impl>
)

target_link_libraries(osal_bsp PUBLIC
    osal_public_api
)

target_include_directories(osal_bsp PRIVATE
    ${OSAL_SOURCE_DIR}/src/bsp/shared/inc
)


#
# Step 2:
# Build the OSAL layer
#

# OSAL_SYSTEM_OSTYPE indicates which of the OS packages
# to build.  If not defined, this may be inferred by the BSP type.
if (NOT DEFINED OSAL_SYSTEM_OSTYPE)
  message(FATAL_ERROR "OSAL_SYSTEM_OSTYPE must be set to the appropriate OS")
endif ()
if (OSAL_EXT_SOURCE_DIR AND IS_DIRECTORY "${OSAL_EXT_SOURCE_DIR}/${OSAL_SYSTEM_OSTYPE}")
    set(OSAL_OS_SOURCE_DIR "${OSAL_EXT_SOURCE_DIR}/${OSAL_SYSTEM_OSTYPE}")
elseif(IS_DIRECTORY "${OSAL_SOURCE_DIR}/src/os/${OSAL_SYSTEM_OSTYPE}")
    set(OSAL_OS_SOURCE_DIR "${OSAL_SOURCE_DIR}/src/os/${OSAL_SYSTEM_OSTYPE}")
else()
  # It is an error if the indicated OSTYPE does not correspond to a subdirectory
  # If this is not caught here then a more obfuscated error will occur later.
  message(FATAL_ERROR  "Error: No source directory found for \"${OSAL_SYSTEM_OSTYPE}\" OS type")
endif()


message(STATUS "OSAL Selection: ${OSAL_SYSTEM_OSTYPE} at ${OSAL_OS_SOURCE_DIR}")

# The implementation-specific OSAL subdirectory should define
# an OBJECT target named "osal_${OSAL_SYSTEM_OSTYPE}_impl"
add_subdirectory(${OSAL_OS_SOURCE_DIR} ${OSAL_SYSTEM_OSTYPE}_impl)

target_compile_definitions(osal_${OSAL_SYSTEM_OSTYPE}_impl PRIVATE
    $<TARGET_PROPERTY:osal_public_api,INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(osal_${OSAL_SYSTEM_OSTYPE}_impl PRIVATE
    $<TARGET_PROPERTY:osal_public_api,INTERFACE_COMPILE_OPTIONS>
)
# The "shared" directory contains internal components which
# are referenced in implementation OSAL modules, but should _NOT_
# be referenced outside the OSAL code
target_include_directories(osal_${OSAL_SYSTEM_OSTYPE}_impl PRIVATE
    ${OSAL_SOURCE_DIR}/src/os/shared/inc
    ${OSAL_SOURCE_DIR}/src/bsp/shared/inc
)

set(OSAL_SRCLIST
    src/os/shared/src/osapi-binsem.c
    src/os/shared/src/osapi-clock.c
    src/os/shared/src/osapi-common.c
    src/os/shared/src/osapi-condvar.c
    src/os/shared/src/osapi-countsem.c
    src/os/shared/src/osapi-dir.c
    src/os/shared/src/osapi-errors.c
    src/os/shared/src/osapi-file.c
    src/os/shared/src/osapi-filesys.c
    src/os/shared/src/osapi-heap.c
    src/os/shared/src/osapi-idmap.c
    src/os/shared/src/osapi-module.c
    src/os/shared/src/osapi-mutex.c
    src/os/shared/src/osapi-network.c
    src/os/shared/src/osapi-printf.c
    src/os/shared/src/osapi-queue.c
    src/os/shared/src/osapi-select.c
    src/os/shared/src/osapi-shell.c
    src/os/shared/src/osapi-sockets.c
    src/os/shared/src/osapi-task.c
    src/os/shared/src/osapi-timebase.c
    src/os/shared/src/osapi-time.c
    src/os/shared/src/osapi-version.c
)

if (OSAL_CONFIG_DEBUG_PRINTF)
    list(APPEND OSAL_SRCLIST
        src/os/shared/src/osapi-debug.c
    )
endif (OSAL_CONFIG_DEBUG_PRINTF)


# Define the external "osal" static library target
# This is a combination of the generic parts with the low level
# system-specific parts
add_library(osal STATIC
    ${OSAL_SRCLIST}
    $<TARGET_OBJECTS:osal_${OSAL_SYSTEM_OSTYPE}_impl>
)

target_include_directories(osal PRIVATE
    ${OSAL_SOURCE_DIR}/src/os/shared/inc
    ${OSAL_SOURCE_DIR}/src/bsp/shared/inc
)

# Link the OSAL with the BSP
target_link_libraries(osal PUBLIC
    osal_public_api
    osal_bsp
)

# The "build_options.cmake" file within each component may
# fine-tune the library for this particular build.  This is included
# AFTER The basic targets are defined, so it may set properties
# on the defined targets and/or use target-specific commands.
include("${OSAL_BSP_SOURCE_DIR}/build_options.cmake" OPTIONAL)
include("${OSAL_OS_SOURCE_DIR}/build_options.cmake" OPTIONAL)

# Report the full set of compile definitions after processing all modules
get_target_property(OSAL_PUBLIC_API_COMPILE_DEFINITIONS osal_public_api INTERFACE_COMPILE_DEFINITIONS)
message(STATUS "OSAL Compile Definitions: ${OSAL_PUBLIC_API_COMPILE_DEFINITIONS}")



#
# UNIT TEST SUPPORT
#
if (ENABLE_UNIT_TESTS)

    enable_testing()

    # Generic function for consistent definition of a unit testing target
    # This is defined here in the top-level OSAL CMakeLists so it can be used
    # in both the "tests" and "unit-tests" subdirectories.
    function(add_osal_ut_exe TGTNAME)

      add_executable(${TGTNAME} ${ARGN})
      target_link_libraries(${TGTNAME} PUBLIC ut_assert osal)
      add_test(${TGTNAME} ${TGTNAME})
      foreach(TGT ${INSTALL_TARGET_LIST})
        install(TARGETS ${TGTNAME} DESTINATION ${TGT}/${UT_INSTALL_SUBDIR})
      endforeach()

    endfunction(add_osal_ut_exe)

    # The "unit-test-coverage" is a white-box line coverage test.
    # It re-compiles each source unit for coverage analysis and links
    # with stub dependencies and a test sequence designed to execute
    # every line of source code within OSAL.
    add_subdirectory(src/unit-test-coverage unit-test-coverage)

    # The "tests" and "unit-tests" subdirectories implement examples and verification tests
    # of the OSAL library.  Note that these are both black box tests that link with the full
    # OSAL (not a stub/coverage test).
    add_subdirectory(src/tests tests)
    add_subdirectory(src/unit-tests unit-tests)

endif (ENABLE_UNIT_TESTS)

# If this build is being performed as a subdirectory within a larger project,
# then export the important data regarding compile flags/dirs to that parent
# This is conditional to avoid warnings in a standalone build.
if (HAS_PARENT)
    # Export the UT coverage compiler/linker flags to the parent build.
    # These flags are based on the target system type and should be used
    # when building code intended for coverage analysis.
    if (NOT OSAL_OMIT_DEPRECATED)
        set(UT_COVERAGE_COMPILE_FLAGS "${UT_COVERAGE_COMPILE_FLAGS}" PARENT_SCOPE)
        set(UT_COVERAGE_LINK_FLAGS "${UT_COVERAGE_LINK_FLAGS}" PARENT_SCOPE)
    endif()
else(HAS_PARENT)
    # In a standalone build, also add the documentation target(s)
    # Note that in a CFE/integrated build, it is expected this will be built separately.
    add_subdirectory(docs/src docs)
endif(HAS_PARENT)

if (OSAL_INSTALL_LIBRARIES)

    # Install and also export this library, so it can be found via
    # "find_package()" from some other CMake build
    install(
        TARGETS osal_public_api osal_bsp osal
        EXPORT nasa-osal-export
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/osal
        INCLUDES DESTINATION include/osal
    )
    install(DIRECTORY
        ${OSAL_SOURCE_DIR}/src/os/inc/
        ${CMAKE_CURRENT_BINARY_DIR}/inc/
      DESTINATION include/osal
    )
    install(EXPORT nasa-osal-export
      FILE NasaOsalTargets.cmake
      DESTINATION lib/cmake
    )
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/NasaOsalConfig.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/NasaOsalConfig.cmake"
        INSTALL_DESTINATION lib/cmake
    )
    install(FILES
          "${CMAKE_CURRENT_BINARY_DIR}/NasaOsalConfig.cmake"
        DESTINATION lib/cmake
    )

endif()
