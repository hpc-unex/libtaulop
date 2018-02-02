//
//  heuristic.hpp
//  TauLopCost
//
//  Created by jarico on 26/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef heuristic_hpp
#define heuristic_hpp

#include "algorithm.hpp"
#include "arrangement.hpp"

#include <iostream>
using namespace std;

class Heuristic {

protected:

//    Algorithm *alg;

//    virtual Heuristic (Algorithm *alg) = 0; // Force to use an algorithm on creation
    
    // Utility functions
    void swap(int *x, int *y);
    long int factorial (int n);
    void permute (int *nodes, int l, int r, int *perm, int &idx);

    void generate_permutations (int *nodes, int r, int *perm);
        
public:
    
    virtual Arrangement ** apply        (Arrangement *r) = 0;
    virtual long int       combinations (Arrangement *r) = 0;
    
//    ~Heuristic ();
    
};

#endif /* heuristic_hpp */
