bin_PROGRAMS = netcode

netcode_SOURCES = \
    src/main.cpp \
    src/base/Engine.cpp \
    src/base/Actor.cpp \
    src/base/Script.cpp
netcode_CPPFLAGS = -g -I$(top_srcdir)/netcode/include \
    -I$(top_srcdir)/netcode/src
netcode_LDFLAGS  = \
    -lev -llua