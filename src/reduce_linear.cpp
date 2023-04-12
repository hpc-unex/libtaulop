//
//  reduce_linear.cpp
//  TauLopCost
//
//  Created by jarico on 27/Mar/23.
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




ReduceLinear::ReduceLinear () {
   
}


ReduceLinear::~ReduceLinear () {   
   
}


TauLopCost * ReduceLinear::evaluate (Communicator *comm, int *size, int root, OpType op) {
   
   TauLopConcurrent *conc = nullptr;
   TauLopSequence   *seq  = nullptr;
   Transmission     *T    = nullptr;
   Computation      *g    = nullptr;
   
   int n   = 1;
   int tau = 1;

   
   int P = comm->getSize();
   
   conc = new TauLopConcurrent ();
   
   // Process P-1 sends and the buffer is not operated.
   if (root != P-1) {
      seq  = new TauLopSequence ();
      
      int node_src = comm->getNode(P-1);
      int node_dst = comm->getNode(root);
      
      Process p_src {P-1,  node_src};
      Process p_dst {root, node_dst};
      
      int channel = (node_src == node_dst) ? 0 : 1;
      
      T = new Transmission(p_src, p_dst, channel, n, *size, tau);
      seq->add(T);
      
      conc->add(seq);
   }

   // Rest of processes.
   for (int rank = P-2; rank >= 0; rank--) {
      
      seq  = new TauLopSequence ();
      
      if (rank != root) {
         
         int node_src = comm->getNode(rank);
         int node_dst = comm->getNode(root);
         
         Process p_src {rank, node_src};
         Process p_dst {root, node_dst};
         
         int channel = (node_src == node_dst) ? 0 : 1;
                  
         T = new Transmission(p_src, p_dst, channel, n, *size, tau);
         seq->add(T);
         
      }

      int     node_src = comm->getNode(rank);
      Process p_src {rank, node_src};
      
      g = new Computation(p_src, *size, op);
      seq->add(g);
                  
      conc->add(seq);

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

