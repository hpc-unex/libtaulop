AM_CXXFLAGS = -I$(includedir)

noinst_PROGRAMS =  test_p2p test_colls test_comm_creation test_comm_simple test_axbt test_wave2d test_nbody 


test_p2p_SOURCES = p2p/p2p.cpp
test_p2p_CXXFLAGS = $(AM_CXXFLAGS)
test_p2p_LDADD = -L$(libdir) -ltaulop

test_colls_SOURCES = colls/colls.cpp
test_colls_CXXFLAGS = $(AM_CXXFLAGS)
test_colls_LDADD = -L$(libdir) -ltaulop

test_comm_simple_SOURCES = communicators/simple.cpp
test_comm_simple_CXXFLAGS = $(AM_CXXFLAGS)
test_comm_simple_LDADD = -L$(libdir) -ltaulop

test_comm_creation_SOURCES = communicators/creation.cpp
test_comm_creation_CXXFLAGS = $(AM_CXXFLAGS)
test_comm_creation_LDADD = -L$(libdir) -ltaulop

test_nbody_SOURCES = NBody/nbody.cpp
test_nbody_CXXFLAGS = $(AM_CXXFLAGS)
test_nbody_LDADD = -L$(libdir) -ltaulop

test_axbt_SOURCES = ParallelAxBt/axbt.cpp
test_axbt_CXXFLAGS = $(AM_CXXFLAGS)
test_axbt_LDADD =  -L$(libdir) -ltaulop

test_wave2d_SOURCES = Wave2D/wave2d.cpp
test_wave2d_CXXFLAGS = $(AM_CXXFLAGS)
test_wave2d_LDADD = -L$(libdir) -ltaulop


