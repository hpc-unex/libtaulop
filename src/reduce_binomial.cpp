//
//  reduce_binomial.cpp
//
//  Created by jarico on 11/04/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "reduce_linear.hpp"

#include "transmission.hpp"
#include "computation.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"
#include "taulop_cost.hpp"

#include <math.h>
#include <iostream>
using namespace std;




ReduceBinomial::ReduceBinomial () {
   
}


ReduceBinomial::~ReduceBinomial () {
   
}


TauLopCost * ReduceBinomial::evaluate (Communicator *comm, int *size, int root, OpType op) {
   
   TauLopConcurrent *conc = nullptr;
   TauLopSequence   *seq  = nullptr;
   Transmission     *T    = nullptr;
   Computation      *g    = nullptr;
   
   
   int P = comm->getSize();
   
   conc = new TauLopConcurrent ();
   
   for (int rank = P-1; rank >= 0; rank--) {
      
      seq  = new TauLopSequence ();
      
      if (rank != root) {
         
         int node_src = comm->getNode(rank);
         int node_dst = comm->getNode(root);
         
         Process p_src {rank, node_src};
         Process p_dst {root, node_dst};
         
         int channel = (node_src == node_dst) ? 0 : 1;
         
         int n   = 1;
         int tau = 1;
         
         T = new Transmission(p_src, p_dst, channel, n, *size, tau);
         seq->add(T);
         
         conc->add(seq);
         
      }
      
      if (rank != P-1) { // First is received but not operated to.
         g = new Computation(*size, op);
         seq->add(g);
      }
      
   }
   
#if TLOP_DEBUG == 1
   cout << " ----  Root " << root << endl;
   conc->show();
#endif
   
   TauLopCost *cost = new TauLopCost();
   
   conc->evaluate(cost);
   
#if TLOP_DEBUG == 1
   cout << "  --------  Cost:  " << endl;
   cost->show();
#endif
   
   delete conc;
   
   return cost;
}

