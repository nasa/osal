########################################################
#
# CMake Recipe to build OSAL API guide documentation
#
########################################################

#
# This CMake script currently defines a top-level target "osal-apiguide"
# to build the OSAL API documentation.  This may be invoked either
# from the main OSAL CMakeLists.txt as a subdirectory (useful in the
# case of a self-contained/standalone build) or by a separate script
# (useful if integrating into a larger project with a separate doc build)
#
# To invoke from a separate documentation build, the following vars
# should be defined by the caller, before adding this subdirectory:
#
# OSAL_API_INCLUDE_DIRECTORIES :
#   The list of directories that have the OSAL API headers
#   This should include the path to osconfig.h to avoid warnings
#   about undefined references.
#
# OSALDOC_PREDEFINED :
#   Not used directly, but passed through to the "osal-common.doxyfile"
#   This may be used to indicate preprocessor definitions that the
#   documentation generator tool should be aware of
#
# Note that OSAL_API_INCLUDE_DIRECTORIES is defined by the parent script
# in a standalone build environment.
#

cmake_minimum_required(VERSION 3.5)
project(OSAL_DOCS NONE)

# List of dox files to include -
# note that order is relevant here, doxygen processes in the order listed.
set(OSAL_DOCFILE_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/osal_frontpage.dox
    ${CMAKE_CURRENT_SOURCE_DIR}/osal_fs.dox
    ${CMAKE_CURRENT_SOURCE_DIR}/osal_timer.dox
)

# For the generated Doxyfiles, the various paths should be in native form
set(OSAL_NATIVE_APIGUIDE_SOURCEFILES)
set(OSAL_NATIVE_INCLUDE_DIRS)
set(OSAL_DOC_DEPENDENCY_LIST)

foreach(SRC ${OSAL_DOCFILE_LIST})
    file(TO_NATIVE_PATH "${SRC}" SRC)
    string(APPEND OSAL_NATIVE_APIGUIDE_SOURCEFILES " \\\n  ${SRC}")
endforeach()

# The complete list of public API include directories may be determined by reading the property of
# the "osal_public_api" interface target.  However, the CFE documentation build may want to influence
# this and provide an alternate path, mainly to provide its own alternate version of "osconfig.h" for
# docs.  It should be preferred to pull this info from the actual source such that it will track any
# changes made within OSAL as far as header organization goes.
if (TARGET osal_public_api AND NOT OSAL_API_INCLUDE_DIRECTORIES)
    get_target_property(OSAL_API_INCLUDE_DIRECTORIES osal_public_api INTERFACE_INCLUDE_DIRECTORIES)
    if (NOT OSAL_API_INCLUDE_DIRECTORIES)
        set (OSAL_API_INCLUDE_DIRECTORIES)
    endif()
    get_target_property(OSAL_API_COMPILE_DEFINITIONS osal_public_api INTERFACE_COMPILE_DEFINITIONS)
    if (NOT OSAL_API_COMPILE_DEFINITIONS)
        set (OSAL_API_COMPILE_DEFINITIONS)
    endif()
endif ()

# Generate the list of actual header files from the directories specified.  This is done
# as a target that runs a separate script such that generator expressions can be evaluated.
# This is done as a custom target such that it runs and gets updated every build
add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/docs/osal-public-api.doxyfile"
    COMMAND ${CMAKE_COMMAND}
        -DINCLUDE_DIRECTORIES="${OSAL_API_INCLUDE_DIRECTORIES}"
        -DCOMPILE_DEFINITIONS="${OSAL_API_COMPILE_DEFINITIONS}"
        -DINPUT_TEMPLATE="${CMAKE_CURRENT_SOURCE_DIR}/osal-public-api.doxyfile.in"
        -DOUTPUT_FILE="${CMAKE_BINARY_DIR}/docs/osal-public-api.doxyfile"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/generate-public-api-doxyfile.cmake"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

add_custom_target(osal_public_api_headerlist
    DEPENDS "${CMAKE_BINARY_DIR}/docs/osal-public-api.doxyfile")

# if building as part of CFS, then generate the doxygen header list as part of the prebuild step
# The "doc-prebuild" target is defined by the CFS build, and thus will not exist if building standalone
if (TARGET doc-prebuild)
   add_dependencies(doc-prebuild osal_public_api_headerlist)
endif ()

file(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/osal-apiguide-warnings.log OSAL_NATIVE_LOGFILE)
file(TO_NATIVE_PATH ${CMAKE_BINARY_DIR}/docs/osal-common.doxyfile OSAL_NATIVE_COMMON_CFGFILE)
file(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/osal-apiguide.doxyfile OSAL_NATIVE_APIGUIDE_CFGFILE)
file(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/default-settings.doxyfile OSAL_NATIVE_DEFAULT_SETTINGS)

# Add a top level source directory if not defined
if (NOT DEFINED MISSION_SOURCE_DIR)
    set(MISSION_SOURCE_DIR ${CMAKE_SOURCE_DIR})
endif()

# generate the configuration files
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/osal-common.doxyfile.in
        ${CMAKE_BINARY_DIR}/docs/osal-common.doxyfile
        @ONLY
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/osal-apiguide.doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/osal-apiguide.doxyfile
        @ONLY
)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/html/index.html"
    COMMAND doxygen ${OSAL_NATIVE_APIGUIDE_CFGFILE}
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/osal-apiguide.doxyfile"
            "${CMAKE_BINARY_DIR}/docs/osal-common.doxyfile"
            "${CMAKE_BINARY_DIR}/docs/osal-public-api.doxyfile"
            ${OSAL_DOCFILE_LIST} ${OSAL_DOC_DEPENDENCY_LIST}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

add_custom_target(osal-apiguide
    COMMAND echo "OSAL API Guide: file://${CMAKE_CURRENT_BINARY_DIR}/html/index.html"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/html/index.html"
)
