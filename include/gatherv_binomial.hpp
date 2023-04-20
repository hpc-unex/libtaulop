//
//  gatherv_binomial_hpp
//  Implementation of a Binomial GatherV
//
//  Created by jarico on 23/Apr/17.
//    Modified by jarico on 20/04/23: include CollParams.
//
//  Copyright © 2017 Juan A. Rico. All rights reserved.
//

#ifndef gatherv_binomial_hpp
#define gatherv_binomial_hpp

#include "coll_params.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"

#include <iostream>



/*  Binomial GatherV.
 
 Support: Any root rank and P != power of two.
 Implements the MPICH 3.1 algorithm for gather (binomial tree)
 
 TODO: Its cost is calculated by stages. It does not take into acount possible overlapping
 of stages.
 */

class GatherVBinomial : public Collective {
   
private:
   
public:
   
    GatherVBinomial ();
   ~GatherVBinomial ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Collectiva parameters, including:
   //      - (m)    Size of the message (vector of P elements with the size of process P_i in pos i).
   //      - (root) Root for this operation.
   TauLopCost * evaluate (Communicator *comm, const CollParams &cparams)  override;
};


#endif /* gatherv_binomial_hpp */
