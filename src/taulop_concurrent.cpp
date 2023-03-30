//
//  taulop_concurrent.cpp
//  TauLopCost
//
//  Created by jarico on 9/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "taulop_concurrent.hpp"
#include "transmission.hpp"
#include "ilist.h"
#include "taulop_config.h"
#include "taulop_operator.hpp"

#include <iostream>
using namespace std;


TauLopConcurrent::TauLopConcurrent () {
   this->l_conc = new IList<TauLopSequence *> ();
}

TauLopConcurrent::~TauLopConcurrent () {
   
   TauLopSequence *s = nullptr;
   
   this->l_conc->moveToBegin();
   while (! this->l_conc->empty()) {
      this->l_conc->consult(s);
      delete s;
      this->l_conc->remove();
   }
   
   delete this->l_conc;
}



void TauLopConcurrent::add (TauLopSequence *s) {
   this->l_conc->moveToEnd();
   this->l_conc->next();
   this->l_conc->insert(s);
}



TauLopSequence * TauLopConcurrent::get () {
   
   TauLopSequence *s = nullptr;
   
   this->l_conc->moveToBegin();
   this->l_conc->consult(s);
   
   return s;
}


bool TauLopConcurrent::empty () {
   if (this->l_conc->empty())
      return true;
   return false;
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
      
      Transmission *min_c = nullptr;
      
      // 1a. An operator is the algorithm for calculating costs
      TauLopOperator *opr = new TauLopOperator ();
      
      remain = false;
      
      // 1b. Get the first element in each Sequential (concurrent comms) for operating
      TauLopSequence *seq;
      this->l_conc->moveToBegin();
      while (! this->l_conc->end()) {
         this->l_conc->consult(seq);
         
         if (! seq->empty()) {
            c = seq->get();
            opr->add(c);
            remain = true;
         }
         
         this->l_conc->next();
      }
      if (!remain) break;
      
      // 1c. Apply the algorithm and take the minimum part that is processable
#if TLOP_DEBUG == 1
      // DBG:
      opr->show_init_comms();
#endif
      
      opr->evaluate();
      min_c = opr->getMinCost();
      
#if TLOP_DEBUG == 1
      // DBG:
      opr->show_concurrent();
      
      // DBG:
      cout << "Minimum cost: " << endl; min_c->show();
#endif
      
      // 1d. Update the current communication. The cost processed is substracted from the
      //     rest of the comms. in the PBR
      this->l_conc->moveToBegin();
      while (! this->l_conc->end()) {
         this->l_conc->consult(seq);
         
         if (! seq->empty()) {
            Transmission *c = seq->get();
            
            if (c != nullptr) {
               int tau = opr->getConcurrency(c);
               if (tau == 0)
                  cout << "DBG: Error, Comm not found in the concurrent list." << endl;
               seq->substract(min_c->getCost(), tau);
               
            }
         }
         
         this->l_conc->next();
      }
      
      // 1e. Add the interval size to the cost.
      tc->add(min_c);
      
      delete min_c;
      
#if TLOP_DEBUG == 1
      // DBG:
      tc->show();
#endif
      
      delete opr;
      
   }
}


void TauLopConcurrent::show () {
   
   TauLopSequence *seq = nullptr;
   int  count = 1;
   
   this->l_conc->moveToBegin();
   while (! this->l_conc->end()) {
      cout << "Seq[" << count << "] *-> " << endl;
      count++;
      
      this->l_conc->consult(seq);
      seq->show();
      this->l_conc->next();
   }
   
}

