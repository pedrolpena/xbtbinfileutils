# 3.10 is the oldest version that current CMake releases still accept
# without a deprecation warning.  CMake 4 dropped everything below 3.5.
cmake_minimum_required(VERSION 3.10)
project(xbtbinfileutils VERSION 1 LANGUAGES CXX)

# The top level Makefile is a wrapper around the build directory, so an
# in-source build would overwrite it.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR
        "In-source builds are not supported, they overwrite the top level Makefile.\n"
        "Run make from the source directory, or configure with a separate build "
        "directory, e.g. mkdir build && cd build && cmake ..\n"
        "Remove the CMakeCache.txt and CMakeFiles that were just created.")
endif()

option(XBTBINFILEUTILS_BUILD_TOOLS "Build the tools found in the tools directory" ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "-Os")
    add_compile_options(-fno-rtti)
    # Do not fuse multiplications and additions so the floating point results,
    # like the inflection points, match the Java version of this library on
    # every platform.
    add_compile_options(-ffp-contract=off)
elseif(MSVC)
    # The library uses the standard C functions MSVC calls unsafe.
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Put the executables and Windows DLLs in one directory so the tools can
# be run from the build directory without installing the library first.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Let installed tools find the installed library on macOS and Linux.
if(APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
elseif(UNIX)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()

# Find source files
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

# Create shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
set_target_properties(${PROJECT_NAME} PROPERTIES
    SOVERSION ${PROJECT_VERSION}
    # The headers have no dllexport markup, so export everything for MSVC.
    # MinGW already does this.
    WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Install library
install(TARGETS ${PROJECT_NAME}
                        ARCHIVE DESTINATION lib
                        LIBRARY DESTINATION lib
                        RUNTIME DESTINATION bin
)

# Install library headers
install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})

if(XBTBINFILEUTILS_BUILD_TOOLS)
    foreach(tool edf2bin editxbtbin xbtbin2ascii)
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tools/${tool}/CMakeLists.txt)
            add_subdirectory(tools/${tool})
        endif()
    endforeach()
endif()

# uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
