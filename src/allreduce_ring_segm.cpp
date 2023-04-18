//
//  allreduce_ring_segm.cpp
//  Ring Alreduce algorithm
//
//  Created by jarico on 17/04/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "allreduce_ring_segm.hpp"

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


TauLopCost * AllreduceRingSegm::evaluate (Communicator *comm, int *size, int root, OpType op) {
   
   TauLopConcurrent *conc = nullptr;
   TauLopSequence   *seq  = nullptr;
   Transmission     *T    = nullptr;
   Computation      *g    = nullptr;
   
   
   cout << endl << "  **********   FALTA POR IMPLEMENTAR   **********" << endl << endl;
   
   int P  = comm->getSize();
   int ms = 32; // Size of a message in bytes (TO BE TESTED ???)
   
   if (*size < (ms * P)) { // REQUIRES m multple of ms and P and *size > ms * P
      return nullptr;
   }
   int steps = ceil(*size / (ms * P));
      
   conc = new TauLopConcurrent ();
   
   /*  Computation phase  */
   for (int p = 0; p < P; p++) {
      
      for (int s = 0; s < steps; s++) {
         
         seq = new TauLopSequence ();
   
         for (int d = 1; d < P; d++) {
            
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
   TauLopCost *cost = new TauLopCost();
   conc->evaluate(cost);
   

   /* Distribution (communication) phase */
   for (int p = 0; p < P; p++) {
      
      seq = new TauLopSequence ();
      
      for (int d = 0; d < P-1; d++) {
         
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
         
         T = new Transmission(p_src, p_dst, channel, n, ms * P, tau);
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

