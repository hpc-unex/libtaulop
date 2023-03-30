//
//  taulop_concurrent.cpp
//  TauLopCost
//
//  Created by jarico on 9/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "taulop_concurrent.hpp"
#include "transmission.hpp"
#include "taulop_config.h"
#include "taulop_operator.hpp"

#include <list>
#include <iostream>
using namespace std;


TauLopConcurrent::TauLopConcurrent () {
   
}


TauLopConcurrent::~TauLopConcurrent () {
   
}


void TauLopConcurrent::add (TauLopSequence *s) {
   this->l_conc.push_back(s);
}


TauLopSequence * TauLopConcurrent::get () {
   return this->l_conc.front();
}


bool TauLopConcurrent::empty () {
   return this->l_conc.empty();
}


void TauLopConcurrent::evaluate (TauLopCost *tc) {
   
   Transmission *c = nullptr;
   
#if TLOP_DEBUG == 1
   // DBG:
   this->show();
#endif
   
   bool remain;
   
   // 1. Calculate the cost in parts. Each minimum part of concurrent comms is
   //    found and added to the cost. Then, it is removed from the rest.
   while (true) {
            
      // 1a. An operator is the algorithm for calculating costs
      TauLopOperator *opr = new TauLopOperator ();
      
      remain = false;
      
      // 1b. Get the first element in each Sequential (concurrent comms) for operating
      TauLopSequence *seq = nullptr;
      list<TauLopSequence *>::iterator it;
      
      for (it = this->l_conc.begin(); it != this->l_conc.end(); it++) {

         seq = *it;
         
         if (! seq->empty()) {
            c = seq->get();
            opr->add(c);
            remain = true;
         }
         
      }
      if (!remain) break;
      
      // 1c. Apply the algorithm and take the minimum part that is processable
#if TLOP_DEBUG == 1
      // DBG:
      opr->show_init_comms();
#endif
      
      opr->evaluate();
      Transmission min_c = opr->getMinCost();
      
#if TLOP_DEBUG == 1
      // DBG:
      opr->show_concurrent();
      
      // DBG:
      cout << "Minimum cost: " << endl; min_c.show();
#endif
      
      // 1d. Update the current communication. The cost processed is substracted from the
      //     rest of the comms. in the PBR
      for (it = this->l_conc.begin(); it != this->l_conc.end(); it++) {
         seq = *it;
         
         if (! seq->empty()) {
            Transmission *c = seq->get();
            
            if (c != nullptr) {
               int tau = opr->getConcurrency(c);
               if (tau == 0)
                  cout << "DBG: Error, Comm not found in the concurrent list." << endl;
               seq->substract(min_c.getCost(), tau);
               
            }
         }
         
      }
      
      // 1e. Add the interval size to the cost.
      tc->add(min_c);
            
#if TLOP_DEBUG == 1
      // DBG:
      tc->show();
#endif
      
      delete opr;
      
   }
}


void TauLopConcurrent::show () {
   
   int  count = 1;
   
   list<TauLopSequence *>::iterator it;
   for (it = this->l_conc.begin(); it != this->l_conc.end(); it++) {
      
      cout << "Seq[" << count << "] *-> " << endl;
      count++;
      
      (*it)->show();
   }
   
}

