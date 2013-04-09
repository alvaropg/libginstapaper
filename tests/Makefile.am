TESTS = gtestutils \
	auth \
	bookmark

AM_CPPFLAGS = -I$(top_srcdir) $(LIBGINSTAPAPER_CFLAGS)
AM_LDFLAGS = $(top_srcdir)/ginstapaper/libginstapaper-@API_VERSION@.la $(LIBGINSTAPAPER_LIBS)

noinst_PROGRAMS = $(TESTS)

gtestutils_SOURCES = gtestutils.c credentials.h

auth_SOURCES = auth.c credentials.h

bookmark_SOURCES = bookmark.c credentials.h

-include $(top_srcdir)/git.mk
