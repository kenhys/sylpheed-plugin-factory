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
