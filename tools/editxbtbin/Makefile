# Makefile for editbin
#
# Assumes xbtbinfileutils headers are in the include path and the
# library is linkable as -lxbtbinfileutils.
#
# Override with:
#   make XBT_INCLUDE=/path/to/headers XBT_LIB=/path/to/lib
#
# Install/uninstall:
#   sudo make install
#   sudo make uninstall

CXX          ?= g++
CXXFLAGS     ?= -std=c++17 -O2 -Wall -Wextra
XBT_INCLUDE  ?=
XBT_LIB      ?=
PREFIX       ?= /usr/local
BINDIR        = $(PREFIX)/bin
TARGET        = editbin

ifneq ($(XBT_INCLUDE),)
  INC_FLAGS = -I$(XBT_INCLUDE)
endif
ifneq ($(XBT_LIB),)
  LIB_FLAGS = -L$(XBT_LIB)
endif

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): EditBinFile.cpp
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) -o $@ $< $(LIB_FLAGS) -lxbtbinfileutils

install: $(TARGET)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(TARGET)

clean:
	rm -f $(TARGET)
