SUBDIRS = src

ACLOCAL_AMFLAGS = -I m4 $$ACLOCAL_ARGS

PLUGIN_NAME = sylplugin_factory
PLUGIN_SO = src/.libs/$(PLUGIN_NAME).so

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = sylplugin-factory.pc

m4datadir = $(datadir)/sylplugin-factory
m4data_DATA = m4/sylplugin_factory.m4 \
	      m4/compiler_flag.m4 \
	      build/common.mk

include build/common.mk


