AC_DEFUN([CHECK_CFLAG], [
  AC_MSG_CHECKING([if gcc supports $1])
  old_CFLAGS=$CFLAGS
  flag=`echo '$1' | sed -e 's,^-Wno-,-W,'`
  CFLAGS="$CFLAGS $flag -Werror"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
    [check_cflag=yes],
    [check_cflag=no])
  CFLAGS="$old_CFLAGS"
  if test "x$check_cflag" = "xyes"; then
    CFLAGS="$CFLAGS $1"
  fi
  AC_MSG_RESULT([$check_cflag])
])

if test "$GCC" = "yes"; then
  CHECK_CFLAG([-Wall])
  CHECK_CFLAG([-Wextra])
  if test "x$check_cflag" = "xno"; then
    CHECK_CFLAG([-W])
  fi
  CHECK_CFLAG([-Wno-unused-but-set-variable]) # FIXME: enable it.
  CHECK_CFLAG([-Wno-unused-parameter])
  CHECK_CFLAG([-Wno-sign-compare])
  CHECK_CFLAG([-Wno-pointer-sign])
  CHECK_CFLAG([-Wno-missing-field-initializers])

  CHECK_CFLAG([-Wformat=2])
  CHECK_CFLAG([-Wstrict-aliasing=2])
  if test "x$check_cflag" = "xyes"; then
    NO_STRICT_ALIASING_CFLAGS="-fno-strict-aliasing"
  fi
  CHECK_CFLAG([-Wdisabled-optimization])
  CHECK_CFLAG([-Wfloat-equal])
  CHECK_CFLAG([-Wpointer-arith])
  CHECK_CFLAG([-Wdeclaration-after-statement])
  CHECK_CFLAG([-Wbad-function-cast])
  if test "$CLANG" = "no"; then
    CHECK_CFLAG([-Wcast-align])
  fi
  CHECK_CFLAG([-Wredundant-decls])
fi
