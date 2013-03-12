SUBDIRS = ginstapaper tests

libginstapaperdocdir = ${prefix}/doc/libginstapaper
libginstapaperdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

%-$(API_VERSION).pc: %.pc
	$(AM_V_GEN)cp $< $@

pkgconfigdir   = $(libdir)/pkgconfig
pkgconfig_DATA = libginstapaper-$(API_VERSION).pc

CLEANFILES = $(pkgconfig_DATA)

all-local: libginstapaper-$(API_VERSION).pc

EXTRA_DIST = $(libginstapaperdoc_DATA)

# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(libginstapaperdocdir)
