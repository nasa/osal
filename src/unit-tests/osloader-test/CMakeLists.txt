# CMake snippet for OSAL core test

set(TEST_MODULE_FILES 
  ut_osloader_module_test.c
  ut_osloader_symtable_test.c    
  ut_osloader_test.c)

add_osal_ut_exe(osal_loader_UT ${TEST_MODULE_FILES})

# build many copies of the test module
# we need to have unique modules to load up to OS_MAX_MODULES
# This will cover up to $OSAL_CONFIG_MAX_MODULES + 1.  If needed
# this can be increased by increasing $OSAL_CONFIG_MAX_MODULES.
math(EXPR MOD "${OSAL_CONFIG_MAX_MODULES} + 1")
while(MOD GREATER 0)
  math(EXPR MOD "${MOD} - 1")
  add_library(MODULE${MOD} SHARED ut_module.c)
  set_target_properties(MODULE${MOD} PROPERTIES 
    COMPILE_DEFINITIONS "MODULE_NAME=module${MOD}" 
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY utmod)
  add_dependencies(osal_loader_UT MODULE${MOD})
  foreach(TGT ${INSTALL_TARGET_LIST})
    install(TARGETS MODULE${MOD} DESTINATION ${TGT}/${UT_INSTALL_SUBDIR}/utmod)
  endforeach()
endwhile(MOD GREATER 0)
