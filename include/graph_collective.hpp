//
//  graph_collective.hpp
//  Implementation of a Collective based on a Graph
//
//  Created by jarico on 16/Sep/20.
//  Copyright © 2020 Juan A. Rico. All rights reserved.
//

#ifndef graph_collective_hpp
#define graph_collective_hpp

#include "graph.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_cost.hpp"

#include <iostream>


/*  Graph based Collective.
 
 */

class GraphCollective : public Collective {
   
private:
   
public:
   
    GraphCollective ();
   ~GraphCollective ();
   
   // Set a graph representing transmission in the collective.
   void  setGraph (Graph &g);
   
   // Parameters:
   //   1. Communicator (including a mapping) of the processes to execute the
   //      collective operation.
   //   2. Size of the message (vector of one element).
   //   3. Root of the operation.
   TauLopCost * evaluate (Communicator *comm, int *size, int root, OpType op = OpType::DEFAULT) override;
};


#endif /* graph_collective_hpp */
