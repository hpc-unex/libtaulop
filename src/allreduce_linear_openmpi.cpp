//
//  allreduce_linear_openmpi.cpp
//
//  Created by jarico on 14/04/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "allreduce_linear_openmpi.hpp"

#include "reduce_linear_openmpi.hpp"
#include "bcast_linear.hpp"

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




AllreduceLinearOpenMPI::AllreduceLinearOpenMPI () {
   
}


AllreduceLinearOpenMPI::~AllreduceLinearOpenMPI () {
   
}


TauLopCost * AllreduceLinearOpenMPI::evaluate (Communicator *comm, const CollParams &cparams) {
      
   int    root = 0;
   int    m    = cparams.getM();
   OpType op   = cparams.getOp();
   
   CollParams cp (m, root, op);
   
   // 1. Reduce to root = 0
   Collective *reduce = new ReduceLinearOpenMPI();
   TauLopCost *cost_red = reduce->evaluate(comm, cp);
   
   // 2. Broadcast from 0 to P processes
   Collective *bcast = new BcastLinear();
   TauLopCost *cost_cast = bcast->evaluate(comm, cp);
   
   // Sum models
   cost_cast->add(cost_red);
      
   return cost_cast;
}

