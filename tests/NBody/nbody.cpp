//
//  main.cpp
//  NBody
//
//  Created by jarico on 20/4/17.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "taulop_kernel.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <vector>
using namespace std;


struct Body {
   double mass;
   double pos_x;
   double pox_y;
   double pos_z;
   double spd_x;
   double spd_y;
   double spd_z;
};
typedef struct Body Body;


// Communication implemented as a set of P2P transmissions
//  to the root node (rank 0)
double nbody_p2p (Communicator *comm, int n_bodies[]) {
   
   int    P = comm->getSize();
   double t = 0.0;
   
   TauLopConcurrent *conc = new TauLopConcurrent ();
   TauLopSequence   *seq;
   
   for (int src = 0; src < P; src++) {
      
      seq = new TauLopSequence ();
      
      for (int dst = 0; dst < P; dst++) {
         
         if (src == dst) continue;
         
         int chn = (comm->getNode(src) == comm->getNode(dst)) ? 0 : 1;
         int m   = sizeof(Body);
         
         seq->add(new Transmission(chn, m, 1));
      }
      conc->add(seq);
   }
   
   TauLopCost *tc = new TauLopCost();
   conc->evaluate(tc);
   t = tc->getTime();
   
   seq = new TauLopSequence ();
   for (int dst = 1; dst < P; dst++) {
      
      int chn = (comm->getNode(dst) == comm->getNode(0)) ? 0 : 1;
      int m   = sizeof(Body) * n_bodies[dst];
      
      seq->add(new Transmission(chn, m, 1));
   }
   conc->add(seq);
   conc->evaluate(tc);
   
   t = t + tc->getTime();
   
   delete tc;
   delete conc;
   
   return t;    
}



double nbody_coll_ring (Communicator *comm, int n_bodies[]) {
   
   // One iteration of the NBody problem
   
   int    P = comm->getSize();
   double t = 0.0;
   vector<int> rsize;
   
   // 1. All processes interchange their mass center
   Collective *allg = new AllgatherRing ();
   int size = sizeof(Body);
   CollParams cp {size};
   
   TauLopCost *tc_allg = allg->evaluate(comm, cp);
   t = tc_allg->getTime();
   delete tc_allg;
   
   // 2. Root gathers all data for showing
   Collective *gbin = new GatherVBinomial ();
   
   for (int i = 0; i < P; i++) {
      rsize.push_back(sizeof(Body) * n_bodies[i]);
   }
   CollParams cpg {rsize, 0};
   
   TauLopCost *tc_gat = gbin->evaluate(comm, cpg);
   t = t + tc_gat->getTime();
   delete tc_gat;
   
   delete (AllgatherRing   *)allg;
   delete (GatherVBinomial *)gbin;
   
   return t;
}


double nbody_coll_rda (Communicator *comm, int n_bodies[]) {
   
   // One iteration of the NBody problem
   
   int    P = comm->getSize();
   double t = 0.0;
   vector<int> rsize;
   
   // 1. All processes interchange their mass center
   Collective *allg = new AllgatherRDA ();
   int size = sizeof(Body);
   CollParams cp {size};
   
   TauLopCost *tc_allg = allg->evaluate(comm, cp);
   t = tc_allg->getTime();
   delete tc_allg;
   
   // 2. Root gathers all data for showing
   Collective *gbin = new GatherVBinomial ();
   
   for (int i = 0; i < P; i++) {
      rsize.push_back(sizeof(Body) * n_bodies[i]);
   }
   CollParams cpg {rsize, 0};
   
   TauLopCost *tc_gat = gbin->evaluate(comm, cpg);
   t = t + tc_gat->getTime();
   delete tc_gat;
   
   delete (AllgatherRDA    *)allg;
   delete (GatherVBinomial *)gbin;
   
   return t;
}






int main (int argc, const char * argv[]) {
   
   TauLopParam::setInstance({"SHM","IB"});
   srand(123);
   
   int P = 32;
   int Q = 4;
   double t = 0.0;
   
   
   // Hetergoeneous number of bodies per process
   int n_bodies[P];
   for (int i = 0; i < P; i++) n_bodies[i] = 500 + rand() % 1000;
   
   
   
   
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
   
   cout << endl << "Sequential Mapping: " << endl;
   map->show();
   
   // 1. NBody using p2p. Heterogeneous version.
   t = nbody_p2p (world, n_bodies);
   cout << "NBody P2P Homogeneous:           " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 2. NBody using colls. Heterogeneous version.
   t = nbody_coll_ring (world, n_bodies);
   cout << "NBody Ring - COLL Homogeneous:   " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 3. NBody using colls. Heterogeneous version.
   t = nbody_coll_rda (world, n_bodies);
   cout << "NBody RDA - COLL Homogeneous:    " << fixed << std::setprecision (9) << t << " usec." << endl;
   
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
   
   cout << endl << "Round Robin Mapping: " << endl;
   map->show();
   
   // 1. NBody using p2p. Heterogeneous version.
   t = nbody_p2p (world, n_bodies);
   cout << "NBody P2P Homogeneous:           " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 2. NBody using colls. Heterogeneous version.
   t = nbody_coll_ring (world, n_bodies);
   cout << "NBody Ring - COLL Homogeneous:   " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 3. NBody using colls. Heterogeneous version.
   t = nbody_coll_rda (world, n_bodies);
   cout << "NBody RDA - COLL Homogeneous:    " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   
   delete map;
   
   delete world;
   
   
   
   
   
   // Irregular mapping with:
   //   P : number of processes
   //   Q : number of processes in each node
   //   M : number of nodes
   Communicator *irr_comm = new Communicator (P);
   int nodes[P];
   int M = 4; // number of nodes
   srand(123);
   for (int i = 0; i < P; i++) {
      nodes[i] = rand() % M; // Random node for testing
   }
   Mapping *irr_map = new Mapping(P, nodes);
   irr_comm->map(irr_map);
   
   cout << endl << "Irregular Mapping: " << endl;
   irr_map->show();
   
   // 1. NBody using p2p. Heterogeneous version.
   t = nbody_p2p (irr_comm, n_bodies);
   cout << "NBody P2P Homogeneous:           " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 2. NBody using colls. Heterogeneous version.
   t = nbody_coll_ring (irr_comm, n_bodies);
   cout << "NBody Ring - COLL Homogeneous:   " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   // 3. NBody using colls. Heterogeneous version.
   t = nbody_coll_rda (irr_comm, n_bodies);
   cout << "NBody RDA - COLL Homogeneous:    " << fixed << std::setprecision (9) << t << " usec." << endl;
   
   delete irr_map;
   delete irr_comm;
   
   
   return 0;
}
