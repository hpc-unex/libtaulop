//
//  gatherv_binomial_hpp
//  Implementation of a Binomial GatherV
//
//  Created by jarico on 23/Apr/17.
//  Copyright © 2017 Juan A. Rico. All rights reserved.
//

#ifndef gatherv_binomial_hpp
#define gatherv_binomial_hpp

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
    //   2. Size of the message (vector of P elements with the size of process P_i in pos i).
    //   3. Root for this operation.
    virtual  TauLopCost * evaluate (Communicator *comm, int *size, int root);
};


#endif /* gatherv_binomial_hpp */
