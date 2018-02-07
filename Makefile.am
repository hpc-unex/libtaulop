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
	@cd tests && $(MAKE) && mv pNBody NBody/ && mv pcommunicators communicators/ \
	&& mv pParallelAxBt ParallelAxBt/ && mv ptest_colls test_colls/ \
	&& mv ptest_p2p test_p2p/ && mv pWave2D Wave2D/


