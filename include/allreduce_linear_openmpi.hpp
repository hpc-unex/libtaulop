//
//  allreduce_linear_openmpi.hpp
//  Implementation of a Linear AllReduce
//
//  Created by jarico on 14/04/2023
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef allreduce_linear_openmpi_hpp
#define allreduce_linear_openmpi_hpp

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
   //   2. Size of the message (vector of one element).
   //   3. Root of the operation (not used. Set to 0).
   //   4. Operation to apply
   TauLopCost * evaluate (Communicator *comm, int *size, int root, OpType op) override;
};


#endif /* allreduce_linear_openmpi_hpp */
