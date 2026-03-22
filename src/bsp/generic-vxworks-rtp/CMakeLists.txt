######################################################################
#
# CMAKE build recipe for LINUX Board Support Package (BSP)
#
######################################################################

# This basic implementation library should be generic enough to use
# on any Linux-based processor board, as well as a standard development PC.
add_library(osal_generic-vxworks-rtp_impl OBJECT
	src/bsp_start.c
	src/bsp_console.c
)

# The "-u" switch is required to ensure that the linker pulls in the OS_BSPMain entry point
target_link_libraries(osal_public_api INTERFACE -umain -uOS_Application_Run -uOS_Application_Start -uCFE_TBL_Manage -uCFE_TBL_GetAddress)

# This BSP only works with "vxworks-rtp" OS layer.
# Confirming this reduces risk of accidental misconfiguration
set_property(TARGET osal_generic-vxworks-rtp_impl PROPERTY OSAL_EXPECTED_OSTYPE "vxworks-rtp")

