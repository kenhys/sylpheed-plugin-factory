lib_LTLIBRARIES = libsylplugin-factory.la

libsylplugin_factory_la_SOURCES = \
	sylplugin_factory.c \
	../include/sylplugin_factory.h

libsylplugin_factory_la_LDFLAGS = -no-undefined -avoid-version

AM_CPPFLAGS = \
	$(GTK_CFLAGS) \
	-I$(includedir) \
	-I$(sylpheed_build_dir)/libsylph \
	-I$(sylpheed_build_dir)/src \
	-I$(top_srcdir) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src

if WITH_LIBSYLPH_IMPL
LIBSYLPH_IMPL = $(libsylph_impl)
else
LIBSYLPH_IMPL = $(sylpheed_build_dir)/libsylph/libsylph-0.la
endif

if WITH_LIBSYLPHEED_IMPL
LIBSYLPHEED_IMPL = $(libslpheed_impl)
else
LIBSYLPHEED_IMPL = $(sylpheed_build_dir)/src/libsylpheed-plugin-0.la
endif

libsylplugin_factory_la_LIBADD = \
	$(GTK_LIBS) \
	$(LIBSYLPH_IMPL) \
	$(LIBSYLPHEED_IMPL)

