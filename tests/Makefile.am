TESTS = auth

AM_CPPFLAGS = -I$(top_srcdir) $(LIBGINSTAPAPER_CFLAGS)
AM_LDFLAGS = $(top_srcdir)/ginstapaper/libginstapaper-@API_VERSION@.la $(LIBGINSTAPAPER_LIBS)

noinst_PROGRAMS = $(TESTS)

auth_SOURCES = auth.c

-include $(top_srcdir)/git.mk
