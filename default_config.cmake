##########################################################################
#
# Default configuration options for OSAL
#
# This file specifies the default values for various compile-time options
# supported by OSAL.  These options can be further tuned by the specific
# OSAL and BSP selection, as well as the user application.
#
# NOTE:
# The defaults in this file should _not_ be directly modified for tuning.
#
# Instead, use the "OSAL_CONFIGURATION_FILE" CMake variable to define
# a custom file which can override/modify the configuration for the items
# which require tuning.
#
##########################################################################

##############################################################
# Argument/Bug-checking options
##############################################################

# OSAL_CONFIG_BUGCHECK_DISABLE
# ----------------------------------
#
# Disable/compile-out the "bugcheck" macro
#
# The bugcheck macro is used to validate the inputs to functions and/or
# assert on other conditions that should _always_ be true.  If any of these
# conditions ever evaluate as false, it indicates a bug in the code -
# either in the OSAL or the application which invoked OSAL.
#
# If set FALSE (default), then the OSAL bugcheck macro will evaluate its
# boolean conditional and generate an action if that conditional evaluates
# false.  (The specific action to take is configured via a different
# directive -- see OSAL_CONFIG_BUGCHECK_STRICT).
#
# These extra bug checks do consume a slight bit of code+data space as
# well as some runtime CPU cycles on every call, depending on the conditions
# being tested.
#
# Once the application has reached a sufficient level of stability and
# confidence is obtained that these bug checks are not possible to be
# triggered, this directive may be set TRUE which disables the bug checks
# completely - rendering these statements as no-ops.
#
set(OSAL_CONFIG_BUGCHECK_DISABLE                FALSE)


# OSAL_CONFIG_BUGCHECK_STRICT
# ----------------------------------
#
# Select a strict implementation for the "bugcheck" macro
#
# If set FALSE (default), then the OSAL bugcheck macro will generate a
# debug message and return an error code if the conditional evaluates
# as false.  This is a soft error - the application will get the
# error code and keep running.
#
# If set to TRUE, then any failure of any bugcheck macro is considered
# fatal and will trigger an abort().  On many platforms this will
# generate an abnormal application exit with a core file for debugging.
#
set(OSAL_CONFIG_BUGCHECK_STRICT                  FALSE)


##############################################################
# Code/Feature Selection Options for the OSAL implementation
##############################################################


# OSAL_CONFIG_INCLUDE_NETWORK
# ----------------------------------
#
# Whether to include the Network API
#
# If set TRUE, the socket abstraction (if applicable on the platform)
# will be included.  If set FALSE, then all calls to the network API will
# return OS_ERR_NOT_IMPLEMENTED.
#
# This can be set FALSE for platforms which do not have a network or
# IP stack available, or to save code space if the application does
# not use network resources.
#
set(OSAL_CONFIG_INCLUDE_NETWORK                 TRUE
    CACHE BOOL "Whether to include the Network API"
)


#
# OSAL_CONFIG_INCLUDE_DYNAMIC_LOADER
# ----------------------------------
#
# Whether to include the capability of loading dynamic code objects
#
# This is normally set TRUE to support modularized applications on
# platforms which have this capability.
#
# For deployments which are always statically linked, this may be set
# FALSE for a smaller library size and reduced linking requirements.
#
set(OSAL_CONFIG_INCLUDE_DYNAMIC_LOADER          TRUE
    CACHE BOOL "Whether to include the Dynamic Loader feature"
)


#
# OSAL_CONFIG_INCLUDE_STATIC_LOADER
# ----------------------------------
#
# Whether to include a compatibility "loader" for statically-linked objects
#
# This feature allows applications normally written for dynamic module loading
# operate transparently in a static link environment.  If this is set TRUE,
# then the application must supply an object named "OS_STATIC_SYMBOL_TABLE" that
# contains the names and addresses of statically-linked symbols that should
# be known to the lookup/load functions.
#
# Note that modules "loaded" using this abstraction are still assigned a
# module ID and still require a slot in the module table even though
# no actual runtime loading is performed (see OSAL_CONFIG_MAX_MODULES).
#
set(OSAL_CONFIG_INCLUDE_STATIC_LOADER           TRUE
    CACHE BOOL "Whether to include the Static Loader feature"
)

#
# OSAL_CONFIG_INCLUDE_SHELL
# ----------------------------------
#
# Whether to include features which utilize the operating system shell.
#
# Remote Shell commands can be very powerful tool for remotely diagnosing
# and mitigating runtime issues in the field, but also have significant
# security implications.  If this is set to "false" then shell functionality
# is disabled and OSAL functions which invoke the shell will return
# OS_ERR_NOT_IMPLEMENTED.
#
set(OSAL_CONFIG_INCLUDE_SHELL                   FALSE
    CACHE BOOL "Whether to include the Shell API"
)


#
# OSAL_CONFIG_DEBUG_PERMISSIVE_MODE
# ----------------------------------
#
# The OSAL_CONFIG_DEBUG_PERMISSIVE_MODE option controls how privileged operations
# are handled by the OSAL in the event that the user does not have sufficient permission.
# In particular this applies to task priorities and message queues.
#
# If set FALSE, then all permissions are enforced, and a failure due to lack of permission
# will cause a failure of the overall operation, which is passed back to the application.
#
# If set to TRUE, this will treat some errors non-fatal and enable a graceful fallback,
# allowing the overall operation to complete in a reduced form.  This makes the
# OSAL library compatible with a non-root (normal user mode) environment.
#
# In the PC-Linux/Posix build, this means:
#  - A message queue deeper than the maximum system limit will be silently truncated
#    to the maximum system limit (no error).
#  - If the user does not have permission to create elevated priority tasks, then the tasks will
#    be created at the default priority (no error).
#
set(OSAL_CONFIG_DEBUG_PERMISSIVE_MODE           FALSE
    CACHE BOOL "Disable enforcement of privileged operations"
)

#
# OSAL_CONFIG_DEBUG_PRINTF
# ----------------------------------
#
# Controls inclusion of OS_DEBUG statements in the code
#
# If set FALSE, all OS_DEBUG statements are compiled out.
#
# If set TRUE, all the "OS_DEBUG" statements will be compiled in and displayed
# on the debug console.  The statements may still be suppressed at runtime.
#
set(OSAL_CONFIG_DEBUG_PRINTF                    FALSE
    CACHE BOOL "Controls inclusion of OS_DEBUG statements in the code"
)

#
# OS_CONFIG_CONSOLE_ASYNC
# ----------------------------------
#
# Controls whether the console device writes (OS_printf) will be deferred
# to a separate utility task or handled directly by the calling task.
#
# If set FALSE, the utility task WILL NOT be spawned, and all OS_printf()
# calls will be synchronously written to the console device.
#
# If set TRUE, an extra utility task WILL be spawned, and the data from
# all OS_printf() calls will be written to an output queue which is then
# transferred to the console device by the utility task.
#
# When this is TRUE (default), it may improve real time performance by not
# requiring the caller to delay on a potentially slow console device output.
#
# However decoupling in this manner requires creation of an extra task and
# stack to handle the output, and a side effect is that the OS_printf() output
# can become decoupled from the event/task where it actually occurred, or
# messages might appear in a different order than they originally occurred.
#
set(OSAL_CONFIG_CONSOLE_ASYNC                   TRUE
    CACHE BOOL "Controls spawning of a separate utility task for OS_printf"
)

#############################################
# Resource Limits for the OS API
#############################################

# Note that CMake does not have a cache type for integers;
# although they are defined as strings the values should be integer numbers.

# The maximum number of concurrently-running tasks to support
set(OSAL_CONFIG_MAX_TASKS               64
    CACHE STRING "Maximum Number of Tasks to support"
)

# The maximum number of queues to support
set(OSAL_CONFIG_MAX_QUEUES              64
    CACHE STRING "Maximum Number of Queues to support"
)

# The maximum number of counting semaphores to support
set(OSAL_CONFIG_MAX_COUNT_SEMAPHORES    20
    CACHE STRING "Maximum Number of Counting Semaphores to support"
)

# The maximum number of binary semaphores to support
set(OSAL_CONFIG_MAX_BIN_SEMAPHORES      20
    CACHE STRING "Maximum Number of Binary Semaphores to support"
)

# The maximum number of mutexes to support
set(OSAL_CONFIG_MAX_MUTEXES             20
    CACHE STRING "Maximum Number of Mutexes to support"
)

# The maximum number of loadable modules to support
# Note that emulating module loading for statically-linked objects also
# requires a slot in this table, as it still assigns an OSAL ID.
set(OSAL_CONFIG_MAX_MODULES             20
    CACHE STRING "Maximum Number of Modules to support"
)

# The maximum number of time base objects (reference for timers)
set(OSAL_CONFIG_MAX_TIMEBASES           5
    CACHE STRING "Maximum Number of Timebases to support"
)

# The maximum number of user timers / app callbacks that can be registered
set(OSAL_CONFIG_MAX_TIMERS              10
    CACHE STRING "Maximum Number of Timers to support"
)

# The maximum number of concurrently open file descriptors to support
set(OSAL_CONFIG_MAX_NUM_OPEN_FILES      50
    CACHE STRING "Maximum Number of Open Files to support"
)

# The maximum number of concurrently open directory descriptors to support
set(OSAL_CONFIG_MAX_NUM_OPEN_DIRS       4
    CACHE STRING "Maximum Number of Open Directories to support"
)

# The maximum number of file systems that can be managed by OSAL
set(OSAL_CONFIG_MAX_FILE_SYSTEMS        14
    CACHE STRING "Maximum Number of File Systems to support"
)

# The maximum length for a file name, including any extension
# (This does not include the directory part)
set(OSAL_CONFIG_MAX_FILE_NAME           20
    CACHE STRING "Maximum Length of file names"
)

# Maximum length for a virtual path name (virtual directory + file)
set(OSAL_CONFIG_MAX_PATH_LEN            64
    CACHE STRING "Maximum Length of path names"
)

# Maximum length allowed for an object (task,queue....) name
set(OSAL_CONFIG_MAX_API_NAME            20
    CACHE STRING "Maximum Length of resource names"
)

# Maximum length of a symbol name for OS_SymbolLookup()
set(OSAL_CONFIG_MAX_SYM_LEN             64
    CACHE STRING "Maximum Length of symbol names"
)

# Maximum length of a network socket address
# This is only relevant if network support is included, and the
# required length depends on the address families in use
set(OSAL_CONFIG_SOCKADDR_MAX_LEN        28
    CACHE STRING "Maximum Size of network socket addresses"
)

# Maximum length of a single message produced by OS_printf()
set(OSAL_CONFIG_PRINTF_BUFFER_SIZE      172
    CACHE STRING "Maximum Length of single printf message"
)

# Maximum number of OS_printf() messages that will be buffered
set(OSAL_CONFIG_PRINTF_BUFFER_DEPTH     100
    CACHE STRING "Maximum Number of printf messages to buffer"
)

# Priority level of a console output helper task
#
# Set logically low (high number) to maximize performance.
#   - Messages from OS_printf() may show on the console with some delay
#     but should have minimal impact to real time tasks.
#
# Set logically high (low number) for debugging
#   - Messages from OS_printf() will have more timely output, but may
#     adversely impact real time tasks.
set(OSAL_CONFIG_UTILITYTASK_PRIORITY    245
    CACHE STRING "Priority level for the background utility task"
)

# Stack size of console output task.
#
# This applies to RTOS layers with precise stack control,
# normally not necessary to change this unless the task implementation
# changes.
set(OSAL_CONFIG_UTILITYTASK_STACK_SIZE  2048
    CACHE STRING "Stack size for the background utility task"
)

# The size of a command that can be passed to the underlying OS
# Only applicable when shell feature is enabled
set(OSAL_CONFIG_MAX_CMD_LEN             1000
    CACHE STRING "Maximum Length of shell command"
)

# The maximum depth of an OSAL message queue.
# On some implementations this may affect the overall OSAL memory footprint
# so it may be beneficial to set this limit accordingly.
set(OSAL_CONFIG_QUEUE_MAX_DEPTH         50
    CACHE STRING "Maximum depth of message queue"
)
