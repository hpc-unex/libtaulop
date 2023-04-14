//
//  reduce_binomial_openmpi.cpp
//
//  Created by jarico on 11/04/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "reduce_binomial_openmpi.hpp"

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




ReduceBinomialOpenMPI::ReduceBinomialOpenMPI () {
   
}


ReduceBinomialOpenMPI::~ReduceBinomialOpenMPI () {
   
}


TauLopCost * ReduceBinomialOpenMPI::evaluate (Communicator *comm, int *size, int root, OpType op) {

   TauLopConcurrent *conc = nullptr;
   TauLopSequence   *seq  = nullptr;
   Transmission     *T    = nullptr;
   Computation      *g    = nullptr;

   
   TauLopCost       *cost = new TauLopCost();
   
   int P = comm->getSize();
   
   int num_stages = ceil(log2(P));
   int mask  = 1 << (num_stages - 1);
   int stage = num_stages - 1;
   
   while (mask > 0) {
            
      conc = new TauLopConcurrent ();
            
      for (int r = 0; r < pow(2, stage); r++) {
         
         if ((mask & r) == 0) {
                        
            // r is the relative rank. We need to get the real rank from it:
            //   rank = (r + root) % P
            int src = r | mask;
            if (src >=  P) continue;
            src = (src + root) % P;
            
            int dst = r;
            dst = (dst + root) % P;
                        
            seq = new TauLopSequence ();
            
            Process p_src {src, comm->getNode(src)};
            Process p_dst {dst, comm->getNode(dst)};
            
            int channel = (p_src.getNode() == p_dst.getNode()) ? 0 : 1;
            
            int m    = *size;
            int n    = 1;
            int tau  = 1;
            
            T = new Transmission(p_src, p_dst, channel, n, m, tau);
            seq->add(T);
            
            g = new Computation(p_dst, m, op);
            seq->add(g);
            
            conc->add(seq);
            
         }
         
      }
      mask = mask >> 1;
      stage--;
            
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

