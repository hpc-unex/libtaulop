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

#include "transmission.hpp"

#include <list>
#include <iostream>


const int CHANNEL_NR = 2;

class TauLopOperator {
   
private:
      
   // Initial communication starting at the same time (possibly concurrent)
   list<Transmission *> l_comm;
   // Real concurrent communication from l_comm
   list<Transmission> l_real_conc;
   
   void  show  (list<Transmission *> l);
   void  show  (list<Transmission> l);
   
public:
   
          TauLopOperator  ();
         ~TauLopOperator  ();
   
   void   add             (Transmission *  c);
   void   evaluate        ();
   Transmission  getMinCost ();
   int    getConcurrency  (Transmission *  c); // Get the concurrency (tau) of the comm in the same channel
   
   void   show_init_comms ();
   void   show_concurrent ();
   void   show_channel    ();
};

#endif /* taulop_operator_hpp */
