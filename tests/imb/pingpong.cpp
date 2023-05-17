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

   // Mapping of processes: create a mapping object just for getting nodes
   //   Nodes assigned depends on Q.
   Mapping *map = nullptr;
   if (args.mapping == Map::User) {
      int* v = (int *)&args.map_user[0]; // Is this safe?
      map = new Mapping (args.P, v);
   } else {
      map = new Mapping (args.P, args.Q, args.mapping);
   }
   int srcnode = map->getNode(0);
   int dstnode = map->getNode(1);
   delete map;
   
   // Set nodes
   src->setNode(srcnode);
   dst->setNode(dstnode);
      
   int chn = (src->getNode() == dst->getNode()) ? 0 : 1;
      
   int n = 1;
   seq->add(new Transmission {src, dst, chn, n, args.m, 1});
   seq->add(new Transmission {dst, src, chn, n, args.m, 1});
   
   conc->add(seq);
   
   TauLopCost *tc = new TauLopCost();
   conc->evaluate(tc);
      
   double lat = (tc->getTime() * 1000000); // Latency in usec, as in IMB
   double bw  = args.m / lat;
   
   delete tc;
   delete conc;

   return {args.m, 1, lat, bw};
}


