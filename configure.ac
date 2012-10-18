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
AM_PATH_GLIB_2_0(2.8.0,, AC_MSG_ERROR(Test for GLib failed. See the 'INSTALL' for help.))
AM_PATH_GTK_2_0(2.8.0,, AC_MSG_ERROR(Test for Gtk failed. See the 'INSTALL' for help.))

AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)


# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_ARG_WITH(sylpheed-build-dir,
	    [AS_HELP_STRING([--with-sylpheed-build-dir=DIR],
	    [specify sylpheed build directory.])],
	    [syldir="$withval"])
if test "x$syldir" = "x"; then
  sylpheed_build_dir=`pwd`/../..
  if test -d "$sylpheed_build_dir/libsylph"; then
    if test -d "$sylpheed_build_dir/src"; then
      syldir_available="yes"
      syldir_available="yes"
    fi
  fi
else
  syldir_available="yes"
  sylpheed_build_dir=$syldir
fi
AC_SUBST(sylpheed_build_dir)
AM_CONDITIONAL([WITH_SYLPHEED], [test "$syldir_available" = "yes"])

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

