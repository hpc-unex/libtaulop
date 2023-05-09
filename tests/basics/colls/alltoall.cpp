//
//  main.cpp
//    alltoall
//
//  Created by jarico on 09/05/23.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "taulop_kernel.hpp"
#include "args.h"

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;


// AlltoAll algorithms

double basic_alltoall (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.Q, args.mapping);
   world->map(map);

   Collective *alltoall  = new AlltoallBasic();
   CollParams cp {args.m};
   
   TauLopCost *tc = alltoall->evaluate(world, cp);
   double t = tc->getTime();
   
   if (args.verbose) {
      world->show();
      tc->show();
   }
   
   delete tc;
   delete world;
   
   return t;
}


double linear_alltoall (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.Q, args.mapping);
   world->map(map);

   Collective *alltoall  = new AlltoallLinear();
   CollParams cp {args.m};
   
   TauLopCost *tc = alltoall->evaluate(world, cp);
   double t = tc->getTime();
   
   if (args.verbose) {
      world->show();
      tc->show();
   }
   
   delete tc;
   delete world;
   
   return t;
}


double pairwise_alltoall (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.Q, args.mapping);
   world->map(map);

   Collective *alltoall  = new AlltoallPairwise();
   CollParams cp {args.m};
   
   TauLopCost *tc = alltoall->evaluate(world, cp);
   double t = tc->getTime();
   
   if (args.verbose) {
      world->show();
      tc->show();
   }
   
   delete tc;
   delete world;
   
   return t;
}



int main (int argc, char *argv[]) {
   
   double t = 0.0;
   
   srand(time(NULL));
   
   // Network parameters
   TauLopParam::setInstance({"SHM","IB"});
   
   // Params
   TaulopArgs &args = TaulopArgs::GetArgs(argc, argv);
      
   
   // Comparison of allgather algorithms
   t = basic_alltoall(args);
   args.printTime(t, "Alltoall Basic:    ");

   t = linear_alltoall(args);
   args.printTime(t, "Alltoall Linear:   ");

   t = pairwise_alltoall(args);
   args.printTime(t, "Alltoall Pairwise: ");

   return 0;
}



