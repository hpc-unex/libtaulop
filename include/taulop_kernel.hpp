//
//  taulop_kernel.hpp
//  taulop
//
//  Created by jarico on 5/5/17.
//  Copyright © 2017 University of Extremadura. All rights reserved.
//

#ifndef taulop_kernel_h
#define taulop_kernel_h


#include "communicator.hpp"
#include "mapping.hpp"
#include "collective.hpp"
#include "graph.hpp"

#include "bcast_linear.hpp"
#include "bcast_binomial.hpp"
#include "bcast_binomial_openmpi.hpp"
#include "gather_binomial.hpp"
#include "gatherv_binomial.hpp"
#include "allgather_ring.hpp"
#include "allgather_rda.hpp"
#include "graph_collective.hpp"

#include "taulop_sequence.hpp"
#include "taulop_concurrent.hpp"
#include "process.hpp"
#include "taulop_params.hpp"
#include "config.h"


#endif /* taulop_kernel_h */
