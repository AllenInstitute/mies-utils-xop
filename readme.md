# Introduction #

The MIES Utilities XOP adds external functions and features to Igor Pro that Igor Pro does not provide on its own. It is required by MIES.

# Build Instructions #

## Windows ##

Install [CMake](https://cmake.org/download/#latest) 4.2 or later and Visual Studio 2026 with the C++ workload.

Put the XOP Toolkit's `XOPSupport` folder into the repository root.

Run `build.bat` from a command prompt. It builds the XOPs for 32-bit and 64-bit and installs them into
`output/win`.

The XOPs link statically against the MSVC runtime, so they do not depend on the installed Visual C++
Redistributable. Pass `-DMSVC_RUNTIME_DYNAMIC=ON` to `build.bat` to link dynamically.
