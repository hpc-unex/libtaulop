//
//  scatter_binomial.cpp
//  TauLopCost
//
//  Created by jarico on 21/Apr/17
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "scatter_binomial.hpp"

#include "coll_params.hpp"
#include "transmission.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"

#include <math.h>
#include <iostream>
using namespace std;




ScatterBinomial::ScatterBinomial () {
   
}

ScatterBinomial::~ScatterBinomial () {
   
}


TauLopCost * ScatterBinomial::evaluate (Communicator *comm, const CollParams &cparams) {
   
   TauLopConcurrent *conc = nullptr;
   TauLopSequence   *seq  = nullptr;
   Transmission     *T    = nullptr;
   
   TauLopCost       *cost = new TauLopCost();
   
   int P    = comm->getSize();
   int root = cparams.getRoot();
   int m    = cparams.getM();
   
   for (int stage = 0; pow(2, stage) < P; stage++) {
      
      conc = new TauLopConcurrent ();
      
      int p = root;
      
      for (int t = 0; t < pow(2, stage); t++) {
         
         seq = new TauLopSequence ();
         
         int src = p;
         int dst = src + P / pow(2, stage + 1);
         dst = dst % P;
         
         Process p_src {src, comm->getNode(src)};
         Process p_dst {dst, comm->getNode(dst)};
         
         int channel = (p_src.getNode() == p_dst.getNode()) ? 0 : 1;
         
         int stagesize = m * pow(2, stage);
         int n    = 1;
         int tau  = 1;
         
         T = new Transmission(p_src, p_dst, channel, n, stagesize, tau);
         seq->add(T);
         
         conc->add(seq);
         
         p = p + (P / pow(2, stage));
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

