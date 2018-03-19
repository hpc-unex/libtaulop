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


// Default mappings constants
const int MAPPING_UNDEF = 0; // Undefined (random)
const int MAPPING_SEQ   = 1; // Sequential mapping  (p / Q, Q = #cores/node)
const int MAPPING_


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
    Mapping  (int P, int Q, int predef_map);
    ~Mapping ();
    
    int getNode (int p);
    int getP();
    
    void show ();
};

#endif /* mapping_hpp */
