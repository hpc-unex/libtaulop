//
//  gather_binomial_hpp
//  Implementation of a Binomial Gather
//
//  Created by jarico on 20/Apr/17.
//  Copyright © 2017 Juan A. Rico. All rights reserved.
//

#ifndef gather_binomial_hpp
#define gather_binomial_hpp

#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"

#include <iostream>



/*  Binomial Gather.
 
 Support: Any root rank and P != power of two.
 Implements the MPICH 3.1 algorithm for gather (binomial tree)
 
 TODO: Its cost is calculated by stages. It does not take into acount possible overlapping
 between stages.
 */

class GatherBinomial : public Collective {
   
private:
   
public:
   
    GatherBinomial ();
   ~GatherBinomial ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Size of the message (vector of one element).
   //   3. Root for this operation.
   TauLopCost * evaluate (Communicator *comm, int *size, int root, OpType op = OpType::DEFAULT) override;
};


#endif /* gather_binomial_hpp */
