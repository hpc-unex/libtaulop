//
//  reduce.cpp
//  Test reduce collective operations
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
#include <unistd.h>
#include <getopt.h>
using namespace std;


// Reduce algorithms

double reduce_linear (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.M, args.Q, args.mapping);
   world->map(map);
      
   Collective *reduce  = new ReduceLinearOpenMPI();
   CollParams cp {args.m, args.root, args.op};
   
   TauLopCost *tc = reduce->evaluate(world, cp);
   
   if (args.verbose) {
      world->show();
      tc->show();
   }
   
   double t = tc->getTime();
   
   delete tc;
   delete world;
   
   return t;
}



double reduce_binomial (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.M, args.Q, args.mapping);
   world->map(map);

   Collective *reduce  = new ReduceBinomialOpenMPI();
   CollParams cp {args.m, args.root, args.op};
   
   TauLopCost *tc = reduce->evaluate(world, cp);
      
   if (args.verbose) {
      world->show();
      tc->show();
   }
   
   double t = tc->getTime();

   delete tc;
   delete world;
   
   return t;
}



int main (int argc, char * argv[]) {
   
   double t = 0.0;
   
   srand(time(NULL));
   
   // Network parameters
   TauLopParam::setInstance({"SHM","IB"});
   
   // Params
   TaulopArgs &args = TaulopArgs::GetArgs(argc, argv);
      
   // Linear Reduce
   t = reduce_linear(args);
   args.printTime(t, "Reduce Linear    ");

   // Binomial Reduce
   t = reduce_binomial(args);
   args.printTime(t, "Reduce Binomial  ");

   return 0;
}



