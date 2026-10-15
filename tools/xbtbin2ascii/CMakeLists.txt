cmake_minimum_required(VERSION 3.10)
project(bin2ascii LANGUAGES CXX)
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
    #Find library to link against

    find_library(XBTLIB
                        NAMES
                             xbtbinfileutils

                        HINTS
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

    find_path(HDRS      BinDecoder.h
                        NAMES
                             BinDecoder.h
                        HINTS
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
add_executable(bin2ascii src/bin2ascii.cpp)
target_link_libraries(bin2ascii ${XBTLIB})

# Let the installed executable find the library installed next to it.
if(APPLE)
    set_target_properties(bin2ascii PROPERTIES INSTALL_RPATH "@executable_path/../lib")
elseif(UNIX)
    set_target_properties(bin2ascii PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
endif()

install(TARGETS bin2ascii DESTINATION bin)

# uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
