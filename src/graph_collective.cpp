//
//  graph_collective.cpp
//
//  Created by jarico on 16/Sep/20.
//  Copyright © 2020 Juan A. Rico. All rights reserved.
//

#include "graph_collective.hpp"

#include "graph.hpp"
#include "transmission.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"

#include <math.h>
#include <iostream>
using namespace std;




GraphCollective::GraphCollective () {
   
}

GraphCollective::~GraphCollective () {
   
   
}


TauLopCost * GraphCollective::evaluate (Communicator *comm, int *size, int root) {
   
   TauLopConcurrent *conc;
   TauLopSequence   *seq;
   Transmission     *c;
   
   TauLopCost       *cost = new TauLopCost();
   
   int P = comm->getSize();
   
   for (int depth = 2; depth <= this->graph.maxDepth(); depth++) {
      
      vector<Edge> edges;
      this->graph.getEdgesInDepth(depth, edges);
      
      conc = new TauLopConcurrent ();
      
      for (Edge e : edges) {
         
         seq = new TauLopSequence ();
         
         int src = e.src;
         int dst = e.dst;
         
         int node_src = comm->getNode(src);
         int node_dst = comm->getNode(dst);
         
         Process p_src {src, node_src};
         Process p_dst {dst, node_dst};
         
         int channel = (node_src == node_dst) ? 0 : 1;
         
         int n   = 1;
         int tau = 1;
         
         c = new Transmission(p_src, p_dst, channel, n, *size, tau);
         seq->add(c);
         
         conc->add(seq);
         
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

