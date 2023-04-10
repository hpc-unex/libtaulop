//
//  computation.cpp
//
//  Created by jarico on 7/4/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "computation.hpp"


Computation::Computation () {
   this->n       = 1;
   this->m       = 0;
   this->tau     = 1;
   this->opType  = OpType::DEFAULT;
   
   this->ceType  = CEType::Computation;
}


Computation::Computation (const Process &p_src, int n, int m) {
   this->process = p_src;
   this->n       = n;
   this->m       = m;
   this->tau     = 1;
   this->opType  = OpType::DEFAULT;
   
   this->ceType  =  CEType::Computation;
}


Computation::Computation (const Process &p_src, int n, int m, OpType opType) {
   this->process = p_src;
   this->n       = n;
   this->m       = m;
   this->tau     = 1;
   this->opType  = opType;
   
   this->ceType  =  CEType::Computation;
}


Computation::Computation (const Process &p_src, int m) {
   this->process = p_src;
   this->n       = 1;
   this->m       = m;
   this->tau     = 1;
   this->opType  = OpType::DEFAULT;
   
   this->ceType  =  CEType::Computation;
}


Computation::Computation (const Process &p_src, int m, OpType opType) {
   this->process = p_src;
   this->n       = 1;
   this->m       = m;
   this->tau     = 1;
   this->opType  = opType;
   
   this->ceType  =  CEType::Computation;
}


Computation::Computation (int m) {
   this->n       = 1;
   this->m       = m;
   this->tau     = 1;
   this->opType  = OpType::DEFAULT;
   
   this->ceType  =  CEType::Computation;
}


Computation::Computation (int m, OpType opType) {
   this->n       = 1;
   this->m       = m;
   this->tau     = 1;
   this->opType  = opType;
   
   this->ceType  =  CEType::Computation;
}


Computation::Computation (const Computation *c) {
   this->process = c->process;
   this->n       = c->n;
   this->m       = c->m;
   this->tau     = c->tau;
   this->opType  = c->opType;
   
   this->ceType  = c->ceType;
}


Computation::~Computation () {
   
}


Computation * Computation::clone() const {
   return new Computation(*this);
}


CEType Computation::getType () const {
   return this->ceType;
}


OpType Computation::getOpType () const {
   return this->opType;
}


int Computation::getRank () const {
   return this->process.getRank();
}


void Computation::initTau () {
   this->tau = 0;
}


void Computation::incrTau (int inc) {
   this->tau += inc;
}


double Computation::getCost () const {
   
   double t = 0.0;
   
   t = this->params->getTime(m, (int)this->opType);
   
   return t * n;
}


long Computation::getBytes (double t, int tau) const {
   
   long b = 0;
   
   b = this->params->getBytes(t, (int)this->opType);
   
   return b;
}


void Computation::getOverlap (const CostElement *c) {
   
   // Create a communication representing the overlap between two concurrent
   //  communications through the same channel.
   // They are concurrent over the minimum value of m and tau is increased.
   // Both communications have the same n
   if (this->n != c->getN())
      cout << "DBG Error: Comms have to have the same m" << endl;
   
   if (this->m > c->getM()) {
      this->m = c->getM();
   }
   
   this->tau += c->getTau();
}


bool Computation::areCompactable (const Computation *c) {
   
   if (this->tau == c->getTau())
      return true;
   else return false;
}


void Computation::compact (const Computation *c) {
   // Used for showing. Compact comms in the same channel and with the same tau
   //  Result has m=1, and all the wieght is in n
   this->m = this->m + (c->getM() * c->getN());
}


bool Computation::areSequential (const Computation *c) {
    
   if (this->process.getRank() == c->process.getRank()) {
      return true;
   }
   return false;
}


void Computation::add (const Computation *c) {
   // Communications must have the same n
   cout << "TBD: Comm::add" << endl;
}


Computation & Computation::operator= (const Computation &c) {
   
   this->process  = c.process;
   this->n        = c.n;
   this->m        = c.m;
   this->tau      = c.tau;
   
   return *this;
}


void Computation::show () const {
   
   cout << process.getRank() << endl;
   cout << this->m << "  " << this->n << endl;
   cout << "(" << this->n * this->m << ")" << endl;
   cout << this->tau << " ||" << endl;
   cout << "t= " << this->getCost() << endl;
   cout << endl;
}


void Computation::notate () const {
   
   string op_types [] = {"def", "max", "min", "sum", "prod", "land", "band", "lor", "bor", "lxor", "bxor", "maxloc", "minloc"};
   string op_type = op_types[(int)this->getOpType()];
   
   cout << this->getN() << "x"
        << this->getTau() << "||"
        << "g^" << op_type
        << "("  << this->getM() << ")";
}

