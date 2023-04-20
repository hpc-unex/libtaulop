//
//  allgather_ring.cpp
//  Ring Algather algorithm
//
//  Created by jarico on 17/Nov/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "allgather_ring.hpp"

#include "coll_params.hpp"
#include "transmission.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"

#include <math.h>
#include <iostream>
using namespace std;




AllgatherRing::AllgatherRing () {
   
}

AllgatherRing::~AllgatherRing () {
   
   
}


TauLopCost * AllgatherRing::evaluate (Communicator *comm, const CollParams &cparams) {
   
   TauLopConcurrent *conc = nullptr;
   TauLopSequence   *seq  = nullptr;
   Transmission     *T    = nullptr;
   
   
   int P = comm->getSize();
   int m = cparams.getM();
   
   conc = new TauLopConcurrent ();
   
   /*  Allgather Ring  */
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
         
         int n = 1;
         
         int tau = 1;
         
         T = new Transmission(p_src, p_dst, channel, n, m, tau);
         seq->add(T);
         
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

