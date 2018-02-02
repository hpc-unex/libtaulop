//
//  algorithm_basic.hpp
//  TauLopCost
//
//  Created by jarico on 22/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef algorithm_basic_hpp
#define algorithm_basic_hpp

#include "algorithm.hpp"
#include "arrangement.hpp"
#include "cost.hpp"

#include <iostream>


/* 
 Calculate the total VOLUME of communications in the arrangement
 as the *sum* of the columns and rows.
 
 The volume is calculated in BLOCKS.
 
 Steps:
 
 (1) Horizontal
     One rectangle (process) communicates to ALL the others in the same row.
     Horizontal cost is the SUM of all processes costs.
 (2) Vertical
     One rectangle (process) communicate to ALL the others in the same column.
     A column cost is the sum of all proesses costs in the column.
     Vertical cost is the SUM of all columns costs.
 (3) Total cost = horizontal cost + vertical cost
 */

class BasicAlgorithm : public Algorithm {
    
private:
    
public:
    
    BasicAlgorithm  ();
    ~BasicAlgorithm ();
    
    virtual  Cost * execute (Arrangement *arrang);
};

#endif /* algorithm_basic_hpp */
