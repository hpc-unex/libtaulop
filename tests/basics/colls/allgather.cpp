//
//  main.cpp
//  allgather algorithms
//
//  Created by jarico on 20/4/17.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "taulop_kernel.hpp"
#include "args.h"

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;


// Allgather algorithms

double rda_allgather (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.M, args.Q, args.mapping);
   world->map(map);

   Collective *rda  = new AllgatherRDA();
   CollParams cp {args.m};
   
   TauLopCost *tc = rda->evaluate(world, cp);
   double t = tc->getTime();
   
   if (args.verbose) {
      world->show();
      tc->show();
   }
   
   delete tc;
   delete world;
   
   return t;
}


double ring_allgather (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.M, args.Q, args.mapping);
   world->map(map);

   Collective *ring  = new AllgatherRing();
   CollParams cp {args.m};
   
   TauLopCost *tc = ring->evaluate(world, cp);
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
   t = rda_allgather(args);
   args.printTime(t, "Allgather RDA:  ");

   t = ring_allgather(args);
   args.printTime(t, "Allgather Ring: ");

   
   return 0;
}



