AM_CXXFLAGS = -I$(includedir) -I$(top_srcdir)/tests/utils

noinst_PROGRAMS =  test_p2p test_computation test_gather test_bcast test_allgather test_alltoall test_reduce test_allreduce test_scatter test_comm_creation test_comm_simple test_axbt test_wave2d test_nbody imb_taulop suara2d_taulop suara3d_taulop suara2dmap_taulop suara3dmap_taulop


test_p2p_SOURCES = basics/p2p/p2p.cpp
test_p2p_CXXFLAGS = $(AM_CXXFLAGS)
test_p2p_LDADD = -L$(libdir) -ltaulop

test_computation_SOURCES = basics/computation/computation.cpp utils/args.cpp
test_computation_CXXFLAGS = $(AM_CXXFLAGS)
test_computation_LDADD = -L$(libdir) -ltaulop

test_gather_SOURCES = basics/colls/gather.cpp utils/args.cpp
test_gather_CXXFLAGS = $(AM_CXXFLAGS)
test_gather_LDADD = -L$(libdir) -ltaulop

test_comm_simple_SOURCES = basics/communicators/simple.cpp
test_comm_simple_CXXFLAGS = $(AM_CXXFLAGS)
test_comm_simple_LDADD = -L$(libdir) -ltaulop

test_comm_creation_SOURCES = basics/communicators/creation.cpp
test_comm_creation_CXXFLAGS = $(AM_CXXFLAGS)
test_comm_creation_LDADD = -L$(libdir) -ltaulop

test_bcast_SOURCES = basics/colls/bcast.cpp utils/args.cpp
test_bcast_CXXFLAGS = $(AM_CXXFLAGS)
test_bcast_LDADD = -L$(libdir) -ltaulop

test_allgather_SOURCES = basics/colls/allgather.cpp utils/args.cpp
test_allgather_CXXFLAGS = $(AM_CXXFLAGS)
test_allgather_LDADD = -L$(libdir) -ltaulop

test_alltoall_SOURCES = basics/colls/alltoall.cpp utils/args.cpp
test_alltoall_CXXFLAGS = $(AM_CXXFLAGS)
test_alltoall_LDADD = -L$(libdir) -ltaulop

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

imb_taulop_SOURCES = imb/imb_taulop.cpp imb/pingpong.cpp imb/broadcast.cpp imb/reduce.cpp imb/allreduce.cpp imb/benchmark.cpp
imb_taulop_CXXFLAGS = $(AM_CXXFLAGS)
imb_taulop_LDADD = -L$(libdir) -ltaulop

suara2d_taulop_SOURCES = Suara/suara2d_grid.cpp Suara/allreduce.cpp
suara2d_taulop_CXXFLAGS = $(AM_CXXFLAGS)
suara2d_taulop_LDADD = -L$(libdir) -ltaulop

suara3d_taulop_SOURCES = Suara/suara3d_grid.cpp Suara/allreduce.cpp
suara3d_taulop_CXXFLAGS = $(AM_CXXFLAGS)
suara3d_taulop_LDADD = -L$(libdir) -ltaulop

suara2dmap_taulop_SOURCES = Suara/suara2d_grid_map.cpp Suara/allreduce.cpp
suara2dmap_taulop_CXXFLAGS = $(AM_CXXFLAGS)
suara2dmap_taulop_LDADD = -L$(libdir) -ltaulop

suara3dmap_taulop_SOURCES = Suara/suara3d_grid_map.cpp Suara/allreduce.cpp
suara3dmap_taulop_CXXFLAGS = $(AM_CXXFLAGS)
suara3dmap_taulop_LDADD = -L$(libdir) -ltaulop


