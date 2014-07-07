#!/bin/bash

PKG_NAME=
DLL_FILE=
OBJS=
LIBSYLPH=./lib/libsylph-0-1.a
LIBSYLPHEED=./lib/libsylpheed-plugin-0-1.a
#LIBS=" -lglib-2.0-0  -lintl"
LIBS=" `pkg-config --libs glib-2.0 gobject-2.0 gtk+-2.0`"
INC=" -I. -I./src -I./lib/sylplugin_factory/src -I../../ -I../../libsylph -I../../src  `pkg-config --cflags glib-2.0 cairo gdk-2.0 gtk+-2.0`"
DEF=" -DHAVE_CONFIG_H"

function run() {
    "$@"
    if test $? -ne 0; then
        echo "Failed $@"
        exit 1
    fi
}

function usage() {
    cat <<EOF 1>&2
Usage:
     $0 [-d|--debug]
        [-p|--po]
        [-m|--mo]
Mandatory args:
  -d,--debug enable debug build
  -p,--po    update po files
  -m,--mo    update mo files
Optional args:
  -h,--help  print this help
EOF
}

function make_def() {
    for pkg in libsylph-0-1 libsylpheed-plugin-0-1; do
	(cd lib;pexports $pkg.dll > $pkg.dll.def)
	(cd lib;dlltool --dllname $pkg.dll --input-def $pkg.dll.def --output-lib $pkg.a)
    done
}

function make_clean() {
    rm -f *.o *.lo *.la *.bak *~
}

function make_distclean() {
    rm -f *.o *.lo *.la *.bak *.dll *.zip
}

function make_pot() {
    mkdir -p po
    run xgettext src/$NAME.c -k_ -kN_ -o po/$NAME.pot
}

function make_po() {
    run msgmerge po/ja.po po/$PKG_NAME.pot -o po/ja.po
}

function make_mo() {
    run msgfmt po/ja.po -o po/$PKG_NAME.mo
    if [ -d "$SYLLOCALEDIR" ]; then
        run cp po/$PKG_NAME.mo $SYLLOCALEDIR/$PKG_NAME.mo
    fi
}

function make_res() {
    run windres -i res/version.rc -o version.o
}


function make_dll() {
    PKG_NAME=$1
    DLL_FILE=$2

    make_res

    for src in `find . -path './test' -prune -o -name '*.c'`; do
	src_base=${src%%.c}
	run gcc -Wall -c -o ${src_base}.o $DEF $INC ${src}
    done

    OBJS=`find . -name '*.o'`
    run gcc -shared -o $DLL_FILE $OBJS -L./lib $LIBSYLPH $LIBSYLPHEED $LIBS -lws2_32 -liconv
    if [ -d "$SYLPLUGINDIR" ]; then
        com="cp $DLL_FILE \"$SYLPLUGINDIR/$DLL_FILE\""
        echo $com
        eval $com
    else
        :
    fi
}

function zip_archive() {
    ZIP_FILE=$1
    EXT=$2
    for ext in `find . -name "$EXT"`; do
	zip -r $ZIP_FILE $ext
    done
}

function make_release() {
    if [ -z "$1" ]; then
	return
    fi
    version=$1
    shift
    if [ -f src/$DLL_FILE ]; then
	mv src/$DLL_FILE .
    fi
    ZIP_FILE=sylpheed-$PKG_NAME-${version}.zip
    zip  $DLL_FILE
    zip_archive $ZIP_FILE '*.txt'
    zip_archive $ZIP_FILE '*.h'
    zip_archive $ZIP_FILE '*.c'
    zip_archive $ZIP_FILE '*.rc'
    zip_archive $ZIP_FILE '*.mo'
    zip_archive $ZIP_FILE '*.xpm'
    zip -r $ZIP_FILE COPYING LICENSE README.md NEWS
    sha1sum $ZIP_FILE > $ZIP_FILE.sha1sum
}

mode=""

if [ $# -eq 0 ]; then
    usage
    exit 1
fi

while [ $# -ne 0 ]; do
    case "$1" in
	-h|--help) usage && exit 0;;
        --build) make_dll $PKG_NAME $DLL_FILE; shift;;
	--po)   make_po; shift;;
	--dclean) make_distclean; shift;;
	--clean) make_clean; shift;;
	--pot)  make_pot; shift;;
        --mo)   make_mo; shift;;
        --def)  make_def; shift;;
	--res)  make_res; shift;;
        --debug) mode=debug; shift;;
	--pkg)
	    PKG_NAME=$2
	    DLL_FILE=$2.dll
	    shift 2
	    ;;
        --release)
	    shift
	    if [ -z "$1" ]; then
		usage && exit 1
	    else
		make_release $1
		shift
	    fi
	    ;;
	*) shift;;
    esac
done

