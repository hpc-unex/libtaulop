//
//  allreduce.cpp
//  Test Allreduce collective operations
//
//  Created by jarico on 08/07/23.
//  Copyright © 2023 jarico. All rights reserved.
//

#include "taulop_kernel.hpp"

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <unistd.h>
#include <getopt.h>
using namespace std;


// AllReduce algorithms

double allreduce_linear_ompi (int P, int Q, int M, int m, int ms, Map mapping, OpType op) {
   
   Communicator *world = new Communicator (P);
   Mapping *map = new Mapping (P, M, Q, mapping);
   world->map(map);
      
   Collective *allred  = new AllreduceLinearOpenMPI();
   CollParams cp {m, op};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   
   double t = tc->getTime();
   
   delete tc;
   delete world;

   return t;
}



double allreduce_nonoverlap_ompi (int P, int Q, int M, int m, int ms, Map mapping, OpType op) {
   
   Communicator *world = new Communicator (P);
   Mapping *map = new Mapping (P, M, Q, mapping);
   world->map(map);
      
   Collective *allred  = new AllreduceNonOverlapOpenMPI();
   CollParams cp {m, op};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   
   double t = tc->getTime();

   delete tc;
   delete world;

   return t;
}



double allreduce_rda_ompi (int P, int Q, int M, int m, int ms, Map mapping, OpType op) {
      
   Communicator *world = new Communicator (P);
   Mapping *map = new Mapping (P, M, Q, mapping);
   world->map(map);
      
   Collective *allred  = new AllreduceRDA();
   CollParams cp {m, op};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   
   double t = tc->getTime();

   delete tc;
   delete world;

   return t;
}



double allreduce_ring_ompi (int P, int Q, int M, int m, int ms, Map mapping, OpType op) {
      
   Communicator *world = new Communicator (P);
   Mapping *map = new Mapping (P, M, Q, mapping);
   world->map(map);
      
   Collective *allred  = new AllreduceRing();
   CollParams cp {m, op};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   
   double t = tc->getTime();

   delete tc;
   delete world;

   return t;
}



double allreduce_ringsegm_ompi (int P, int Q, int M, int m, int ms, Map mapping, OpType op) {
      
   Communicator *world = new Communicator (P);
   Mapping *map = new Mapping (P, M, Q, mapping);
   world->map(map);
      
   Collective *allred  = new AllreduceRingSegm();
   CollParams cp {m, op};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   
   double t = tc->getTime();

   delete tc;
   delete world;

   return t;
}



double allreduce_rabenseifner_ompi (int P, int Q, int M, int m, int ms, Map mapping, OpType op) {
      
   Communicator *world = new Communicator (P);
   Mapping *map = new Mapping (P, M, Q, mapping);
   world->map(map);
      
   Collective *allred  = new AllreduceRabenseifner();
   CollParams cp {m, op};
   
   TauLopCost *tc = allred->evaluate(world, cp);
   
   double t = tc->getTime();

   delete tc;
   delete world;

   return t;
}
