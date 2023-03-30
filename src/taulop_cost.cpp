//
//  taulop_cost.cpp
//  TauLopCost
//
//  Created by jarico on 9/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "taulop_cost.hpp"

#include "transmission.hpp"

#include <list>
#include <iostream>
using namespace std;


TauLopCost::TauLopCost () {
   
}


TauLopCost::TauLopCost (const TauLopCost *tc) {
   
   Transmission *T = nullptr;
   
   list<Transmission *>::const_iterator it;
   
   for (it = tc->l_cost.begin(); it != tc->l_cost.end(); it++) {
      T = *it;
      this->l_cost.push_back(new Transmission(T));
   }

}


TauLopCost::~TauLopCost () {
   
   Transmission *c = nullptr;
   while (! this->l_cost.empty()) {
      c = this->l_cost.front();
      delete c;
      this->l_cost.pop_front();
   }
   
}


void TauLopCost::add (Transmission *c) {
   
   // Duplicate the Transmission object.
   Transmission *c_aux = new Transmission (c);
   
   this->l_cost.push_back(c_aux);
}


void TauLopCost::add (const TauLopCost *tc) {
   
   Transmission *c   = nullptr;
   Transmission *aux = nullptr;
      
   list<Transmission *>::const_iterator it;
   
   for (it = tc->l_cost.begin(); it != tc->l_cost.end(); it++) {
      
      c = *it;
      
      aux = new Transmission(c);
      this->l_cost.push_back(aux);
   }
}


double TauLopCost::getTime () const {
   
   double  t = 0.0;
   
   list<Transmission *>::const_iterator it;
   for (it = this->l_cost.begin(); it != this->l_cost.end(); it++) {
      t = t + (*it)->getCost();
   }
   
   return t;
}


double TauLopCost::getTime (int chn) const {
   
   double  t = 0.0;
   
   list<Transmission *>::const_iterator it;
   for (it = this->l_cost.begin(); it != this->l_cost.end(); it++) {

      if ((*it)->getChannel() == chn) {
         t = t + (*it)->getCost();
      }
   }
   
   return t;
}


long TauLopCost::getVolume () const {
   
   long vol = 0;
   list<Transmission *>::const_iterator it;
   
   for (it = this->l_cost.begin(); it != this->l_cost.end(); it++) {
      vol = vol + ((*it)->getN() * (*it)->getM());
   }
   
   return vol;
}


long TauLopCost::getVolume (int chn) const {
   
   long vol = 0;
   
   list<Transmission *>::const_iterator it;
   for (it = this->l_cost.begin(); it != this->l_cost.end(); it++) {
         
      if ((*it)->getChannel() == chn) {
         vol = vol + ((*it)->getN() * (*it)->getM());
      }
   }
   
   return vol;
}



void TauLopCost::compact () {
   
   Transmission *c  = nullptr;
   Transmission *c2 = nullptr;
   
   list<Transmission *> l_aux;
   list<Transmission *>::iterator it;
   
   
   while (! this->l_cost.empty()) {
   
      it = this->l_cost.begin();
      if (it != this->l_cost.end()) {
         c = *it;
         c->putM(c->getM() * c->getN());
         c->putN(1);
         this->l_cost.erase(it);
      }
      
      while (it != this->l_cost.end()) {
         
         c2 = *it;
         
         if (c->areCompactable(c2)) {
            
            c->compact(c2);
            this->l_cost.erase(it);
            delete c2;
            
         } else {
            it++;
         }
         
      }
      
      // Insert in order: channel + tau
      Transmission *c_aux;
      
      bool enc = false;
      list<Transmission *>::iterator it_aux = l_aux.begin();

      while ((it_aux != l_aux.end()) && !enc) {
         c_aux = *it_aux;
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
   
   while (! l_aux.empty()) {
      c = l_aux.front();
      this->l_cost.push_back(c);
      l_aux.pop_front();
   }
   
}


void TauLopCost::show () const {
   
   Transmission *c = nullptr;
   
   list<Transmission *>::const_iterator it;
   for (it = this->l_cost.begin(); it != this->l_cost.end(); it++) {
      
      c = *it;
      
      cout << c->getN() << "x" << c->getTau() << "||T^" << c->getChannel() << "(" << c->getM() << ")";

      if (it != this->l_cost.end()) cout << " + ";
   }
   cout << endl;
}
