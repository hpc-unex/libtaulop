//
//  allreduce_nonoverlap_openmpi.hpp
//  Implementation of a NonOverlap AllReduce
//
//  Created by jarico on 14/04/2023
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef allreduce_nonoverlap_openmpi_hpp
#define allreduce_nonoverlap_openmpi_hpp

#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"
#include "computation.hpp"

#include <iostream>



/*  NonOverlapping AllReduce.
 
 It models the NonOverlapping AllReduce algorithm implemented in Open MPI 4.1.5.
 
 Just a Reduce plus a Broadcast. Both Reduce and Broadcast are executed using
 several algorithms. By now, this implementation models binomial algorithms for
 both collectives.
  
 */

class AllReduceNonOverlapOpenMPI : public Collective {
   
private:
   
public:
   
    AllReduceNonOverlapOpenMPI ();
   ~AllReduceNonOverlapOpenMPI ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Size of the message (vector of one element).
   //   3. Root of the operation (not used. Set to 0).
   //   4. Operation to apply
   TauLopCost * evaluate (Communicator *comm, int *size, int root, OpType op) override;
};


#endif /* allreduce_nonoverlap_openmpi_hpp */
