//
//  allreduce_linear_openmpi.cpp
//
//  Created by jarico on 14/04/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "allreduce_linear_openmpi.hpp"

#include "reduce_linear_openmpi.hpp"
#include "bcast_linear.hpp"

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


TauLopCost * AllreduceLinearOpenMPI::evaluate (Communicator *comm, int *size, int root, OpType op) {
      
   int P  = comm->getSize();
   
   Communicator *world = new Communicator (P);
   
   // 1. Reduce to root = 0
   Collective *reduce = new ReduceLinearOpenMPI();
   TauLopCost *cost_red = reduce->evaluate(world, size, root, OpType::SUM);
   
   // 2. Broadcast from 0 to P processes
   Collective *bcast = new BcastLinear();
   TauLopCost *cost_cast = bcast->evaluate(world, size, root);
   
   // Sum models
   cost_cast->add(cost_red);
   
   delete world;
   
   return cost_cast;
}

