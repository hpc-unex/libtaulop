//
//  alltoall_basic_hpp
//  Basic IN PLACE algorithm for the Alltoall collective
//  AlltoAll basic IN_PLACE cost of the algorithm implemented in Open MPI 3.1 coll/tuned component
//
//  Created by jarico on 1/Dic/18.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef alltoall_basic_hpp
#define alltoall_basic_hpp

#include "collective.hpp"
#include "communicator.hpp"

#include <iostream>


/*  Alltoall basic:
 
 1.  It fills the concurrent/sequence structure and calls the kernel. It means that the
 overlapping between stages is considered correctly.
 
 2.  In this algorithm, every process sends a message to all the other ones in sequence.
 
 3.  Algorithm is that implemented in Open MPI 3.1 for IN PLACE
 
 */

class AlltoallBasic : public Collective {
   
private:
   
public:
   
    AlltoallBasic  ();
   ~AlltoallBasic  ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Size of the message (vector of one element).
   //   3. No Root for this operation is allowed. It is needed only for supporting
   //      the Collective pure virtual method.
   TauLopCost * evaluate (Communicator *comm, int *size, int root = RANK_UNDEFINED, OpType op = OpType::DEFAULT)  override;
};


#endif /* alltoall_basic_hpp */
