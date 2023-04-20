//
//  reduce_binomial_openmpi.hpp
//  Implementation of a Binomial Reduce
//
//  Created by jarico on 11/04/2023
//    Modified by jarico on 20/04/23: include CollParams.
//
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef reduce_binomial_openmpi_hpp
#define reduce_binomial_openmpi_hpp

#include "coll_params.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"
#include "computation.hpp"

#include <iostream>



/*  Binomial Reduce.
 
 Binomial tree pattern.
 The algorithm is implemented in Open MPI 4.1.5 base colls.
 
 Non-segmented model of the binomial reduce.
 
 */

class ReduceBinomialOpenMPI : public Collective {
   
private:
   
public:
   
    ReduceBinomialOpenMPI ();
   ~ReduceBinomialOpenMPI ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Collective parameters, including:
   //      - (m)    Size of the message (vector of one element).
   //      - (root) Root of the operation.
   //      - (op)   Operation to apply.
   TauLopCost * evaluate (Communicator *comm, const CollParams &cparams)  override;
};


#endif /* reduce_binomial_openmpi_hpp */
