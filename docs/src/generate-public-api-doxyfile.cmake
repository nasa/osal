#################################################################
#
# CMake helper script to generate list of header for API guide
#
#################################################################

# This helper script is needed to evaluate the "INTERFACE_INCLUDE_DIRECTORIES" and
# "INTERFACE_COMPILE_DEFINITIONS" properties on the osal_public_api target properly.
# Note that either of these property values may contain generator expressions, and
# thus can only be correctly evaluated in the context of a target command, hence the
# need for this helper script.
#
# The property values are passed in via the command line, and this converts it into
# a doxygen snippet containing of header files and predefined macro values.

message(STATUS "Generating OSAL API documentation input list")
set(OSAL_HEADERFILE_LIST)
set(OSALDOC_PREDEFINED)
separate_arguments(INCLUDE_DIRECTORIES)
separate_arguments(COMPILE_DEFINITIONS)
foreach(INPUT ${INCLUDE_DIRECTORIES})
    if (IS_DIRECTORY ${INPUT})
        message(STATUS "OSAL API: Scanning directory ${INPUT}")
        file(GLOB INPUT "${INPUT}/*.h")
    endif()
    list(APPEND OSAL_HEADERFILE_LIST ${INPUT})
endforeach()

foreach(HDR ${OSAL_HEADERFILE_LIST})
    list(APPEND OSAL_DOC_DEPENDENCY_LIST ${HDR})
    file(TO_NATIVE_PATH "${HDR}" HDR)
    string(APPEND OSAL_NATIVE_APIGUIDE_SOURCEFILES " \\\n  ${HDR}")
endforeach()

foreach(INPUT ${COMPILE_DEFINITIONS})
    string(APPEND OSALDOC_PREDEFINED " \\\n  ${INPUT}")
endforeach()

configure_file(${INPUT_TEMPLATE} ${OUTPUT_FILE} @ONLY)
