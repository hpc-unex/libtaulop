//
//  imb_taulop.cpp
//  IMB-style taulop benchmarks.
//    Print similar output in order to compare results.
//
//  Created by jarico on 30/4/23.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "benchs.h"
#include "taulop_kernel.hpp"

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;



Results_t pingpong_m (int m) {
   
   TauLopConcurrent *concurrent = new TauLopConcurrent ();
   TauLopSequence   *seq = nullptr;
   
   seq = new TauLopSequence ();
   seq->add(new Transmission {0, m, 1});
   seq->add(new Transmission {0, m, 1});
   concurrent->add(seq);
      
   TauLopCost *tc = new TauLopCost();
   concurrent->evaluate(tc);
      
   double lat = (tc->getTime() * 1000000) / 2.0; // Latency in usec, as in IMB (and is a PingPong, so /2)
   double bw  = m / lat;
   
   delete tc;
   delete concurrent;

   return {m, 1, lat, bw};
}



// T^c(m) + T^c(m)
vector<Results_t> pingpong (vector<int> msglen) {

   TauLopParam::setInstance({"SHM","IB"});
   
   vector<Results_t> results;
   
   for (auto const &m: msglen) {
      Results_t res = pingpong_m (m);
      results.push_back(res);
   }
   
   return results;
}


