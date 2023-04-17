//
//  allreduce_rabenseifner.cpp
//  Rabenseifner's Allreduce algorithm
//
//  Created by jarico on 17/04/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "allreduce_rabenseifner.hpp"

#include "transmission.hpp"
#include "computation.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"

#include <math.h>
#include <iostream>
using namespace std;




AllreduceRabenseifner::AllreduceRabenseifner () {
   
}


AllreduceRabenseifner::~AllreduceRabenseifner () {
   
}


TauLopCost * AllreduceRabenseifner::evaluate (Communicator *comm, int *size, int root, OpType op) {
   
   TauLopConcurrent *conc = nullptr;
   TauLopSequence   *seq  = nullptr;
   Transmission     *T    = nullptr;
   Computation      *g    = nullptr;
   
   
   cout << endl << endl << " *********  RABENSEIFNER POR IMPLEMENTAR   **********" << endl << endl;
   
   int P = comm->getSize();
   
   conc = new TauLopConcurrent ();
   
   /*  Allreduce Ring  */
   for (int p = 0; p < P; p++) {
      
      seq = new TauLopSequence ();
      
      for (int stage = 0; stage < P-1; stage++) {
         
         /* Does not mind the process rank */
         int src = p;
         int dst = (p + 1) % P;
         
         int node_src = comm->getNode(src);
         int node_dst = comm->getNode(dst);
         
         Process p_src {src, node_src};
         Process p_dst {dst, node_dst};
         
         int channel = (node_src == node_dst) ? 0 : 1;
         
         int n   = 1;
         int tau = 1;
         
         T = new Transmission(p_src, p_dst, channel, n, *size, tau);
         seq->add(T);
         
         g = new Computation(p_src, *size, op);
         seq->add(g);
         
      }
      conc->add(seq);
   }
   
#if TLOP_DEBUG == 1
   cout << "P = " << P << endl;
   conc->show();
#endif
   
   TauLopCost *cost = new TauLopCost();
   
   conc->evaluate(cost);
   
#if TLOP_DEBUG == 1
   cout << " -- Cost: " << endl;
   cost->show();
#endif
   
   delete conc;
   
   return cost;
}

