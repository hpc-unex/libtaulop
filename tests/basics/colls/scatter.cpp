//
//  main.cpp
//  test_scatter
//
//  Created by jarico on 19/04/23.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "taulop_kernel.hpp"
#include "args.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;


// Scatter algorithms

double scatter (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.M, args.Q, args.mapping);
   world->map(map);

   Collective *scatter = new ScatterBinomial();
   CollParams cp {args.m, args.root};
   
   TauLopCost *tc = scatter->evaluate(world, cp);
   double t = tc->getTime();
 
   if (args.verbose) {
      world->show();
      tc->show();
   }
   delete tc;
   delete world;
   
   return t;
}


double scatter_v (const TaulopArgs &args, int *m) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.M, args.Q, args.mapping);
   world->map(map);

   Collective *scatter = new ScatterBinomial();
   
   vector<int> vm {m, m + args.P}; // initial and end elements
   CollParams cp {vm, args.root};
   
   TauLopCost *tc = scatter->evaluate(world, cp);
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

   
   // Binomial broadcast
   t = scatter (args);
   args.printTime(t, "Binomial Scatter: ");

   // Binomial broadcast _V 
   int *mptr = new int [args.P];
   for (int i = 0; i < args.P; i++) mptr[i] = args.m;
   t = scatter_v(args, mptr);
   args.printTime(t, "Scatter _V:       ");
   delete [] mptr;

   return 0;
}


