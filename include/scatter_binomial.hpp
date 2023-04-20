//
//  scatter_binomial_hpp
//  Implementation of a Binomial Scatter
//
//  Created by jarico on 21/Apr/17.
//    Modified by jarico on 20/04/23: include CollParams.
//
//  Copyright © 2017 Juan A. Rico. All rights reserved.
//

#ifndef scatter_binomial_hpp
#define scatter_binomial_hpp

#include "coll_params.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"

#include <iostream>



/*  Binomial Scatter.
 
 1.  TODO: Its cost is calculated by stages. It does not take into acount possible overlapping
 of stages.
 
 2.  Follow the MPICH algorithm (p -> p + 2^stage)
 
 3.  Root can be any process rank.
 
 4.  REQUIRED (TBD): P = power of 2.
 
 */

class ScatterBinomial : public Collective {
   
private:
   
public:
   
    ScatterBinomial ();
   ~ScatterBinomial ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Collective Parameters, including:
   //      - (m)    Size of the message (vector of one element).
   //      - (root) Root of the operation.
   TauLopCost * evaluate (Communicator *comm, const CollParams &cparams)  override;
};


#endif /* scatter_binomial_hpp */
