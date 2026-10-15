@echo off
rem Builds and installs the xbtbinfileutils library and the tools in the
rem tools directory on Windows with CMake and Visual Studio, no MinGW or
rem GNU make needed. From a command prompt in this directory
rem
rem   make
rem   make install      (from a command prompt run as administrator)
rem   make uninstall
rem   make clean
rem   make distclean    (removes the build directory)
rem
rem In PowerShell run .\make instead of make.
rem
rem Optional settings, set them before running make, e.g. set PREFIX=C:\xbt
rem   BUILD_DIR   build directory (build)
rem   BUILD_TYPE  Release or Debug (Release)
rem   PREFIX      install prefix (CMake's default, C:\Program Files\xbtbinfileutils)
rem   GENERATOR   CMake generator (CMake's default, the newest Visual Studio)
rem   CMAKE_ARGS  any other arguments for CMake, e.g. -DXBTBINFILEUTILS_BUILD_TOOLS=OFF
rem   CMAKE       cmake executable (cmake)
rem
rem The settings are read when the build directory is first configured, run
rem make distclean before changing them.

setlocal

rem "%~dp0." rather than "%~dp0", a trailing backslash would escape the quote.
set "SOURCE_DIR=%~dp0."
if not defined BUILD_DIR set "BUILD_DIR=%~dp0build"
if not defined BUILD_TYPE set "BUILD_TYPE=Release"
if not defined CMAKE set "CMAKE=cmake"

set "TARGET=%~1"
if not defined TARGET set "TARGET=all"

if /i "%TARGET%"=="all" goto build
if /i "%TARGET%"=="install" goto build
if /i "%TARGET%"=="uninstall" goto build
if /i "%TARGET%"=="clean" goto clean
if /i "%TARGET%"=="distclean" goto distclean
echo Unknown target %TARGET%, use all, install, uninstall, clean or distclean. 1>&2
exit /b 1

:build
if exist "%BUILD_DIR%\CMakeCache.txt" goto configured
"%CMAKE%" -E make_directory "%BUILD_DIR%"
if errorlevel 1 exit /b 1
set "GENERATOR_ARG="
if defined GENERATOR set GENERATOR_ARG=-G "%GENERATOR%"
set "PREFIX_ARG="
if defined PREFIX set PREFIX_ARG="-DCMAKE_INSTALL_PREFIX=%PREFIX%"
"%CMAKE%" -E chdir "%BUILD_DIR%" "%CMAKE%" %GENERATOR_ARG% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% %PREFIX_ARG% %CMAKE_ARGS% "%SOURCE_DIR%"
if errorlevel 1 exit /b 1

:configured
set "TARGET_ARG="
if /i not "%TARGET%"=="all" set "TARGET_ARG=--target %TARGET%"
"%CMAKE%" --build "%BUILD_DIR%" --config %BUILD_TYPE% %TARGET_ARG%
exit /b %errorlevel%

:clean
if not exist "%BUILD_DIR%\CMakeCache.txt" exit /b 0
"%CMAKE%" --build "%BUILD_DIR%" --config %BUILD_TYPE% --target clean
exit /b %errorlevel%

:distclean
"%CMAKE%" -E remove_directory "%BUILD_DIR%"
exit /b %errorlevel%
