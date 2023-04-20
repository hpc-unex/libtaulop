//
//  alltoall_linear_hpp
//  Implementation of linear algorithm for alltoall collective in Open MPI 3.1
//
//  Created by jarico on 1/Dic/18.
//    Modified by jarico on 20/04/23: include CollParams.
//
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef alltoall_linear_hpp
#define alltoall_linear_hpp

#include "coll_params.hpp"
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
   //   2. Collective parameters, including:
   //      - (m) Size of the message (vector of one element).
   TauLopCost * evaluate (Communicator *comm, const CollParams &cparams)  override;
};


#endif /* alltoall_linear_hpp */
