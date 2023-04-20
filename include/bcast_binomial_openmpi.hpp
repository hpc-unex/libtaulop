//
//  bcast_binomial_hpp
//  Implementation of a Binomial Broadcast
//
//  Created by jarico on 17/Nov/16.
//    Modified by jarico on 20/04/23: include CollParams.
//
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef bcast_binomial_openmpi_hpp
#define bcast_binomial_openmpi_hpp

#include "coll_params.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"

#include <iostream>



/*  Binomial Broadcast Open MPI.
 
 1.  TODO: Its cost is calculated by stages. It does not take into acount possible overlapping
 of stages.
 
 2.  Follow the Open MPI algorithm (p -> p + 2^stage)
 
 3.  Root can be any process rank.
 
 4.  REQUIRED (TBD): P = power of 2.
 
 */

class BcastBinomialOpenMPI : public Collective {
   
private:
   
public:
   
    BcastBinomialOpenMPI ();
   ~BcastBinomialOpenMPI ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. CollParams, including:
   //       - (m)    Size of the message (vector of one element).
   //       - (root) Root of the operation.
   TauLopCost * evaluate (Communicator *comm, const CollParams &cparams)  override;
};


#endif /* bcast_binomial_openmpi_hpp */
