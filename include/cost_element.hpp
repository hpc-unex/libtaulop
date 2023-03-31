//
//  cost_element.hpp
//
//  Created by jarico on 31/3/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//
//   Represents a Cost imputable element (Transmission or Computation).
//

#ifndef cost_element_hpp
#define cost_element_hpp


#include <iostream>

// Class represents a cost imputable element (Transmission or Computation)

class CostElement {
      
public:
   
               CostElement ();
              ~CostElement ();
      
   virtual double getCost  ()          = 0;
   virtual long   getBytes (double t)  = 0;
   
};

#endif /* cost_element_hpp */
