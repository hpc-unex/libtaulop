//
//  taulop_concurrent_hpp
//  TauLopCncurrent
//
//  Created by jarico on 30/10/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef taulop_concurrent_hpp
#define taulop_concurrent_hpp

#include <list>
#include "taulop_sequence.hpp"
#include "taulop_cost.hpp"

#include <iostream>


class TauLopConcurrent {
    
private:
    
    list<TauLopSequence *> l_conc;
    
public:
    
    TauLopConcurrent  ();
    ~TauLopConcurrent ();
    
    void             add       (TauLopSequence *s);
    TauLopSequence  *get       ();
    bool             empty     ();
    
    void             evaluate  (TauLopCost *tc);

    void  show ();
};

#endif /* taulop_concurrent_hpp */
