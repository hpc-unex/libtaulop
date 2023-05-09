AM_CXXFLAGS = -I$(includedir) -I$(top_srcdir)/tests/utils

noinst_PROGRAMS =  test_p2p test_computation test_colls test_reduce test_allreduce test_scatter test_comm_creation test_comm_simple test_axbt test_wave2d test_nbody imb_taulop


test_p2p_SOURCES = basics/p2p/p2p.cpp
test_p2p_CXXFLAGS = $(AM_CXXFLAGS)
test_p2p_LDADD = -L$(libdir) -ltaulop

test_computation_SOURCES = basics/computation/computation.cpp utils/args.cpp
test_computation_CXXFLAGS = $(AM_CXXFLAGS)
test_computation_LDADD = -L$(libdir) -ltaulop

test_colls_SOURCES = basics/colls/colls.cpp
test_colls_CXXFLAGS = $(AM_CXXFLAGS)
test_colls_LDADD = -L$(libdir) -ltaulop

test_comm_simple_SOURCES = basics/communicators/simple.cpp
test_comm_simple_CXXFLAGS = $(AM_CXXFLAGS)
test_comm_simple_LDADD = -L$(libdir) -ltaulop

test_comm_creation_SOURCES = basics/communicators/creation.cpp
test_comm_creation_CXXFLAGS = $(AM_CXXFLAGS)
test_comm_creation_LDADD = -L$(libdir) -ltaulop

test_reduce_SOURCES = basics/colls/reduce.cpp utils/args.cpp
test_reduce_CXXFLAGS = $(AM_CXXFLAGS)
test_reduce_LDADD = -L$(libdir) -ltaulop

test_allreduce_SOURCES = basics/colls/allreduce.cpp utils/args.cpp
test_allreduce_CXXFLAGS = $(AM_CXXFLAGS)
test_allreduce_LDADD = -L$(libdir) -ltaulop

test_scatter_SOURCES = basics/colls/scatter.cpp utils/args.cpp
test_scatter_CXXFLAGS = $(AM_CXXFLAGS)
test_scatter_LDADD = -L$(libdir) -ltaulop

test_nbody_SOURCES = NBody/nbody.cpp
test_nbody_CXXFLAGS = $(AM_CXXFLAGS)
test_nbody_LDADD = -L$(libdir) -ltaulop

test_axbt_SOURCES = ParallelAxBt/axbt.cpp
test_axbt_CXXFLAGS = $(AM_CXXFLAGS)
test_axbt_LDADD =  -L$(libdir) -ltaulop

test_wave2d_SOURCES = Wave2D/wave2d.cpp
test_wave2d_CXXFLAGS = $(AM_CXXFLAGS)
test_wave2d_LDADD = -L$(libdir) -ltaulop

imb_taulop_SOURCES = imb/imb_taulop.cpp imb/pingpong.cpp
imb_taulop_CXXFLAGS = $(AM_CXXFLAGS)
imb_taulop_LDADD = -L$(libdir) -ltaulop


