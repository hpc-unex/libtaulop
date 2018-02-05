//
//  transmission.cpp
//  TauLopCost
//
//  Created by jarico on 3/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "transmission.hpp"
#include "taulop_params.hpp"



Transmission::Transmission () {
    this->p_src    =  NULL;
    this->p_dst    =  NULL;
    this->node_src =  NODE_UNDEFINED;
    this->node_dst =  NODE_UNDEFINED;
    this->channel  = -1;
    this->n        =  0;
    this->m        =  0;
    this->tau      =  0;
    
    this->params = TauLopParam::getInstance();
}


Transmission::Transmission (Process *p_src, Process *p_dst, int channel, int n, int m, int tau) {
    this->p_src    = p_src;
    this->p_dst    = p_dst;
    this->node_src = p_src->getNode();
    this->node_dst = p_dst->getNode();
    this->channel  = channel;
    this->n        = n;
    this->m        = m;
    this->tau      = tau;
    
    this->params = TauLopParam::getInstance();
}


Transmission::Transmission (Process *p_src, Process *p_dst, int n, int m, int tau) {
    this->p_src    = p_src;
    this->p_dst    = p_dst;
    this->node_src = p_src->getNode();
    this->node_dst = p_dst->getNode();
    this->channel = (this->node_src == this->node_dst) ? 0 : 1;
    this->n        = n;
    this->m        = m;
    this->tau      = tau;
    
    this->params = TauLopParam::getInstance();
}

Transmission::Transmission (Process *p_src, Process *p_dst, int m, int tau) {
    this->p_src    = p_src;
    this->p_dst    = p_dst;
    this->node_src = p_src->getNode();
    this->node_dst = p_dst->getNode();
    this->channel = (this->node_src == this->node_dst) ? 0 : 1;
    this->n        = 1;
    this->m        = m;
    this->tau      = tau;
    
    this->params = TauLopParam::getInstance();
}

Transmission::Transmission (int channel, int n, int m, int tau) {
    this->p_src    = NULL;
    this->p_dst    = NULL;
    this->node_src = NODE_UNDEFINED;
    this->node_dst = NODE_UNDEFINED;
    this->channel  = channel;
    this->n        = n;
    this->m        = m;
    this->tau      = tau;
    
    this->params = TauLopParam::getInstance();
}

Transmission::Transmission (int channel, int m, int tau) {
    this->p_src    = NULL;
    this->p_dst    = NULL;
    this->node_src = NODE_UNDEFINED;
    this->node_dst = NODE_UNDEFINED;
    this->channel  = channel;
    this->n        = 1;
    this->m        = m;
    this->tau      = tau;
    
    this->params = TauLopParam::getInstance();
}


Transmission::Transmission (const Transmission *c) {
    this->p_src    = c->p_src;
    this->p_dst    = c->p_dst;
    this->node_src = c->node_src;
    this->node_dst = c->node_dst;
    this->channel  = c->channel;
    this->n        = c->n;
    this->m        = c->m;
    this->tau      = c->tau;
    
    this->params = TauLopParam::getInstance();
}


Transmission::~Transmission () {
    
}


void Transmission::putProcSrc (Process *p) {
    this->p_src = p;
}


void Transmission::putProcDst (Process *p) {
    this->p_dst = p;
}


int Transmission::getMsgSize () {
    return (this->n);
}


void Transmission::putChannel (int channel) {
    this->channel = channel;
}

int Transmission::getChannel () {
    return this->channel;
}


int Transmission::getSrcRank () {
    
    int rank = -1;
    
    if (this->p_src != NULL) {
        rank = this->p_src->getRank();
    }
    
    return rank;
}


int Transmission::getDstRank () {

    int rank = -1;
    
    if (this->p_dst != NULL) {
        rank = this->p_dst->getRank();
    }
    
    return rank;
}


int Transmission::getSrcNode () {
    return this->node_src;
}

int Transmission::getDstNode () {
    return this->node_dst;
}


void Transmission::putM (long m) {
    this->m = m;
}

long Transmission::getM () {
    return this->m;
}


void Transmission::putN (int n) {
    this->n = n;
}

int Transmission::getN () {
    return this->n;
}


void Transmission::initTau () {
    this->tau = 0;
}

void Transmission::incrTau (int inc) {
    this->tau += inc;
}

int Transmission::getTau() {
    return this->tau;
}


double Transmission::getCost () {
    
    double t = 0.0;
    
    t = this->params->getTime(m, tau, channel);
    
    return t * n;
}


long Transmission::getBytes (double t, int tau) {
  
    long b = 0;
    
    b = this->params->getBytes(t, tau, channel);
    
    return b;
}


bool Transmission::areConcurrent (const Transmission *c) {
    
    // Condition for considering two communications as concurrent
    if ( (this->channel == c->channel) &&
        (this->node_dst == c->node_dst) )
        
    //if (this->channel == c->channel)
        return true;
    
    return false;
}


void Transmission::getOverlap (const Transmission *c) {

    // Create a communication representing the overlap between two concurrent
    //  communications through the same channel.
    // They are concurrent over the minimum value of m and tau is increased.
    // Both communications have the same n
    if (this->n != c->n)
        cout << "DBG Error: Comms have to have the same m" << endl;
    
    if (this->m > c->m) {
        this->m = c->m;
    }
    
    this->tau += c->tau;
}


bool Transmission::areCompactable (const Transmission *c) {
    
    if ((this->tau == c->tau) && (this->channel == c->channel))
        return true;
    else return false;
}


void Transmission::compact (const Transmission *c) {
    // Used for showing. Compact comms in the same channel and with the same tau
    //  Result has m=1, and all the wieght is in n
    this->m = this->m + (c->m * c->n);
}


bool Transmission::areSequential (const Transmission *c) {

    int r1_src = -1;
    int r2_src = -1;
    
    if (this->p_src != NULL)  r1_src = p_src->getRank();
    if (c->p_src    != NULL)  r2_src = c->p_src->getRank();
    
    if (r1_src == r2_src)
        return true;
    return false;
}


void Transmission::add (const Transmission *c) {
    // Communications must have the same n
    cout << "TBD: Comm::add" << endl;
}



void Transmission::show () {
    
    if ((p_src != NULL) && (p_dst != NULL)) {
        cout << p_src->getRank() << " -> " << p_dst->getRank() << endl;
    } else {
        cout << "X -> Y" << endl;
    }
    cout << channel << endl;
    cout << this->m << "  " << this->n << endl;
    cout << "(" << this->n * this->m << ")" << endl;
    cout << this->tau << " ||" << endl;
    cout << this->node_dst << endl;
    cout << "t= " << this->getCost() << endl;
    cout << endl;
}

