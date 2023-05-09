//
//  mapping.hpp
//  taulop
//
//  Created by jarico on 17/11/16.
//  Copyright © 2016 University of Extremadura. All rights reserved.
//

#ifndef mapping_hpp
#define mapping_hpp

#include <iostream>


// Mapping types
//  Default:    all processes to node 0 (ocerride mapping and number of
//               nodes/processes per node).
//  Sequential: processes are placed to fill node 0, and then node 1, etc.
//  RoundRobin: processes are placed to node 0, node 1, etc. with wraparound.
//  Random:     processes are placed randomly in nodes (controls the number of processes
//               per node)
enum class Map {Default, Sequential, RoundRobin, Random};


class Mapping {
    
protected:
    
    int  P; // # of processes
    // Array of nodes per rank.
    //  Index: rank in the communicator
    //  Value: number of node (in taulop code is 0 .. M-1)
    int *nodes;
    
public:
    
     Mapping  (int P);
     Mapping  (int P, int *nodes);
     Mapping  (int P, int Q, Map map);
    ~Mapping  ();
    
    Mapping& operator = (const Mapping &m);
    
    int getNode (int p);
    int getP();
    
    void show ();
};

#endif /* mapping_hpp */
