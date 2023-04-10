//
//  scatter_binomial_hpp
//  Implementation of a Binomial Scatter
//
//  Created by jarico on 21/Apr/17.
//  Copyright © 2017 Juan A. Rico. All rights reserved.
//

#ifndef scatter_binomial_hpp
#define scatter_binomial_hpp

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
   
   TauLopCost * evaluate (Communicator *comm, int *size, int root, OpType op = OpType::DEFAULT) override;
};


#endif /* scatter_binomial_hpp */
