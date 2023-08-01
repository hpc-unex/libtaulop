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

double allreduce_linear_ompi (Communicator *comm, int P, int m, int ms, OpType op) {
         
   Collective *allred  = new AllreduceLinearOpenMPI();
   CollParams cp {m, op};
   
   TauLopCost *tc = allred->evaluate(comm, cp);
   
   double t = tc->getTime();
   
   delete tc;

   return t;
}



double allreduce_nonoverlap_ompi (Communicator *comm, int P, int m, int ms, OpType op) {
   
   Collective *allred  = new AllreduceNonOverlapOpenMPI();
   CollParams cp {m, op};
   
   TauLopCost *tc = allred->evaluate(comm, cp);
   
   double t = tc->getTime();

   delete tc;

   return t;
}



double allreduce_rda_ompi (Communicator *comm, int P, int m, int ms, OpType op) {
            
   Collective *allred  = new AllreduceRDA();
   CollParams cp {m, op};
   
   TauLopCost *tc = allred->evaluate(comm, cp);
   
   double t = tc->getTime();

   delete tc;

   return t;
}



double allreduce_ring_ompi (Communicator *comm, int P, int m, int ms, OpType op) {
      
   Collective *allred  = new AllreduceRing();
   CollParams cp {m, op};
   
   TauLopCost *tc = allred->evaluate(comm, cp);
   
   double t = tc->getTime();

   delete tc;

   return t;
}



double allreduce_ringsegm_ompi (Communicator *comm, int P, int m, int ms, OpType op) {
      
   Collective *allred  = new AllreduceRingSegm();
   CollParams cp {m, op};
   
   TauLopCost *tc = allred->evaluate(comm, cp);
   
   double t = tc->getTime();

   delete tc;

   return t;
}



double allreduce_rabenseifner_ompi (Communicator *comm, int P, int m, int ms, OpType op) {
            
   Collective *allred  = new AllreduceRabenseifner();
   CollParams cp {m, op};
   
   TauLopCost *tc = allred->evaluate(comm, cp);
   
   double t = tc->getTime();

   delete tc;

   return t;
}
