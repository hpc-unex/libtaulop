//
//  taulop_concurrent.cpp
//  TauLopCost
//
//  Created by jarico on 9/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "taulop_concurrent.hpp"
#include "cost_element.hpp"
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
            CostElement *c = seq->get();
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
      CostElement *min_c = opr.getMinCost();
      
#if TLOP_DEBUG == 1
      opr.show_concurrent();
      
      cout << "Minimum cost: " << endl;
      min_c->show();
#endif
      
      // 1d. Update the current Transmissions.
      for (it = this->l_conc.begin(); it != this->l_conc.end(); it++) {
         
         seq = *it;
         
         if (! seq->empty()) {

            CostElement *C = seq->get();
            long curr_m = C->getM();
            
            // How is progressing T with its concurrent transmissions?
            int tau_real = 1;
            long overlap = 0;
            
            // 1a. If transmission T and min_t are concurrent, just reduce T in "min_t->m" bytes
            if (opr.areConcurrent(C, min_c)) {
               //tau_real = min_c->getTau();       
               overlap  = curr_m - min_c->getM();

            // 1b. If they are not concurrent, reduce T in the number of bytes that T transmits
            //      in the time in which min_t is working. (Important) Note that T progresses
            //      concurrently with other transmissions (or maybe not), that is the reason for
            //      tau_real, that is found in Operator::getConcurrency (in list l_conc_real).
            } else {
               tau_real = opr.getConcurrency(C); // Concurrent with others (or none)
               overlap  = curr_m - C->getBytes(min_c->getCost(), tau_real);
            }
            
#if TLOP_DEBUG == 1
            if (tau_real == 0) {
               cout << "ERROR: Comm not found in the concurrent list." << endl;
            }
            
            if (overlap < 0) {
               // TODO: aquí hay un problema porque overlap a veces es menor que 0
               //       y no deberia. Viene de la parte "else" anterior. No sé el
               //       motivo. HAY QUE ESTUDIARLO DESPACIO.
               //       Por ahora, se asume el error.
               cout << "ERROR: overlap should be >= 0: " << overlap << endl;
            }
#endif
            // 2. If they fully overlap, delete. Otherwise, reduce proportional m.
            if(overlap > 0) {
               C->putM(overlap);
            } else {
               seq->remove();
               delete C;
            }
               
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

