OPTIONS=""

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

PREFIX=/tmp/local
SYLPHEED_BUILD_DIR=`pwd`/sylpheed
SYLPHEED_PLUGIN_DIR=$PREFIX/lib/sylpheed/plugins
export PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig
./configure $OPTIONS \
	    --with-sylpheed-build-dir=$SYLPHEED_BUILD_DIR \
	    --with-sylpheed-plugin-dir=$SYLPHEED_PLUGIN_DIR
