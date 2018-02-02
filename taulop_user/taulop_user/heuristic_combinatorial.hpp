//
//  heuristic_combinatorial.hpp
//  TauLopCost
//
//  Created by jarico on 28/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef heuristic_combinatorial_hpp
#define heuristic_combinatorial_hpp


#include "heuristic.hpp"
#include "algorithm.hpp"
#include "arrangement.hpp"

#include <iostream>
using namespace std;

class CombinatorialHeuristic : public Heuristic {
    
private:
    
    Algorithm *alg;
    
    /* Struct for storing an arrangment and its cost in time */
    struct Config {
        Arrangement *r;
        double       time;
    };
    typedef struct Config Config_t;
    
    // Create combinations of entire columns
    void createCombinationsCols (Arrangement *r, int col, Config_t *c[2], int &num_cfg);
    // Given a Column combination, combine processes in the arrangement
    void createCombinations     (Arrangement *r, int col, Config_t *c[2], int &num_cfg);
    
    // TEMPORAL
    string network;
    string comm_mode;
    
public:
    
     CombinatorialHeuristic  (Algorithm *alg, string comm_mode, string network);
    ~CombinatorialHeuristic  ();
    
    virtual  Arrangement ** apply        (Arrangement *r);
    virtual  long int       combinations (Arrangement *r);    
};

#endif /* heuristic_combinatorial_hpp */
