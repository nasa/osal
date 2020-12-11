######################################################################
#
# CMAKE build recipe for QT OSAL implementation
#
######################################################################

#enable_language(CXX)
find_package(Qt5Core)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core network REQUIRED)

# This CMake script generates targets specific to the QT implementation
# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui

# Set up Qt
set(CMAKE_AUTOMOC ON)                    
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED) 
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Gui_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")


include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)


# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")


# The basic set of files which are always built
set(QT_BASE_SRCLIST
    src/os-impl-binsem.cxx
    src/os-impl-common.cxx
    src/os-impl-console.cxx
    src/os-impl-countsem.cxx
    src/os-impl-dirs.cxx
    src/os-impl-errors.cxx
    src/os-impl-files.cxx
    src/os-impl-filesys.cxx
    src/os-impl-heap.cxx
    src/os-impl-idmap.cxx
    src/os-impl-mutex.cxx
    src/os-impl-queues.cxx
    src/os-impl-tasks.cxx
    src/os-impl-timebase.cxx
    src/os-impl-qt-gettime.cxx
)


# Use portable blocks for basic I/O
set(QT_IMPL_SRCLIST
    ../portable/os-impl-posix-gettime.c
    ../portable/os-impl-console-bsp.c
    ../portable/os-impl-bsd-select.c
    ../portable/os-impl-posix-io.c
    ../portable/os-impl-posix-files.c
    ../portable/os-impl-posix-dirs.c
)

if (OSAL_CONFIG_INCLUDE_SHELL)
    list(APPEND QT_IMPL_SRCLIST
       src/os-impl-shell.cxx
    )
else ()
    list(APPEND QT_IMPL_SRCLIST
       ../portable/os-impl-no-shell.c
    )
endif ()

# If some form of module loading is configured,
# then build the module loader
if (OSAL_CONFIG_INCLUDE_DYNAMIC_LOADER)
    list(APPEND QT_IMPL_SRCLIST
        src/os-impl-loader.cxx
        ../portable/os-impl-posix-dl-loader.c
        ../portable/os-impl-posix-dl-symtab.c
    )
else ()
    list(APPEND QT_IMPL_SRCLIST
        src/os-impl-no-module.cxx
        ../portable/os-impl-no-loader.c
        ../portable/os-impl-no-symtab.c
    )
endif ()

if (OSAL_CONFIG_INCLUDE_NETWORK)
    list(APPEND QT_IMPL_SRCLIST
        ../portable/os-impl-bsd-sockets.c   # Use BSD socket layer implementation
        ../portable/os-impl-posix-network.c # Use QT-defined hostname/id implementation
    )
else()
    list(APPEND QT_IMPL_SRCLIST
        ../portable/os-impl-no-network.c    # non-implemented versions of all network APIs
        ../portable/os-impl-no-sockets.c    # non-implemented versions of all socket APIs
    )
endif ()

# Defines an OBJECT target named "osal_qt_impl" with selected source files
add_library(osal_qt_impl OBJECT
    ${QT_BASE_SRCLIST}
    ${QT_IMPL_SRCLIST}
)

set_target_properties(osal_qt_impl
    PROPERTIES
    CXX_STANDARD 11
    CXX_EXTENSIONS off
    )

#enable_language(CXX)
#target_compile_features(osal_qt_impl PRIVATE cxx_std_17)


