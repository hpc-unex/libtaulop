//
//  reduce.cpp
//  Test Allreduce collective operations
//
//  Created by jarico on 12/05/23.
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


// AllReduce algorithms

Results_t allreduce_linear_ompi (const Arguments_t &args) {
   
   Communicator *world = new Communicator (args.P);

   Mapping *map = nullptr;
   if (args.mapping == Map::User) {
      int* v = (int *)&args.map_user[0]; // Is this safe?
      map = new Mapping (args.P, v);
   } else {
      map = new Mapping (args.P, args.M, args.Q, args.mapping);
   }
   world->map(map);
      
   Collective *allred  = new AllreduceLinearOpenMPI();
   CollParams cp {args.m, args.root, args.optype};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   
   double lat = tc->getTime();
   double bw  = args.m / lat;
   
   delete tc;
   delete world;

   return {args.m, 1, lat, bw};
}



Results_t allreduce_nonoverlap_ompi (const Arguments_t &args) {
   
   Communicator *world = new Communicator (args.P);

   Mapping *map = nullptr;
   if (args.mapping == Map::User) {
      int* v = (int *)&args.map_user[0]; // Is this safe?
      map = new Mapping (args.P, v);
   } else {
      map = new Mapping (args.P, args.M, args.Q, args.mapping);
   }
   world->map(map);
      
   Collective *allred  = new AllreduceNonOverlapOpenMPI();
   CollParams cp {args.m, args.root, args.optype};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   
   double lat = tc->getTime();
   double bw  = args.m / lat;

   delete tc;
   delete world;

   return {args.m, 1, lat, bw};
}



Results_t allreduce_rda_ompi (const Arguments_t &args) {
   
   Communicator *world = new Communicator (args.P);

   Mapping *map = nullptr;
   if (args.mapping == Map::User) {
      int* v = (int *)&args.map_user[0]; // Is this safe?
      map = new Mapping (args.P, v);
   } else {
      map = new Mapping (args.P, args.M, args.Q, args.mapping);
   }
   world->map(map);
      
   Collective *allred  = new AllreduceRDA();
   CollParams cp {args.m, args.root, args.optype};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   
   double lat = tc->getTime();
   double bw  = args.m / lat;

   delete tc;
   delete world;

   return {args.m, 1, lat, bw};
}



Results_t allreduce_ring_ompi (const Arguments_t &args) {
   
   Communicator *world = new Communicator (args.P);

   Mapping *map = nullptr;
   if (args.mapping == Map::User) {
      int* v = (int *)&args.map_user[0]; // Is this safe?
      map = new Mapping (args.P, v);
   } else {
      map = new Mapping (args.P, args.M, args.Q, args.mapping);
   }
   world->map(map);
      
   Collective *allred  = new AllreduceRing();
   CollParams cp {args.m, args.root, args.optype};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   
   double lat = tc->getTime();
   double bw  = args.m / lat;

   delete tc;
   delete world;

   return {args.m, 1, lat, bw};
}



Results_t allreduce_ringsegm_ompi (const Arguments_t &args) {
   
   Communicator *world = new Communicator (args.P);

   Mapping *map = nullptr;
   if (args.mapping == Map::User) {
      int* v = (int *)&args.map_user[0]; // Is this safe?
      map = new Mapping (args.P, v);
   } else {
      map = new Mapping (args.P, args.M, args.Q, args.mapping);
   }
   world->map(map);
      
   Collective *allred  = new AllreduceRingSegm();
   CollParams cp {args.m, args.root, args.optype};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   
   double lat = tc->getTime();
   double bw  = args.m / lat;

   delete tc;
   delete world;

   return {args.m, 1, lat, bw};
}



Results_t allreduce_rabenseifner_ompi (const Arguments_t &args) {
   
   Communicator *world = new Communicator (args.P);

   Mapping *map = nullptr;
   if (args.mapping == Map::User) {
      int* v = (int *)&args.map_user[0]; // Is this safe?
      map = new Mapping (args.P, v);
   } else {
      map = new Mapping (args.P, args.M, args.Q, args.mapping);
   }
   world->map(map);
      
   Collective *allred  = new AllreduceRabenseifner();
   CollParams cp {args.m, args.root, args.optype};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   
   double lat = tc->getTime();
   double bw  = args.m / lat;

   delete tc;
   delete world;

   return {args.m, 1, lat, bw};
}
