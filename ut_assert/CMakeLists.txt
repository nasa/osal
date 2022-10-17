######################################################################
#
# CMAKE recipe for the UT assert library
#
######################################################################

#
# The "ut_assert" library is the core GSFC-provided unit test library
#
project(UT_ASSERT C)

set(UT_ASSERT_SOURCE_LIST
    src/utassert.c
    src/utlist.c
    src/utstubs.c
    src/uttest.c
    src/uttools.c
)

# The "ut_assert" library is usable by ANY and ALL subsystem(s) that need
# to do unit testing of any kind.  This library implements an OSAL application
# that contains APIs to aid in unit testing.  It provides the OS_Application_Startup
# and OS_Application_Run functions that a normal standalone OSAL application would.

# It uses the same OSAL BSP as a normal application would use to provide the basic
# startup procedure and text message output abstractions.

# NOTE: This library does NOT include any stub functions here, as the configuration
# of stubs vs. real implementations are specific to the unit being tested.  All
# stub functions are compiled as separate libraries.

add_library(ut_assert STATIC
    ${UT_ASSERT_SOURCE_LIST}
    src/utbsp.c
)

target_include_directories(ut_assert PRIVATE
    ${OSAL_SOURCE_DIR}/src/bsp/shared/inc
)
	
target_include_directories(ut_assert PUBLIC
    $<BUILD_INTERFACE:${UT_ASSERT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include/ut_assert>
)
target_compile_definitions(ut_assert PUBLIC
    "_UNIT_TEST_"
)
target_link_libraries(ut_assert PUBLIC
    osal_bsp
)


# The "pic" variant of ut_assert is compiled as an
# object library to be included in another object,
# such as a loadable test app for CFE.
# It is compiled as position independent code (PIC)
# to support dynamic loading.
add_library(ut_assert_pic OBJECT EXCLUDE_FROM_ALL
    ${UT_ASSERT_SOURCE_LIST}
)
set_target_properties(ut_assert_pic PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE
)
target_include_directories(ut_assert_pic PUBLIC
    $<BUILD_INTERFACE:${UT_ASSERT_SOURCE_DIR}/inc>
    $<TARGET_PROPERTY:osal_public_api,INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(ut_assert_pic PUBLIC
    $<TARGET_PROPERTY:osal_public_api,INTERFACE_COMPILE_DEFINITIONS>
)

# The "ut_coverage_compile" is an interface target that contains the
# compiler options/definitions to enable coverage instrumentation in the
# generated objects.  It should be specified on files compiled for coverage
# analysis.
add_library(ut_coverage_compile INTERFACE)
target_link_libraries(ut_coverage_compile INTERFACE ut_assert)

# The "ut_coverage_link" is an interface target that contains options/definitions
# and any link libraries to enable coverage instrumentation in the final executable.
# It should be specified on coverage test executable targets.
add_library(ut_coverage_link INTERFACE)
target_link_libraries(ut_coverage_link INTERFACE ut_assert)

if (OSAL_INSTALL_LIBRARIES)

    install(
        TARGETS ut_assert ut_coverage_compile ut_coverage_link
        EXPORT nasa-osal-export
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/ut_assert
        INCLUDES DESTINATION include/ut_assert
    )
    install(DIRECTORY
        ${UT_ASSERT_SOURCE_DIR}/inc/
        DESTINATION include/ut_assert
    )
    install(
        PROGRAMS scripts/generate_stubs.pl
        DESTINATION bin
        RENAME utassert_generate_stubs.pl
    )

endif()
