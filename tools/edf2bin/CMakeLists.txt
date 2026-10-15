cmake_minimum_required(VERSION 3.10)
project(edf2bin LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Os -fno-rtti)
endif()

if(TARGET xbtbinfileutils)
    # Built from the xbtbinfileutils source tree, link against the library
    # target being built alongside it, it brings its own include directory.
    set(XBTLIB xbtbinfileutils)
else()
    # To build against a library that is not installed, point these at the
    # xbtbinfileutils include directory and build directory, e.g.
    # cmake -DXBTBINFILEUTILS_INCLUDE_DIR=../xbtbinfileutils/include -DXBTBINFILEUTILS_LIBRARY_DIR=../xbtbinfileutils/build ..
    set(XBTBINFILEUTILS_INCLUDE_DIR "" CACHE PATH "xbtbinfileutils include directory")
    set(XBTBINFILEUTILS_LIBRARY_DIR "" CACHE PATH "xbtbinfileutils library directory")

    #Find library to link against

    find_library(XBTLIB
                        NAMES
                             xbtbinfileutils

                        HINTS
                             ${XBTBINFILEUTILS_LIBRARY_DIR}
                             ${CMAKE_INSTALL_PREFIX}
                        PATHS
                             ${CMAKE_INSTALL_PREFIX}
                        PATH_SUFFIXES
                              "lib"
                              "local/lib"
                              "xbtbinfileutils"
                              "local/lib/xbtbinfileutils"
                              "xbtbinfileutils/lib"
                              "xbtbinfileutils/bin"

                             )

    # Find header files

    find_path(HDRS      BinEncoder.h
                        NAMES
                             BinEncoder.h
                        HINTS
                             ${XBTBINFILEUTILS_INCLUDE_DIR}
                             ${CMAKE_INSTALL_PREFIX}
                             ${CMAKE_SYSTEM_PREFIX_PATH}
                        PATHS
                             ${CMAKE_INSTALL_PREFIX}
                             ${CMAKE_SYSTEM_PREFIX_PATH}
                        PATH_SUFFIXES
                              "include/xbtbinfileutils"
                              "xbtbinfileutils"
                              "xbtbinfileutils/include/xbtbinfileutils"

                             )

    include_directories(${HDRS})

    message(STATUS "xbtbinfileutils library: ${XBTLIB}")
    message(STATUS "xbtbinfileutils headers: ${HDRS}")
endif()

#create executable
add_executable(edf2bin src/edf2bin.cpp)
target_link_libraries(edf2bin ${XBTLIB})

# Let the installed executable find the library installed next to it.
if(APPLE)
    set_target_properties(edf2bin PROPERTIES INSTALL_RPATH "@executable_path/../lib")
elseif(UNIX)
    set_target_properties(edf2bin PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
endif()

install(TARGETS edf2bin DESTINATION bin)

# uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
