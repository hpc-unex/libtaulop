//
//  algorithm_taulop_pbc.hpp
//  TauLopCost
//
//  Created by jarico on 9/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef algorithm_taulop_pbc_hpp
#define algorithm_taulop_pbc_hpp

#include "taulop_cost.hpp"
#include "arrangement.hpp"
#include "taulop_sequence.hpp"

#include <iostream>

class TauLopPBC {
    
private:
        
    int  overlap (Arrangement *r, Rectangle *p_src, Rectangle *p_dst, Rectangle *p_orig);
    void fillRow (TauLopSequence *seq, Arrangement *r, int b_ini, int col, int row);

public:
    
    TauLopPBC  ();
    ~TauLopPBC ();
    
    void  getPBCCost (Arrangement *r, double &time);
    
};

#endif /* algorithm_taulop_pbc_hpp */
