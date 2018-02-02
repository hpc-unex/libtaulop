//
//  heuristic_malik.hpp
//  TauLopCost
//
//  Created by jarico on 26/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef heuristic_malik_hpp
#define heuristic_malik_hpp

#include "heuristic.hpp"
#include "algorithm.hpp"
#include "arrangement.hpp"

#include <iostream>
using namespace std;

class MalikHeuristic : public Heuristic {

private:
    
    Algorithm *alg;
    
    /* Struct for storing an arrangment and its cost in time */
    struct Config {
        Arrangement *r;
        double       time;
    };
    typedef struct Config Config_t;

    // Given a Column combination, combine processes in the arrangement
    void createCombinations     (Arrangement *r, Config_t *c, int &num_cfg);

    
    // TEMPORAL
    string network;
    string comm_mode;

public:

    MalikHeuristic  (Algorithm *alg, string comm_mode, string network);
    ~MalikHeuristic ();

    virtual  Arrangement ** apply        (Arrangement *r);
    virtual  long int       combinations (Arrangement *r);

};

#endif /* heuristic_malik_hpp */
