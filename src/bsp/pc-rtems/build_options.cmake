##########################################################################
#
# Build options for "pc-rtems" BSP
#
##########################################################################

# Link the RTEMS BSP with the "rtemscpu" system library
if(CMAKE_SYSTEM_VERSION GREATER 5)
    target_link_libraries(osal_bsp
         rtemscpu
         networking
    )
else()
    target_link_libraries(osal_bsp
          rtemscpu
    )
endif(CMAKE_SYSTEM_VERSION GREATER 5)
