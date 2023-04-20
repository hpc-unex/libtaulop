//
//  alltoall_linear.cpp
//  Pairwise algorithm for the Alltoall collective implemented in Open MPI 3.1 (tuned component)
//
//  Created by jarico on 1/Dic/18.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "alltoall_pairwise.hpp"

#include "coll_params.hpp"
#include "transmission.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"

#include <cmath>
#include <iostream>
using namespace std;



AlltoallPairwise::AlltoallPairwise () {
   
}

AlltoallPairwise::~AlltoallPairwise () {
   
}


TauLopCost * AlltoallPairwise::evaluate (Communicator *comm, const CollParams &cparams) {
   
   TauLopConcurrent *conc = nullptr;
   TauLopSequence   *seq  = nullptr;
   Transmission     *T    = nullptr;
   
   // Every process uses sendrecv of a message of size (*size):
   //   sendto: (rank + step) % size;
   //   recvfrom = (rank + size - step) % size;
   
   TauLopCost *cost = new TauLopCost();
   
   int P = comm->getSize();
   int m = cparams.getM();
   
   conc = new TauLopConcurrent ();
   
   for (int p = 0; p < P; p++) {
      
      seq = new TauLopSequence ();
      
      for (int step = 1; step < P + 1; step++) {
         
         int src = p;
         int dst = (p + step) % P;
         
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
      
#if TLOP_DEBUG == 1
      cout << "P = " << P << endl;
      conc->show();
#endif
   }
   
   conc->evaluate(cost);
#if TLOP_DEBUG == 1
   cout << " -- Cost: " << endl;
   cost->show();
#endif
   
   delete conc;
   
   return cost;
}
