//
//  computation.cpp
//  TauLopCost
//
//  Created by jarico on 27/3/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "computation.hpp"
#include "taulop_params.hpp"



Computation::Computation () {
    this->node  =  NODE_UNDEFINED;
    this->m     =  0;
    this->op    =  0;
    
    this->params = TauLopParam::getInstance();
}


Computation::Computation (const Process &proc, int m, int op) {
    this->proc  = proc;
    this->node  = proc.getNode();
    this->m     = m;
    this->op    = op;
    
    this->params = TauLopParam::getInstance();
}


Computation::Computation (const Computation &c) {
    this->proc  = c.proc;
    this->node  = c.node;
    this->m     = c.m;
    this->op    = c.op;
    
    this->params = TauLopParam::getInstance();
}


Computation::~Computation () {
    
}


void Computation::putProcess (const Process &p) {
    this->proc = p;
}


int Computation::getRank () const {
    return this->proc.getRank();
}


int Computation::getNode () const {
    return this->node;
}


void Computation::putM (long m) {
    this->m = m;
}


long Computation::getM () const {
    return this->m;
}


double Computation::getCost () {
    
    double t = 0.0;
    
    t = this->params->getTime(m, 1, 0);  // TODO: eliminar channel
    
    return t;
}


long Computation::getBytes (double t) {
  
    long b = 0;
    
    b = this->params->getBytes(t, 1, 0);  // TODO: eliminar channel
    
    return b;
}


void Computation::compact (const Computation &c) {
    // Used for showing. Compact comms in the same channel and with the same tau
    //  Result has m=1, and all the wieght is in n
    this->m = this->m + c.m;
}


void Computation::add (const Computation &c) {
    // Communications must have the same n
    cout << "TBD: Comm::add" << endl;
}


void Computation::show () {
    
    cout << proc.getRank() << endl;
    cout << this->m << endl;
    cout << this->node << endl;
    cout << "t= " << this->getCost() << endl;
    cout << endl;
}

