AM_CXXFLAGS = -I$(includedir)

noinst_PROGRAMS = pcomm_creation pcomm_simple pNBody pParallelAxBt ptest_colls ptest_p2p pWave2D

pcomm_creation_SOURCES = communicators/creation.cpp
pcomm_creation_LDADD = -L$(libdir) -ltaulop

pcomm_simple_SOURCES = communicators/simple.cpp
pcomm_simple_LDADD = -L$(libdir) -ltaulop

pNBody_SOURCES = NBody/nbody.cpp
pNBody_LDADD = -L$(libdir) -ltaulop

pParallelAxBt_SOURCES = ParallelAxBt/axbt.cpp
pParallelAxBt_LDADD =  -L$(libdir) -ltaulop

ptest_colls_SOURCES = colls/colls.cpp
ptest_colls_LDADD = -L$(libdir) -ltaulop

ptest_p2p_SOURCES = p2p/p2p.cpp
ptest_p2p_LDADD = -L$(libdir) -ltaulop

pWave2D_SOURCES = Wave2D/wave2d.cpp
pWave2D_LDADD = -L$(libdir) -ltaulop


