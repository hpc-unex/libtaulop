AM_CXXFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src

lib_LIBRARIES        = libtaulop.a
libtaulop_adir       = $(includedir)
noinst_HEADERS       =
libtaulop_a_HEADERS  = # set via include/Makefile.am
libtaulop_a_SOURCES  = # set via src/Makefile.am
EXTRA_DIST           =


include include/Makefile.am
include src/Makefile.am

build_tests: all
	cd tests && $(MAKE)

tests : build_tests
	@mkdir -p $(exec_prefix)/bin && mv test_* $(exec_prefix)/bin
