//
//  reduce_binomial_openmpi.hpp
//  Implementation of a Binomial Reduce
//
//  Created by jarico on 11/04/2023
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef reduce_binomial_openmpi_hpp
#define reduce_binomial_openmpi_hpp

#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"
#include "computation.hpp"

#include <iostream>



/*  Binomial Reduce.
 
 Binomial tree pattern.
 The algorithm is implemented in Open MPI 4.1.5 base colls.
 
 */

class ReduceBinomialOpenMPI : public Collective {
   
private:
   
public:
   
    ReduceBinomialOpenMPI ();
   ~ReduceBinomialOpenMPI ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Size of the message (vector of one element).
   //   3. Root of the operation.
   //   4. Operation to apply.
   TauLopCost * evaluate (Communicator *comm, int *size, int root, OpType op) override;
};


#endif /* reduce_binomial_openmpi_hpp */
