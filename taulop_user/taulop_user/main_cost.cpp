//
//  main.cpp
//  TauLopCost
//
//  Created by jarico on 18/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "matrix.hpp"
#include "algorithm.hpp"
#include "arrangement.hpp"
#include "cost.hpp"

#include "algorithm_MxM.hpp"

#include "taulop_params.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;


int main_cost_ (int argc, const char * argv[]) {
    
    TauLopParam::setInstance("IB");
    
    // 1. Original arrangement and creation of algorithms.
    Matrix *m = new Matrix ();
    m->load("/Users/jarico/Google Drive/code_tau_lop/tmp/part.2dist");
    
    Arrangement *r_orig = m->getArrangement(0);
    r_orig->show();
    r_orig->plot("/Users/jarico/Google Drive/code_tau_lop/tmp/squares.gpl");
    
    return 0;
    
    // 2. Calculate the t-Lop cost
    Algorithm *alg_tlop = new MxM_Algorithm ();
    cout << "tau-Lop Cost _____________________" << endl;
    Cost *c_tlop = alg_tlop->execute (r_orig);
    cout << fixed << setprecision(3) << "t-Lop Cost: " << c_tlop->getTime() / 1000000.0 << endl;

    // 3. Delete structures and objects !!!
    delete m;
    delete c_tlop;
    delete (MxM_Algorithm *)alg_tlop;
    
    return 0;
}

