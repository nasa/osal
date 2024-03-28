######################################################################
#
# CMAKE build recipe for GENERIC-RTEMS Board Support Package (BSP)
#
######################################################################

# Basic set of files
set(OS_BSP_SRCLIST
    src/bsp_console.c
    src/bsp_init.c
    src/bsp_start.c
)

# Source select file system setup implementation
if (RTEMS_INCLUDE_TARFS)
    list(APPEND OS_BSP_SRCLIST
        src/bsp_tarfs_setupfs.c
    )
else ()
    # NOTE: rtems config needs to define supporting configuration (FILESYSTEM and DRIVERs)
    list(APPEND OS_BSP_SRCLIST
        src/bsp_mount_setupfs.c
    )
endif ()

# Link rtemscpu to osal public api if not dynamic loading
if (NOT RTEMS_DYNAMIC_LOAD)
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

if (RTEMS_NO_CMDLINE)
    list(APPEND OS_BSP_SRCLIST
        src/bsp_no_cmdline.c
    )
else ()
    list(APPEND OS_BSP_SRCLIST
        src/bsp_cmdline.c
    )
endif ()

add_library(osal_generic-rtems_impl OBJECT
    ${OS_BSP_SRCLIST}
)

# This definition is needed for the gethostname call
# By defining this, it avoids the need to use the -std=gnu99
#  instead of the preferred -std=c99 GCC switch
target_compile_definitions(osal_public_api INTERFACE
   _BSD_SOURCE
)

set_property(TARGET osal_generic-rtems_impl PROPERTY OSAL_EXPECTED_OSTYPE "rtems")

# The list of header files that control configuration
set(BSP_RTEMS_CONFIG_FILE_LIST
  bsp_rtems_cfg.h
)

# Create wrappers around all the config header files
# This makes them individually overridable by the missions, without modifying
# the distribution default copies
foreach(BSP_RTEMS_CFGFILE ${BSP_RTEMS_CONFIG_FILE_LIST})
  get_filename_component(CFGKEY "${BSP_RTEMS_CFGFILE}" NAME_WE)
  if (DEFINED BSP_RTEMS_CFGFILE_SRC_${CFGKEY})
    set(DEFAULT_SOURCE GENERATED_FILE "${BSP_RTEMS_CFGFILE_SRC_${CFGKEY}}")
  else()
    set(DEFAULT_SOURCE FALLBACK_FILE "${CMAKE_CURRENT_LIST_DIR}/config/default_${BSP_RTEMS_CFGFILE}")
  endif()
  generate_config_includefile(
    FILE_NAME           "${BSP_RTEMS_CFGFILE}"
    ${DEFAULT_SOURCE}
  )
endforeach()
