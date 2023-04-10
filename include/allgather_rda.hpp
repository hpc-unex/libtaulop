//
//  allgather_rda_hpp
//  Implementation of a Recursive Doubling Allgather algorithm
//
//  Created by jarico on 17/Nov/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef allgather_rda_hpp
#define allgather_rda_hpp

#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"

#include <iostream>


/*  Allgather RDA
 
 1.  It fills the concurrent/sequence structure and calls the kernel. It means that the
 overlapping between stages is considered correctly.
 
 2.  Number of processes must be a power of two.
 
 */

class AllgatherRDA : public Collective {
   
private:
   
public:
   
    AllgatherRDA  ();
   ~AllgatherRDA ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Size of the message (vector of one element).
   //   3. No Root for this operation is allowed. It is needed only for supporting
   //      the Collective pure virtual method.
   TauLopCost * evaluate (Communicator *comm, int *size, int root = RANK_UNDEFINED, OpType op = OpType::DEFAULT) override;
};


#endif /* allgather_rda_hpp */
