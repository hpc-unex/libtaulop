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
#include <getopt.h>
#include <unistd.h>

using namespace std;


// Allreduce algorithms

double allreduce_linear (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.Q, args.mapping);
   world->map(map);

   Collective *allred  = new AllreduceLinearOpenMPI();
   CollParams cp {args.m, args.op};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   double t = tc->getTime();
   
   if (args.verbose) {
      world->show();
      tc->show();
   }
   delete tc;
   delete world;
   
   return t;
}



double allreduce_nono (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.Q, args.mapping);
   world->map(map);

   Collective *allred  = new AllreduceNonOverlapOpenMPI();
   CollParams cp {args.m, args.op};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   double t = tc->getTime();
   
   if (args.verbose) {
      world->show();
      tc->show();
   }
   delete tc;
   delete world;
   
   return t;
}



double allreduce_rda (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.Q, args.mapping);
   world->map(map);

   Collective *allred  = new AllreduceRDA();
   CollParams cp {args.m, args.op};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   double t = tc->getTime();
   
   if (args.verbose) {
      world->show();
      tc->show();
   }
   delete tc;
   delete world;
   
   return t;
}



double allreduce_ring (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.Q, args.mapping);
   world->map(map);

   Collective *allred  = new AllreduceRing();
   CollParams cp {args.m, args.op};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   double t = tc->getTime();
   
   if (args.verbose) {
      world->show();
      tc->show();
   }
   delete tc;
   delete world;
   
   return t;
}



double allreduce_ring_segm (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.Q, args.mapping);
   world->map(map);

   Collective *allred  = new AllreduceRingSegm();
   CollParams cp {args.m, args.op};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   double t = tc->getTime();
   
   if (args.verbose) {
      world->show();
      tc->show();
   }
   delete tc;
   delete world;
   
   return t;
}



double allreduce_rabenseifner (const TaulopArgs &args) {
   
   Communicator *world = new Communicator (args.P);
   Mapping *map = new Mapping (args.P, args.Q, args.mapping);
   world->map(map);

   Collective *allred  = new AllreduceRabenseifner();
   CollParams cp {args.m, args.op};
   
   TauLopCost *tc = allred->evaluate(world, cp);
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

   
   // linear allreduce
   t = allreduce_linear(args);
   args.printTime(t, "Allreduce Linear          ");

   // Non Overlapping allreduce
   t = allreduce_nono(args);
   args.printTime(t, "Allreduce Non Overlapping ");

   // RDA allreduce
   t = allreduce_rda(args);
   args.printTime(t, "Allreduce RDA             ");

   // Ring allreduce
   t = allreduce_ring(args);
   args.printTime(t, "Allreduce Ring            ");

   // Ring allreduce with segmentation
   t = allreduce_ring_segm(args);
   args.printTime(t, "Allreduce Ring Segmented  ");

   // Rabenseifner's allreduce
   t = allreduce_rabenseifner(args);
   args.printTime(t, "Allreduce Rabenseifner    ");

   return 0;
}



