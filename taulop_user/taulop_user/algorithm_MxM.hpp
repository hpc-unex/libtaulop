//
//  algorithm_taulop_block.hpp
//  TauLopCost
//
//  Created by jarico on 5/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//
//  Matrix Multiplication SUMMA algorithm communication prediction cost.
//

#ifndef algorithm_MxM_hpp
#define algorithm_MxM_hpp

#include "algorithm.hpp"

#include "ilist.h"

#include <iostream>



class MxM_Algorithm : public Algorithm {
    
private:
    
public:
    
    MxM_Algorithm  ();
    ~MxM_Algorithm ();
    
    virtual  Cost * execute (Arrangement *r);
};


#endif /* algorithm_MxM_hpp */
