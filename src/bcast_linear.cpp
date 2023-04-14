//
//  bcast_binomial.cpp
//  TauLopCost
//
//  Created by jarico on 17/Nov/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "bcast_linear.hpp"

#include "transmission.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"
#include "taulop_cost.hpp"

#include <math.h>
#include <iostream>
using namespace std;




BcastLinear::BcastLinear () {
   
}

BcastLinear::~BcastLinear () {
   
   
}


TauLopCost * BcastLinear::evaluate (Communicator *comm, int *size, int root, OpType op) {
   
   TauLopConcurrent *conc = nullptr;
   TauLopSequence   *seq  = nullptr;
   Transmission     *T    = nullptr;
   
   int P = comm->getSize();
   
   conc = new TauLopConcurrent ();
   seq  = new TauLopSequence ();
   
   for (int rank = 0; rank < P; rank++) {
      
      if (rank == root)  continue;
      
      int node_src = comm->getNode(root);
      int node_dst = comm->getNode(rank);
      
      Process p_src {root, node_src};
      Process p_dst {rank, node_dst};
      
      int channel = (node_src == node_dst) ? 0 : 1;
      
      int tau = 1;
      
      int n = 1;
      
      T = new Transmission(p_src, p_dst, channel, n, *size, tau);
      seq->add(T);
   }
   
   conc->add(seq);
   
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

