# Introduction #

The MIES Utilities XOP adds external functions and features to Igor Pro that Igor Pro does not provide on its own. It is required by MIES.

# Build Instructions #

## Windows ##

Install [CMake](https://cmake.org/download/#latest) 4.2 or later and Visual Studio 2026 with the C++ workload.

Put the XOP Toolkit's `XOPSupport` folder into the repository root.

Run `build.bat` from a command prompt. It builds the Release XOPs for 32-bit and 64-bit and installs them into
`output/win/x86/Release` and `output/win/x64/Release`.

The XOPs link statically against the MSVC runtime, so they do not depend on the installed Visual C++
Redistributable. Pass `-DMSVC_RUNTIME_DYNAMIC=ON` to `build.bat` to link dynamically.

## clang-tidy ##

Install the Visual Studio component "C++ Clang tools for Windows", which provides clang-tidy.

Run `clang-tidy.bat` from a command prompt. It checks the sources with the rules of `.clang-tidy` and applies
the fixes clang-tidy suggests. The CI runs it as well and provides the fixes as a patch if it fails.
