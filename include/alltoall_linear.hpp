//
//  alltoall_linear_hpp
//  Implementation of linear algorithm for alltoall collective in Open MPI 3.1
//
//  Created by jarico on 1/Dic/18.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef alltoall_linear_hpp
#define alltoall_linear_hpp

#include "collective.hpp"
#include "communicator.hpp"

#include <iostream>


/*  Alltoall linear:
 
 1.  It fills the concurrent/sequence structure and calls the kernel. It means that the
 overlapping between stages is considered correctly.
 
 2.  In this algorithm, every process sends a message to all the other ones in sequence.
 
 */

class AlltoallLinear : public Collective {
   
private:
   
public:
   
    AlltoallLinear  ();
   ~AlltoallLinear  ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Size of the message (vector of one element).
   //   3. No Root for this operation is allowed. It is needed only for supporting
   //      the Collective pure virtual method.
   TauLopCost * evaluate (Communicator *comm, int *size, int root = RANK_UNDEFINED, OpType op = OpType::DEFAULT) override;
};


#endif /* alltoall_linear_hpp */
