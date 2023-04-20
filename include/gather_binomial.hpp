//
//  gather_binomial_hpp
//  Implementation of a Binomial Gather
//
//  Created by jarico on 20/Apr/17.
//    Modified by jarico on 20/04/23: include CollParams.
//
//  Copyright © 2017 Juan A. Rico. All rights reserved.
//

#ifndef gather_binomial_hpp
#define gather_binomial_hpp

#include "coll_params.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"

#include <iostream>



/*  Binomial Gather.
 
 Support: Any root rank and P != power of two.
 Implements the MPICH 3.1 algorithm for gather (binomial tree)
 
 TODO: Its cost is calculated by stages. It does not take into acount possible overlapping
 between stages.
 
 TODO: Test if working for root != 0 and P != power of two
 */

class GatherBinomial : public Collective {
   
private:
   
public:
   
    GatherBinomial ();
   ~GatherBinomial ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Collective Parameters, including:
   //      - (m)    Size of the message.
   //      - (root) Root for this operation.
   TauLopCost * evaluate (Communicator *comm, const CollParams &cparams)  override;
};


#endif /* gather_binomial_hpp */
