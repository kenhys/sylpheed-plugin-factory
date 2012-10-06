#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([sylplugin_factory], [0.1.0], [kenhys@gmail.com])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/sylpf_utility.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE($PACKAGE_NAME, $PACKAGE_VERSION)

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

sylpheed_build_dir='../../'
AC_ARG_WITH(sylpheed_build_dir,
	[  --with-sylpheed-build-dir=DIR    Sylpheed build directory],
	[sylpheed_build_dir="$withval"])
AC_SUBST(sylpheed_build_dir)

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT

dnl Output the configuration summary
echo ""
echo "$PACKAGE $VERSION"
echo ""
echo "Sylpheed build directory : $sylpheed_build_dir"
echo ""
echo "The binary will be installed in $prefix/lib/sylpheed/plugins"
echo ""
echo "Configure finished, type 'make' to build."

