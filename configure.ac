AC_PREREQ(2.50)

AC_INIT([taulop], [0.1], [jarico@unex.es])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
#AC_PROG_MAKE_SET
AC_PROG_RANLIB


# Options
# (Feb. 6th, 2018) C++ 11 extensions (as nullptr):
CXXFLAGS=" $CXXFLAGS -std=c++11 "


AC_CONFIG_FILES([Makefile])
AC_OUTPUT

