Travis-CI: [![Build Status](https://travis-ci.com/nasa/osal.svg)](https://travis-ci.com/nasa/osal)

Operating System Abstraction Layer Project
==========================================

![OSAL Logo by Michael Cudmore](./doc/OSAL-Logo.png)


The OS Abstraction Layer was originally devloped by the Flight Software Systems Branch at NASA Goddard Space Flight Center.

OS Abstraction Layer information:
=================================

This distribution contains:

1. The OS Abstraction Layer Library
2. Tests and example applications
3. A directory structure and makefiles to manage it all.

Version Notes:
==============
- 5.0.10: DEVELOPMENT
  - Minor updates (see https://github.com/nasa/osal/pull/401)
- 5.0.9: DEVELOPMENT
  - Documentation updates (see https://github.com/nasa/osal/pull/375)
- 5.0.8: DEVELOPMENT
  - Minor updates (see https://github.com/nasa/osal/pull/369)
- 5.0.7: DEVELOPMENT
  - Fixes memset bug
  - Minor updates (see https://github.com/nasa/osal/pull/361)
- 5.0.6: DEVELOPMENT
  - Minor updates (see https://github.com/nasa/osal/pull/355)
- 5.0.5: DEVELOPMENT
  - Fixed osal_timer_UT test failure case
  - Minor updates (see https://github.com/nasa/osal/pull/350)
- 5.0.4: DEVELOPMENT
  - Minor updates (see https://github.com/nasa/osal/pull/334)
- 5.0.3: DEVELOPMENT
  - Minor updates (see https://github.com/nasa/osal/pull/292)
- 5.0.2: DEVELOPMENT
  - Bug fixes and minor updates (see https://github.com/nasa/osal/pull/281)
- 5.0.1: DEVELOPMENT
  - Minor updates (see https://github.com/nasa/osal/pull/264)
- 5.0.0: RELEASE CANDIDATE
  - In build verification testing to be considered for official release
  - Release documentation in work
  - This is a point release from an internal repository
- **4.2.1a OFFICIAL RELEASE**
  - Released under the NOSA license, see [LICENSE](LICENSE)
  - See [version description document](OSAL%204.2.1.0%20Version%20Description%20Document.pdf)
  - This is a point release from an internal repository

Getting Started:
================

See the document *doc/OSAL-Configuration-Guide.doc* for complete details.

An easy way to get started is to use the Linux port and classic build:

1. Set the *OSAL_SRC* environment variable to point to the OSAL source code.
     - Running setvars.sh will set the variable for you ($source ./setvars.sh)
2. Edit the *build/osal-config.mak* file and set the following options:
     - BSP - Set this to the board you are running on. For a PC running linux, use *pc-linux*
     - OS - Set this to the OS you are running. For a PC running linux, use *posix*.
     - OSAL_M32 - Uncomment/Add this build variable for building 32-bit images using "native"
       GCC on a 64-bit X86 Linux platform

Buiding on a PC running linux:

    export OSAL_SRC = /home/acudmore/osal/src

In build/osal-config.mak:

    OS=posix
    BSP=pc-linux

    Optional: OSAL_M32 = -m32 (Note: Usage of this flag may require an optional 'multilib' (or similar)
    package to be installed. Refer to your operating system and toolchain documentation for details, if
    adding the appropriate flag causes your builds to fail due to (for example) missing 32-bit or
    multilib related headers or libraries.)

Optional:  Some Linux systems may require an additional linker option in
    src/bsp/pc-linux/make/link-rules.mak:

    LDFLAGS ?= $(OSAL_M32) -export-dynamic

    If the symbol-api-test fails, then you need this option.

Now just type *make config; make* from the build directory and it should build the OSAL core files, tests, and sample
applications for you. The binary for each application is in its own directory
(i.e. build/examples/tasking-example/tasking-example.bin) You can switch to that directory and run it. You
can also debug it using GDB.

NOTE: Running on linux may require root or adjusting the posix message queue maximum sizes.

The Embedded targets take a little more work to run, because they must be cross compiled and booted on the board.
By copying a target, you should be able to come up with a new target.

If you would like just the OSAL itself, just look in src/os/inc for the include files and src/os/<your os here>
for the OSAL implementation.

The API documentation is in the *doc* directory.

There are two sets of tests: build/tests and build/unit-tests.  To build and the unit tests,
perform the build steps above then *make unit-tests* in the build directory.

Instructions on how to use the newely supported cmake build system are provided in the OSAL Configuration Guide
located in the *doc* directory.

Contact Information:
====================

    Alan Cudmore
    NASA Goddard Space Flight Center
    Code 582.0
    Greenbelt, MD 20771
    Alan.P.Cudmore@nasa.gov

Copyright notice:
=================

Copyright United States Government as represented by the Administrator of the National Aeronautics and Space Administration

License information:
====================

This software is licensed under NASAs Open Source Agreement. The release of the software is conditional upon the recipients acceptance of the Open Source Agreement. Please see the file: NASA_Open_Source_Agreement_1_3-OS_AbstractionLayer.txt
