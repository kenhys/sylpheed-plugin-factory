libdir = $(plugindir)

lib_LTLIBRARIES = sylplugin_factory.la

sylplugin_factory_la_SOURCES = sylpf_utility.c sylpf_utility.h
sylplugin_factory_la_LDFLAGS = -module -no-undefined -avoid-version


INCLUDES = \
	$(GTK_CFLAGS) \
	-I$(includedir) \
	-I$(sylpheed_build_dir)/libsylph \
	-I$(sylpheed_build_dir)/src \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src \

sylplugin_factory_la_LIBADD = \
	$(GTK_LIBS) \
	$(sylpheed_build_dir)/src/libsylpheed-plugin-0.la \
	$(sylpheed_build_dir)/libsylph/libsylph-0.la

install:
install-plugin: install-am
