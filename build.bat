@echo off
rem Build and install the XOPs for both architectures into output\win, see readme.md
rem Additional arguments are passed to the CMake configure step, e.g. -DMSVC_RUNTIME_DYNAMIC=ON

setlocal
cd /d "%~dp0"

for %%A in (Win32 x64) do (
  cmake -G "Visual Studio 18 2026" -A %%A -S src -B build-%%A %* || exit /b 1
  cmake --build build-%%A --config Release --target install || exit /b 1
)
