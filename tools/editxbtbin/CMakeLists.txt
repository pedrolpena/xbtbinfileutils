cmake_minimum_required(VERSION 3.10)
project(editbin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------
# Find the xbtbinfileutils library.
#
# If the library is installed system-wide, the headers and .so/.a
# should be found automatically.  If installed to a custom prefix,
# pass -DCMAKE_PREFIX_PATH=/path/to/prefix when configuring.
#
# Alternatively set these manually:
#   -DXBT_INCLUDE_DIR=/path/to/include
#   -DXBT_LIBRARY=/path/to/libxbtbinfileutils.so
# ------------------------------------------------------------------

if(TARGET xbtbinfileutils)
    # Built from the xbtbinfileutils source tree, link against the library
    # target being built alongside it, it brings its own include directory.
    set(XBT_FOUND TRUE)
    set(XBT_INCLUDE_DIRS "")
    set(XBT_LIBRARIES xbtbinfileutils)
else()
    # Try pkg-config first
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(XBT QUIET xbtbinfileutils)
    endif()
endif()

if(NOT XBT_FOUND)
    # Fall back to manual search.  EDFFormat.h only exists in versions of
    # the library with the type 4 EDF fields, which editbin needs.
    find_path(XBT_INCLUDE_DIR EDFFormat.h
        HINTS ${CMAKE_INSTALL_PREFIX}/include /usr/local/include /usr/include
        PATH_SUFFIXES xbtbinfileutils)

    find_library(XBT_LIBRARY
        NAMES xbtbinfileutils
        HINTS ${CMAKE_INSTALL_PREFIX}/lib /usr/local/lib /usr/lib)

    if(XBT_INCLUDE_DIR AND XBT_LIBRARY)
        set(XBT_FOUND TRUE)
        set(XBT_INCLUDE_DIRS ${XBT_INCLUDE_DIR})
        set(XBT_LIBRARIES ${XBT_LIBRARY})
    else()
        message(FATAL_ERROR
            "Could not find xbtbinfileutils library.\n"
            "Set -DXBT_INCLUDE_DIR=... and -DXBT_LIBRARY=... or "
            "install the library system-wide.")
    endif()
endif()

# The executable
add_executable(editbin EditBinFile.cpp)
target_include_directories(editbin PRIVATE ${XBT_INCLUDE_DIRS})
target_link_libraries(editbin ${XBT_LIBRARIES})

# Let the installed executable find the library installed next to it.
if(APPLE)
    set_target_properties(editbin PROPERTIES INSTALL_RPATH "@executable_path/../lib")
elseif(UNIX)
    set_target_properties(editbin PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
endif()

install(TARGETS editbin DESTINATION bin)
