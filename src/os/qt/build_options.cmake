#
# For POSIX systems based on glibc (i.e. Linux) certain features must be enabled
# Glibc headers support multiple standards and multiple versions of the standards,
# so the definitions of these macros determine which standard we will use
#
# OSAL needs conformance to at least POSIX.1c (aka POSIX 1995) - this includes all the
# real-time support and threading extensions.
#
# When compiling against glibc, using "_XOPEN_SOURCE=600" enables the X/Open 6 standard.
# XPG6 includes all necessary XPG5, POSIX.1c features as well as SUSv2/UNIX98 extensions.
# Although current POSIX implementation does not specifically use anything from XPG6 
# (only 5) it may be beneficial to enable XPG6 anyway, so applications can use the real
# time features that it adds.
#
# Note that this definition assumes glibc -- in case of compiling OSAL for some platform
# that supports POSIX but does not use glibc (e.g. uclibc) this definition shouldn't
# harm anything, but it may need to be tweaked.  
#
# See http://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html
# for a more detailed description of the feature test macros and available values
#
set(OSAL_C_FLAGS "${OSAL_C_FLAGS} -D_XOPEN_SOURCE=600")

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

#find_package(Qt5Core CONFIG REQUIRED)
#find_package(Qt5Core)
find_package(Qt5 COMPONENTS Core network REQUIRED)


SET(CMAKE_INCLUDE_CURRENT_DIR ON)


# OSAL_LINK_LIBS determines which system-level libraries must be included in the
# link command in order to produce the final binary.  These libs will be used for
# ALL targets that utilize the POSIX OS layer.  Additional target-specific libraries 
# may also be specified in the BSP or the cross-compile toolchain. 
set (OSAL_LINK_LIBS ${OSAL_LINK_LIBS}  dl rt Qt5 )

set (OSAL_INCLUDEDIR ${OSAL_INCLUDEDIR} ${QT5_INCLUDE_DIR} ${Qt5Core_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS}  )

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()