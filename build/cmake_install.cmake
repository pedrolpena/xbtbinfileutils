# Install script for directory: /home/pedro/Downloads/src/xbtbinfileutils

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libxbtbinfileutils.so.1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libxbtbinfileutils.so.1")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libxbtbinfileutils.so.1"
         RPATH "$ORIGIN/../lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/pedro/Downloads/src/xbtbinfileutils/build/libxbtbinfileutils.so.1")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libxbtbinfileutils.so.1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libxbtbinfileutils.so.1")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libxbtbinfileutils.so.1"
         OLD_RPATH "::::::::::::::"
         NEW_RPATH "$ORIGIN/../lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libxbtbinfileutils.so.1")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/pedro/Downloads/src/xbtbinfileutils/build/libxbtbinfileutils.so")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/xbtbinfileutils" TYPE FILE FILES
    "/home/pedro/Downloads/src/xbtbinfileutils/include/Agency.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/BinDecoder.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/BinEncoder.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/DataQuality.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/DataResolution.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/DepthCalculator.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/EDFFormat.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/FormatConverter.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/Launchers.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/MessageType.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/PressureCalculator.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/SoundSpeedInSeaWater.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/SurfaceCurrentInstrument.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/WindInstrument.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/XBTProbe.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/XBTProfile.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/XBTProfileDataRanges.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/XBTRecorder.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/XBTResistanceToTemperatureConverter.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/__aux.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/bitvector.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/mlinterp.h"
    "/home/pedro/Downloads/src/xbtbinfileutils/include/utility.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/pedro/Downloads/src/xbtbinfileutils/build/tools/edf2bin/cmake_install.cmake")
  include("/home/pedro/Downloads/src/xbtbinfileutils/build/tools/editxbtbin/cmake_install.cmake")
  include("/home/pedro/Downloads/src/xbtbinfileutils/build/tools/xbtbin2ascii/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/pedro/Downloads/src/xbtbinfileutils/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
