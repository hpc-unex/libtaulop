//
//  taulop_cost.cpp
//  TauLopCost
//
//  Created by jarico on 9/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "taulop_cost.hpp"

#include "cost_element.hpp"

#include <list>
#include <iostream>
using namespace std;


TauLopCost::TauLopCost () {
   
}


TauLopCost::TauLopCost (const TauLopCost *tc) {
   
   // Duplicate objects
   list<CostElement *>::const_iterator it;
   
   for (it = tc->l_cost.begin(); it != tc->l_cost.end(); it++) {
      this->l_cost.push_back((*it)->clone());
   }

}


TauLopCost::~TauLopCost () {

   // Delete transmissions
   while (!this->l_cost.empty()) {
      CostElement *T = this->l_cost.front();
      this->l_cost.pop_front();
      delete T;
   }

}


void TauLopCost::add (CostElement *c) {
   this->l_cost.push_back(c->clone());
}


void TauLopCost::add (const TauLopCost *tc) {
   
   // Duplicate objects
   list<CostElement *>::const_iterator it;
   
   for (it = tc->l_cost.begin(); it != tc->l_cost.end(); it++) {
      this->l_cost.push_back((*it)->clone());
   }
}


double TauLopCost::getTime () const {
   
   double  t = 0.0;
   
   list<CostElement *>::const_iterator it;
   for (it = this->l_cost.begin(); it != this->l_cost.end(); it++) {
      t = t + (*it)->getCost();
   }
   
   return t;
}


long TauLopCost::getVolume () const {
   
   long vol = 0;
   list<CostElement *>::const_iterator it;
   
   for (it = this->l_cost.begin(); it != this->l_cost.end(); it++) {
      vol = vol + ((*it)->getN() * (*it)->getM());
   }
   
   return vol;
}


void TauLopCost::compact () {
 /*
   CostElement *c = nullptr;
   
   list<CostElement *> l_aux;
   list<CostElement *>::iterator it;
   
   
   while (! this->l_cost.empty()) {
      
      // 1. Compact transmission
      c = this->l_cost.front();
      this->l_cost.pop_front();
      c->putM(c->getM() * c->getN());
      c->putN(1);

      // 2. Compact with the rest (sma echannel and tau)
      it = this->l_cost.begin();
      while (it != this->l_cost.end()) {
         
         CostElement *c2 = *it;
         
         if (c->areCompactable(c2)) {
            
            c->compact(c2);
            this->l_cost.erase(it);
            delete c2;
            
         } else {
            it++;
         }
         
      }
      
      // 3. Insert in order in aux: channel + tau
      bool enc = false;
      list<CostElement *>::iterator it_aux = l_aux.begin();

      while ((it_aux != l_aux.end()) && !enc) {
         CostElement *c_aux = *it_aux;
         if (c_aux->getChannel() > c->getChannel()) {
            enc = true;
         } else if ((c_aux->getChannel() == c->getChannel()) && (c_aux->getTau() > c->getTau())) {
            enc = true;
         } else {
            it_aux++;
         }
      }
      l_aux.insert(it_aux, c);
      
   }
   
   // 4. Restore member list
   while (! l_aux.empty()) {
      CostElement *c = l_aux.front();
      this->l_cost.push_back(c);
      l_aux.pop_front();
   }
*/
}


void TauLopCost::show () const {
      
   list<CostElement *>::const_iterator it;
   for (it = this->l_cost.begin(); it != this->l_cost.end(); ++it) {
      
      if (it != this->l_cost.begin()) cout << " + ";
      
      CostElement *c = *it;
      c->notate();
   }
   
   cout << endl;
}
