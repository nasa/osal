######################################################################
#
# CMAKE build recipe for PC-RTEMS Board Support Package (BSP)
#
######################################################################

# Basic set of files
set(OS_BSP_SRCLIST
    src/bsp_console.c
    src/bsp_start.c
)

# If not dynamic loading, include Init and config
if (NOT RTEMS_DYNAMIC_LOAD)
    list(APPEND OS_BSP_SRCLIST
        src/bsp_init.c
        src/bsp_setupfs.c # TODO move to only if enabled
    )

    # TODO could add the two flags for FS support vs adding from include

    # Link the RTEMS BSP with the "rtemscpu" system library
    target_link_libraries(osal_public_api INTERFACE
          rtemscpu
    )
endif ()

if (RTEMS_NO_SHELL)
    list(APPEND OS_BSP_SRCLIST
        src/bsp_no_shell.c
    )
else ()
    list(APPEND OS_BSP_SRCLIST
        src/bsp_shell.c
    )
endif ()

# TODO add the cmdline if not RTEMS_NO_CMDLINE

add_library(osal_pc-rtems_impl OBJECT
    ${OS_BSP_SRCLIST}
)

# This definition is needed for the gethostname call
# By defining this, it avoids the need to use the -std=gnu99
#  instead of the preferred -std=c99 GCC switch
target_compile_definitions(osal_public_api INTERFACE
   _BSD_SOURCE
)

set_property(TARGET osal_pc-rtems_impl PROPERTY OSAL_EXPECTED_OSTYPE "rtems")
