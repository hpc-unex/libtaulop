//
//  algorithm_MxM.cpp
//
//  Created by jarico on 5/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "algorithm_MxM.hpp"

#include "cost.hpp"
#include "algorithm.hpp"
#include "arrangement.hpp"
#include "taulop_cost.hpp"
#include "taulop_operator.hpp"
#include "algorithm_taulop_pbr.hpp"
#include "algorithm_taulop_pbc.hpp"

#include <iostream>
using namespace std;




MxM_Algorithm::MxM_Algorithm () {
    
}

MxM_Algorithm::~MxM_Algorithm () {
    
    
}


Cost * MxM_Algorithm::execute (Arrangement *r) {
    
    double tvert  = 0.0;
    double thoriz = 0.0;
    
    // Column (vertical) communications (Matrix B)
    TauLopPBR *pbr = new TauLopPBR ();
    pbr->getPBRCost(r, tvert);
    
    // Row (horizontal) communications (Matrix A)
    TauLopPBC *pbc = new TauLopPBC ();
    pbc->getPBCCost(r, thoriz);
    
    
    // Show
    double tm = tvert + thoriz;

    Cost *t = new Cost(0, tm);

    // Fill Cost object
    t->putTimeVert(tvert);
    t->putTimeHoriz(thoriz);
    t->putTime(tvert + thoriz);
    
    delete pbr;
    delete pbc;
    
    return t;
}

