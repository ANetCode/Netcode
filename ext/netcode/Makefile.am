AUTOMAKE_OPTIONS=foreign

pkglib_LTLIBRARIES = netcode.la
netcode_la_SOURCES =         \
    lua/lua_netcode.cpp      \
    lua/lua_utils.cpp        \
    lua/lua_connection.cpp   \
    netcode.cpp              \
    network/socket_utils.cpp \
    network/connection.cpp   \
    common/common.cpp        \
    common/buffer.cpp

netcode_la_CPPFLAGS =               \
    -g                              \
    -I$(top_srcdir)/ext/common/

netcode_la_LDFLAGS = -module -avoid-version -shared -llua -ldl -lm -fPIC -lev
