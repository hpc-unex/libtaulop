//
//  transmission.cpp
//  TauLopCost
//
//  Created by jarico on 3/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "transmission.hpp"
//#include "taulop_params.hpp"



Transmission::Transmission () {
   this->channel  = -1;
   this->n        =  0;
   this->m        =  0;
   this->tau      =  0;
   
   this->ceType   =  CEType::Transmission;
}


Transmission::Transmission (const Process &p_src, const Process &p_dst, int channel, int n, int m, int tau) {
   this->process  = p_src;
   this->p_dst    = p_dst;
   this->channel  = channel;
   this->n        = n;
   this->m        = m;
   this->tau      = tau;
   
   this->ceType   =  CEType::Transmission;
}


Transmission::Transmission (const Process &p_src, const Process &p_dst, int n, int m, int tau) {
   this->process  = p_src;
   this->p_dst    = p_dst;
   this->channel  = (this->process.getNode() == this->p_dst.getNode()) ? 0 : 1;
   this->n        = n;
   this->m        = m;
   this->tau      = tau;
   
   this->ceType   =  CEType::Transmission;
}

Transmission::Transmission (const Process &p_src, const Process &p_dst, int m, int tau) {
   this->process  = p_src;
   this->p_dst    = p_dst;
   this->channel  = (this->process.getNode() == this->p_dst.getNode()) ? 0 : 1;
   this->n        = 1;
   this->m        = m;
   this->tau      = tau;
   
   this->ceType   =  CEType::Transmission;
}

Transmission::Transmission (int channel, int n, int m, int tau) {
   this->channel  = channel;
   this->n        = n;
   this->m        = m;
   this->tau      = tau;
   
   this->ceType   =  CEType::Transmission;
}

Transmission::Transmission (int channel, int m, int tau) {
   this->channel  = channel;
   this->n        = 1;
   this->m        = m;
   this->tau      = tau;
   
   this->ceType   =  CEType::Transmission;
}


Transmission::Transmission (const Transmission *c) {
   this->process  = c->process;
   this->p_dst    = c->p_dst;
   this->channel  = c->channel;
   this->n        = c->n;
   this->m        = c->m;
   this->tau      = c->tau;
   
   this->ceType   = c->ceType;
}


Transmission::~Transmission () {
   
}


Transmission * Transmission::clone () const {
   return new Transmission(*this);
}


CEType Transmission::getType () const {
   return this->ceType;
}


void Transmission::putProcDst (const Process &p) {
   this->p_dst = p;
}


int Transmission::getMsgSize () const {
   return (this->n);
}


void Transmission::putChannel (int channel) {
   this->channel = channel;
}


int Transmission::getChannel () const {
   return this->channel;
}


int Transmission::getRank () const {
   return this->process.getRank();
}


int Transmission::getDstRank () const {
   return this->p_dst.getRank();
}


int Transmission::getDstNode () const {
   return this->p_dst.getNode();
}


void Transmission::initTau () {
   this->tau = 0;
}


void Transmission::incrTau (int inc) {
   this->tau += inc;
}


double Transmission::getCost () const {
   
   double t = 0.0;
   
   t = this->params->getTime(m, tau, channel);
   
   return t * n;
}


long Transmission::getBytes (double t, int tau) const {
   
   long b = 0;
   
   b = this->params->getBytes(t, tau, channel);
   
   return b;
}


void Transmission::getOverlap (const CostElement *c) {
   
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


bool Transmission::areCompactable (const Transmission *c) {
   
   if ((this->tau == c->getTau()) && (this->channel == c->getChannel()))
      return true;
   else return false;
}


void Transmission::compact (const Transmission *c) {
   // Used for showing. Compact comms in the same channel and with the same tau
   //  Result has m=1, and all the wieght is in n
   this->m = this->m + (c->getM() * c->getN());
}


bool Transmission::areSequential (const Transmission *c) {
    
   if (this->process.getRank() == c->process.getRank()) {
      return true;
   }
   return false;
}


void Transmission::add (const Transmission *c) {
   // Communications must have the same n
   cout << "TBD: Comm::add" << endl;
}


Transmission & Transmission::operator= (const Transmission &c) {
   
   this->process  = c.process;
   this->p_dst    = c.p_dst;
   this->channel  = c.channel;
   this->n        = c.n;
   this->m        = c.m;
   this->tau      = c.tau;
   
   return *this;
}


void Transmission::show () const {
   
   cout << process.getRank() << " -> " << p_dst.getRank() << endl;
   cout << channel << endl;
   cout << this->m << "  " << this->n << endl;
   cout << "(" << this->n * this->m << ")" << endl;
   cout << this->tau << " ||" << endl;
   cout << this->p_dst.getNode() << endl;
   cout << "t= " << this->getCost() << endl;
   cout << endl;
}


void Transmission::notate () const {
   cout << this->getN() << "x"
        << this->getTau() << "||"
        << "T^" << this->getChannel()
        << "(" << this->getM() << ")";
}
