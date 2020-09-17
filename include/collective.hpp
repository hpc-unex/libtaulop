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
#include "communicator.hpp"
#include "taulop_config.h"
#include "taulop_cost.hpp"

#include <iostream>
using namespace std;


class Collective {

private:
	
protected:
	
	Graph  graph;
    
public:
    
     Collective ();
    ~Collective ();
	
	void setGraph (Graph &g);
	
    virtual TauLopCost * evaluate (Communicator *comm, int *size, int root = RANK_UNDEFINED) = 0;
};

#endif /* collective_hpp */
