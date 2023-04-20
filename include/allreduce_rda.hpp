//
//  allrduce_rda.hpp
//  Implementation of a Recursive Doubling Allreduce algorithm
//
//  Created by jarico on 17/04/23.
//    Modified by jarico on 20/04/23: include CollParams.
//
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef allrduce_rda_hpp
#define allrduce_rda_hpp

#include "coll_params.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"

#include <iostream>


/*  Allrduce RDA
 
 Algorithm implemented in Open MPI 4.1.5.
 
 1.  It fills the concurrent/sequence structure and calls the kernel. It means that the
 overlapping between stages is considered correctly.
 
 2.  Number of processes must be a power of two.
 
 3. In every stage, processes compute the operation on the receiveed buffer (pairwise exchange)
 
 */

class AllreduceRDA : public Collective {
   
private:
   
public:
   
    AllreduceRDA  ();
   ~AllreduceRDA ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Collective parameters, including:
   //      - (m)  Size of the message (vector of one element).
   //      - (op) Operation to apply to the buffer.
   TauLopCost * evaluate (Communicator *comm, const CollParams &cparams) override;
};


#endif /* allrduce_rda_hpp */
