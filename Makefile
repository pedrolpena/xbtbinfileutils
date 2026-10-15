# Builds and installs the xbtbinfileutils library and the tools in the
# tools directory with CMake, so all it takes is
#
#   make
#   sudo make install
#
# Optional settings, e.g. make PREFIX=$HOME/.local BUILD_TYPE=Debug
#   BUILD_DIR   build directory (build)
#   BUILD_TYPE  Release or Debug (Release)
#   PREFIX      install prefix (CMake's default, /usr/local on Linux and macOS)
#   GENERATOR   CMake generator (CMake's default, Visual Studio on Windows)
#   CMAKE_ARGS  any other arguments for CMake, e.g. -DXBTBINFILEUTILS_BUILD_TOOLS=OFF
#   DESTDIR     staging directory for make install
#
# The settings are read when the build directory is first configured, run
# make distclean before changing them.
#
# On Windows make.bat does the same without GNU make, a command prompt
# runs it for make.
#
# The Debug and Release directories hold the makefiles Eclipse generates for
# its managed build and are not used here.

CMAKE      ?= cmake
BUILD_DIR  ?= build
BUILD_TYPE ?= Release
PREFIX     ?=
CMAKE_ARGS ?=

ifneq ($(GENERATOR),)
GENERATOR_ARG = -G "$(GENERATOR)"
endif
ifneq ($(PREFIX),)
PREFIX_ARG = -DCMAKE_INSTALL_PREFIX="$(PREFIX)"
endif

# CMake's install step reads DESTDIR from the environment.
export DESTDIR

.PHONY: all install uninstall clean distclean

all: $(BUILD_DIR)/CMakeCache.txt
	+$(CMAKE) --build $(BUILD_DIR) --config $(BUILD_TYPE)

$(BUILD_DIR)/CMakeCache.txt:
	$(CMAKE) -E make_directory $(BUILD_DIR)
	$(CMAKE) -E chdir $(BUILD_DIR) $(CMAKE) $(GENERATOR_ARG) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(PREFIX_ARG) $(CMAKE_ARGS) "$(CURDIR)"

install: all
	+$(CMAKE) --build $(BUILD_DIR) --config $(BUILD_TYPE) --target install

uninstall: $(BUILD_DIR)/CMakeCache.txt
	+$(CMAKE) --build $(BUILD_DIR) --config $(BUILD_TYPE) --target uninstall

clean:
	-+$(CMAKE) --build $(BUILD_DIR) --config $(BUILD_TYPE) --target clean

distclean:
	$(CMAKE) -E remove_directory $(BUILD_DIR)
