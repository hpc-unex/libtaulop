//
//  main_MxM.cpp
//  TauLopCost
//
//  Created by jarico on 18/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//
//  Matrix Multiplication communication prediction cost
//

#include "matrix.hpp"
#include "algorithm.hpp"
#include "arrangement.hpp"
#include "cost.hpp"

#include "algorithm_MxM.hpp"
#include "algorithm_W2D.hpp"
#include "algorithm_W1D.hpp"

#include "taulop_params.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;


int main_PREDICT (int argc, const char * argv[]) {
    
    if (argc != 10) {
        cerr << "ERROR: Invalid nummber of arguments (" << argc << "). Expected: 9." << endl;
        return -1;
    }
    
    string folder    = argv[1];
    string kernel    = argv[2];
    string mode      = argv[3];
    string net       = argv[4];
    string sizes     = argv[5];
    string patts     = argv[6];
    string nodes     = argv[7];
    string blocks    = argv[8];
    string rtype     = argv[9]; // delta, alpha, omega
    string name   = kernel + "_M" + nodes + "_" + net + "_" + sizes + "_B" + blocks + "_" + patts + "_" + mode;
    
    string part_file = folder + name + "_" + rtype + "_part.2dist";
    
    string squares_file = folder + name + "_" + rtype + "_squares.gpl";

    TauLopParam::setInstance(net);
    
    // 1. Original arrangement and creation of algorithms.
    Matrix *m = new Matrix ();
    m->load(part_file);
    
    Arrangement *r_orig = m->getArrangement(0);
    r_orig->plot(squares_file);

    
    // Output file
    string output_file = folder + name + "_" + rtype + "_estim_tlop.txt";
    
    ofstream ofs;
    ofs.open(output_file);
    if (ofs.fail()) {
        cerr << "ERROR: creating file: " << output_file << endl;
        return -1;
    }
    ofs << "#[t-Lop] Configuration: " << name << endl;
    ofs << "# \t PBR (V) \t PBC (H) \t Time (sec) " << endl;
    

    // 2. Calculate the t-Lop cost
    Algorithm *alg_tlop = nullptr;
    
    if (kernel == "MxM") {
        alg_tlop = new MxM_Algorithm ();
    } else if (kernel == "MxM1D") {
        alg_tlop = new MxM_Algorithm ();
    } else if (kernel == "W2D") {
        alg_tlop = new W2D_Algorithm ();
    } else if (kernel == "W1D") {
        alg_tlop = new W1D_Algorithm ();
    } else {
        cerr << "ERROR: algorithm not found!: " << kernel << endl;
        return -1;
    }
    
    
    if        (patts == "p2p") {
        column_algorithm = PT2PT;
        row_algorithm = PT2PT;
    } else if (patts == "ring") {
        column_algorithm = RING;
        row_algorithm = RING;
    } else if (patts == "bcast") {
        column_algorithm = BCAST_BIN_OPENMPI;
        row_algorithm = RING;
    } else {
        cerr << "ERROR: communication pattern for columns not defined: " << column_algorithm << endl;
    }

    
    
    cout << "\n[tau-Lop] Configuration: " << name << "  Comm: " << patts << endl;
    Cost *c_tlop = alg_tlop->execute (r_orig);
    cout << fixed << setprecision(6) << "Time: " << c_tlop->getTime() / 1000000.0 << endl;
    r_orig->show();

    
    // Write the file with only one data (time predicted for the configuration)
    ofs << fixed << setprecision(6) << "MAX \t " << c_tlop->getTimeVert()  / 1000000.0 << " \t "
                                                 << c_tlop->getTimeHoriz() / 1000000.0 << " \t "
                                                 << c_tlop->getTime()      / 1000000.0 << endl;
    ofs.close();

    // 3. Delete structures and objects !!!
    delete m;
    delete c_tlop;
    
    if (kernel == "MxM") {
        delete (MxM_Algorithm *)alg_tlop;
    } else if (kernel == "MxM1D") {
        delete (MxM_Algorithm *)alg_tlop;
    } else if (kernel == "W2D") {
        delete (W2D_Algorithm *)alg_tlop;
    } else if (kernel == "W1D") {
        delete (W1D_Algorithm *)alg_tlop;
    }
    
    
    return 0;
}
