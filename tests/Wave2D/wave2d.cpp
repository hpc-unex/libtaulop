//
//  main.cpp
//  Wave2D
//
//  Created by jarico on 4/5/17.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "taulop_kernel.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
using namespace std;



int getBoundary (Communicator *comm, int N, int src, int dst) {
   
   // TODO: only works for P=^2 and homogeneous sequential distributes ranks on columns
   //    P = 16:  0  1  2  3
   //             4  5  6  7
   //             8  9 10 11
   //            12 13 14 15
   
   int P = comm->getSize();
   int col_procs = sqrt(P);
   
   if (src == dst)
      return 0;
   
   int src_col = src % (int)sqrt(P);
   int dst_col = dst % (int)sqrt(P);
   
   if (src_col == dst_col) {
      if (src == (dst + col_procs))
         return N / P;
      if (src == (dst - col_procs))
         return N / P;
   } else if ((src_col == dst_col + 1) || (src_col == dst_col - 1)) {
      if (src == (dst + 1))
         return N / P;
      if (src == (dst - 1))
         return N / P;
   }
   
   return 0;
}


// Heterogeneous or homogeneous. It depends on the implementation of getBoundary.
double wave2d_p2p (Communicator *comm, int N, int b) {
   
   double t = 0.0;
   int P = comm->getSize();
   
   TauLopConcurrent *conc = new TauLopConcurrent();
   
   for (int src = 0; src < P; src++) {
      
      TauLopSequence *seq = new TauLopSequence();
      
      for (int dst = 0; dst < P; dst++) {
         
         int size = getBoundary(comm, N, src, dst);
         
         if (size > 0) {
            
            int chn = (comm->getNode(src) == comm->getNode(dst)) ? 0 : 1;
            int m   = size * b * sizeof(double);
            
            seq->add(new Transmission(chn, m, 1));
            
         }
      }
      conc->add(seq);
   }
   
   TauLopCost *tc = new TauLopCost();
   conc->evaluate(tc);
   t = tc->getTime();
   
   delete tc;
   delete conc;
   
   return t;
}




double wave2d_coll (Communicator *comm, int N, int b) {
   
   double t_bcast = 0.0;
   
   // How to implement it?
   
   return t_bcast;
}



int main (int argc, const char * argv[]) {
   
   TauLopParam::setInstance({"SHM","IB"});
   srand(123);
   
   int P = 16;
   int Q = 4;
   int M = P / Q;
   double t = 0.0;
   
   int N = 256;
   int b = 32;
   
   
   // Homogeneous mapping with:
   //   P : number of processes
   //   Q : number of processes in each node
   //   M : (calculated) number of nodes
   //   Map::Sequential : processes
   //      ranks 0 .. Q-1  to node 0
   //      ranks Q .. 2Q-1 to node 1
   //       ...
   Communicator *world = new Communicator (P);
   Mapping *map = new Mapping(P, M, Q, Map::Sequential);
   world->map(map);
   
   cout << endl << "Sequential Mapping: " << endl;
   map->show();
   
   // 1. Wave 2D using p2p. Both Homogeneous & Heterogeneous version.
   t = wave2d_p2p (world, N, b);
   cout << "Parallel AxB P2P Homogeneous:    " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 2. Wave 2D using colls. Both Homogeneous & Heterogeneous version.
   t = wave2d_coll (world, N, b);
   cout << "Parallel AxB COLL Homogeneous:   " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   delete map;
   
   
   
   
   // Homogeneous mapping with:
   //   P : number of processes
   //   Q : number of processes in each node
   //   M : (calculated) number of nodes
   //   Map::RoundRobin : processes
   //      ranks 0  Q    2Q   .. Q(Q-1)   to node 0
   //      ranks 1  Q+1  2Q+1 .. Q(Q-1)+1 to node
   map = new Mapping(P, M, Q, Map::RoundRobin); 
   world->map(map);
   
   cout << endl << "Round Robin Mapping: " << endl;
   map->show();
   
   // 1. Wave 2D using p2p. Both Homogeneous & Heterogeneous version.
   t = wave2d_p2p (world, N, b);
   cout << "Parallel AxB P2P Homogeneous:    " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 2. Wave 2D using colls. Both Homogeneous & Heterogeneous version.
   t = wave2d_coll (world, N, b);
   cout << "Parallel AxB COLL Homogeneous:   " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   delete map;
   
   delete world;
   
   
   
   
   
   // Homogeneous mapping with:
   //   P : number of processes
   //   Q : number of processes in each node
   //   M : (calculated) number of nodes
   //   MAPPING_SEQ : processes
   //      ranks 0 .. Q-1  to node 0
   //      ranks Q .. 2Q-1 to node 1
   //       ...
   Communicator *irr_comm = new Communicator (P);
   int nodes[P];
   srand(123);
   for (int i = 0; i < P; i++) {
      nodes[i] = rand() % M; // Random node for testing
   }
   Mapping *irr_map = new Mapping(P, nodes);
   irr_comm->map(irr_map);
   
   cout << endl << "Irregular Mapping: " << endl;
   irr_map->show();
   
   // 1. Wave 2D using p2p. Both Homogeneous & Heterogeneous version.
   t = wave2d_p2p (irr_comm, N, b);
   cout << "Parallel AxB P2P Homogeneous:    " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 2. Wave 2D using colls. Both Homogeneous & Heterogeneous version.
   t = wave2d_coll (irr_comm, N, b);
   cout << "Parallel AxB COLL Homogeneous:   " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   delete irr_map;
   delete irr_comm;
   
   
   return 0;
}
