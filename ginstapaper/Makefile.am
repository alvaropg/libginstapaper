lib_sources = \
	ginstapaper-proxy.c		\
	ginstapaper-bookmark.c		\
	ginstapaper-bookmarks-call.c
lib_headers = \
	ginstapaper.h 			\
	ginstapaper-proxy.h		\
	ginstapaper-bookmark.h		\
	ginstapaper-bookmarks-call.h

lib_LTLIBRARIES = libginstapaper-@API_VERSION@.la

libginstapaper_@API_VERSION@_la_CFLAGS = $(LIBGINSTAPAPER_CFLAGS) -I$(top_srcdir) \
	-Wall -g -DG_LOG_DOMAIN=\"GInstapaper\"
libginstapaper_@API_VERSION@_la_LDFLAGS = -no-undefined
libginstapaper_@API_VERSION@_la_LIBADD = $(LIBGINSTAPAPER_LIBS)
libginstapaper_@API_VERSION@_la_SOURCES = $(lib_sources) $(lib_headers)
libginstapaper_@API_VERSION@_la_HEADERS = $(lib_headers)
libginstapaper_@API_VERSION@_ladir = $(includedir)/ginstapaper-@API_VERSION@/ginstapaper

# Introspection
-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir)
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir)

if HAVE_INTROSPECTION
introspection_sources = $(libginstapaper_@API_MAJOR@_@API_MINOR@_la_SOURCES)

GInstapaper-@API_VERSION@.gir: libginstapaper-@API_VERSION@.la
GInstapaper_@API_MAJOR@_@API_MINOR@_gir_INCLUDES = GObject-2.0 Rest-0.7 Json-1.0
GInstapaper_@API_MAJOR@_@API_MINOR@_gir_CFLAGS = $(LIBGINSTAPAPER_CFLAGS) -I$(top_srcdir)
GInstapaper_@API_MAJOR@_@API_MINOR@_gir_LIBS = libginstapaper-@API_VERSION@.la
GInstapaper_@API_MAJOR@_@API_MINOR@_gir_FILES = $(lib_sources) $(lib_headers)
GInstapaper_@API_MAJOR@_@API_MINOR@_gir_NAMESPACE = GInstapaper
GInstapaper_@API_MAJOR@_@API_MINOR@_gir_EXPORT_PACKAGES = libginstapaper.@API_VERSION@
GInstapaper_@API_MAJOR@_@API_MINOR@_gir_SCANNERFLAGS = \
	--nsversion=$(API_VERSION) \
	--symbol-prefix=ginstapaper \
	--c-include="ginstapaper/ginstapaper.h"

INTROSPECTION_GIRS += GInstapaper-$(API_VERSION).gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = ../girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES = $(gir_DATA) $(typelib_DATA)

endif

-include $(top_srcdir)/git.mk
