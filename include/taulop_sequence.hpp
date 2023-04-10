//
//  taulop_sequence_hpp
//
//  Created by jarico on 8/4/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef taulop_sequence_hpp
#define taulop_sequence_hpp

#include "cost_element.hpp"

#include <list>
#include <iostream>

class TauLopSequence {
   
private:
   
   list<CostElement *> l_seq;
   
public:
   
            TauLopSequence  ();
           ~TauLopSequence  ();
   
   void         add       (CostElement *c);
   CostElement *get       ()  const;
   void         remove    ();
   bool         empty     ()  const;
   void         compact   ();
   
   // TBD ??
   //void   apply     (TaulopCost *tc);
   
   void         show      ()  const;
};

#endif /* taulop_sequence_hpp */
