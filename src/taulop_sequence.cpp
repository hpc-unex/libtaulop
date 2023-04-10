//
//  algorithm_taulop_pbrcol.cpp
//
//  Created by jarico on 8/4/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "taulop_sequence.hpp"
#include "cost_element.hpp"
#include "taulop_operator.hpp"
#include "taulop_config.h"

#include <list>
#include <iostream>
using namespace std;


TauLopSequence::TauLopSequence () {
      
}


TauLopSequence::~TauLopSequence () {
   
}


void TauLopSequence::add (CostElement *c) {
   this->l_seq.push_back(c);
}


CostElement* TauLopSequence::get () const {
   return this->l_seq.front();
}


void TauLopSequence::remove() {
   this->l_seq.pop_front();
}


bool TauLopSequence::empty () const {
   return this->l_seq.empty();
}


void TauLopSequence::compact () {
   
   if (this->l_seq.empty()) {
      return;
   }
      
   CostElement *c = this->l_seq.front();
   
   list<CostElement *>::iterator it = this->l_seq.begin();
   it++;

   while (it != this->l_seq.end()) {
      
      CostElement *c2 = *it;
      TauLopOperator opr;
      
      if (opr.areConcurrent(c, c2) && (c->getTau() == c2->getTau())) {
         
         c->putM(c->getM() + c2->getM());
         this->l_seq.erase(it);
         delete c2;
         
      } else {
         it++;
         c = c2;
      }
   }
}


void TauLopSequence::show () const {
   
   list<CostElement *>::const_iterator it;
   for (it = this->l_seq.begin(); it != this->l_seq.end(); it++) {
      
      const CostElement *c = *it;
      c->notate();
      
      if (it != this->l_seq.end()) cout << "  *-/ ";
      else                         cout << "  *-> \t";
      
   }
   cout << endl;
}

