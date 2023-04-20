//
//  bcast_linear.hpp
//  Implementation of a Linear Broadcast
//
//  Created by jarico on 2/May/2017
//    Modified by jarico on 20/04/23: include CollParams.
//
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef bcast_linear_hpp
#define bcast_linear_hpp

#include "coll_params.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"

#include <iostream>



/*  Linear Broadcast.
 
 Star pattern. Root sends to all the other processes in turns
 
 */

class BcastLinear : public Collective {
   
private:
   
public:
   
    BcastLinear ();
   ~BcastLinear ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Collective Parameters, including:
   //      - (m)    Size of the message (vector of one element).
   //      - (root) Root of the operation.
   TauLopCost * evaluate (Communicator *comm, const CollParams &cparams)  override;
};


#endif /* bcast_linear_hpp */
