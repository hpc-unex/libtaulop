//
//  cost_element.cpp
//
//  Created by jarico on 31/3/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "cost_element.hpp"



CostElement::CostElement () {
   this->params = TauLopParam::getInstance();
}


CostElement::~CostElement () {
    
}


void CostElement::putM (long m) {
   this->m = m;
}


long CostElement::getM () const {
   return this->m;
}


void CostElement::putN (int n) {
   this->n = n;
}


int CostElement::getN () const {
   return this->n;
}


int CostElement::getTau() const {
   return this->tau;
}


void CostElement::putProcess (const Process &p) {
   this->process = p;
}


int CostElement::getNode () const {
   return this->process.getNode();
}

