//
//  process.cpp
//  TauLopCost
//
//  Created by jarico on 04/Nov/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "process.hpp"
#include "taulop_config.h"
#include <iostream>
using namespace std;


Process::Process () {
    this->rank = RANK_UNDEFINED;
    this->node = NODE_UNDEFINED;
}

Process::Process (int rank) {
    this->rank = rank;
    this->node = NODE_UNDEFINED;
}

Process::Process (int rank, int node) {
    this->rank = rank;
    this->node = node;
}


Process::Process (const Process *p) {
    this->rank      = p->rank;
    this->rankIntra = p->rankIntra;
    this->node      = p->node;
}


Process::~Process () {
}


void Process::setRankIntra (int rank) {
    this->rankIntra = rank;
}


void Process::setRank (int rank) {
    this->rank = rank;
}


void Process::setNode (int node) {
    this->node = node;
}


int Process::getRank ()  const {
    return this->rank;
}


int Process::getRankIntra ()  const {
    return this->rankIntra;
}


int Process::getNode ()  const {
    return this->node;
}


void Process::show () const {
    cout << "[Process: " << this->rank << "/" << this->rankIntra << " running on: " << this->node<< "] " << endl;
}

ostream& Process::operator<< (ostream& outs) {
    outs << " [" << this->rank << "]: ";
    return outs;
}


void Process::operator = (const Process &c) {
   this->rank      = c.rank;
   this->rankIntra = c.rankIntra;
   this->node      = c.node;
}
