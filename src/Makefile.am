lib_LTLIBRARIES = libsylplugin-factory.la

libsylplugin_factory_la_SOURCES = \
	sylplugin_factory.c \
	sylplugin_factory.h

libsylplugin_factory_la_LDFLAGS = -module -no-undefined -avoid-version

sylpluginincludedir = $(includedir)/sylplugin-factory
sylplugininclude_HEADERS = \
	sylplugin_factory.h \
	sylplugin_i18n.h

AM_CPPFLAGS = \
	$(GTK_CFLAGS) \
	-I$(includedir) \
	-I$(sylpheed_build_dir)/libsylph \
	-I$(sylpheed_build_dir)/src \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src

libsylplugin_factory_la_LIBADD = \
	$(GTK_LIBS) \
	$(sylpheed_build_dir)/src/libsylpheed-plugin-0.la \
	$(sylpheed_build_dir)/libsylph/libsylph-0.la

