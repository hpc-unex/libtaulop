//
//  collective.hpp
//  Implement a collective algorithm
//
//  Created by jarico on 21/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef collective_hpp
#define collective_hpp

#include "graph.hpp"
#include "coll_params.hpp"
#include "communicator.hpp"
#include "taulop_config.h"
#include "taulop_cost.hpp"
#include "transmission.hpp"
#include "computation.hpp"

#include <iostream>
using namespace std;


class Collective {
   
private:
   
protected:
   
   Graph  graph;
   
public:
   
         Collective ();
        ~Collective ();
   
   void  setGraph (Graph &g);

   // ABC. 
   virtual TauLopCost * evaluate (Communicator *comm, const CollParams &cparams) = 0;
};

#endif /* collective_hpp */
