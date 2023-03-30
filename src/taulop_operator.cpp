//
//  taulop_operator.cpp
//  TauLopCost
//
//  Created by jarico on 29/3/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "taulop_operator.hpp"



// UTILITY Functions

Transmission & MIN (Transmission &a, Transmission &b) {
   
   if (a.getM() == 0) {
      return b;
   }
   if (b.getM() == 0) {
      return a;
   }
      
   if (a.getCost() > b.getCost()) {
      return b;
   } else {
      return a;
   }
}



// PRIVATE methods


void TauLopOperator::show (list<Transmission *> l) {
   
   Transmission *c = nullptr;
   
   for (int field = 0; field < 7; field++) {
      
      list<Transmission *>::iterator it;
      for (it = l.begin(); it != l.end(); ++it) {
         
         c = *it;
         
         switch (field) {
            case 0: // show procs
               if (c != nullptr) {
                  cout << c->getSrcRank() << " -> " << c->getDstRank() << "\t\t";
               } else {
                  cout << "      " << "\t\t";
               }
               break;
            case 1: // show channel
               if (c != nullptr) {
                  cout << "  " << c->getChannel() << "    \t\t";
               } else {
                  cout << "        " << "\t\t";
               }
               break;
            case 2: // show n and m
               if (c != nullptr) {
                  cout << c->getM() << "  " << c->getN() << " \t\t";
               } else {
                  cout << "        " << "\t\t";
               }
               break;
            case 3: // show m x n
               if (c != nullptr) {
                  cout << "(" << c->getN() * c->getM() << ")  \t\t";
               } else {
                  cout << "        " << "\t\t";
               }
               break;
            case 4: // show tau
               if (c != nullptr) {
                  cout << " " << c->getTau() << " || \t\t";
               } else {
                  cout << "       " << "\t\t";
               }
               break;
            case 5: // show node_dst
               if (c != nullptr) {
                  cout << " -> " << c->getDstNode() << "  \t\t";
               } else {
                  cout << "        " << "\t\t";
               }
               break;
            case 6: // Time cost
               if (c != nullptr) {
                  cout << "t=" << c->getCost() << " \t";
               } else {
                  cout << "      " << "\t\t";
               }
               break;
               
         }
         
      }
      cout << endl;
      
   }
   cout << endl;
}


void TauLopOperator::show (list<Transmission> l) {
   
   for (int field = 0; field < 7; field++) {
      
      list<Transmission>::iterator it;
      for (it = l.begin(); it != l.end(); ++it) {
         
         Transmission &c = *it;
         
         switch (field) {
            case 0: // show procs
               cout << c.getSrcRank() << " -> " << c.getDstRank() << "\t\t";
               break;
            case 1: // show channel
               cout << "  " << c.getChannel() << "    \t\t";
               break;
            case 2: // show n and m
               cout << c.getM() << "  " << c.getN() << " \t\t";
               break;
            case 3: // show m x n
               cout << "(" << c.getN() * c.getM() << ")  \t\t";
               break;
            case 4: // show tau
               cout << " " << c.getTau() << " || \t\t";
               break;
            case 5: // show node_dst
               cout << " -> " << c.getDstNode() << "  \t\t";
               break;
            case 6: // Time cost
               cout << "t=" << c.getCost() << " \t";
               break;
               
         }
         
      }
      cout << endl;
      
   }
   cout << endl;
}




// PUBLIC interface

TauLopOperator::TauLopOperator () {

}

TauLopOperator::~TauLopOperator () {
   
}


void TauLopOperator::add (Transmission *c) {
   this->l_comm.push_back(c);
}


void TauLopOperator::evaluate () {
   
   Transmission *c_comm = nullptr;
   list<Transmission>::iterator it;

   while (!this->l_comm.empty()) {
      
      c_comm = l_comm.front();
      
      bool found = false;
      
      for (it = l_real_conc.begin(); (it != l_real_conc.end()) && !found; ++it) {
         
         Transmission &c_real = *it;
         
         if (c_real.areConcurrent(c_comm)) {
            found = true;
            c_real.getOverlap(c_comm);
         }
         
      }
      this->l_comm.pop_front();
      
      if (!found) {
         this->l_real_conc.push_back(*c_comm);
      }
            
   }
   
}


Transmission * TauLopOperator::getMinCost () {

   Transmission *c = nullptr;
   Transmission c_min;
   
   list<Transmission>::iterator it;
   for (it = l_real_conc.begin(); it != l_real_conc.end(); ++it) {
      
      Transmission &c_aux = *it;
      c_min = MIN (c_aux, c_min);
      
   }
   
   c = new Transmission(c_min);
   return c;
}



int TauLopOperator::getConcurrency (Transmission * c) {
   
   bool  found = false;
   int   tau   = 0;
   
   list<Transmission>::iterator it;
   for (it = l_real_conc.begin(); (it != l_real_conc.end()) && !found; ++it) {
      
      Transmission &c_aux = *it;
      if (c_aux.areConcurrent(c)) {
         tau = c_aux.getTau();
         found = true;
      }

   }
   
   return tau;
}


void TauLopOperator::show_init_comms () {
   cout << "Communications starting at the same time (possibly concurrent): " << endl;
   show(this->l_comm);
}


void TauLopOperator::show_concurrent () {
   cout << "Concurrent communications (really concurrent): " << endl;
   show(this->l_real_conc);
}

