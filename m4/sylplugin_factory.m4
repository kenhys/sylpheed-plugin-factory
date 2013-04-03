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

AC_ARG_WITH(sylplugin-factory-source-dir,
            [AS_HELP_STRING([--with-sylplugin-factory-source-dir=DIR],
            [specify sylpheed-plugin-factory source directory.])],
            [sylpfdir="$withval"])
if test "x$sylpfdir" = "x"; then
  sylplugin_factory_source_dir=`pwd`/..
  if test -d "$sylplugin_factory_source_dir"; then
    if test -d "$sylplugin_factory_source_dir/src"; then
      sylpf_dir_available="yes"
    fi
  fi
else
  sylpfdir_available="yes"
  sylplugin_factory_source_dir=$sylpfdir
fi
AC_SUBST(sylplugin_factory_source_dir)
AM_CONDITIONAL([WITH_SYLPLUGIN_FACTORY], [test "$sylpfdir_available" = "yes"])

