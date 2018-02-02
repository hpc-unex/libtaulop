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
#include "algorithm_W2D.hpp"
#include "algorithm_W1D.hpp"
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


int column_algorithm = PT2PT;
int row_algorithm = RING;



int main_MALIK (int argc, const char * argv[]) {
    
    if (argc != 6) {
        cerr << "ERROR: invalid number of parameters: " << argc << "  Expected: 5." << endl;
        cerr << "comb folder config network comm_mode kernel" << endl;
        return -1;
    }
    
    string  folder    = argv[1];
    string  config    = argv[2]; // Name prefix for files
    string  network   = argv[3];
    string  comm_mode = argv[4];
    string  kernel    = argv[5]; // Kernel to run
    
    string part_file             = folder + config + "_delta_part.2dist";
    string out_part_alpha_file   = folder + config + "_alpha_part.2dist";
    string out_part_omega_file   = folder + config + "_omega_part.2dist";
    string out_square_delta_file = folder + config + "_delta_squares.gpl";
    string out_square_alpha_file = folder + config + "_alpha_squares.gpl";
    string out_square_omega_file = folder + config + "_omega_squares.gpl";
    
    
    cout << "Part file: " << part_file << endl;
    
    
    TauLopParam::setInstance(network);
    
    Matrix *m = new Matrix ();
    //m->load(); // Synthetic matrix for testing
    m->load(part_file);
    
    // 1. Algorithms
    Arrangement *r_orig = m->getArrangement(0);
    
    r_orig->show();
    
    cout << endl
    << "=========================================" << endl;
    cout << "Comm. mode: " << comm_mode << endl;
    cout << "Network: "    << network << endl;
    cout << "N: "          << r_orig->getWidth() << "x" << r_orig->getHeight() << endl;
    cout << "Kernel: "     << kernel << endl;
    cout << "=========================================" << endl;
    
    
    // 2. New algorithm
    Algorithm *alg_tlop = nullptr;
    
    if        (kernel == "MxM") {
        alg_tlop = new MxM_Algorithm ();
    } else if (kernel == "MxM1D") {
        alg_tlop = new MxM_Algorithm ();
    } else if (kernel == "W2D") {
        alg_tlop = new W2D_Algorithm  ();
    } else if (kernel == "W1D") {
        alg_tlop = new W1D_Algorithm  ();
    } else {
        cerr << "ERROR: algorithm not found: " << kernel << endl;
        return -1;
    }
    
    if        (comm_mode == "p2p") {
        column_algorithm = PT2PT;
        row_algorithm = PT2PT;
    } else if (comm_mode == "ring") {
        column_algorithm = RING;
        row_algorithm = RING;
    } else if (comm_mode == "bcast") {
        column_algorithm = BCAST_BIN_OPENMPI;
        row_algorithm = RING;
    } else {
        cerr << "ERROR: communication pattern for columns not defined: " << column_algorithm << endl;
    }
    
    // 3. Improve data partition using Heuristics
    Heuristic *hc = new CombinatorialHeuristic (alg_tlop, comm_mode, network);
    cout << endl << "__________ Combinatorial Heuristic (tau-Lop): " << hc->combinations(r_orig) << endl;
    Arrangement **rc = hc->apply(r_orig);
    
    // 3.a) DELTA arrangement
    Cost *oc  = alg_tlop->execute (r_orig);
    cout << fixed << setprecision(6) << "DELTA arrangement: Combinatorial (tau-Lop) Cost: " << oc->getTime()  / 1000000.0 << endl;
    r_orig->show();
    r_orig->plot(out_square_delta_file);
    //r_orig->part2D(out_part_alpha_file);
    delete oc;
    
    
    // 3.b) ALPHA arrangement
    Arrangement *alphac = (Arrangement *)rc[0];
    Cost *cc  = alg_tlop->execute (alphac);
    cout << fixed << setprecision(6) << "ALPHA arrangement: Combinatorial (tau-Lop) Cost: " << cc->getTime()  / 1000000.0 << endl;
    alphac->show();
    alphac->plot(out_square_alpha_file);
    alphac->part2D(out_part_alpha_file);
    
    // 3.c) OMEGA arrangement
    Arrangement *omegac = (Arrangement *)rc[1];
    cc  = alg_tlop->execute (omegac);
    cout << fixed << setprecision(6) << "OMEGA arrangement: Combinatorial (tau-Lop) Cost: " << cc->getTime()  / 1000000.0 << endl;
    omegac->show();
    omegac->plot(out_square_omega_file);
    omegac->part2D(out_part_omega_file);

    
    // 4. Improve data partition using Malik Heuristics
    Heuristic *hm = new MalikHeuristic (alg_tlop, comm_mode, network);
    cout << endl << "__________ Malik Heuristic (tau-Lop): " << hm->combinations(r_orig) << endl;
    Arrangement **rm = hm->apply(r_orig);
    
    // 4.a) DELTA arrangement
    Cost *om  = alg_tlop->execute (r_orig);
    cout << fixed << setprecision(6) << "DELTA arrangement: Malik (tau-Lop) Cost: " << om->getTime()  / 1000000.0 << endl;
    r_orig->show();
    r_orig->plot(out_square_delta_file);
    r_orig->part2D(out_part_alpha_file);
    delete om;
    
    
    // 4.b) ALPHA arrangement
    Arrangement *alpham = (Arrangement *)rm[0];
    Cost *cm  = alg_tlop->execute (alpham);
    cout << fixed << setprecision(6) << "ALPHA arrangement: Malik (tau-Lop) Cost: " << cm->getTime()  / 1000000.0 << endl;
    alpham->show();
    alpham->plot(out_square_alpha_file);
    alpham->part2D(out_part_alpha_file);
    
    // 4.c) Malik does not return OMEGA arrangement, so use DELTA
    Arrangement *omegam = (Arrangement *)r_orig;
    cm  = alg_tlop->execute (omegam);
    cout << fixed << setprecision(6) << "OMEGA (DELTA) arrangement:  Malik (tau-Lop) Cost: " << cm->getTime()  / 1000000.0 << endl;
    omegam->show();
    omegam->plot(out_square_omega_file);
    omegam->part2D(out_part_omega_file);

    
    // 6. Delete structures and objects
    delete m;
    
    if        (kernel == "MxM") {
        delete (MxM_Algorithm *)alg_tlop;
    } else if (kernel == "MxM1D") {
        delete (MxM_Algorithm *)alg_tlop;
    } else if (kernel == "W2D") {
        delete (W2D_Algorithm *)alg_tlop;
    } else if (kernel == "W1D") {
        delete (W1D_Algorithm *)alg_tlop;
    }
    
    int i = 0;
    while (rm[i] != nullptr) {
        delete (Arrangement *)rm[i];
        i++;
    }
    
    return 0;
}
