//
//  allreduce_linear_openmpi.cpp
//
//  Created by jarico on 14/04/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "allreduce_nonoverlap_openmpi.hpp"

#include "reduce_binomial_openmpi.hpp"
#include "bcast_binomial_openmpi.hpp"

#include "coll_params.hpp"
#include "transmission.hpp"
#include "computation.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"
#include "taulop_cost.hpp"

#include <math.h>
#include <iostream>
using namespace std;




AllreduceNonOverlapOpenMPI::AllreduceNonOverlapOpenMPI () {
   
}


AllreduceNonOverlapOpenMPI::~AllreduceNonOverlapOpenMPI () {
   
}


TauLopCost * AllreduceNonOverlapOpenMPI::evaluate (Communicator *comm, const CollParams &cparams) {
      
   int P  = comm->getSize();
   
   Communicator *world = new Communicator (P);
   int root  = 0;
   int m     = cparams.getM();
   OpType op = cparams.getOp();
   
   CollParams cp (m, root, op);
   
   // 1. Reduce to root = 0
   Collective *reduce = new ReduceBinomialOpenMPI();
   TauLopCost *cost_red = reduce->evaluate(world, cp);
   
   // 2. Broadcast from 0 to P processes
   Collective *bcast = new BcastBinomialOpenMPI();
   TauLopCost *cost_cast = bcast->evaluate(world, cp);
   
   // Sum models
   cost_cast->add(cost_red);
   
   delete world;
   
   return cost_cast;
}

