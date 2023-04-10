//
//  taulop_operator.cpp
//  TauLopCost
//
//  Created by jarico on 29/3/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "transmission.hpp"
#include "cost_element.hpp"
#include "taulop_operator.hpp"

#include <list>
#include <iostream>
using namespace std;


// UTILITY Functions

CostElement* MIN (CostElement *a, CostElement *b) {
   
   if (a == nullptr) {
      return b;
   }
   
   if (b == nullptr) {
      return a;
   }
   
   if (a->getCost() > b->getCost()) {
      return b;
   }
   return a;
}



// PRIVATE methods


void TauLopOperator::show (list<CostElement *> l) const {
   
   CostElement *c = nullptr;
   
   list<CostElement *>::iterator it;
   for (it = l.begin(); it != l.end(); ++it) {
      c = *it;
      c->notate();
   }
   cout << endl;
}



// PUBLIC interface

TauLopOperator::TauLopOperator () {

}

TauLopOperator::~TauLopOperator () {
   // Delete l_real_conc transmissions
   while (!this->l_real_conc.empty()) {
      CostElement *T = this->l_real_conc.front();
      this->l_real_conc.pop_front();
      delete T;
   }
}


void TauLopOperator::add (CostElement *c) {
   this->l_comm.push_back(c);
}


void TauLopOperator::evaluate () {
   
   list<CostElement *>::iterator it;

   while (!this->l_comm.empty()) {
      
      CostElement *c_comm = l_comm.front();
      
      bool found = false;
      
      for (it = l_real_conc.begin(); (it != l_real_conc.end()) && !found; ++it) {
         
         CostElement *c_real = *it;
         
         if (this->areConcurrent(c_real, c_comm)) {
            found = true;
            c_real->getOverlap(c_comm);
         }
         
      }
      
      if (!found) {
         this->l_real_conc.push_back(c_comm->clone());
      }
      
      this->l_comm.pop_front();
            
   }
   
}


CostElement* TauLopOperator::getMinCost () {

   CostElement *c_min = nullptr;
   
   list<CostElement *>::iterator it;
   for (it = l_real_conc.begin(); it != l_real_conc.end(); ++it) {
      
      CostElement *c_aux = *it;
      c_min = MIN (c_aux, c_min);
      
   }
   
   return c_min;
}



int TauLopOperator::getConcurrency (const CostElement *c) {
   
   bool  found = false;
   int   tau   = 0;
   
   list<CostElement *>::iterator it;
   for (it = this->l_real_conc.begin(); (it != this->l_real_conc.end()) && !found; ++it) {
      
      CostElement *c_aux = *it;
      if (this->areConcurrent(c_aux, c)) {
         tau = c_aux->getTau();
         found = true;
      }

   }
   
   return tau;
}


bool TauLopOperator::areConcurrent (const CostElement *c1, const CostElement *c2) const {
   
   // TODO: Just to this method ends my OOP knowledge. A clean design should provide a way to
   //       compare CostElements independently of its actual type (Transmissions or Computation).
   //       I suppose the code needs a redesign. I do not know.
   //       Solution: use dynamic_cast and a type in each object (Transmission or Computation).
   //                 Uage of a type makes code easier (and probably more efficient).
   
   // We have 3 cases to consider two CostElement concurrent or parallel (without interaction).
   //  Note that not concurrent means that CostElements progress in parallel without interaction,
   //  and that if they are sequential, they are not evaluated here.
   
   // 1st case) A Transmission and a Computation are never considered concurrent.
   //           They progress without interaction.
   if (c1->getType() != c2->getType()) {
      return false;
   }
   
   // 2nd case) Both are computations. They are considered not to interact (by now), but
   //            they are concurrent if they execute in the same node.
   //            If they are in the same node and by the same process, they are sequential
   //            (this case is not covered here).
   if (c1->getType() == CEType::Computation) {
      if ((c1->getNode() == c2->getNode()) &&
          (c1->getRank() == c2->getRank()))    {
         return true;
      }
      return false;
   }
      
   // 3rd case) Condition for considering two communications as concurrent
   const Transmission* T1 = dynamic_cast<const Transmission *> (c1);
   const Transmission* T2 = dynamic_cast<const Transmission *> (c2);
   if ((T1->getChannel() == T2->getChannel()) &&
       (T1->getDstNode() == T2->getDstNode()) )   {
      
      return true;
      
   }
   
   return false;
}



void TauLopOperator::show_init_comms () const {
   cout << "Communications starting at the same time (possibly concurrent): " << endl;
   show(this->l_comm);
}


void TauLopOperator::show_concurrent () const {
   cout << "Concurrent communications (really concurrent): " << endl;
   show(this->l_real_conc);
}

