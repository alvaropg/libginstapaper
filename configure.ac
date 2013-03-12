m4_define([api_major], [0])
m4_define([api_minor], [1])
m4_define([api_micro], [0])

m4_define([ginstapaper_version], [api_major.api_minor.api_micro])

AC_PREREQ([2.63])
AC_INIT([ginstapaper], [ginstapaper_versino],
        [],
        [ginstapaper],
        [https://github.com/alvaropg/libginstapaper.git])

AC_CONFIG_SRCDIR([ginstapaper/ginstapaper-proxy.h])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

API_MAJOR=api_major
API_MINOR=api_minor
AC_SUBST([API_VERSION],[$API_MAJOR.$API_MINOR])
AC_SUBST([API_VERSION_AM],[$API_MAJOR\_$API_MINOR])
AC_DEFINE_UNQUOTED(API_VERSION, [$API_VERSION], [API version])

AC_PROG_CC


LT_INIT


PKG_CHECK_MODULES(LIBGINSTAPAPER, [glib-2.0 rest-0.7 gobject-2.0 json-glib-1.0])

AC_OUTPUT([
Makefile
libginstapaper.pc
ginstapaper/Makefile
tests/Makefile])
