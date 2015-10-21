TESTS = gtestutils

AM_CPPFLAGS = -I$(top_srcdir) $(LIBGINSTAPAPER_CFLAGS)
AM_LDFLAGS = $(top_srcdir)/ginstapaper/libginstapaper-@API_VERSION@.la $(LIBGINSTAPAPER_LIBS)

noinst_PROGRAMS = $(TESTS)

gtestutils_SOURCES = gtestutils.c credentials.h

-include $(top_srcdir)/git.mk
