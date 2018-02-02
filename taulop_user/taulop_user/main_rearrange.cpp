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


//int column_algorithm = PT2PT;
//int row_algorithm = RING;


int main (int argc, const char * argv[]) {
    
    if (argc != 7) {
        cerr << "ERROR: invalid number of parameters: " << argc << "  Expected: 6." << endl;
        cerr << "rearrange  folder  config  network  comm_mode  kernel  heuristic" << endl;
        return -1;
    }
    
    string  folder    = argv[1];
    string  config    = argv[2]; // Name prefix for files
    string  network   = argv[3];
    string  comm_mode = argv[4];
    string  kernel    = argv[5]; // Kernel to run
    string  heuristic = argv[6];
    
    string part_file             = folder + config + "_delta_part.2dist";
    string out_part_alpha_file   = folder + config + "_alpha_part.2dist";
    string out_part_omega_file   = folder + config + "_omega_part.2dist";
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
    Heuristic *hm = nullptr;
    if (heuristic == "Combinatorial") {
        cout << endl << "Combinatorial Heuristic (tau-Lop) _____________________" << endl;
        hm = new CombinatorialHeuristic (alg_tlop, comm_mode, network);
    } else if (heuristic == "Malik") {
        cout << endl << "Malik H2 Heuristic (tau-Lop) _____________________" << endl;
        hm = new MalikHeuristic (alg_tlop, comm_mode, network);
    }
    cout << "Number of combinations: " << hm->combinations(r_orig) << endl;
    Arrangement **rm = hm->apply(r_orig);

    /*
    cout << "DELTA arrangement: " << endl;
    Cost *om  = alg_tlop->execute (r_orig);
    cout << fixed << setprecision(6) << "Combinatorial (tau-Lop) Cost: " << om->getTime()  / 1000000.0 << endl;
    r_orig->show();
    r_orig->plot(out_square_alpha_file);
    r_orig->part2D(out_part_alpha_file);
    delete om;
     */
    
    // 4. Best arrangement
    cout << "ALPHA arrangement: " << endl;
    Arrangement *alpha = (Arrangement *)rm[0];
    Cost *cm  = alg_tlop->execute (alpha);
    cout << fixed << setprecision(6) << heuristic << " (tau-Lop) Cost: " << cm->getTime()  / 1000000.0 << endl;
    alpha->show();
    alpha->plot(out_square_alpha_file);
    alpha->part2D(out_part_alpha_file);

    // 5. OMEGA arrangement
    Arrangement *omega = nullptr;
    cout << "OMEGA arrangement: " << endl;
    if (heuristic == "Combinatorial") {
        omega = (Arrangement *)rm[1];
    } else if (heuristic == "Malik") {
        // Malik heuristic does not return the worst arrangement, so original is taken
        omega = (Arrangement *)r_orig;
    }
    cm  = alg_tlop->execute (omega);
    cout << fixed << setprecision(6) << heuristic << " (tau-Lop) Cost: " << cm->getTime()  / 1000000.0 << endl;
    omega->show();
    omega->plot(out_square_omega_file);
    omega->part2D(out_part_omega_file);

    
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

    if (heuristic == "Combinatorial") {
        delete (CombinatorialHeuristic *)hm;
    } else if (heuristic == "Malik") {
        delete (MalikHeuristic *)hm;
    }

    return 0;
}
