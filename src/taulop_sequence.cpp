//
//  algorithm_taulop_pbrcol.cpp
//  TauLopCost
//
//  Created by jarico on 9/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "taulop_sequence.hpp"
#include "transmission.hpp"
#include "taulop_config.h"

#include <list>
#include <iostream>
using namespace std;


TauLopSequence::TauLopSequence () {
      
}


TauLopSequence::~TauLopSequence () {
   
}


void TauLopSequence::add (Transmission *c) {
   this->l_seq.push_back(c);
}


Transmission* TauLopSequence::get () const {
   return this->l_seq.front();
}


void TauLopSequence::substract (Transmission *min_t, int tau_real) {
   
   Transmission *T = this->l_seq.front();
   long curr_m     = T->getM();
   
   
   // 1. Compute the overlap in bytes of transmissions
   long overlap = 0;

   if (min_t->areConcurrent(T)) {
      overlap = curr_m - min_t->getM();
   } else {
      overlap = curr_m - T->getBytes(min_t->getCost(), tau_real);
   }
   
   if (overlap < -100) { // TODO: aquí hay un problema porque overlap a veces es menor que 0
                      //       y no deberia. Viene de la parte "else" anterior. No sé el
                      //       motivo. HAY QUE ESTUDIARLO DESPACIO.
                      //       Por ahora, se asume el error.
      cout << "ERROR: overlap should be >= 0: " << overlap << endl;
   }
   
   // 2. If they fully overlap, delete. Otherwise, reduce proportional m.
   if (overlap > 0) { // TODO: a threshold for rounding issues could be convenient
      T->putM(overlap);
   } else {   // <= 0
      this->l_seq.pop_front();
      delete T;
   }

}


bool TauLopSequence::empty () const {
   return this->l_seq.empty();
}


void TauLopSequence::compact () {
   
   if (this->l_seq.empty()) {
      return;
   }
      
   Transmission *c = this->l_seq.front();
   
   list<Transmission *>::iterator it = this->l_seq.begin();
   it++;

   while (it != this->l_seq.end()) {
      
      Transmission *c2 = *it;
      
      if (c->areConcurrent(c2) && (c->getTau() == c2->getTau())) {
         
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
   
   for (int field = 0; field < 3; field++) {
      
      list<Transmission *>::const_iterator it;
      for (it = this->l_seq.begin(); it != this->l_seq.end(); it++) {

         const Transmission *c = *it;
         
         switch (field) {
            case 0: // show procs
               if (c != nullptr) {
                  cout << c->getSrcRank() << " -> " << c->getDstRank() << "\t\t\t";
               } else {
                  cout << "       " << "\t\t";
               }
               break;
            case 1: // show Transmission
               if (c != nullptr) {
                  cout << c->getTau() << "||"  << c->getN() << "xT^" << c->getChannel() << "(" << c->getM() << ")";
                  if (it != this->l_seq.end()) cout << "  *-/ ";
                  else                         cout << "  *-> \t";
               } else {
                  cout << "           " << "\t";
               }
               break;
            case 2: // show nodes
               if (c != nullptr) {
                  cout << c->getSrcNode() << " -> " << c->getDstNode() << "\t\t\t";
               } else {
                  cout << "          " << "\t\t";
               }
               break;
               
         }
         
      }
      cout << endl;
   }
   cout << endl;
}

