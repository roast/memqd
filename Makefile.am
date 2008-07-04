bin_PROGRAMS = memqd memqd-debug

memqd_SOURCES = memqd.c slabs.c items.c assoc.c memqd.h
memqd_debug_SOURCES = $(memqd_SOURCES)
memqd_CPPFLAGS = -DNDEBUG
memqd_LDADD = @LIBOBJS@
memqd_debug_LDADD = $(memqd_LDADD)

SUBDIRS = doc
DIST_DIRS = scripts
EXTRA_DIST = doc scripts TODO t

test:	memqd-debug
	prove t

dist-hook:
	rm -rf $(distdir)/doc/.svn/
	rm -rf $(distdir)/scripts/.svn/
	rm -rf $(distdir)/t/.svn/
	rm -rf $(distdir)/t/lib/.svn/
