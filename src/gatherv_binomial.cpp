//
//  gatherv_binomial.cpp
//  TauLopCost
//
//  Created by jarico on 23/Apr/17
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "gatherv_binomial.hpp"

#include "transmission.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"

#include <math.h>
#include <iostream>
using namespace std;




GatherVBinomial::GatherVBinomial () {
   
}

GatherVBinomial::~GatherVBinomial () {
   
}


TauLopCost * GatherVBinomial::evaluate (Communicator *comm, int *size, int root) {
   
   TauLopConcurrent *conc;
   TauLopSequence   *seq;
   Transmission     *c;
   
   int P = comm->getSize();
   
   conc = new TauLopConcurrent ();
   
   for (int rank = 0; rank < P; rank++) {
      
      seq = new TauLopSequence ();
      
      if (rank == root) continue;
      
      Process p_src {rank, comm->getNode(rank)};
      Process p_dst {root, comm->getNode(root)};
      
      int channel = (p_src.getNode() == p_dst.getNode()) ? 0 : 1;
      
      // How many bytes to send? (needed if P is not power of 2)
      int m = size[rank];
      
      int n    = 1;
      int tau  = 1;
      
      c = new Transmission(p_src, p_dst, channel, n, m, tau);
      seq->add(c);
      
      conc->add(seq);
   }
   
   TauLopCost *cost = new TauLopCost();
   
#if TLOP_DEBUG == 1
   conc->show();
#endif
   
   conc->evaluate(cost);
   
#if TLOP_DEBUG == 1
   cout << "  --------  Cost:  " << endl;
   cost->show();
#endif
   
   delete conc;
   
   return cost;
}

