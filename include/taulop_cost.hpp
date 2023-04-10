//
//  taulop_cost.hpp
//  TauLopCost
//
//  Created by jarico on 9/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef taulop_cost_hpp
#define taulop_cost_hpp

#include "cost_element.hpp"

#include <list>
#include <iostream>


class TauLopCost {
    
private:
    
    list<CostElement *> l_cost;
    
public:
    
           TauLopCost  ();
           TauLopCost  (const TauLopCost *tc);
          ~TauLopCost  ();
    
    void   add         (CostElement *c);
    void   add         (const TauLopCost *tc);
    void   compact     ();   

    double getTime     ()   const;
    long   getVolume   ()   const;
    
    void   show        ()   const;
    
};

#endif /* taulop_cost_hpp */
