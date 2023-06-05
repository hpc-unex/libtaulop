//
//  reduce.cpp
//  Test reduce collective operations
//
//  Created by jarico on 09/05/23.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "taulop_kernel.hpp"
#include "benchmark.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <unistd.h>
#include <getopt.h>
using namespace std;


// Reduce algorithms

Results_t reduce_linear (const Arguments_t &args) {
   
   Communicator *world = new Communicator (args.P);

   Mapping *map = nullptr;
   if (args.mapping == Map::User) {
      int* v = (int *)&args.map_user[0]; // Is this safe?
      map = new Mapping (args.P, v);
   } else {
      map = new Mapping (args.P, args.Q, args.mapping);
   }
   world->map(map);
      
   Collective *reduce  = new ReduceLinearOpenMPI();
   CollParams cp {args.m, args.root, args.optype};
   
   TauLopCost *tc = reduce->evaluate(world, cp);
   
   double lat = tc->getTime();
   double bw  = args.m / lat;

   delete tc;
   delete world;

   return {args.m, 1, lat, bw};
}



Results_t reduce_binomial_ompi (const Arguments_t &args) {
   
   Communicator *world = new Communicator (args.P);

   Mapping *map = nullptr;
   if (args.mapping == Map::User) {
      int* v = (int *)&args.map_user[0]; // Is this safe?
      map = new Mapping (args.P, v);
   } else {
      map = new Mapping (args.P, args.Q, args.mapping);
   }
   world->map(map);
      
   Collective *reduce  = new ReduceBinomialOpenMPI();
   CollParams cp {args.m, args.root, args.optype};
   
   TauLopCost *tc = reduce->evaluate(world, cp);
   
   double lat = tc->getTime();
   double bw  = args.m / lat;

   delete tc;
   delete world;

   return {args.m, 1, lat, bw};
}


