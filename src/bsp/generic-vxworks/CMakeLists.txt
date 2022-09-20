######################################################################
#
# CMAKE build recipe for Generic VxWorks Board Support Package (BSP)
#
######################################################################

add_library(osal_generic-vxworks_impl OBJECT
	src/bsp_start.c
	src/bsp_console.c
)

# The "-u" switch is required to ensure that the linker pulls in the OS_BSPMain entry point
target_link_libraries(osal_public_api INTERFACE -uOS_BSPMain)

# This BSP only works with "vxworks" OS layer.
# Confirming this reduces risk of accidental misconfiguration
set_property(TARGET osal_generic-vxworks_impl PROPERTY OSAL_EXPECTED_OSTYPE "vxworks")
