OPTIONS=" --with-sylpheed-build-dir=`pwd`/sylpheed "

if [ -n "$USE_WEBKITGTK" ]; then
    OPTIONS="$OPTIONS --enable-webkitgtk"
fi
if [ -n "$USE_GTKHTML" ]; then
    OPTIONS="$OPTIONS --enable-gtkhtml"
fi
if [ -n "$USE_DEBUG" ]; then
    OPTIONS="$OPTIONS --enable-debug"
fi

./configure $OPTIONS
