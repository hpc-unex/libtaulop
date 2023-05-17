//
//  imb_taulop.cpp
//  IMB-style taulop benchmarks.
//    Print similar output in order to compare results.
//
//  Created by jarico on 30/4/23.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "benchmark.h"
#include "taulop_kernel.hpp"

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;



Results_t pingpong (const Arguments_t &args) {
      
   TauLopConcurrent *conc = new TauLopConcurrent ();
   TauLopSequence   *seq  = new TauLopSequence ();
   
   Process *src = new Process(0);
   Process *dst = new Process(1);

   // Mapping of processes
   int srcnode = 0;
   int dstnode = 0;
   if (args.mapping == Map::User) {
      srcnode = args.map_user[0];
      dstnode = args.map_user[1];
   } else if ((args.mapping == Map::Default) || (args.mapping == Map::Sequential)) {
      srcnode = 0 / args.Q;
      dstnode = 1 / args.Q;
   } else { // RoundRobin and Random
      srcnode = 0 % args.M;
      dstnode = 1 % args.M;
   }
   
   src->setNode(srcnode);
   dst->setNode(dstnode);
      
   int chn = (src->getNode() == dst->getNode()) ? 0 : 1;
      
   int n = 1;
   seq->add(new Transmission {src, dst, chn, n, args.m, 1});
   seq->add(new Transmission {dst, src, chn, n, args.m, 1});
   
   conc->add(seq);
   
   TauLopCost *tc = new TauLopCost();
   conc->evaluate(tc);
      
   double lat = (tc->getTime() * 1000000); // Latency in usec, as in IMB (divided by 2.0 ???)
   double bw  = args.m / lat;
   
   delete tc;
   delete conc;

   return {args.m, 1, lat, bw};
}


