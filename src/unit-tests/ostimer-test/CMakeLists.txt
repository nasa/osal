# CMake snippet for OSAL timer test

set(TEST_MODULE_FILES 
  ut_ostimer_timerio_test.c
  ut_ostimer_test.c)
  
add_osal_ut_exe(osal_timer_UT ${TEST_MODULE_FILES})

# Inject clock_getres() only in these Linux test executables.  The real OSAL
# initialization and public timer APIs run with independently tabulated inputs.
if(OSAL_SYSTEM_OSTYPE STREQUAL "posix" AND CMAKE_SYSTEM_NAME STREQUAL "Linux"
   AND CMAKE_C_COMPILER_ID MATCHES "^(GNU|Clang)$")
  function(add_timer_resolution_test RESOLUTION_NS EXPECTED_US)
    set(TGTNAME osal_timer_resolution_${RESOLUTION_NS}ns_UT)
    add_osal_ut_exe(${TGTNAME} ut_ostimer_resolution_test.c)
    target_compile_definitions(${TGTNAME} PRIVATE
      UT_CLOCK_RESOLUTION_NS=${RESOLUTION_NS}
      UT_EXPECTED_ACCURACY_US=${EXPECTED_US})
    set_property(TARGET ${TGTNAME} APPEND_STRING PROPERTY
      LINK_FLAGS " -Wl,--wrap=clock_getres")
    set_tests_properties(${TGTNAME} PROPERTIES TIMEOUT 30)
  endfunction()

  # [nanoseconds, microseconds]; nearest rounding, with exact ties rounded up.
  add_timer_resolution_test(1 0)
  add_timer_resolution_test(499 0)
  add_timer_resolution_test(500 1)
  add_timer_resolution_test(501 1)
  add_timer_resolution_test(999 1)
  add_timer_resolution_test(1000 1)
  add_timer_resolution_test(1499 1)
  add_timer_resolution_test(1500 2)
  add_timer_resolution_test(1501 2)
  add_timer_resolution_test(999999499 999999)
  add_timer_resolution_test(999999500 1000000)
  add_timer_resolution_test(999999999 1000000)
endif()

