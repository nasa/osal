# Introduction

## Scope

The purpose of this document is to provide guidelines and conventions
for the configuration and deployment of the Operating System Abstraction
Layer (OSAL) to a desired platform or platforms.

## Background

The goal OS Abstraction Layer is to promote the creation of portable and
reusable real time embedded system software. Given the necessary OS
abstraction layer implementations, the same embedded software should
compile and run on a number of platforms ranging from spacecraft
computer systems to desktop PCs.

## Applicable Documents

| **Document ID** | **Document Title** |
| --- | --- |
| | |


## Acronyms

|   **Acronym**  |  **Description** |
|---|---|
| OS  | Operating System  |
|  API | Application Programming Interface  |
| CM  | Configuration Management  |
| CPU | Central Processing Unit  |
|  EEPROM |  Electrically Erasable Programmable Read-Only Memory |
|  HW, H/W | Hardware  |
|  RAM |  Random-Access Memory |
|  SW, S/W | Software |
|  TBD | To Be Determined |

## Glossary of Terms

The following table defines the terms used throughout this document.
These terms are identified as proper nouns and are capitalized.

| **Term**    |  **Definition**     |
|---|---|
| Application  (APP)   |  A generic term for a computer program in a desktop or embedded system.  An Application is generally not part of the operating system.  |
|  Application  Programmer's  Interface (API) | A set of routines, protocols, and tools for building software applications  |
| Board Support  Package (BSP)  | A collection of user-provided facilities that interface an OS and the cFE  with a specific hardware platform. The BSP is responsible for hardware initialization.  |
| Core Flight  Executive (cFE)  |  A runtime environment and a set of services for hosting FSW Applications  |
|  Cyclic  Redundancy  Check | A polynomial based method for checking that a data set has remained unchanged from one time period to another.    |
|  Developer  | Anyone who is coding a software Application. |
|  Hardware  Platform  | The target hardware that hosts the Operating System and Application. |
| Interface  Control  Document |  A document that describes the software interface, in detail, to another piece  of software or hardware.  |
|  I/O Data     | Any data being written to and read from an I/O port. No structure is placed  on the data and no distinction as  to the type of I/O device. I/O data is defined separately  from memory data because it has a separate API and it's an optional interface of the cFE.  |
| Log   |   A collection of data that an application stores that provides information to diagnose  and debug FSW problems.  |
|  Memory Data |  Any data being written to and read from memory. No structure is placed  on the data and no distinction as to the type of memory is made.  |
| MMU   |  Memory Management Unit. A piece of hardware that manages virtual memory systems.  It automatically translates addresses into physical addresses so that an application can  be linked with one set of addresses but actually reside in a different part of memory. |
| Network | A connection between subsystems used for communication purposes.  |
| Platform  | See "Hardware Platform" above.   |
| User  | Anyone who interacts with the Software Application or system in its operational state.  A user can be a developer, a tester, an operator, or a maintainer.

# How to Configure, Build, and Run the OSAL


The OSAL distribution includes a complete development environment with
support for a number of processors and operating systems. The OSAL
development environment has been designed to isolate the portable OS
source code from the OSAL applications, configuration parameters, and
build products. The development environment is an example of how to
configure and build portable software using the OSAL code, but it is by
no means a requirement to use the OSAL. The included platforms for the
OSAL can be used as starting points for other boards and CPUs.

The following sections provide instructions on how to:

-   Setup the build environment

-   Configure the build directory for an OSAL application

-   Configure an OSAL Application

-   Build the OSAL Application

-   Load the OSAL Application on to the target platform

-   Run the OSAL Application on the target platform

In the current OSAL release, the CMake tool is used to generate makefiles
to perform actual build.  The "cmake" build offers increased features by
introducing additional scripting and build-time configurability,
allowing direct inclusion into larger projects with less need to modify
files to support a specific target or configuration.

## Setup the Build Environment

This section details the steps needed to setup the OSAL source
distribution and prepare the host development environment to build the
OSAL.

### Setup the OSAL Source Distribution

Get a copy of the OSAL source distribution directory on your build
machine. The source distribution has the following directories:

#### OSAL source distribution directories

| **Directory** |  **Description**  |
|---|---|
| `osal`                        | The top level OSAL source distribution directory.  |
| `osal/src`                    | The src directory contains the OSAL source, and make rules. |
| `osal/src/examples`           | The sample directory contains the sample applications for the osal.  |
| `osal/src/tests`              | Contains a suite of tests intended to validate the general functionality of OSAL services  |
| `osal/src/unit-tests`         | Contains a suite of OSAL unit tests intended to validate the OSAL API, checking inputs, return codes, and outputs for each function.  |
| `osal/src/unit-test-coverage` | Contains a suite of tests intended to execute every possible branch/error handling path in the OSAL implementation.   |
| `osal/src/bsp`                | The bsp directory contains the platform specific code for the OSAL as well  as code to make the OSAL run on a particular platform. Everything in  this directory is used to adapt the OSAL and Applications to a particular  hardware platform. This directory also contains the startup code for the  example programs. The included platforms are generic enough that they may be  easy to port to other platforms and processor architectures. For example:  The bsp/mcf5235-rtems board support package was ported to an ARM processor  running RTEMS with minimal effort.  |
| `osal/src/os`                 | The os directory is the heart of the OSAL, containing the implementation of  the OSAL for each supported operating system. There is a sub-directory for  each supported operating system in this directory. The OSAL include files  are also contained in this directory (src/os/inc).  |
| `osal/src/inc`                | The inc directory contains system wide include files that are used by the  OSAL on all platforms.  |
| `osal/docs`                   | The docs directory contains the documentation and release notes for the OSAL. |


The osal directory can go just about anywhere on a host development
system.

#### Example directory structure locations

| **Host Operating System**    |  **Example Directory** |  **Notes** |
|---|---|---|
|  Windows/vxWorks 6 Development Shell  | C:\\osalproject\\osal  | Building on Windows with the vxWorks 6.x development tools requires using the "vxWorks Development Shell". The system will not build on a standard Cygwin Shell, or a windows DOS prompt.  |
| Linux | /home/osaluser/osal| |

## Configure the OSAL Parameter File

The file `default_config.cmake` contains all available configuration parameters that
allow the OSAL build configuration to be tailored for a particular target environment.
Most parameters set upper bounds on the number of OS objects that can be created, as
the OSAL keeps track of allocated OS objects using fixed size tables.

If customization is desired, one should not modify the `default_config.cmake` file directly,
but rather provide alternative values by one of the override methods:

- If configuring OSAL as a standalone build for testing, values for options can be
  specified as `-D` options on the cmake command.  This is a common method of specifying
  options in a CMake project; see `man cmake` for more information on the command line options.
- If integrating OSAL into a larger application build, or if numerous values need to be configured
  and/or the configuration needs to be version controlled, a separate configuration file can be used.
  The path to this file is specified via the `OSAL_CONFIGURATION_FILE` option.

When using a configuration file, the contents should be in the form of CMake "set" commands
for the various options that need to be configured.  It is generally only necessary to specify
values which need to be changed from the default.  For any value _not_ specified in the configuration
file, the default value from `default_config.cmake` will be used.

#### OSAL configuration parameters

This is a list of commonly-used tuning parameters, but it is _not_ a complete list.  The specific set of
configurable options may vary between versions of OSAL.  For a complete list of options supported by this
version of OSAL, refer to the `default_config.cmake` file in the top level directory.

| **Parameter**  | **Description**  |
|---|---|
| `OSAL_CONFIG_MAX_TASKS`  | The maximum number of tasks that can be created in the running OSAL application. |
| `OSAL_CONFIG_MAX_QUEUES`  | The maximum number of queues that can be created in the running OSAL application. |
| `OSAL_CONFIG_MAX_COUNT_SEMAPHORES` | The maximum number of counting semaphores that can be created in the running OSAL application.  |
| `OSAL_CONFIG_MAX_BIN_SEMAPHORES` | The maximum number of binary semaphores that can be created in the running OSAL application.  |
| `OSAL_CONFIG_MAX_MUTEXES` | The maximum number of mutexes that can be created in the running OSAL application. |
| `OSAL_CONFIG_MAX_PATH_LEN` | The maximum length for an absolute path length in the OSAL File API.|
| `OSAL_CONFIG_MAX_FILE_NAME` | The maximum length for an individual file name in the OSAL File API.|
| `OSAL_CONFIG_MAX_MODULES` | Used for defining the maximum number of loadable modules that OSAL can have loaded simultaneously. |
| `OSAL_CONFIG_MAX_SYM_LEN` | Used for setting the maximum length of a symbol name in the symbol API.|
| `OSAL_CONFIG_MAX_TIMERS` | Used for defining the maximum number of timers in the OSAL.|
| `OSAL_CONFIG_DEBUG_PERMISSIVE_MODE` | For debugging as a normal/non-root user, causes OSAL to ignore/bypass certain errors related to lack of permission. |

All configurable CMake parameters are translated to a generated configuration header file
called `osconfig.h`, which replaces the directly-managed header file that was found in previous
OSAL versions.  For compatibility with existing code, the directives provided in the header file
have an `OS_` prefix to match the preprocessor directives in previous versions.


## Setting up the "cmake" build

This section covers how to set up the host machine for building OSAL
using the cmake build system. Rather than using pre-written makefiles as
the classic build of previous OSAL versions had, the cmake build generates
makefiles entirely from instructions specified in files contained with the
source code itself. This has several advantages:

-   Enhanced script-like capabilities

-   Generally no need to modify files to adapt to a particular target.

-   Easier integration with larger mission projects

Typically, OSAL is not built by itself, but rather as a library to be
used within a larger application. The OSAL cmake build system allows
both options; building as a standalone entity for testing purposes, or
building as a sub-component within a larger project. The same scripts
are used in both cases, and no modification is required.

### Prerequisites

In order to build OSAL using cmake, the "cmake" package must be
installed for the host/development machine. On Linux, this is generally
available via the respective Linux distribution package management
system, i.e. "yum" on RedHat and derivatives, or "apt-get" on Debian and
derivatives. For other operating systems, the cmake tool is available in
source and binary form from <http://cmake.org>. OSAL requires at least
version 3.5 of the cmake tool.

**RHEL/CentOS users**: For RHEL releases prior to 8.0, a compatible
cmake version is available via the EPEL repository in a package called
"cmake3".  If using this package, the `cmake` command in the examples in
this document should be replaced with `cmake3`.

### Variables that must be specified

The OSAL cmake build is controlled by several user-supplied variables
when the build is first provisioned:

| **CMake variable**  | **Valid selections**   |  **Notes** |
|---|---|---|
| `OSAL_EXT_SOURCE_DIR`  | Any directory name  | Optional.  If specified, indicates an external filesystem directory that contains user-defined OS or BSP implementations |
| `OSAL_SYSTEM_BSPTYPE`  | Any directory name that exists under `osal/src/bsp` or `${OSAL_EXT_SOURCE_DIR}`  | Selects the BSP to use.  All BSP-specific source files in this directory will be built. An optional, BSP-specific "build-options.cmake" file will also be included which may add necessary compiler options specific to that BSP.  |
| `OSAL_SYSTEM_OSTYPE`   | Any directory name that exists under `osal/src/os` or `${OSAL_EXT_SOURCE_DIR}`  | Optional.  Selects the OS abstraction to use.  This variable is typically implied by the BSP selection in `OSAL_SYSTEM_BSPTYPE` but may be forced/overridden using this option. |
| `OSAL_CONFIGURATION_FILE`  |  A file providing compile-time configuration settings  | Optional; also may be a list.  If specified, the file(s) specified will be included prior to configuring/building OSAL.  The files should contain SET directives (in CMake syntax) for compile options listed in `default_config.cmake` for tuning the OSAL build.  This is an alternative to specifying individual options via `-D` CMake parameters when configuring OSAL.  |
| `ENABLE_UNIT_TESTS`  | TRUE or FALSE  |  Optional; defaults to "FALSE" if not specified. If set to TRUE, the included unit test code will also be built in addition to the runtime library. |
| `OSAL_USER_C_FLAGS`  | Any valid switches for the compiler in use.  | Optional; the user may specify any arbitrary compiler switches to use.  |


It is important to note that the values specified for these variables
are **automatically cached** by the cmake build system. It is only
necessary to specify these values when first provisioning/configuring a
build; these are *not* required when simply building the binaries.

This caching function removes the need for environment variables to be
set in the development shell. The generated makefiles do not require the
user to set any environment variables, as all necessary context information
is automatically stored in the cache when the build is first provisioned.

### Setting up a standalone OSAL build manually

The OSAL may be built standalone in order to evaluate the library for a
particular target and/or execute the included unit tests.

In the cmake build system, all generated files are placed in a dedicated
"binary" directory that is separate from the source tree. To provision a
build, the user must first create the binary directory by issuing the
"mkdir" command (on Linux), preferably outside the OSAL source tree.
Then, the "cmake" provisioning tool is invoked to generate the actual
makefiles, supplying values for the required variables.

The following example will generate a build tree that allows testing
of the OSAL library on a standard Linux workstation:

```bash
$ mkdir build
$ cd build
$ cmake \
    -DCMAKE_BUILD_TYPE=debug \
    -DENABLE_UNIT_TESTS=true \
    -DOSAL_SYSTEM_BSPTYPE=generic-linux \
    -DOSAL_CONFIG_DEBUG_PERMISSIVE_MODE=true \
     /path/to/osal/source
```

The cmake provisioning tool generates standard makefiles in the build
directory. To build the OSAL binaries, simply run "make" in the build
directory.

Note that the OSAL is not typically built as a standalone library; although a standalone
build can be done for testing and development purposes, it is expected that the OSAL will
be utilized as a component in a larger application.

### Setting up a standalone OSAL build using the Makefile wrapper

To simplify the process of invoking the cmake tool for typical standalone test/debug builds,
a sample Makefile wrapper script `Makefile.sample` is provided in the OSAL root directory.
This is similar to the sample script provided with CFE.  It provides several simple targets
which in turn invoke CMake with a set of typically-used options.

All makefile operations operate on an output directory referred to by the `O` variable, which
is set to `build` by default.

| **Makefile Target** | **Task Description** |
|---------------------|----------------------|
| `prep`              | Invokes the `cmake` command to set up a new build, or regenerate existing makefiles in the $(O) directory |
| `all`               | Calls `make all` in the $(O) directory |
| `install`           | Calls `make install` in the $(O) directory |
| `clean`             | Calls `make clean` in the $(O) directory |
| `test`              | Calls `make test` in the $(O) directory |
| `lcov`              | Invokes the `lcov` and `genhtml` tools to generate a coverage report |
| `distclean`         | Entirely removes the $(O) directory, bringing back to a fully clean slate |

Variables recognized/used by `Makefile.sample`:

| **Variable**        | **Default Value** | **Description**      |
|---------------------|-------------------|----------------------|
| `O`                 | `build`           | Name of the build directory to operate on |
| `BSPTYPE`           | `generic-linux`   | The name of the target board (BSP) to build OSAL for, also implies OS type |
| `BUILDTYPE`         | `debug`           | Translated to the `CMAKE_BUILD_TYPE` variable (set to `release` to build with optimization) |
| `INSTALLPREFIX`     | `build`           | Translated to the `CMAKE_INSTALL_PREFIX` variable |
| `DESTDIR`           | `$(O)`            | Local destination directory for `install` |
| `ENABLE_UNIT_TESTS` | false/off         | If set to "true" then all tests will be built in addition to the basic OSAL libraries |
| `PERMISSIVE_MODE`   | false/off         | Translated to the `OSAL_CONFIG_DEBUG_PERMISSIVE_MODE` variable (see above) |

For more information on the `BUILDTYPE` and `INSTALLPREFIX`, see the CMake documentation:

https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html


### Integrating OSAL into a larger build

Modularity is a key feature of the cmake system. As such, the OSAL cmake
build system can be directly used as a component within a larger
"mission" build, as long as the same variables are supplied via the
parent cmake script:

```cmake
SET(OSAL_SYSTEM_OSTYPE "posix")
SET(OSAL_SYSTEM_BSPTYPE "generic-linux")
ADD_SUBDIRECTORY(path/to/osal)
```

The values for the variables can be obtained by any means, shown here is
just a simplified example of how it can be done for a known target.

This example assumes the parent application project also uses the CMake build
system.  This is the preferred method, as any required compiler definitions
and/or include directories are automatically propagated to any executable
that links with the OSAL library.  This is done via the TARGET_LINK_LIBRARIES
command, for example:

```
ADD_EXECUTABLE(my_program my_program.c)
TARGET_LINK_LIBRARIES(my_program osal)
```

This way, the program sources will be built with the public API include directories
from OSAL, as well as any compile definitions required by the selected OS/BSP.

Otherwise, if the application does not use CMake, OSAL can be built as a standalone library
and "installed" to a designated directory where the application can use the library.  In
this type of setup, the developer must manually propagate the include directories (i.e.
the path to the `src/os/inc` subdirectory) and any necessary compile definitions to the
parent application.

#### Linking OSAL into another target
```cmake
add_executable(my_executable main.c)
target_link_libraries(my_executable osal)
```

### Cross compiling with Cmake

To cross compile, cmake uses a separate "toolchain file" that indicates
the specific compiler to use, and any machine-specific compiler options
that may be required. Documentation for the toolchain files is available
on the cmake website at <http://cmake.org>. The toolchain file is
specified when the build is provisioned. No OSAL build scripts need to
be modified in order to cross compile or add extra machine-specific
options.

## Check over or customize the OSAL BSP directory

The glue logic that ties an OSAL application to a specific processor
board and platform is in the src/bsp directory. This directory contains
the BSP code, which contains all of the specific rules, glue code, and
startup code to make an OSAL application run on a particular board with
a particular OS.

The platforms supported in the OSAL distribution should run out of the
box. They provide a starting point for a complete port to a new
processor board.

This section will be expanded in the future to include information
needed for new OSAL ports.

## Configure one or more OSAL Applications

Once the OSAL is configured and ready to build, an OSAL application can
be configured in the build directory. Multiple OSAL applications can be
created in this directory. The application source code can come from the
src/examples directory, or the applications can be contained completely
within the build directory. The OSAL source distribution has a set of
test and example applications in the src/examples and src/tests
directories and a set of corresponding application directories and
makefiles in build directory.

### Configure a sample application in the build directory

The following show the files needed for a sample OSAL application in the
build directory.


#### Sample OSAL Applications and the associated files

|  **File**    |  **Description** |
|---|---|
| `build/examples/tasking-example` | Directory for the included OSAL example Application.  |
| `build/examples/tasking-example/Makefile` | Makefile for the example OSAL app. Because the source is in the src/examples/tasking-example directory, there is no need to include it here. The Makefile will find it using the OSAL_SRC environment variable. The source could be copied here in order to customize it.  |
| `build/examples/new_osal_app`  |  Directory for a new OSAL application. |
| `build/examples/new_osal_app/Makefile`  |  Makefile for a new OSAL application. |
| `build/examples/new_osal_app/new_osal_app.c` | Source file for the new OSAL application.  |
| `build/examples/new_osal_app/new_osal_app.h` | Header file for the new OSAL application.  |


The Application Makefiles have a specific format, so it is best to copy
one of the application Makefiles from the build directory, such as
build/examples/tasking-example.

### Configure the application's main entry point

The OSAL development environment provides the main entry point/startup
code for the Application. This code is located in the src/\<bsp\>/src
directory. The startup code will call the Application's entry point
which is named: void OS_Application_Startup(void)

## Build the OSAL core and Applications

Once the OSAL Core and Applications are set up in a build directory,
everything can be compiled. The OSAL Core or any of the Applications can
be built from individual make files, or they can be built from the
top-level Makefile in the build directory.

#### Build Commands

|  **Shell command** |  **Description** |
|---|---|
| **\$ cd build**   | Change to the build directory.  |
| **\$ make**   | Build the OSAL Core, and all Applications.  |
| **\$ make clean**  |  Clean the OSAL Core, and all Applications. |

Once the OSAL Applications are built, they are ready to load and execute
on the target. The filename of the executable is dependent on the OS it
is built for.

#### OSAL Application executable name

| **Target Operating System**  | **Application executable name**  | **Notes**  |
|---|---|---|
| vxWorks 6.x dynamic link  | example1.elf  | The vxWorks PowerPC platforms use a dynamically loaded object without the kernel.  |
| Linux  |  example1.bin |   |
| Rtems/Coldfire  |  example1.nxe |  This is a static linked executable, linked with the RTEMS kernel and BSP. |
|   RTEMS/SIS  | example1.nxe  |  This is a static linked executable, linked with the RTEMS kernel and BSP. |

## Load and Run the OSAL Applications

Depending on the Target, it is usually straightforward to run an OSAL
Application on a target platform. On desktop platforms, it is just a
matter of running the executable program. On vxWorks, the example
programs are loadable modules.

### Load the OSAL Application Executable on the Target

On desktop targets the cFE Core can be run from the directory where it
was compiled. On embedded targets, the Application has to be loaded into
a remote file system, or booted over the network. On the vxWorks PowerPC
targets, the Application can be loaded into the EEPROM or Flash disk
after the vxWorks kernel is booted. On RTEMS targets, the Application
can be loaded using the CEXP dynamic loader or it can be linked in with
an RTEMS Binary. See the target specific sections for details on each
platform.

### Setup the Target File Systems

Because the OSAL runs on many different platforms, it must be able to
deal with different file system types and different paths. The OSAL
accomplishes this by using a file system abstraction. The abstracted
OSAL file system is similar to a UNIX file system, where the root
directory starts with "/" and all disks are mounted on directory trees.
For example:

-   /ram0/apps/ RAM disk 0, apps subdirectory

-   /ram1/data/ RAM disk 1, data subdirectory

-   /hd0/tables/ Hard Disk 0, tables subdirectory

Using this abstraction, a file "datafile1.dat" on RAM disk 1 might be
accessed from the OSAL by using the path "/ram1/data/datafile1.dat".
Using the host vxWorks tools, the path to the same file would be:
"RAM:0/data/datafile1.dat". If the OSAL is running on a Linux
development workstation, the file might be located at:
"/tmp/ramdev1/data/datafile1.dat". The important part is that the OSAL
Application can access the files using a generic path, allowing the
software to remain portable.

There are a few ways to map these host file systems to OSAL file
systems:

-   **Map existing target file systems to an OSAL path**. This is one of
the most common ways to map the Non-Volatile disk to the OSAL. The OSAL
relies on the target OS to create/mount a file system and it simply is
given a mapping to the disk to allow the OSAL to access it.

-   **Create EEPROM/Flash/ATA File systems**. The OSAL has the ability
on some targets to format or initialize an EEPROM or ATA disk device.
This is less commonly used.

-   **Create RAM File Systems**. The OSAL can create RAM disks on the
vxWorks targets. The OSAL will create or re-initialize the RAM disk for
the vxWorks targets.

#### OSAL File system mapping

In new OSAL versions, no filesystem mappings are assumed or "hard-coded" by
default.  OSAL no longer requires a Volume Table object to be provided by the
application to specify the fixed filesystem mappings.

Instead, the application is expected to configure all required file system
mappings via API calls during its startup.  This may be done any time after calling
`OS_API_Init()`.

For example, for Core Flight System (cFS) configurations that require persistent
storage to be provided under a virtual directory called `/cf`, the platform-specific
CFE PSP layer configures the mapping in an appropriate manner by calling
the `OS_FileSysAddFixedMap()` API during its OS_Application_Startup() implementation.

The following table shows examples of these file system mappings on
various hosts. **Note** the change in the way the POSIX ports are
mapped. Linux will no longer remove or create sub-directories based on
the volume name. The path mapping for the FS_BASED option is now a
simple mapping from an OSAL path to a host path. This makes the OSAL
easier to use on linux platforms:

### Start the OSAL Application on the Target

Starting an OSAL Application is a highly target-dependent activity. The
following table gives examples of how to start an Application on various
platforms. For full details see the notes for each section.

#### How to start an OSAL Application on Various Target Systems:

|  **"Target" operating system**   | **How to start the cFE**  |
|---|---|
|RTEMS / mcf5235  | Loaded through GDB/BDM using a shell script: "debug.sh" |
| RTEMS / SIS | Loaded through GDB/SIS simulator: \$ sparc-rtems4.10-gdb tasking-example.n (gdb) target sim  (gdb) load (gdb) run  |
| vxWorks 6.2 / RAD750  | Started from the vxWorks Target Shell commands: Vx\> ld \< tasking-example.elf Vx\> OS_BSPMain |
| Linux | Start directly from the linux shell: \$ ./tasking-example.bin  |


# Target Specific Instructions

This section provides details on how to load and run each of the
supported OSAL configurations.

## Generic vxWorks Platform:

The test applications are tested on the Motorola MCP750 running vxWorks 6.9,
but should work similarly on other platforms/versions.

On this platform, the OSAL Applications are built as dynamic loadable vxWorks
modules, rather than being linked to the vxWorks kernel/BSP. The OSAL Applications
are loaded into the compact flash disk on the MCP750, so it can be started from a
vxWorks shell or startup script after the kernel comes up.

Building for this platform requires use of a CMake toolchain to specify the
correct cross compiler and configuration to use.  For examples of toolchain
files including a toolchain file that may be used to build this platform, see
the NASA cFS software repository.

### How to run the OSAL Applications on the MCP750 or RAD750

1. Load the kernel. The custom vxWorks kernel is loaded into the MCP750
via TFTP. We use a vxWorks boot image (Rather than the Motorola boot
monitor/loader ) to boot the MCP750 board, TFTP the "real" kernel to
RAM, and execute it. This vxWorks boot image also sets the network
settings for the "real" kernel image. On our OSAL/cFE development
system, we keep the loadable vxWorks kernel image in a TFTP directory on
the development workstation. So the vxWorks kernel image goes in
/tftpboot/cpu1/cfecpu1.st. **( \$ cp
/opt/workspace/mcp750image/default/vxWorks /tftpboot/cpu1/cfecpu1.st )**

2. Copy the application program file ( or other executable name ) loadable module
into the non-volatile disk. On the MCP750, this is done simply by FTPing
the file to the target:

```
$ ftp 192.168.1.4


ftp> username: target

ftp> password: password

ftp> cd "CF:0"

ftp> binary

ftp> put tasking-example.elf

```

3. Load the example Application in the vxWorks shell:
```
vx> cd "CF:0"

vx> ld < tasking-example.elf
```

4. Run the example Application in the vxWorks shell:
```
vx> OS_BSPMain
```

(The entry point for the examples and test programs is always
OS_BSPMain)

## QEMU + pc686 / RTEMS 4.11 - 5.x:

OSAL supports the RTEMS OS, which can be easily tested without any specific
hardware using the QEMU hypervisor/virtualization software.  This relies
on using the "pc-rtems" BSP.

Like VxWorks, this requires use of a cross compile toolchain to build
applications for this target.  Example toolchains can also be found in the
NASA cFS project(s).

In particular the OS_SYSTEM_BSPTYPE should be set to "pc-rtems" by the toolchain
to enable this BSP/OS abstraction layer.

### How to run the OSAL Applications on QEMU with RTEMS 4.11 - 5.x

By default, RTEMS applications are built as a single, monolithic kernel + application
image.  This simplifies development and testing.  However, even in the default setup,
applications may still utilize the module load API to extend or replace logic.  However,
by modifying the BSP and build rules, the kernel can also be built separately, with
the application also built as a loadable module.

The monolithic image is trivial to boot in the QEMU hypervisor, as follows:

```
qemu-system-i386 -m 128 -no-reboot -display none -serial stdio \
     -kernel <image>.exe
```

Where `<image>.exe` refers to any of the test programs or applications
that are provided with OSAL.  This command puts the virtual console on
the same terminal where the QEMU emulator is started.  The program
should run automatically after the kernel boots.  The system may be
stopped by issuing the "shutdown" command at the shell, or by pressing
the CTRL+A followed by CTRL+C which terminates QEMU.


## Generic Linux Platform

The OSAL can run on many Linux distributions. Testing is done with currently
supported Ubuntu LTS versions (up through 20.04 at the time of the current
release) as well as supported RHEL/CentOS versions (7.x and 8.x series),
and Linux-based embedded development boards such as the Raspberry Pi and
BeagleBone series.

The generic-linux BSP should also operate with customized Linux builds
built from the Yocto/OpenEmbedded development environment, Buildroot, or
PTXdist.  Any reasonably recent Linux kernel/library version should work,
however it must have support for native POSIX threads (NPTL) and POSIX
message queues (mqueue).


### How to Run the OSAL on the PC / Linux Platform

1. To run an OSAL Application, simply execute the binary from a shell
prompt:

```
build/examples/tasking-example$ ./tasking-example.bin
```

# OSAL Unit Tests and UT assert

The OSAL distribution includes a suite of black box unit tests, white
box unit tests (/src/unit-test-coverage), and functional tests
(/src/tests). Tam Ngo at NASA Johnson Space Flight Center developed the
suite of black box unit tests located in the /src/unit-tests directory.

Additionally, OSAL provides a framework for testing called "UT assert".
The test framework actually uses some of the OSAL BSP facilities to
execute the tests in a platform-independent manner.

Unit tests are built based on the `ENABLE_UNIT_TESTS` CMake configuration
variable.  When this is set to TRUE, extra test programs are built to
validate all of OSAL functionality.

The "black box" functional tests under /src/tests and /src/unit-tests
operate with an unmodified OSAL library, using a real instance of the OSAL
to run the tests.

The "white box" coverage test under /src/unit-test-coverage does not use
OSAL, it only uses the BSP for start up and low-level console access,
combined with UT assert for the basic framework.  In this testing mode,
each OSAL subsystem is specially compiled with special flags to enable
profiling (to track line and branch coverage) and linked with _stub_
implementations of all other dependencies.  This way, test cases can
trigger error paths from library calls that would otherwise not be possible
in a black box test environment.

These test programs are executed just as any other application program.

Complete coverage testing is only available for the "shared" layer and
the "VxWorks" low level implementation layer.  However, because all platforms
share the same "shared" layer, this coverage test applies to all platforms.
The low level (OS-specific) implementation layers should be only to translate
the OSAL calls to the specific C library/OS API provided by that platform,
while all basic state/table maintenance and error checking is performed at
the shared layer.

**Note**: The coverage tests are built for the same platform as all other
applications.  Notably, because the coverage test does not actually use
the facilities provided by the OS (all OS calls are replaced with stubs),
the OS-specific coverage test can execute on any platform, it is not limited
to running on the platform that it is abstracting.  As a result, the VxWorks
coverage test is typically executed on a Linux host.

## How to Run the OSAL Tests on the PC / Linux Platform

To build and run the OSAL unit tests, first make sure OSAL was initially configured
with the `ENABLE_UNIT_TESTS` option enabled.  Test programs are simply separate application
executables that can then be executed as normal.  The test programs are built along with
the OSAL library as part of a normal `make all` or `make install` command.  During `install`,
the test programs will be copied to the output location/staging area.

### Option 1: Execute single test directly from a build tree

The tests can be executed directly in-place in the build directory.  This method often useful
when debugging, as the executable can also be run in a debugger (i.e. gdb) this way.

The following example executes the task subsystem coverage test for the shared layer, but any
other test can be run in a similar fashion.

|  **Shell command**      |    **Description**|
|---|---|
| `$ cd build/unit-test-coverage/shared` | Change to the build directory containing coverage tests |
| `$ make` | Build the test program(s) if not already done |
| `$ ./coverage-shared-task-testrunner` | Execute the shared task coverage test application |


### Option 2: Execute tests in batch mode using ctest and/or makefile wrapper target

The Makefiles include a "test" wrapper target which in turn invokes the "ctest" program
that will execute all test programs in a batch mode.

Note that the "ctest" program shows a quiet output by default, indicating only the test name
and a pass/fail status.  To see a complete log of tests executed, the test program may be run
directly from the command line (see above).

For example:

`make test`

**Note**: The "test" target may also be invoked in a build subdirectory, to invoke only the tests
defined/built in (or below) that subdirectory.

### Code Coverage analysis

The top level sample Makefile also include an "lcov" wrapper target which in turn invokes the "lcov" and
"genhtml" programs to collect branch/path coverage information into an HTML report.

For example:

`make lcov`

**Note**: The "test" target should be executed before running the "lcov" target.

# Revision History

| Revision Number |  Release Date | Changes to Prior Revision  | Approval |
|---|---|---|---|
| 1.0  | 10/17/07 | Initial Release.  | A. Cudmore  |
| 1.1 | 02/13/08 | Updates for RTEMS, Linux, and Cygwin for 2.11 release | A. Cudmore  |
| 1.2   | 09/05/08 | Updates for OSAL 2.12 release  | A. Cudmore  |
| 1.3   | 03/10/10 | Updates for OSAL 3.1 release   | A. Cudmore  |
| 1.4   | 05/25/11 | Updates for OSAL 3.3 release   | A. Cudmore  |
| 1.5   | 12/13/11 | Updates for OSAL 3.4 release -- added support for sis-rtems  Removed cFE configuration text  | A. Cudmore  |
| 1.6   | 12/21/12 | Updates for OSAL 4.0 release -- Removed Cygwin and OS X configurations  | A. Cudmore  |
| 4.1    | 01/17/14  | Updates for OSAL 4.1 release. Sync document version ID to software version. Add information for building and running unit tests.   | A. Cudmore  |
| 4.2   | 01/31/16 | Moved osconfig.h description into section 2.2.h Consolidated the  \"classic\" build and prerequisites setup into section 2.3. Added new section 2.4 on provisioning a build using cmake. Minor </br> modifications to subsequent sections only where there was a  difference between cmake and classic builds; i.e. the cmake  build has no \"make config\" or \"make depend\". Updated title  page to replace Code 582 banner with cFS. Added header.  | J.Hickey S.Strege   |
