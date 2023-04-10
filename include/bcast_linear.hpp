//
//  bcast_linear.hpp
//  Implementation of a Linear Broadcast
//
//  Created by jarico on 2/May/2017
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef bcast_linear_hpp
#define bcast_linear_hpp

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
    //   2. Size of the message (vector of one element).
    //   3. Root of the operation.
    TauLopCost * evaluate (Communicator *comm, int *size, int root, OpType op = OpType::DEFAULT) override;
};


#endif /* bcast_linear_hpp */
