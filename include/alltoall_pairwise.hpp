//
//  alltoall_pairwise_hpp
//  Implementation of pairwise algorithm for alltoall collective in Open MPI 3.1 (tuned component)
//
//  Created by jarico on 1/Dic/18.
//    Modified by jarico on 20/04/23: include CollParams.
//
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef alltoall_pairwise_hpp
#define alltoall_pairwise_hpp

#include "coll_params.hpp"
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
   //   2. Collective parameters, including:
   //      - (m) Size of the message (vector of one element).
   TauLopCost * evaluate (Communicator *comm, const CollParams &cparams) override;
};


#endif /* alltoall_pairwise_hpp */
