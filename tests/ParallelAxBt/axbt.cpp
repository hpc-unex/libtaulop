//
//  main.cpp
//  ParallelAxBt
//
//  Created by jarico on 3/5/17.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "taulop_kernel.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
using namespace std;




double parallelAxB_p2p (Communicator *comm, int N, int b, int nrows[]) {
   
   // Suppose N is multiple of b
   int P = comm->getSize();
   
   double t = 0.0;
   
   int pivotRow  = 0;
   int pivotProc = 0;
   
   for (int k = 0; k < N; k++, pivotRow++) {
      
      if (pivotRow >= nrows[pivotProc]) {
         pivotProc++;
         pivotRow = 0;
      }
      
      int size = N * b * sizeof(double);
      
      TauLopConcurrent *conc = new TauLopConcurrent ();
      TauLopSequence   *seq  = new TauLopSequence   ();
      
      for (int dst = 0; dst < P; dst++) {
         if (dst == pivotProc) continue;
         
         int chn = comm->getNode(pivotProc) == comm->getNode(dst) ? 0 : 1;
         seq->add(new Transmission(chn, size, 1));
         
      }
      
      conc->add(seq);
      TauLopCost *tc = new TauLopCost();
      conc->evaluate(tc);
      
      t += tc->getTime();
      
      delete tc;
      delete conc;
   }
   
   return t;
}


double parallelAxB_coll (Communicator *comm, int N, int b, int nrows[]) {
   
   // Suppose N is multiple of b
   double t_bcast = 0.0;
   
   int pivotRow  = 0;
   int pivotProc = 0;
   
   Collective *bcast = new BcastBinomial ();
   //Collective *bcast = new BcastLinear ();
   for (int k = 0; k < N; k++, pivotRow++) {
      
      if (pivotRow >= nrows[pivotProc]) {
         pivotProc++;
         pivotRow = 0;
      }
      int size = N * b * sizeof(double);
      TauLopCost *tc = bcast->evaluate(comm, &size, pivotProc);
      t_bcast = t_bcast + tc->getTime();
      delete tc;
   }
   
   return t_bcast;
}






int main (int argc, const char * argv[]) {
   
   TauLopParam::setInstance({"SHM","IB"});
   srand(123);
   
   int P = 32;
   int Q = 4;
   int M = 8;
   double t = 0.0;
   
   int N = 512;
   int b = 32;
   
   int nrows_het [P];
   
   // Heterogeneous random hand-made partitions
   int tot = 0;
   for (int i = 0; i < P - 1; i++) {
      int range = (N / P) * 0.6;
      nrows_het[i] = (N / P) + (N / P * 0.3 - (rand() % range));
      tot += nrows_het[i];
   }
   nrows_het[P-1] = N - tot;
   
   cout << "Heterogeneous partition: ";
   for (int i = 0; i < P; i++) {
      cout << "  " << nrows_het[i];
   }
   cout << endl;
   
   
   int nrows_hom [P];
   // For the heterogeneous versions
   for (int i = 0; i < P; i++) {
      nrows_hom[i] = N / P;
   }
   cout << "Homogeneous partition: ";
   for (int i = 0; i < P; i++) {
      cout << "  " << nrows_hom[i];
   }
   cout << endl;
   
   
   // Homogeneous mapping with:
   //   P : number of processes
   //   Q : number of processes in each node
   //   M : (calculated) number of nodes
   //   MAPPING_SEQ : processes
   //      ranks 0 .. Q-1  to node 0
   //      ranks Q .. 2Q-1 to node 1
   //       ...
   Communicator *world = new Communicator (P);
   Mapping *map = new Mapping(P, Q, MAPPING_SEQ);
   world->map(map);
   
   cout << endl << "Homogeneous Sequential Mapping: " << endl;
   map->show();
   
   // 1. Parallel C=AxB^t using p2p. Homogeneous version.
   t = parallelAxB_p2p (world, N, b, nrows_hom);
   cout << "Parallel AxB P2P Homogeneous:    " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 2. Parallel C=AxB^t using p2p. Heterogeneous version.
   t = parallelAxB_p2p (world, N, b, nrows_het);
   cout << "Parallel AxB P2P Heterogeneous   " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 3. Parallel C=AxB^t using collectives. Homogeneous version.
   t = parallelAxB_coll (world, N, b, nrows_hom);
   cout << "Parallel AxB COLL Homogeneous:   " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 4. Parallel C=AxB^t using collectives. Heterogeneous version.
   t = parallelAxB_coll (world, N, b, nrows_het);
   cout << "Parallel AxB COLL Heterogeneous: " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   
   delete map;
   
   
   // Homogeneous mapping with:
   //   P : number of processes
   //   Q : number of processes in each node
   //   M : (calculated) number of nodes
   //   MAPPING_RR : processes
   //      ranks 0  Q    2Q   .. Q(Q-1)   to node 0
   //      ranks 1  Q+1  2Q+1 .. Q(Q-1)+1 to node
   map = new Mapping(P, Q, MAPPING_RR);
   world->map(map);
   
   cout << endl << "Homogeneous Round Robin Mapping: " << endl;
   map->show();
   
   // 1. Parallel C=AxB^t using p2p. Homogeneous version.
   t = parallelAxB_p2p (world, N, b, nrows_hom);
   cout << "Parallel AxB P2P Homogeneous:    " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 2. Parallel C=AxB^t using p2p. Heterogeneous version.
   t = parallelAxB_p2p (world, N, b, nrows_het);
   cout << "Parallel AxB P2P Heterogeneous   " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 3. Parallel C=AxB^t using collectives. Homogeneous version.
   t = parallelAxB_coll (world, N, b, nrows_hom);
   cout << "Parallel AxB COLL Homogeneous:   " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 4. Parallel C=AxB^t using collectives. Heterogeneous version.
   t = parallelAxB_coll (world, N, b, nrows_het);
   cout << "Parallel AxB COLL Heterogeneous: " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   delete map;
   
   delete world;
   
   
   
   
   // Irregular mapping with:
   //   P : number of processes
   //   Q : number of processes in each node
   //   M : number of nodes
   Communicator *irr_comm = new Communicator (P);
   int nodes [P];
   srand(123);
   for (int i = 0; i < P; i++) {
      nodes[i] = rand() % M; // Random node for testing
   }
   Mapping *irr_map = new Mapping(P, nodes);
   irr_comm->map(irr_map);
   
   cout << endl << "Irregular Mapping and heterogeneous workload distribution: " << endl;
   irr_map->show();
   
   
   // 1. Parallel C=AxB^t using p2p. Homogeneous version.
   t = parallelAxB_p2p (irr_comm, N, b, nrows_hom);
   cout << "Parallel AxB P2P Homogeneous:    " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 2. Parallel C=AxB^t using p2p. Heterogeneous version.
   t = parallelAxB_p2p (irr_comm, N, b, nrows_het);
   cout << "Parallel AxB P2P Heterogeneous   " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 3. Parallel C=AxB^t using collectives. Homogeneous version.
   t = parallelAxB_coll (irr_comm, N, b, nrows_hom);
   cout << "Parallel AxB COLL Homogeneous:   " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 4. Parallel C=AxB^t using collectives. Heterogeneous version.
   t = parallelAxB_coll (irr_comm, N, b, nrows_het);
   cout << "Parallel AxB COLL Heterogeneous: " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   delete irr_map;
   delete irr_comm;
   
   
   return 0;
}
