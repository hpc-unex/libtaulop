//
//  algorithm_half_perimeter.hpp
//  TauLopCost
//
//  Created by jarico on 10/10/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef algorithm_half_perimeter_hpp
#define algorithm_half_perimeter_hpp

#include "algorithm.hpp"
#include "arrangement.hpp"
#include "cost.hpp"

#include <iostream>


/* 
 
 Calculate the "cost" of communications in the arrangement
 as the Beaumont half perimeter of the rectangles in the matrix.
 See: Beaumont et al, "Matrix Multiplication in Heterogeneous Platforms", 2001
 
 */

class HalfPerimeterAlgorithm : public Algorithm {
    
private:
    
public:
    
    HalfPerimeterAlgorithm  ();
    ~HalfPerimeterAlgorithm ();
    
    virtual  Cost * execute (Arrangement *arrang);
};

#endif /* algorithm_half_perimeter_hpp */
