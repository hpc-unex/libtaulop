//
//  gather_binomial.cpp
//  TauLopCost
//
//  Created by jarico on 20/Apr/17
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "gather_binomial.hpp"

#include "transmission.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"

#include <math.h>
#include <iostream>
using namespace std;




GatherBinomial::GatherBinomial () {
   
}

GatherBinomial::~GatherBinomial () {
   
}


TauLopCost * GatherBinomial::evaluate (Communicator *comm, const CollParams &cparams) {
   
   TauLopConcurrent *conc = nullptr;
   TauLopSequence   *seq  = nullptr;
   Transmission     *T    = nullptr;
   
   TauLopCost       *cost = new TauLopCost();
   
   int P    = comm->getSize();
   int m    = cparams.getM();
   int root = cparams.getRoot();
   
   int mask  = 1;
   int stage = 0;
   
   while (mask < P) {
      
      conc = new TauLopConcurrent ();
            
      for (int r = 0; r < P; r += pow(2, stage)) {
         
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
            
            // How many bytes to send? (needed if P is not power of 2)
            int recvblks = mask;
            if (2 * recvblks > P) {
               recvblks = P - mask;
            }
            
            int n    = 1;
            int tau  = 1;
            
            T = new Transmission(p_src, p_dst, channel, n, m * recvblks, tau);
            seq->add(T);
            
            conc->add(seq);
            
         }
         
      }
      mask <<= 1;
      stage++;
            
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

