#AC_PREREQ([2.71])

AC_INIT([taulop],[1.2],[jarico@unex.es])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

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





###############################################################################
# with-params
###############################################################################

AC_ARG_WITH(params,
[  --with-params[[=path]] - Specify the path of the parameter values in case of they are located in a custom location.],,with_params=empty)


if test "$with_params" = "empty"; then

  AC_DEFINE_UNQUOTED(PARAMS_FOLDER, "./params", [Parameter values are in the ./params subfolder.])

else

  AC_DEFINE_UNQUOTED(PARAMS_FOLDER, "$with_params", [Parameter values are in the custom folder: $with_params])

fi





AC_CONFIG_FILES([
	Makefile
	tests/Makefile
])


AC_OUTPUT


echo \
"------------------------------------------------------------------------------
------------------------------------------------------------------------------
 ${PACKAGE_NAME} version ${PACKAGE_VERSION}

 General Options

  Prefix . . . . . . .: '${prefix}'
  Compiler . . . . . .: '${CXX} ${CXXFLAGS}'

 Debug Options

 Library Configurations

  Parameter Values . .: '$with_params'


 Now type 'make @<:@<target>@:>@', where the optional <target> is:

  all        - build library
  install    - install everything
  dist       - create the distribution tarball
  tests      - build, but not execute, all tests
------------------------------------------------------------------------------
------------------------------------------------------------------------------"

