@echo off
rem Run clang-tidy over the sources and apply its fixes, see readme.md
rem The compile database requires Ninja, which needs the environment of the Visual Studio developer command prompt

setlocal
cd /d "%~dp0"

for /f "usebackq delims=" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -find VC\Auxiliary\Build\vcvars64.bat`) do call "%%i" || exit /b 1

cd src
cmake --workflow --preset=ci-clang-tidy --fresh || exit /b 1
