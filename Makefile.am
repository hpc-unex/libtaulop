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
	@mkdir -p $(exec_prefix)/bin && cd tests && $(MAKE)
	@mv -f tests/test_p2p $(exec_prefix)/bin/p2p
	@mv -f tests/test_computation $(exec_prefix)/bin/computation
	@mv -f tests/test_gather $(exec_prefix)/bin/gather
	@mv -f tests/test_bcast $(exec_prefix)/bin/bcast
	@mv -f tests/test_allgather $(exec_prefix)/bin/allgather
	@mv -f tests/test_alltoall $(exec_prefix)/bin/alltoall
	@mv -f tests/test_reduce $(exec_prefix)/bin/reduce
	@mv -f tests/test_allreduce $(exec_prefix)/bin/allreduce
	@mv -f tests/test_scatter $(exec_prefix)/bin/scatter
	@mv -f tests/test_comm_creation $(exec_prefix)/bin/comm_creation ;
	@mv -f tests/test_comm_simple $(exec_prefix)/bin/comm_simples ;
	@mv -f tests/test_axbt $(exec_prefix)/bin/axbt ;
	@mv -f tests/test_nbody $(exec_prefix)/bin/nbody ;
	@mv -f tests/test_wave2d $(exec_prefix)/bin/wave2d ;
	@mv -f tests/imb_taulop $(exec_prefix)/bin/imb_taulop ;
	@mv -f tests/suara2d_taulop $(exec_prefix)/bin/suara2d_taulop ;
	@mv -f tests/suara3d_taulop $(exec_prefix)/bin/suara3d_taulop ;
	@mv -f tests/suara2dmap_taulop $(exec_prefix)/bin/suara2dmap_taulop ;
	@mv -f tests/suara3dmap_taulop $(exec_prefix)/bin/suara3dmap_taulop ;
	@mkdir -p $(exec_prefix)/scripts ;
	@mkdir -p $(exec_prefix)/scripts/compare ;
	@mkdir -p $(exec_prefix)/scripts/transform ;
	@cp -f $(top_srcdir)/tests/imb/compare/* $(exec_prefix)/scripts/compare ;
	@cp -f $(top_srcdir)/params/transform/* $(exec_prefix)/scripts/transform

