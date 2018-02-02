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
#include "algorithm_W2D.hpp"
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




int main_tm (int argc, const char * argv[]) {
    
    string network;
    
    //    network = "IB";
    //    TauLopParam::setInstance(network);
    //
    //    Matrix *m = new Matrix ();
    //    //m->load(); // Synthetic matrix for testing
    //    m->load("/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/part_M2_8_MxM_1D_synt/part_256_2_IB.2dist");
    //
    //    // !. Algorithms
    //    Arrangement *r_orig = m->getArrangement(0);
    //
    //    // 2. Find out the best arrangment using half-perimeter and Beaumont algorithm
    //    Algorithm *alg_tlop = new MxM_Algorithm ();
    //    Cost *c_tlop = alg_tlop->execute (r_orig);
    //    cout << fixed << setprecision(3) << "Time: " << c_tlop->getTime() / 1000000.0 << endl;
    //    r_orig->show();
    //
    //    string squares_file = "/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/part_M2_8_MxM_1D_synt/part_256_2_IB.gpl";
    //    r_orig->plot(squares_file);
    //
    //
    //
    //    // 4. Delete structures and objects
    //    delete m;
    //    delete c_tlop;
    //    delete (MxM_Algorithm *)alg_tlop;
    //
    //
    //
    //    return 0;
    
    
    
    
    
    
    
    
    network = "TCP";
    TauLopParam::setInstance(network);
    
    
    Matrix *m = new Matrix ();
    //m->load(); // Synthetic matrix for testing
    m->load("/Users/jarico/Development/data/ciemat/kernels_32/W2D/M4_P16/part_256_TCP.2dist");
    
    // !. Algorithms
    Arrangement *r_orig = m->getArrangement(0);
    
    cout << endl << "=========================================" << endl;
    cout << "Network: " << network << endl;
    cout << "N: " << r_orig->getWidth() << "x" << r_orig->getHeight() << endl;
    cout << endl << "=========================================" << endl;
    
    
    // 2. Find out the best arrangment using half-perimeter and Beaumont algorithm
    Algorithm *alg_tlop = new W2D_Algorithm ();
    Cost *c_tlop = alg_tlop->execute (r_orig);
    cout << fixed << setprecision(3) << "Time (usec): " << c_tlop->getTime() / 1000000.0 << endl;
    r_orig->show();
    
    //string squares_file = "/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/alpha.gpl";
    //r_orig->plot(squares_file);
    
    
    // 3. Improve data partition using Heuristics
    cout << endl << "Combinatorial Heuristic (tau-Lop) _____________________" << endl;
    Heuristic *hm = new CombinatorialHeuristic (alg_tlop, "p2p", network);
    cout << "Number of combinations (Combinatorial): " << hm->combinations(r_orig) << endl;
    Arrangement **rm = hm->apply(r_orig);
    cout << "Best Combinatorial arrangement: " << endl;
    Cost *cm  = alg_tlop->execute (*rm);
    cout << fixed << setprecision(3) << "Combinatorial (tau-Lop) Cost: " << cm->getTime()  / 1000000.0 << endl;
    (*rm)->show();
    //rm->plot("/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/alphastar.gpl");
    //rm->part2D("/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/part_tmp.2dist");
    
    
    // 4. Delete structures and objects
    delete m;
    delete c_tlop;
    delete (W2D_Algorithm *)alg_tlop;
    
    
    
    return 0;
    
}
