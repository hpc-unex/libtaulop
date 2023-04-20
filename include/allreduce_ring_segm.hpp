//
//  allreduce_ring_segm.hpp
//  Implementation of a Ring Allreduce algorithm with segmentation
//
//  Created by jarico on 17/04/23.
//    Modified by jarico on 20/04/23: include CollParams.
//
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef allreduce_ring_segm_hpp
#define allreduce_ring_segm_hpp

#include "coll_params.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"

#include <iostream>


/*  Allreduce Ring segmented
 
 1.  TODO: a direct optimization is to calculate one stage cost and multiply by P-1.
 
 */

class AllreduceRingSegm : public Collective {
   
private:
   
public:
   
    AllreduceRingSegm ();
   ~AllreduceRingSegm ();
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Collective Parameters, including:
   //      - (m)  Size of the message (vector of one element).
   //      - (op) Operation to apply.
   TauLopCost * evaluate (Communicator *comm, const CollParams &cparams)  override;
};


#endif /* allreduce_ring_segm_hpp */
