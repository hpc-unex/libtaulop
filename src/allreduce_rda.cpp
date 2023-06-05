//
//  allrduce_rda.cpp
//  Recursive Doubling Alreduce algorithm
//
//  Created by jarico on 17/04/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "allreduce_rda.hpp"

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




AllreduceRDA::AllreduceRDA () {
   
}


AllreduceRDA::~AllreduceRDA () {
      
}


TauLopCost * AllreduceRDA::evaluate (Communicator *comm, const CollParams &cparams) {
   
   TauLopConcurrent *conc = nullptr;
   TauLopSequence   *seq  = nullptr;
   Transmission     *T    = nullptr;
   Computation      *g    = nullptr;
      
   TauLopCost       *cost = new TauLopCost();
   
   int    P  = comm->getSize();
   int    m  = cparams.getM();
   OpType op = cparams.getOp();
   
   /* Allreduce RDA  (P^2 processes) */
   for (int stage = 0; pow(2, stage) < P; stage++) {
      
      conc = new TauLopConcurrent ();
                  
      for (int p = 0; p < P; p++) {
         
         /* Does not mind the process rank */
         int src = p;
         int b = pow(2, stage);
         int dst = p ^ b;
         
         if (dst < src) continue;
         
         seq = new TauLopSequence ();
         
         int node_src = comm->getNode(src);
         int node_dst = comm->getNode(dst);
         
         Process p_src {src, node_src};
         Process p_dst {dst, node_dst};
         
         int channel = (node_src == node_dst) ? 0 : 1;
         
         int n   = 1;
         int tau = 2; // Sendrecv
         
         T = new Transmission(p_src, p_dst, channel, n, m, tau);
         seq->add(T);
                           
         g = new Computation(p_src, m, op);
         seq->add(g);
         
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
   }
   
   return cost;
}
