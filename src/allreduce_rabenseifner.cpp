//
//  allreduce_rabenseifner.cpp
//  Rabenseifner's Allreduce algorithm
//
//  Created by jarico on 17/04/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "allreduce_rabenseifner.hpp"

#include "coll_params.hpp"
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


TauLopCost * AllreduceRabenseifner::evaluate (Communicator *comm, const CollParams &cparams) {
   
   TauLopConcurrent *conc = nullptr;
   TauLopSequence   *seq  = nullptr;
   Transmission     *T    = nullptr;
   Computation      *g    = nullptr;
   
      
   int    P  = comm->getSize();
   int    m  = cparams.getM();
   OpType op = cparams.getOp();
   
   TauLopCost *cost = new TauLopCost();
   
   conc = new TauLopConcurrent ();
   
   /*  Allreduce Rabenseifner = Reduce_scatter with buffer halving  and distance doubling +
                                Allgather      with buffer doubling and distance halving     */
   
   if (m / sizeof(int) < P) { // REQUIRES count >= P
      cerr << "[allreduce_rabenseifner]  AllReduce Rabenseifner requires count >= P." << endl;
      cerr << "[allreduce_rabenseifner] Furthermore, by now, it requires P power of 2." << endl;
      return cost;
   }

      
   /* We assume (by now) P pover of 2 */
   /* PHASE 1: reduce scatter */
   for (int stage = 1; stage < P; stage <<= 1) {
      
      int send_m = m / (2 * stage);
      
      for (int p = 0; p < P; p++) {
         
         /* Does not mind the process rank */
         int src = p;
         int dst = p ^ stage;
                  
         if (dst < src) continue;
         
         seq = new TauLopSequence ();
         
         int node_src = comm->getNode(src);
         int node_dst = comm->getNode(dst);
         
         Process p_src {src, node_src};
         Process p_dst {dst, node_dst};
         
         int channel = (node_src == node_dst) ? 0 : 1;
         
         int n   = 1;
         int tau = 2; // Sendrecv
         
         T = new Transmission(p_src, p_dst, channel, n, send_m, tau);
         seq->add(T);
         
         g = new Computation(p_src, send_m, op);
         seq->add(g);
         
         conc->add(seq);
      }
   }
   
   conc->evaluate(cost);
   

   /* PHASE 2: allgather */
   for (int stage = P >> 1; stage > 0; stage >>= 1) {
      
      int send_m = m / (2 * stage);
      
      for (int p = 0; p < P; p++) {
         
         /* Does not mind the process rank */
         int src = p;
         int dst = p ^ stage;
                  
         if (dst < src) continue;
         
         seq = new TauLopSequence ();
         
         int node_src = comm->getNode(src);
         int node_dst = comm->getNode(dst);
         
         Process p_src {src, node_src};
         Process p_dst {dst, node_dst};
         
         int channel = (node_src == node_dst) ? 0 : 1;
         
         int n   = 1;
         int tau = 2; // Sendrecv
         
         T = new Transmission(p_src, p_dst, channel, n, send_m, tau);
         seq->add(T);
                  
         conc->add(seq);
      }
   }
   
   
#if TLOP_DEBUG == 1
   cout << "P = " << P << endl;
   conc->show();
#endif
   
   conc->evaluate(cost);
   
#if TLOP_DEBUG == 1
   cout << " -- Cost: " << endl;
   cost->show();
#endif
   
   delete conc;
   
   return cost;
}

