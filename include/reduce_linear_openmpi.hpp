//
//  reduce_linear_openmpi.hpp
//  Implementation of a Linear Reduce
//
//  Created by jarico on 27/Ma/2023
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef reduce_linear_openmpi_hpp
#define reduce_linear_openmpi_hpp

#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"
#include "computation.hpp"

#include <iostream>



/*  Linear Reduce.
 
 Star pattern. Root receives from all the other processes in turns and compute.
 The algorithm is implemented in Open MPI 4.1.5. All processes send messages
 (concurrently) to the root process, which computes buffer in inverse order
 of rank (from P-1 to 0).
 
 TODO: Not sure that this model match the implementation:
 
 1. Simplest model is the accumulative. All processes send in order (P-1 to 0) to the root, and
    root receives and operates each buffer. This is a: O(P,m) = SUM_p=0^P-1 [T^c(m) + g^op(m)]
 
 2. An alternative model is to consider that all sends are concurrent and process root takes
    messages and operates in order (P-1 to 0). Hence: O(P, m) = ||_p=0^P-1 [T^c(m) + g^op(m)]
 
 Note that, in case of long messages (> H), rendezvous protocol has an influence in the sends,
 that should be in sequence (P-1 to 0).
 
 BY NOW: it is implementes the model (1) for all message sizes.
 
 */

class ReduceLinearOpenMPI : public Collective {
   
private:
   
public:
   
    ReduceLinearOpenMPI ();
   ~ReduceLinearOpenMPI ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Size of the message (vector of one element).
   //   3. Root of the operation.
   TauLopCost * evaluate (Communicator *comm, int *size, int root, OpType op) override;
};


#endif /* reduce_linear_openmpi_hpp */
