# Introduction #

The MIES Utilities XOP adds external functions and features to Igor Pro that Igor Pro does not provide on its own. It is required by MIES.

# Build Instructions #

## Windows ##

Install [CMake](https://cmake.org/download/#latest) 4.2 or later and Visual Studio 2026 with the C++ workload.

Put the XOP Toolkit's `XOPSupport` folder into the repository root.

Put the following code into a batch file in the repository root:

    cmake -G "Visual Studio 18 2026" -A Win32 -S src -B build-Win32
    cmake --build build-Win32 --config Release --target install
    cmake -G "Visual Studio 18 2026" -A x64 -S src -B build-x64
    cmake --build build-x64 --config Release --target install

The XOPs link statically against the MSVC runtime, so they do not depend on the installed Visual C++
Redistributable. Pass `-DMSVC_RUNTIME_DYNAMIC=ON` to the first `cmake` call of an architecture to link dynamically.

Run the batch file from a command prompt.
