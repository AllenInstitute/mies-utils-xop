# Introduction #

The MIES Utilities XOP adds external functions and features to Igor Pro that Igor Pro does not provide on its own. It is required by MIES.

# Build Instructions #

## Windows ##

Install [latest CMAKE](https://cmake.org/download/#latest).

Requires also an installed Visual Studio.

put the following code into a batch file in the projects folder:

    md build
    md build-64
    cd build
    cmake -G "Visual Studio 15 2017" ..\src
    cmake --build . --config Release --target install
    cd ..\build-64
    cmake -G "Visual Studio 15 2017 Win64" ..\src
    cmake --build . --config Release --target install
    cd ..

The build environment string "Visual Studio 15 2017" may differ depending on your installed build environment, e.g. for older Visual Studio installation "Visual Studio 14 2015" works.

Open your Visual Studio Developer Console, which setups your build environment and run the batch file.
