//
//  main.cpp
//  test_colls
//
//  Created by jarico on 20/4/17.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "taulop_kernel.hpp"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;



// Platform parameters
const int M = 4;
const int Q = 8;
const int P = 8;


// Evaluation parameters
const int MIN_SIZE = 128;
const int MAX_SIZE = 8 * 1024; // * 1024;

const int MIN_P = 2;
const int MAX_P = P;



double ex1_bcast (int P) {
   
   Communicator *world = new Communicator (P);
   world->show();
   
   Collective *bcast  = new BcastBinomial();
   int m = 128 * 1024;
   CollParams cp {m};
   
   TauLopCost *tc = bcast->evaluate(world, cp);
   double t = tc->getTime();
   
   delete tc;
   delete world;
   
   return t;
}

double ex2_bcast (int P) {
   
   Communicator *world = new Communicator (P);
   
   Mapping *map = new Mapping (P, Q, Map::Sequential);
   world->map(map);
   world->show();
   
   Collective *bcast  = new BcastBinomial();
   int m = 128 * 1024;
   CollParams cp {m};
   
   TauLopCost *tc = bcast->evaluate(world, cp);
   double t = tc->getTime();
   
   delete tc;
   delete map;
   delete world;
   
   return t;
}


double ex3_bcast (int P) {
   
   Communicator *world = new Communicator (P);
   
   Mapping *map = new Mapping(P, Q, Map::Sequential);
   world->map(map);
   world->show();
   
   Collective *bcast  = new BcastLinear();
   int m = 128 * 1024;
   CollParams cp {m};
   
   TauLopCost *tc = bcast->evaluate(world, cp);
   double t = tc->getTime();
   
   delete tc;
   delete map;
   delete world;
   
   return t;
}


int ex4_colls (int P) {
   
   double t = 0.0;
   
   // 1. Create a communicator (group of P processes)
   //    Default mapping is all processes in the same node.
   Communicator *world = new Communicator (P);
   
   
   // 2. Create collective algoithms
   Collective *bin  = new BcastBinomial();
   Collective *rda  = new AllgatherRDA();
   Collective *ring = new AllgatherRing();
   
   
   // 3. Create the virtual topology (mapping of processes to nodes)
   Mapping *map = new Mapping(P, Q, Map::Sequential);
   // 3.1. Assign the mapping to a communicator
   world->map(map);
   world->show();
   
   // 4. Set the ranks vector for creating a communicator
   int ranks[P];
   for (int p = 0; p < P; p++) {
      ranks[p] = p;
   }
   Communicator *comm_coll = NULL;
   
   // 5. Execute collectives over a range of p and m
   for (int p = MIN_P; p <= MAX_P; p *= 2) {
      
      // 5.1. Create the communicator derived from world with the first p processes
      comm_coll = world->create(p, ranks);
      
      // 5.2. Execute algorithms
      cout << endl << "P = " << p << "  \t Bcast Bin. \t Allg. RDA \t Allg. Ring " << endl;
      cout << "---------------------------------------------------------------" << endl;
      for (int m = MIN_SIZE; m <= MAX_SIZE; m *= 2) {
         
         cout << "[" << p << "," << m << "] \t ";
         
         CollParams cp {m, 0};
         
         TauLopCost *t_bin  = bin->evaluate (comm_coll, cp);
         cout << fixed << setprecision(9) << t_bin->getTime() << "  \t  ";
         delete t_bin;
         
         TauLopCost *t_rda  = rda->evaluate (comm_coll, cp);
         cout << fixed << setprecision(9) << t_rda->getTime() << "  \t  ";
         delete t_rda;
         
         TauLopCost *t_ring = ring->evaluate (comm_coll, cp);
         cout << fixed << setprecision(9) << t_ring->getTime() << "  \t  ";
         delete t_ring;
         
         cout << endl;
      }
      
      delete comm_coll;
   }
   
   
   // 6. Create a communicator with a random mapping
   int nodes[P/2];
   for (int i = 0; i < P/2; i++) {
      nodes[i] = rand() % M; // Random
   }
   Mapping *map_comm = new Mapping (P/2, nodes);
   Communicator *comm = world->create(P/2, ranks);
   comm->map(map_comm);
   comm->show();
   
   // 7. Cost of the binomial broadcast for different roots and P/2
   cout << endl << "P = " << comm->getSize() << "  \t Root = 0 \t 1 \t 2 ... " << endl;
   cout << "---------------------------------------------------------------" << endl;
   
   Mapping *map_seq = new Mapping (P/2, 4, Map::Sequential);
   comm->map(map_seq);
   
   for (int m = MIN_SIZE; m <= MAX_SIZE; m *= 2) {
      
      cout << "[" << comm->getSize() << "," << m << "] \t ";
      
      for (int root = 0; root < comm->getSize(); root++) {
         
         CollParams cp {m, root};
         
         TauLopCost *t_bin  = bin->evaluate (comm, cp);
         cout << fixed << setprecision(9) << t_bin->getTime() << "  \t  ";
         delete t_bin;
         
      }
      
      cout << endl;
   }
   
   delete comm;
   
   // 8. Delete objects
   delete (BcastBinomial *) bin;
   delete (AllgatherRDA  *) rda;
   delete (AllgatherRing *) ring;
   delete map;
   delete map_comm;
   delete map_seq;
   delete world;
   return 0;
}



int ex5_allgather () {
   
   
   Collective *rda  = new AllgatherRDA();
   Collective *ring = new AllgatherRing();
   
   const int P = 8;
   int nodes [P] = {0,1,1,2,0,2,1,0};
   Communicator *comm = new Communicator (P);
   Mapping *map = new Mapping(P, nodes);
   comm->map(map);
   comm->show();
   
   for (int m = MIN_SIZE; m <= MAX_SIZE; m *= 2) {
      
      cout << "[" << comm->getSize() << "," << m << "] \t ";
      
      CollParams cp {m};
      
      TauLopCost *t_rda  = rda->evaluate  (comm, cp);
      TauLopCost *t_ring = ring->evaluate (comm, cp);
      
      cout << fixed << setprecision(9) << t_rda->getTime()  << "  \t  ";
      cout << fixed << setprecision(9) << t_ring->getTime() << "  \t  ";
      
      delete t_rda;
      delete t_ring;
      
      cout << endl;
   }
   
   delete map;
   delete comm;
   
   delete (AllgatherRDA  *) rda;
   delete (AllgatherRing *) ring;
   return 0;
}




int main (int argc, const char * argv[]) {
   
   double t      = 0.0;
   
   // Network parameters
   TauLopParam::setInstance({"SHM","IB"});
   
   // Binomial broadcast (default mapping)
   t = ex1_bcast(16);
   cout << "Binomial Broadcast (def) - Time: " << fixed << setprecision(9) << t << endl << endl;
   
   // Binomial broadcast (Sequential mapping)
   t = ex2_bcast(16);
   cout << "Binomial Broadcast (SEQ) - Time: " << fixed << setprecision(9) << t << endl << endl;
   
   // Linear broadcast (Sequential mapping)
   t = ex3_bcast(16);
   cout << "Linear Broadcast (SEQ) - Time:   " << fixed << setprecision(9) << t << endl << endl;
   
   // Comparison of collectives
   ex4_colls(16);
   
   // Comparison of allgather algorithms
   ex5_allgather();
   
   return 0;
}



