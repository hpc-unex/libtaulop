//
//  allreduce_ring.cpp
//  Ring Alreduce algorithm
//
//  Created by jarico on 17/04/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "allreduce_ring.hpp"

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




AllreduceRing::AllreduceRing () {
   
}


AllreduceRing::~AllreduceRing () {
   
}


TauLopCost * AllreduceRing::evaluate (Communicator *comm, const CollParams &cparams) {
   
   TauLopConcurrent *conc = nullptr;
   TauLopSequence   *seq  = nullptr;
   Transmission     *T    = nullptr;
   Computation      *g    = nullptr;
      
   int    P  = comm->getSize();
   int    m  = cparams.getM() / P; // We assume m a multiple of P
   OpType op = cparams.getOp();
   
   TauLopCost *cost = new TauLopCost();
      
   conc = new TauLopConcurrent ();
   
   /*  Allreduce Ring: put in a concurrent structure a sequence of P-1 send+operation
                       (computation phase), evaluate it and then put a sequence of
                       P-1 send-recv in a ring (distribution phase) and evaluate. */
   
   /* Computation phase */
   for (int p = 0; p < P; p++) {
      
      seq = new TauLopSequence ();
            
      for (int stage = 0; stage < P-1; stage++) {
         
         int src = p;
         int dst = (p + 1) % P;
         
         int node_src = comm->getNode(src);
         int node_dst = comm->getNode(dst);
         
         Process p_src {src, node_src};
         Process p_dst {dst, node_dst};
         
         int channel = (node_src == node_dst) ? 0 : 1;
         
         int n   = 1;
         int tau = 1;
         
         T = new Transmission(p_src, p_dst, channel, n, m, tau);
         seq->add(T);
         
         g = new Computation(p_src, m, op);
         seq->add(g);
         
      }
      conc->add(seq);
   }
   
   conc->evaluate(cost);
   //delete conc;

   /* Distribution phase */
   //conc = new TauLopConcurrent ();
   
   for (int p = 0; p < P; p++) {
      
      seq = new TauLopSequence ();

      for (int stage = 0; stage < P-1; stage++) {
                  
         int src = p;
         int dst = (p + 1) % P;
         
         int node_src = comm->getNode(src);
         int node_dst = comm->getNode(dst);
         
         Process p_src {src, node_src};
         Process p_dst {dst, node_dst};
         
         int channel = (node_src == node_dst) ? 0 : 1;
         
         int n   = 1;
         int tau = 1;
         
         T = new Transmission(p_src, p_dst, channel, n, m, tau);
         seq->add(T);
         
      }
      conc->add(seq);
   }
   
   conc->evaluate(cost);
   
#if TLOP_DEBUG == 1
   cout << "P = " << P << endl;
   conc->show();
#endif
         
#if TLOP_DEBUG == 1
   cout << " -- Cost: " << endl;
   cost->show();
#endif
   
   delete conc;
   
   return cost;
}

