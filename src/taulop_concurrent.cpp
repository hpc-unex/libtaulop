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


TauLopSequence * TauLopConcurrent::get () const {
   return this->l_conc.front();
}


bool TauLopConcurrent::empty () const {
   return this->l_conc.empty();
}


void TauLopConcurrent::evaluate (TauLopCost *tc) {
      
#if TLOP_DEBUG == 1
   this->show();
#endif
   
   bool remain;
   
   // 1. Calculate the cost in parts. Each minimum part of concurrent comms is
   //    found and added to the cost. Then, it is removed from the rest.
   while (true) {
            
      // 1a. An operator is the algorithm for calculating costs
      TauLopOperator opr;
      
      remain = false;
      
      // 1b. Get the first element in each Sequential (concurrent comms) for operating
      TauLopSequence *seq = nullptr;
      list<TauLopSequence *>::iterator it;
      
      for (it = this->l_conc.begin(); it != this->l_conc.end(); it++) {

         seq = *it;
         
         if (! seq->empty()) {
            Transmission *c = seq->get();
            opr.add(c);
            remain = true;
         }
         
      }
      if (!remain) break;
      
      // 1c. Apply the algorithm and take the minimum part that is processable
#if TLOP_DEBUG == 1
      opr.show_init_comms();
#endif
      
      opr.evaluate();
      Transmission *min_c = opr.getMinCost();
      
#if TLOP_DEBUG == 1
      opr.show_concurrent();
      
      cout << "Minimum cost: " << endl;
      min_c->show();
#endif
      
      // 1d. Update the current Transmissions.
      for (it = this->l_conc.begin(); it != this->l_conc.end(); it++) {
         
         seq = *it;
         
         if (! seq->empty()) {

            Transmission *T = seq->get();
            
            // How is progressing T with its concurrent transmissions?
            int tau_real = 1;
            if (T->areConcurrent(min_c)) {
               tau_real = min_c->getTau();       // Concurrent with the minimum
            } else {
               tau_real = opr.getConcurrency(T); // Concurrent with others (or none)
            }
            
            if (tau_real == 0) {
               cout << "ERROR: Comm not found in the concurrent list." << endl;
            }
            
            // Substract proportional part of min_c to the other transmissions.
            seq->substract(min_c, tau_real);
               
         }
         
      }
      
      // 1e. Add the interval size to the cost.
      tc->add(min_c);
      
#if TLOP_DEBUG == 1
      tc->show();
#endif
      
   }
}


void TauLopConcurrent::show () const {
   
   int  count = 1;
   
   list<TauLopSequence *>::const_iterator it;
   for (it = this->l_conc.begin(); it != this->l_conc.end(); it++) {
      
      cout << "Seq[" << count << "] *-> " << endl;
      count++;
      
      (*it)->show();
   }
   
}

