//
//  main.cpp
//    broadcasts
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



double linear_bcast (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.Q, args.mapping);
   world->map(map);
   
   Collective *bcast  = new BcastLinear();
   CollParams cp {args.m, args.root};
   
   TauLopCost *tc = bcast->evaluate(world, cp);
   
   if (args.verbose) {
      world->show();
      tc->show();
   }

   double t = tc->getTime();
   
   delete tc;
   delete world;
   
   return t;
}


double binomial_bcast (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.Q, args.mapping);
   world->map(map);
   
   Collective *bcast  = new BcastBinomial();
   CollParams cp {args.m, args.root};
   
   TauLopCost *tc = bcast->evaluate(world, cp);
   
   if (args.verbose) {
      world->show();
      tc->show();
   }

   double t = tc->getTime();
   
   delete tc;
   delete map;
   delete world;
   
   return t;
}


double binomial_ompi_bcast (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.Q, args.mapping);
   world->map(map);
   
   Collective *bcast  = new BcastBinomialOpenMPI();
   CollParams cp {args.m, args.root};
   
   TauLopCost *tc = bcast->evaluate(world, cp);
   
   if (args.verbose) {
      world->show();
      tc->show();
   }

   double t = tc->getTime();
   
   delete tc;
   delete map;
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
      

   // Linear broadcast
   t = linear_bcast(args);
   args.printTime(t, "Linear Broadcast:            ");
   
   // Binomial broadcast
   t = binomial_bcast(args);
   args.printTime(t, "Binomial Broadcast MPICH2:   ");

   // Binomial broadcast Open MPI algorithm
   t = binomial_ompi_bcast(args);
   args.printTime(t, "Binomial Broadcast Open MPI: ");

   
   return 0;
}



