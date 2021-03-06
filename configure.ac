#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
m4_define([SYLPF_MAJOR], [0])
m4_define([SYLPF_MINOR], [2])
m4_define([SYLPF_MICRO], [0])
m4_define([SYLPF_VERSION],
	  [SYLPF_MAJOR.SYLPF_MINOR.SYLPF_MICRO])
AC_INIT([sylplugin_factory], [SYLPF_VERSION], [kenhys@gmail.com])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/sylplugin_factory.c])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign])

AC_SUBST([MAJOR_VERSION], [SYLPF_MAJOR])
AC_SUBST([MINOR_VERSION], [SYLPF_MINOR])
AC_SUBST([MICRO_VERSION], [SYLPF_MICRO])
m4_define(SYLPF_REVISION, m4_include(version))
AC_SUBST([BUILD_REVISION], [SYLPF_REVISION])

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

AC_DEFINE([SYLPF_ID], [PACKAGE_NAME], [""])
AC_DEFINE([SYLPF_OPTION], [sylplugin_factory_option], [""])


m4_include(m4/compiler_flag.m4)

# Checks for libraries.
AM_PATH_GLIB_2_0(2.8.0,, AC_MSG_ERROR(Test for GLib failed. See the 'INSTALL' for help.))
AM_PATH_GTK_2_0(2.8.0,, AC_MSG_ERROR(Test for Gtk failed. See the 'INSTALL' for help.))

AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)
AC_DEFINE([BUILD_SYLPLUGIN_FACTORY_STANDALONE], [], [standalone build])


# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

m4_include(m4/sylplugin_factory.m4)

AC_CONFIG_FILES([Makefile
                 sylplugin-factory.pc
                 include/Makefile
                 src/Makefile
                 res/version.rc
])
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

