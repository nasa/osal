##########################################################################
#
# Build options for "pc-rtems" BSP
#
##########################################################################

# Link the RTEMS BSP with the "rtemscpu" system library
target_link_libraries(osal_bsp
      rtemscpu
)

# Add the networking library for RTEMS 6+
if(CMAKE_SYSTEM_VERSION GREATER 5)
    target_link_libraries(osal_bsp
         networking
    )
endif(CMAKE_SYSTEM_VERSION GREATER 5)
