//
//  allreduce_rabenseifner.hpp
//  Implementation of a Ring Allreduce algorithm
//
//  Created by jarico on 17/04/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef allreduce_rabenseifner_hpp
#define allreduce_rabenseifner_hpp

#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"

#include <iostream>


/*  Allreduce Rabenseifner
 
 1.  TODO: a direct optimization is to calculate one stage cost and multiply by P-1.
 
 */

class AllreduceRabenseifner : public Collective {
   
private:
   
public:
   
    AllreduceRabenseifner ();
   ~AllreduceRabenseifner ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Size of the message (vector of one element).
   //   3. No Root for this operation is allowed. It is needed only for supporting
   //      the Collective pure virtual method.
   //   4. Operation to apply.
   TauLopCost * evaluate (Communicator *comm, int *size, int root = RANK_UNDEFINED, OpType op = OpType::DEFAULT) override;
};


#endif /* allreduce_rabenseifner_hpp */
