//
//  communicator.cpp
//  taulop
//
//  Created by jarico on 17/11/16.
//  Copyright © 2016 University of Extremadura. All rights reserved.
//

#include "communicator.hpp"
#include "config.h"

#include <iostream>
using namespace std;


Communicator::Communicator  (int P) {
    
    this->P = P;
    this->mapping = new Mapping(P);
    
    this->ranks = new int [P];
    
    for (int p = 0; p < P; p++) {
        this->ranks[p] = p;
    }
}


Communicator::~Communicator () {
    delete [] this->ranks;
    delete this->mapping;
}

// TODO: There is a problem with the meaning of the Q parameter. It means number-of-cores when SEQ,
//       number-of-nodes when RR. Change the interfaz or the parameter for solving this issue.
Communicator *Communicator::create (int P, int *ranks) {
    
    Communicator *new_comm = new Communicator (P);
    int *nodes = new int [new_comm->P];
    
    for (int p = 0; p < P; p++) {
        new_comm->ranks[p] = this->ranks[ranks[p]];
        nodes[p] = this->mapping->getNode(ranks[p]);
    }
    
    new_comm->mapping = new Mapping (new_comm->P, nodes);
    
    delete [] nodes;
    return new_comm;
}


void Communicator::map (Mapping *map) {
// TODO: Verify if size of mapping is equal to P
    mapping = *map; // assignation overloaded
}


void Communicator::getMap (Mapping *map) {
    *map = *mapping; // assignation overloaded
}


int Communicator::getRank (int p) {
    return this->ranks[p];
}


int Communicator::getSize () {
    return this->P;
}


int Communicator::getNode (int p) {
    return this->mapping->getNode(p);
}


void Communicator::show () {
    
    cout << "Communicator. P = " << this->P << "  Mapping: " << this->mapping << endl;
    cout << "Ranks: [";
    for (int p = 0; p < this->P; p++) {
        cout << this->ranks[p];
        if (p != P-1) cout << ", ";
    }
    cout << "]" << endl;

    this->mapping->show();
}
