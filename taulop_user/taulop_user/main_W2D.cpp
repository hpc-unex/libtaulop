//
//  main_W2D.cpp
//  TauLopCost
//
//  Created by jarico on 18/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//
//  Finite Elements communication cost prediction
//     The code calculates only ONE iteration of the Finite Elements algorithm.
//

#include "matrix.hpp"
#include "algorithm.hpp"
#include "arrangement.hpp"
#include "cost.hpp"

#include "algorithm_W2D.hpp"

#include "taulop_params.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;







int main_W2D (int argc, const char * argv[]) {

    string folder = argv[1];
    string part_file = folder + "/part.2dist";
    
    string kernel = argv[2];
    string mode   = argv[3];
    string net    = argv[4];
    string blocks = argv[5];
    string name   = kernel + "_" + mode + "_" + net + "_" + blocks;
    
    string squares_file = folder + "/output/squares_" + name + ".gpl";
/*
    string folder = "./";
    string kernel = "W2D";
    string mode = "Barrier";
    string blocks = "128";
    string net = "IB";
    string part_file = "p.2dist";
    string name   = kernel + "_" + mode + "_" + net + "_" + blocks;
    string squares_file = folder + name + ".gpl";
    */
    
    TauLopParam::setInstance(net);
    
    // 1. Original arrangement and creation of algorithms.
    Matrix *m = new Matrix ();
    m->load(part_file);
    
    Arrangement *r_orig = m->getArrangement(0);
    r_orig->plot(squares_file);

    
    // Output file
    string output_file = folder + "/output/tlop_times_" + name + ".txt";
    ofstream ofs;
    ofs.open(output_file);
    ofs << "#[t-Lop] Configuration: " << name << endl;
    ofs << "# \t  -  (V) \t  -  (H) \t Time (sec) " << endl;
    

    // 2. Calculate the t-Lop cost
    Algorithm *alg_tlop = new W2D_Algorithm ();
    // r_orig->show();
    cout << "\n[tau-Lop] Configuration: " << name << endl;
    Cost *c_tlop = alg_tlop->execute (r_orig);
    cout << fixed << setprecision(9) << "Time: " << c_tlop->getTime() / 1000000.0 << endl;
    r_orig->show();

    
    // Write the file with only one data (time predicted for the configuration)
    ofs << fixed << setprecision(9) << "MAX \t " << c_tlop->getTimeVert()  / 1000000.0 << " \t "
                                                 << c_tlop->getTimeHoriz() / 1000000.0 << " \t "
                                                 << c_tlop->getTime()      / 1000000.0 << endl;
    ofs.close();

    // 3. Delete structures and objects !!!
    delete m;
    delete c_tlop;
    delete (W2D_Algorithm *)alg_tlop;
    
    return 0;
}
