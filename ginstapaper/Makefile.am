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

-include $(top_srcdir)/git.mk
