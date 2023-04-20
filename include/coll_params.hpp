//
//  coll_params.hpp
//  Collective operation parameters.
//    Different collective operations requires different parameters.
//    However, this avoids a common interface. Pack all parameters in a structure
//     (class CollParams):
//      - root: for rooted-collectives (broacast, gather/scatter, reduce)
//      - m:    size for simple collectives. A vector of sizes for _v collectives.
//      - op:   operation for reduced-collectives (reduce and allreduce)
//
//  Created by jarico on 20/04/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef coll_params_hpp
#define coll_params_hpp

#include "taulop_config.h"
#include <vector>

#include <iostream>
using namespace std;


// MPI operation types
// DEFAULT = SUM, arbitrary.
enum class OpType {DEFAULT, MAX, MIN, SUM, PROD, LAND, BAND, LOR, BOR, LXOR, BXOR, MAXLOC, MINLOC};


class CollParams {
   
private:
   
   int          root;
   vector<int>  m;
   OpType       op;
   
public:
   
    CollParams ();
    CollParams (int         m, int root);              // Broadcast, gather, scatter, etc.
    CollParams (int         m);                        // Allgather, alltoall, etc.
    CollParams (int         m, int root,  OpType op);  // Reduce
    CollParams (int         m,            OpType op);  // Allreduce
    CollParams (vector<int> m, int root);              // Scatter_v, gather_v, etc.
   
   ~CollParams ();
   
   int    getRoot ()       const;
   int    getM    ()       const;
   int    getM    (int p)  const;
   OpType getOp   ()       const;
   
   void   show    ()       const;
   
};

#endif /* coll_params_hpp */
