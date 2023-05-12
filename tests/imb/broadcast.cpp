//
//  broadcast linear
//
//  Created by jarico on 23/05/11.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "benchmark.h"
#include "taulop_kernel.hpp"

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;



Results_t bcast_linear (const Arguments_t &args) {
   
   Communicator *world = new Communicator (args.P);
   
   Mapping *map = nullptr;
   if (args.mapping == Map::User) {
      int* v = (int *)&args.map_user[0]; // Is this safe?
      map = new Mapping (args.P, v);
   } else {
      map = new Mapping (args.P, args.Q, args.mapping);
   }
   world->map(map);
      
   Collective *bcast = new BcastLinear();
   CollParams cp {args.m, args.root};
   
   TauLopCost *tc = bcast->evaluate(world, cp);
      
   double lat = (tc->getTime() * 1000000); // Latency in usec, as in IMB (divided by 2.0 ???)
   double bw  = args.m / lat;
   
   delete tc;
   delete world;

   return {args.m, 1, lat, bw};
}



Results_t bcast_binomial (const Arguments_t &args) {
   
   Communicator *world = new Communicator (args.P);
   
   Mapping *map = nullptr;
   if (args.mapping == Map::User) {
      int* v = (int *)&args.map_user[0]; // Is this safe?
      map = new Mapping (args.P, v);
   } else {
      map = new Mapping (args.P, args.Q, args.mapping);
   }
   world->map(map);
      
   Collective *bcast = new BcastBinomial();
   CollParams cp {args.m, args.root};
   
   TauLopCost *tc = bcast->evaluate(world, cp);
      
   double lat = (tc->getTime() * 1000000); // Latency in usec, as in IMB (divided by 2.0 ???)
   double bw  = args.m / lat;
   
   delete tc;
   delete world;

   return {args.m, 1, lat, bw};
}


Results_t bcast_binomial_ompi (const Arguments_t &args) {
   
   Communicator *world = new Communicator (args.P);
   
   Mapping *map = nullptr;
   if (args.mapping == Map::User) {
      int* v = (int *)&args.map_user[0]; // Is this safe?
      map = new Mapping (args.P, v);
   } else {
      map = new Mapping (args.P, args.Q, args.mapping);
   }
   world->map(map);
      
   Collective *bcast = new BcastBinomialOpenMPI();
   CollParams cp {args.m, args.root};
   
   TauLopCost *tc = bcast->evaluate(world, cp);
      
   double lat = (tc->getTime() * 1000000); // Latency in usec, as in IMB (divided by 2.0 ???)
   double bw  = args.m / lat;
   
   delete tc;
   delete world;

   return {args.m, 1, lat, bw};
}
