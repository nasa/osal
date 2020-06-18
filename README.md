# Core Flight System : Framework : Operating System Abstraction Layer

This repository contains NASA's Operating System Abstraction Layer (OSAL), which is a framework component of the Core Flight System.

This is a collection of abstractio APIs and associated framework to be located in the `osal` subdirectory of a cFS Mission Tree. The Core Flight System is bundled at <https://github.com/nasa/cFS>, which includes build and execution instructions.

## References
- [Configuration guide](doc/OSAL-Configuration-Guide.pdf)
- [API documentation](doc/OSAL%20Library%20API.pdf)

## Version History

- **5.0.0: OFFICIAL RELEASE**
  - Changes are detailed in [cFS repo](https://github.com/nasa/cFS) release 6.7.0 documentation
  - Released under the Apache 2.0 license
- **4.2.1a OFFICIAL RELEASE**
  - Released under the NOSA license
  - See [version description document](OSAL%204.2.1.0%20Version%20Description%20Document.pdf)
  - This is a point release from an internal repository

## Quick start

Typically OSAL is built and tested as part of cFS as detailed in: [cFS repo](https://github.com/nasa/cFS)

OSAL library build pc-linux example (from the base osal directory):
```
mkdir build_osal
cd build_osal
cmake -DOSAL_SYSTEM_OSTYPE=pc-linux -DOSAL_SYSTEM_BSPTYPE=pc-linux -DOSAL_INCLUDEDIR=src/bsp/pc-linux/config/ ..
make
```
See the Configuration Guide for more information.

## Known issues

See all open issues and closed to milestones later than this version.

## Getting Help

For best results, submit issues:questions or issues:help wanted requests at <https://github.com/nasa/cFS>.

Official cFS page: <http://cfs.gsfc.nasa.gov>
