libdir = $(plugindir)

lib_LTLIBRARIES = sylplugin_factory.la

sylplugin_factory_la_SOURCES = sylpf_utility.c sylpf_utility.h
sylplugin_factory_la_LDFLAGS = -module -no-undefined -avoid-version


INCLUDES = \
	$(GTK_CFLAGS) \
	-I$(includedir) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/libsylph \
	-I$(top_srcdir)/src

sylplugin_factory_la_LIBADD = \
	$(GTK_LIBS) \
        $(top_builddir)/src/libsylpheed-plugin-0.la \
	$(top_builddir)/libsylph/libsylph-0.la

install:
install-plugin: install-am
