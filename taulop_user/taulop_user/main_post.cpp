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

#include "algorithm_basic.hpp"
#include "algorithm_malik.hpp"
#include "algorithm_MxM.hpp"
#include "algorithm_half_perimeter.hpp"

#include "heuristic_malik.hpp"
#include "heuristic_combinatorial.hpp"
#include "heuristic_beaumont.hpp"
#include "heuristic_beaumont_mod.hpp"

#include "taulop_operator.hpp"
#include "taulop_cost.hpp"

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <fstream>
#include "ilist.h"
#include <string>
#include <sstream>
#include <limits>
#include <iomanip>
using namespace std;




int main_POST (int argc, const char * argv[]) {
    
    TauLopParam::setInstance("IB");
    
    Matrix *m = new Matrix ();
    //m->load(); // Synthetic matrix for testing
    m->load("/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/r_def.2dist");
    
    // !. Algorithms
    Arrangement *r_orig = m->getArrangement(0);
    
    // 2. Find out the best arrangment using half-perimeter and Beaumont algorithm
    Algorithm *alg_tlop = new MxM_Algorithm ();
    Cost *c_tlop = alg_tlop->execute (r_orig);
    cout << fixed << setprecision(3) << "Time: " << c_tlop->getTime() / 1000000.0 << endl;
    r_orig->show();
    
    string squares_file = "/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/r_def.gpl";
    r_orig->plot(squares_file);
    
    
    // 4. Delete structures and objects
    delete m;
    delete c_tlop;
    delete (MxM_Algorithm *)alg_tlop;
    
    return 0;
}
