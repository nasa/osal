######################################################################
#
# CMAKE build recipe for PC-RTEMS Board Support Package (BSP)
#
######################################################################

add_library(osal_pc-rtems_impl OBJECT
	src/bsp_start.c
	src/bsp_console.c
)

# This definition is needed for the gethostname call
# By defining this, it avoids the need to use the -std=gnu99
#  instead of the preferred -std=c99 GCC switch
target_compile_definitions(osal_public_api INTERFACE
   _BSD_SOURCE
)

# Link the RTEMS BSP with the "rtemscpu" system library
target_link_libraries(osal_public_api INTERFACE
      rtemscpu
)

set_property(TARGET osal_pc-rtems_impl PROPERTY OSAL_EXPECTED_OSTYPE "rtems")
