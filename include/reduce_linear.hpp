//
//  reduce_linear.hpp
//  Implementation of a Linear Reduce
//
//  Created by jarico on 27/Ma/2023
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef reduce_linear_hpp
#define reduce_linear_hpp

#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"
#include "computation.hpp"

#include <iostream>



/*  Linear Reduce.
 
 Star pattern. Root receives from all the other processes in turns and compute.
 The algorithm is implemented in Ope MPI 4.1.5. All processes send messages
 (concurrently) to the root process, which computes buffer in inverse order
 of rank (from P-1 to 0).
 
 */

class ReduceLinear : public Collective {
   
private:
   
public:
   
    ReduceLinear ();
   ~ReduceLinear ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Size of the message (vector of one element).
   //   3. Root of the operation.
   TauLopCost * evaluate (Communicator *comm, int *size, int root, OpType op) override;
};


#endif /* reduce_linear_hpp */
