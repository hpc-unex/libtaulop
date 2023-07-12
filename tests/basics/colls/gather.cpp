//
//  main.cpp
//  test_colls
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


// Gather algorithms

double binomial_gather (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.M, args.Q, args.mapping);
   world->map(map);

   Collective *gather  = new GatherBinomial();
   CollParams cp {args.m, args.root};
   
   TauLopCost *tc = gather->evaluate(world, cp);
   double t = tc->getTime();
   
   if (args.verbose) {
      world->show();
      tc->show();
   }
   
   delete tc;
   delete world;
   
   return t;
}


double binomial_v_gather (const TaulopArgs &args, int *m) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.M, args.Q, args.mapping);
   world->map(map);

   Collective *gatherv = new GatherVBinomial();
   
   vector<int> vm {m, m + args.P}; // initial and end elements
   CollParams cp {vm, args.root};
   
   TauLopCost *tc = gatherv->evaluate(world, cp);
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
      

   // Gather
   t = binomial_gather(args);
   args.printTime(t, "Gather Binomial:    ");

   
   // Gather V
   int *mptr = new int [args.P];
   for (int i = 0; i < args.P; i++) mptr[i] = args.m;

   t = binomial_v_gather(args, mptr);
   args.printTime(t, "Gather V Binomial:  ");
   
   delete [] mptr;

   
   return 0;
}



