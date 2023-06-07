//
//  allreduce_ring_segm.cpp
//  Ring Alreduce algorithm
//
//  Created by jarico on 17/04/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "allreduce_ring_segm.hpp"

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




AllreduceRingSegm::AllreduceRingSegm () {
   
}


AllreduceRingSegm::~AllreduceRingSegm () {
   
}


TauLopCost * AllreduceRingSegm::evaluate (Communicator *comm, const CollParams &cparams) {
   
   TauLopConcurrent *conc = nullptr;
   TauLopSequence   *seq  = nullptr;
   Transmission     *T    = nullptr;
   Computation      *g    = nullptr;
   
   TauLopCost *cost = new TauLopCost();
   
   int    P  = comm->getSize();
   int    m  = cparams.getM();
   OpType op = cparams.getOp();
   
   int ms = 128; // Size of a message in bytes (TO BE TESTED ???)

   if (m < (ms * P)) { // REQUIRES m multple of ms and P and *size > ms * P
      cerr << "[allreduce_ring_segm] AllReduce Ring Segmented requires m >= (ms x P)." << endl;
      return cost;
   }
      
   int steps   = ceil(m / (ms * P));
   int last_ms = m % ms; // Last segment could be not complete
   if (last_ms == 0)  last_ms = ms;
      
   conc = new TauLopConcurrent ();
   
   /*  Computation phase  */
   for (int p = 0; p < P; p++) {
      
      seq  = new TauLopSequence ();
      
      for (int s = 0; s < steps; s++) {
         
         if (s == steps - 1)  ms = last_ms;
   
         for (int d = 2; d < P; d++) {
            
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
            
            T = new Transmission(p_src, p_dst, channel, n, ms, tau);
            seq->add(T);
            
            g = new Computation(p_src, ms, op);
            seq->add(g);
         }
         
      }
      
      conc->add(seq);
   }
   
   // Both phases are sequential:
   conc->evaluate(cost);
   

   /* Distribution (communication) phase */
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
         
         T = new Transmission(p_src, p_dst, channel, n, m / P, tau);
         seq->add(T);
      }
      
      conc->add(seq);
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

