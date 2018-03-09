//
//  mapping.cpp
//  taulop
//
//  Created by jarico on 17/11/16.
//  Copyright © 2016 University of Extremadura. All rights reserved.
//

#include "mapping.hpp"
#include "taulop_config.h"
#include <cstdlib>
#include <iostream>
using namespace std;


Mapping::Mapping  (int P) {
    this->P = P;
    this->nodes = new int [P];

    for (int p = 0; p < P; p++) {
        this->nodes[p] = 0;
    }
}


Mapping::Mapping  (int P, int *nodes) {
    
    this->P = P;
    this->nodes = new int [P];
    
    for (int p = 0; p < P; p++) {
        this->nodes[p] = nodes[p];
    }
}


Mapping::Mapping (int P, int Q, int predef_map) {

    this->P = P;
    this->nodes = new int [P];
    
    for (int p = 0; p < P; p++) {
        
        switch (predef_map) {
                
            case MAPPING_UNDEF:
                this->nodes[p] = rand() % Q;
                break;
                
            case MAPPING_SEQ:
                this->nodes[p] = p / Q;
                break;
                
            case MAPPING_RR:
                this->nodes[p] = p % Q;
                break;
                
            default:
                this->nodes[p] = 0;
        }
    }
}



Mapping::~Mapping () {
    delete this->nodes;
}


int Mapping::getNode (int p) {
    return this->nodes[p];
}


int Mapping::getP() {
    return this->P;
}



void Mapping::show () {
    
    cout << "Nodes: [";
    for (int p = 0; p < this->P; p++) {
        cout << this->nodes[p];
        if (p != P-1) cout << ", ";
    }
    cout << "]" << endl;

}

