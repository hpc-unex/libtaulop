//
//  taulop_cost.hpp
//  TauLopCost
//
//  Created by jarico on 9/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef taulop_cost_hpp
#define taulop_cost_hpp

#include "ilist.h"
#include "transmission.hpp"

#include <iostream>


class TauLopCost {
    
private:
    
    IList<Transmission *> *cost;
    
public:
    
     TauLopCost ();
     TauLopCost (const TauLopCost *tc);
    ~TauLopCost ();
    
    void   add     (Transmission *c);
    void   add     (const TauLopCost *tc);
    void   compact ();  // Show cost in BLOCKS

    double getTime     ();
    double getTime     (int chn);
    long   getVolume   ();
    long   getVolume   (int chn);
    
    void   show ();
    
};

#endif /* taulop_cost_hpp */
