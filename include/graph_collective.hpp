//
//  graph_collective.hpp
//  Implementation of a Collective based on a Graph
//
//  Created by jarico on 16/Sep/20.
//    Modified by jarico on 20/04/23: include CollParams.
//
//  Copyright © 2020 Juan A. Rico. All rights reserved.
//

#ifndef graph_collective_hpp
#define graph_collective_hpp

#include "coll_params.hpp"
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
   //   2. Collective parameters, including:
   //      - (m)    Size of the message (vector of one element).
   TauLopCost * evaluate (Communicator *comm, const CollParams &cparams)  override;
};


#endif /* graph_collective_hpp */
