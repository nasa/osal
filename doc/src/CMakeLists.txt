########################################################
#
# CMake Recipe to build OSAL API guide documentation
#
########################################################

#
# This CMake script currently defines a top-level target "apiguide"
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
    ${CMAKE_CURRENT_SOURCE_DIR}/osalmain.dox
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

foreach(DIR ${OSAL_API_INCLUDE_DIRECTORIES})
    file(GLOB OSAL_HEADERFILE_LIST ${DIR}/*.h)
    foreach(HDR ${OSAL_HEADERFILE_LIST})
        list(APPEND OSAL_DOC_DEPENDENCY_LIST ${HDR})
        file(TO_NATIVE_PATH "${HDR}" HDR)
        string(APPEND OSAL_NATIVE_APIGUIDE_SOURCEFILES " \\\n  ${HDR}")
    endforeach()
    file(TO_NATIVE_PATH "${DIR}" DIR)
    string(APPEND OSAL_NATIVE_INCLUDE_DIRS " \\\n  ${DIR}")
endforeach()

file(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/osal-apiguide-warnings.log OSAL_NATIVE_LOGFILE)
file(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/osal-common.doxyfile OSAL_NATIVE_COMMON_CFGFILE)
file(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/osal-apiguide.doxyfile OSAL_NATIVE_APIGUIDE_CFGFILE)

# generate the configuration files
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/osal-common.doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/osal-common.doxyfile
        @ONLY
)
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/osal-apiguide.doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/osal-apiguide.doxyfile
        @ONLY
)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/apiguide/html/index.html"
    COMMAND doxygen ${OSAL_NATIVE_APIGUIDE_CFGFILE}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/osal-apiguide.doxyfile ${OSAL_DOCFILE_LIST} ${OSAL_DOC_DEPENDENCY_LIST}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

add_custom_target(osal-apiguide
    COMMAND echo "OSAL API Guide: file://${CMAKE_CURRENT_BINARY_DIR}/apiguide/html/index.html"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/apiguide/html/index.html"
)
