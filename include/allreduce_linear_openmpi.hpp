//
//  allreduce_linear_openmpi.hpp
//  Implementation of a Linear AllReduce
//
//  Created by jarico on 14/04/2023
//    Modified by jarico on 20/04/23: include CollParams.
//
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef allreduce_linear_openmpi_hpp
#define allreduce_linear_openmpi_hpp

#include "coll_params.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"
#include "computation.hpp"

#include <iostream>



/*  Linear AllReduce.
 
 It models the Linea AllReduce algorithm implemented in Open MPI 4.1.5.
 
 Just a Linear Reduce plus a Linear Broadcast.
  
 */

class AllreduceLinearOpenMPI : public Collective {
   
private:
   
public:
   
    AllreduceLinearOpenMPI ();
   ~AllreduceLinearOpenMPI ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Collective parameters, including:
   //      - (m)  Size of the message (vector of one element).
   //      - (op) Operation to apply
   TauLopCost * evaluate (Communicator *comm, const CollParams &cparams)  override;
};


#endif /* allreduce_linear_openmpi_hpp */
