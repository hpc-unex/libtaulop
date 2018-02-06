//
//  simple.cpp
//  communicators
//
//  Created by jarico on 20/4/17.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "taulop_kernel.hpp"

#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;


const int P = 8;

int main (int argc, const char * argv[]) {
    
    TauLopParam::setInstance("IB");
    
    // 1. Create a communicator (group of P processes)
    //    Default mapping is all processes in the same node.
    Communicator *world = new Communicator (P);
    
    
    // 2. Create the virtual topology (mapping of processes to nodes)
    Mapping *map = new Mapping(P);
    // 3.1. Assign the mapping to a communicator
    world->map(map);
    world->show();
    
    // 3. Execute an algorithm
    Collective *bin = new BcastBinomial();
    int bsize = 128 * 1024;
    TauLopCost *t_bin = bin->evaluate(world, &bsize, 0);

    cout << "Time: " << fixed << std::setprecision (3) << t_bin->getTime() << endl;
    delete t_bin;
    
    // 5. Delete objects
    delete (BcastBinomial *)bin;
    delete world;
    return 0;
}



