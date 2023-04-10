//
//  taulop_operator.hpp
//  TauLopCost
//
//  (23-03-29): Using STL list template (instead of IList)
//
//  Created by jarico on 9/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef taulop_operator_hpp
#define taulop_operator_hpp

#include "cost_element.hpp"

#include <list>
#include <iostream>


const int CHANNEL_NR = 2;

class TauLopOperator {
   
private:
      
   // Initial communication starting at the same time (possibly concurrent)
   list<CostElement *> l_comm;
   // Real concurrent communication from l_comm
   list<CostElement *> l_real_conc;
   
   void  show  (list<CostElement *> l) const;
   
public:
   
          TauLopOperator  ();
         ~TauLopOperator  ();
   
   void   add             (CostElement *c);
   void   evaluate        ();
   CostElement*  getMinCost ();
   int    getConcurrency  (const CostElement *c); // Get the concurrency (tau) of the comm in the same channel
   
   bool   areConcurrent   (const CostElement *c1, const CostElement *c2) const;
   
   void   show_init_comms ()  const;
   void   show_concurrent ()  const;
   void   show_channel    ()  const;
};

#endif /* taulop_operator_hpp */
