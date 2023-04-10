//
//  bcast_binomial.cpp
//  TauLopCost
//
//  Created by jarico on 17/Nov/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "bcast_binomial_openmpi.hpp"

#include "transmission.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"

#include <math.h>
#include <iostream>
using namespace std;




BcastBinomialOpenMPI::BcastBinomialOpenMPI () {
   
}

BcastBinomialOpenMPI::~BcastBinomialOpenMPI () {
   
   
}


TauLopCost * BcastBinomialOpenMPI::evaluate (Communicator *comm, int *size, int root, OpType op) {
   
   TauLopConcurrent *conc;
   TauLopSequence   *seq;
   Transmission     *c;
   
   TauLopCost       *cost = new TauLopCost();
   
   int P = comm->getSize();
   
   for (int stage = 0; pow(2, stage) < P; stage++) {
      
      conc = new TauLopConcurrent ();
      
      int p = root;
      
      for (int t = 0; t < pow(2, stage); t++) {
         
         seq = new TauLopSequence ();
         
         int src = p;
         int dst = src + pow(2, stage);
         dst = dst % P;
         
         int node_src = comm->getNode(src);
         int node_dst = comm->getNode(dst);
         
         Process p_src {src, node_src};
         Process p_dst {dst, node_dst};
         
         int channel = (node_src == node_dst) ? 0 : 1;
         
         int n   = 1;
         int tau = 1;
         
         c = new Transmission(p_src, p_dst, channel, n, *size, tau);
         seq->add(c);
         
         conc->add(seq);
         
         p = p + 1;
         p = p % P;
         
      }
      
#if TLOP_DEBUG == 1
      cout << " ----  Stage " << stage << endl;
      conc->show();
#endif        
      
      conc->evaluate(cost);
      
#if TLOP_DEBUG == 1
      cout << "  --------  Cost:  " << endl;
      cost->show();
#endif
      delete conc;
   }
   
   return cost;
}

