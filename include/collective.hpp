//
//  collective.hpp
//  Implement a collective algorithm
//
//  Created by jarico on 21/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef collective_hpp
#define collective_hpp

#include "communicator.hpp"
#include "taulop_config.h"

#include <iostream>
using namespace std;


class Collective {
    
private:
    
public:
    
    Collective  ();
    ~Collective ();
    
    virtual double evaluate (Communicator *comm, int *size, int root = RANK_UNDEFINED) = 0;
};

#endif /* collective_hpp */
