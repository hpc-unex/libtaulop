//
//  allgather_ring_hpp
//  Implementation of a Ring Allgather algorithm
//
//  Created by jarico on 17/Nov/16.
//    Modified by jarico on 20/04/23: include CollParams.
//
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef allgather_ring_hpp
#define allgather_ring_hpp

#include "coll_params.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"

#include <iostream>


/*  Allgather Ring
 
 1.  TODO: a direct optimization is to calculate one stage cost and multiply by P-1.
 
 */

class AllgatherRing : public Collective {
   
private:
   
public:
   
    AllgatherRing ();
   ~AllgatherRing ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Collective Params, including:
   //      - (m)  Size of the message (vector of one element).
   TauLopCost * evaluate (Communicator *comm, const CollParams &cparams)  override;
};


#endif /* allgather_ring_hpp */
