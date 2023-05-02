//
//  main.cpp
//  communicators
//
//  Created by jarico on 20/4/17.
//  Copyright © 2017 jarico. All rights reserved.
//


#include "taulop_kernel.hpp"

#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;



// Platform parameters
const int M = 4;
const int Q = 4;
const int P = 16;




int main (int argc, const char * argv[]) {
   
   TauLopParam::setInstance({"SHM","IB"});
   
   // 1. Create a communicator (group of P processes)
   //    Default mapping is all processes in the same node.
   Communicator *world = new Communicator (P);
   
   
   // 2. Create the virtual topology (mapping of processes to nodes)
   Mapping *map = new Mapping(P, Q, MAPPING_SEQ);
   // 3.1. Assign the mapping to a communicator
   world->map(map);
   world->show();
   
   // 3. Create another communicator (derive from world), with a different mapping
   //    Mapping is specified with a vector of process Q nodes.
   int ranks[P/2];
   for (int p = 0; p < P/2; p++) {
      ranks[p] = 2 * p;
   }
   int nodes[P/2];
   for (int i = 0; i < P/2; i++) {
      nodes[i] = rand() % Q; // Random
   }
   Mapping *map_comm = new Mapping (P/2, nodes);
   Communicator *comm = world->create(P/2, ranks);
   comm->map(map_comm);
   comm->show();
   
   
   // 4. Create one more communicator (it inherits the mapping)
   int r_ranks[P/2];
   for (int p = 0; p < comm->getSize(); p++) {
      r_ranks[p] = comm->getSize() - (p + 1);
   }
   Communicator *otro = comm->create(P/2, r_ranks);
   otro->show();
   delete otro;
   
   
   // 5. Delete objects
   delete map;
   delete map_comm;
   delete comm;
   delete world;
   return 0;
}



