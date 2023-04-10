//
//  alltoall_pairwise_hpp
//  Implementation of pairwise algorithm for alltoall collective in Open MPI 3.1 (tuned component)
//
//  Created by jarico on 1/Dic/18.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef alltoall_pairwise_hpp
#define alltoall_pairwise_hpp

#include "collective.hpp"
#include "communicator.hpp"

#include <iostream>


/*  Alltoall pairwise:
 
 1.  It fills the concurrent/sequence structure and calls the kernel. It means that the
       overlapping between stages is considered correctly.
 
 2.  In this algorithm, every process sends a message to all the other ones.
 
 */

class AlltoallPairwise : public Collective {
    
private:
    
public:
    
     AlltoallPairwise  ();
    ~AlltoallPairwise  ();
    
    // Parameters:
    //   1. Communicator (including a mapping) of the processes to execute the
    //      collective operation.
    //   2. Size of the message (vector of one element).
    //   3. No Root for this operation is allowed. It is needed only for supporting
    //      the Collective pure virtual method.
    virtual  TauLopCost * evaluate (Communicator *comm, int *size, int root = 0);
};


#endif /* alltoall_pairwise_hpp */
