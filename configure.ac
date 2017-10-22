#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([netcode], [0.1.0], [bug-report@netcode.com])
AM_INIT_AUTOMAKE([foreign -Werror subdir-objects])

AC_PROG_CXX

AC_ENABLE_SHARED
AC_DISABLE_STATIC
AC_PROG_LIBTOOL(libtool)
LT_INIT([disable-static])

CXXFLAGS="$CXXFLAGS -std=c++0x"
AC_CONFIG_SRCDIR([netcode])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    Makefile
    netcode/Makefile
    ext/Makefile
    ext/logger/Makefile
    ext/netcode/Makefile
])

AC_OUTPUT
