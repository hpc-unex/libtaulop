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


void TauLopSequence::substract (double t_min, int tau) {
      
   if (! this->l_seq.empty()) {
      
      Transmission *c = this->l_seq.front();
      
#if BUG_T == 0
      // Substract proportional time in blocks
      //long overlap = (c->getN() * t_min) / c->getCost();
      long overlap = 0;
      
      //        if (c->areConcurrent(min_c)) {
      //            overlap = min_c->getN();
      //        } else {
      //            overlap = c->getBytes(t_min, 1);
      //        }
      
      overlap = c->getBytes(t_min, tau);
      
      if (overlap < 0)
         cout << "DBG ERROR: pbrcol - not allowed overlap < 0" << endl;
      
      long curr_m = c->getM();
            
      if ((curr_m - overlap) > 0) { // TBD: a threshold for rounding issues could be convenient
         c->putM(curr_m - overlap);
      } else { // == 0
         this->l_seq.pop_front();
      }
      
#else
      
      double tprime = c->getCost() - t_min;
      
      long   mx = 0;
      double epsilon = 0.001;
      
      if (tprime < epsilon) {
         tprime = 0.0;
         mx = 0;
      } else {
         mx = c->getBytes(tprime, tau);
      }
      
      if (mx < 0) {
         cout << "DBG ERROR: pbrcol - not allowed overlap < 0" << endl;
         mx = 0;
      }
      
      if (mx == 0) {
         this->l_seq.pop_front();
      } else {
         c->putN(mx);
      }
      
#endif
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

