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




int main_MxM_combinatorial (int argc, const char * argv[]) {
    
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
    
    
    {
        int col_alg = BCAST_BIN_OPENMPI;
        
        //column_algorithm = col_alg;
        
        string comm_mode;
        
        switch (col_alg) {
                
            case PT2PT:
                comm_mode = "p2p";
                break;
                
            case RING:
                comm_mode = "ring";
                break;
                
            case BCAST_BIN_OPENMPI:
                comm_mode = "bcast";
                break;
                
            default:
                cout << "ERROR:  communication mode!!! " << col_alg << endl;
                exit(1);
        }
        
        Matrix *m = new Matrix ();
        //m->load(); // Synthetic matrix for testing
        m->load("/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/part_M2_8_MxM_1D_synt/part_512_2_TCP.2dist");
        
        // !. Algorithms
        Arrangement *r_orig = m->getArrangement(0);
        
        cout << endl << "=========================================" << endl;
        cout << "Communication mode: " << comm_mode << endl;
        cout << "Network: " << network << endl;
        cout << "N: " << r_orig->getWidth() << " x " << r_orig->getHeight() << endl;
        cout << endl << "=========================================" << endl;
        
        
        // 2. Find out the best arrangment using half-perimeter and Beaumont algorithm
        Algorithm *alg_tlop = new MxM_Algorithm ();
        Cost *c_tlop = alg_tlop->execute (r_orig);
        cout << fixed << setprecision(3) << "Time: " << c_tlop->getTime() / 1000000.0 << endl;
        r_orig->show();
        
        //string squares_file = "/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/alpha.gpl";
        //r_orig->plot(squares_file);
        
        
        // 3. Improve data partition using Heuristics
        cout << endl << "Combinatorial Heuristic (tau-Lop) _____________________" << endl;
        Heuristic *hm = new CombinatorialHeuristic (alg_tlop, comm_mode, network);
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
        delete (MxM_Algorithm *)alg_tlop;
        
    }
    
    return 0;
    

    
    
    
    
    
    
    network = "IB";
    TauLopParam::setInstance(network);
    

    for (int col_alg = PT2PT; col_alg <= BCAST_BIN_OPENMPI; col_alg++) {
        
        if (col_alg == BCAST_BIN_MPICH) continue;
        
        string comm_mode;
        
        switch (col_alg) {
                
            case PT2PT:
                comm_mode = "p2p";
                break;
                
            case RING:
                comm_mode = "ring";
                break;
                
            case BCAST_BIN_OPENMPI:
                comm_mode = "bcast";
                break;
                
            default:
                cout << "ERROR:  communication mode!!! " << col_alg << endl;
                exit(1);
        }
        
        Matrix *m = new Matrix ();
        //m->load(); // Synthetic matrix for testing
        m->load("/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/part_M6_18/part_256_6_IB.2dist");
        
        // !. Algorithms
        Arrangement *r_orig = m->getArrangement(0);
        
        cout << endl << "=========================================" << endl;
        cout << "Communication mode: " << comm_mode << endl;
        cout << "Network: " << network << endl;
        cout << "N: " << r_orig->getWidth() << "x" << r_orig->getHeight() << endl;
        cout << endl << "=========================================" << endl;

        
        // 2. Find out the best arrangment using half-perimeter and Beaumont algorithm
        Algorithm *alg_tlop = new MxM_Algorithm ();
        Cost *c_tlop = alg_tlop->execute (r_orig);
        cout << fixed << setprecision(3) << "Time: " << c_tlop->getTime() / 1000000.0 << endl;
        r_orig->show();
        
        //string squares_file = "/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/alpha.gpl";
        //r_orig->plot(squares_file);
        
        
        // 3. Improve data partition using Heuristics
        cout << endl << "Combinatorial Heuristic (tau-Lop) _____________________" << endl;
        Heuristic *hm = new CombinatorialHeuristic (alg_tlop, comm_mode, network);
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
        delete (MxM_Algorithm *)alg_tlop;
        
    }
    

    
    
    
    
    network = "IB";
    TauLopParam::setInstance(network);
    
    
    for (int col_alg = PT2PT; col_alg <= BCAST_BIN_OPENMPI; col_alg++) {
        
        if (col_alg == BCAST_BIN_MPICH) continue;
        
        string comm_mode;
        
        switch (col_alg) {
                
            case PT2PT:
                comm_mode = "p2p";
                break;
                
            case RING:
                comm_mode = "ring";
                break;
                
            case BCAST_BIN_OPENMPI:
                comm_mode = "bcast";
                break;
                
            default:
                cout << "ERROR:  communication mode!!! " << col_alg << endl;
                exit(1);
        }
        
        Matrix *m = new Matrix ();
        //m->load(); // Synthetic matrix for testing
        m->load("/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/part_M6_18/part_512_6_IB.2dist");
        
        // !. Algorithms
        Arrangement *r_orig = m->getArrangement(0);
        
        cout << endl << "=========================================" << endl;
        cout << "Communication mode: " << comm_mode << endl;
        cout << "Network: " << network << endl;
        cout << "N: " << r_orig->getWidth() << "x" << r_orig->getHeight() << endl;
        cout << endl << "=========================================" << endl;
        
        
        // 2. Find out the best arrangment using half-perimeter and Beaumont algorithm
        Algorithm *alg_tlop = new MxM_Algorithm ();
        Cost *c_tlop = alg_tlop->execute (r_orig);
        cout << fixed << setprecision(3) << "Time: " << c_tlop->getTime() / 1000000.0 << endl;
        r_orig->show();
        
        //string squares_file = "/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/alpha.gpl";
        //r_orig->plot(squares_file);
        
        
        // 3. Improve data partition using Heuristics
        cout << endl << "Combinatorial Heuristic (tau-Lop) _____________________" << endl;
        Heuristic *hm = new CombinatorialHeuristic (alg_tlop, comm_mode, network);
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
        delete (MxM_Algorithm *)alg_tlop;
        
    }

    
    
    
    
    
    network = "TCP";
    TauLopParam::setInstance(network);
    
    
    for (int col_alg = PT2PT; col_alg <= BCAST_BIN_OPENMPI; col_alg++) {
        
        if (col_alg == BCAST_BIN_MPICH) continue;
        
        string comm_mode;
        
        switch (col_alg) {
                
            case PT2PT:
                comm_mode = "p2p";
                break;
                
            case RING:
                comm_mode = "ring";
                break;
                
            case BCAST_BIN_OPENMPI:
                comm_mode = "bcast";
                break;
                
            default:
                cout << "ERROR:  communication mode!!! " << col_alg << endl;
                exit(1);
        }
        
        Matrix *m = new Matrix ();
        //m->load(); // Synthetic matrix for testing
        m->load("/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/part_M6_18/part_256_6_TCP.2dist");
        
        // !. Algorithms
        Arrangement *r_orig = m->getArrangement(0);
        
        cout << endl << "=========================================" << endl;
        cout << "Communication mode: " << comm_mode << endl;
        cout << "Network: " << network << endl;
        cout << "N: " << r_orig->getWidth() << "x" << r_orig->getHeight() << endl;
        cout << endl << "=========================================" << endl;
        
        
        // 2. Find out the best arrangment using half-perimeter and Beaumont algorithm
        Algorithm *alg_tlop = new MxM_Algorithm ();
        Cost *c_tlop = alg_tlop->execute (r_orig);
        cout << fixed << setprecision(3) << "Time: " << c_tlop->getTime() / 1000000.0 << endl;
        r_orig->show();
        
        //string squares_file = "/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/alpha.gpl";
        //r_orig->plot(squares_file);
        
        
        // 3. Improve data partition using Heuristics
        cout << endl << "Combinatorial Heuristic (tau-Lop) _____________________" << endl;
        Heuristic *hm = new CombinatorialHeuristic (alg_tlop, comm_mode, network);
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
        delete (MxM_Algorithm *)alg_tlop;
        
    }

    
    
    
    
    
    network = "TCP";
    TauLopParam::setInstance(network);
    
    
    for (int col_alg = PT2PT; col_alg <= BCAST_BIN_OPENMPI; col_alg++) {
        
        if (col_alg == BCAST_BIN_MPICH) continue;
        
        string comm_mode;
        
        switch (col_alg) {
                
            case PT2PT:
                comm_mode = "p2p";
                break;
                
            case RING:
                comm_mode = "ring";
                break;
                
            case BCAST_BIN_OPENMPI:
                comm_mode = "bcast";
                break;
                
            default:
                cout << "ERROR:  communication mode!!! " << col_alg << endl;
                exit(1);
        }
        
        Matrix *m = new Matrix ();
        //m->load(); // Synthetic matrix for testing
        m->load("/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/part_M6_18/part_512_6_TCP.2dist");
        
        // !. Algorithms
        Arrangement *r_orig = m->getArrangement(0);
        
        cout << endl << "=========================================" << endl;
        cout << "Communication mode: " << comm_mode << endl;
        cout << "Network: " << network << endl;
        cout << "N: " << r_orig->getWidth() << "x" << r_orig->getHeight() << endl;
        cout << endl << "=========================================" << endl;
        
        
        // 2. Find out the best arrangment using half-perimeter and Beaumont algorithm
        Algorithm *alg_tlop = new MxM_Algorithm ();
        Cost *c_tlop = alg_tlop->execute (r_orig);
        cout << fixed << setprecision(3) << "Time: " << c_tlop->getTime() / 1000000.0 << endl;
        r_orig->show();
        
        //string squares_file = "/Users/jarico/Google Drive/code_tau_lop/taulop_code/ccgrid/alpha.gpl";
        //r_orig->plot(squares_file);
        
        
        // 3. Improve data partition using Heuristics
        cout << endl << "Combinatorial Heuristic (tau-Lop) _____________________" << endl;
        Heuristic *hm = new CombinatorialHeuristic (alg_tlop, comm_mode, network);
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
        delete (MxM_Algorithm *)alg_tlop;
        
    }

    
    return 0;
}
