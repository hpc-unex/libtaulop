//
//  algorithm_malik.hpp
//  TauLopCost
//
//  Created by jarico on 22/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef algorithm_malik_hpp
#define algorithm_malik_hpp

#include "algorithm.hpp"
#include "arrangement.hpp"
#include "cost.hpp"

#include <iostream>


/* 
 
 Calculate the VOLUME of communications in the arrangement
 as the Tanja Malik cost in "Network-aware optimization of
 communications for parallel matrix multiplication on hierarchical
 HPC platforms", by Malik, Rychkov and Lastovetsky.
 
 The volume is calculated in BLOCKS.
 
 Steps:
 
 (1) Horizontal
 
 (2) Vertical
 
 (3) Total cost = horizontal cost + vertical cost

 */

class MalikAlgorithm : public Algorithm {
    
private:
    
public:
    
    MalikAlgorithm  ();
    ~MalikAlgorithm ();
    
    virtual  Cost * execute (Arrangement *arrang);
};

#endif /* algorithm_malik_hpp */
