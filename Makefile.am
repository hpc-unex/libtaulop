AM_CXXFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src

lib_LIBRARIES        = libtaulop.a
libtaulop_adir       = $(includedir)
noinst_HEADERS       =
libtaulop_a_HEADERS  = 
libtaulop_a_SOURCES  =
EXTRA_DIST           =


include include/Makefile.am
include src/Makefile.am


tests : all
	@cd tests && mkdir $(exec_prefix)/bin && cd tests && $(MAKE) && mv test_* $(exec_prefix)/bin


