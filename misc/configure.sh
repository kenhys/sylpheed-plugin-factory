OPTIONS=" --with-sylpheed-build-dir=`pwd`/sylpheed "

if [ -n "$USE_WEBKITGTK" ]; then
    OPTIONS="$OPTIONS --enable-webkitgtk"
fi
if [ -n "$USE_GTKHTML" ]; then
    OPTIONS="$OPTIONS --enable-gtkhtml"
fi
if [ -n "$USE_LIBNOTIFY" ]; then
    OPTIONS="$OPTIONS --enable-libnotify"
fi
if [ -n "$USE_LIBAPPINDICATOR" ]; then
    OPTIONS="$OPTIONS --enable-libappindicator"
fi
if [ -n "$USE_DEBUG" ]; then
    OPTIONS="$OPTIONS --enable-debug"
fi


SYLPHEED_BUILD_DIR=`pwd`/sylpheed
PLUGIN_DIR=/usr/lib/sylpheed/plugins
./configure $OPTIONS --with-sylpheed-build-dir=$SYLPHEED_BUILD_DIR --with-sylpheed-plugin-dir=$PLUGIN_DIR
