//
//  taulop_params.cpp
//  TauLopCost
//
//  Created by jarico on 15/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "taulop_params.hpp"

#include <cmath>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;


// Initialization of tatic variables
TauLopParam* TauLopParam::single = nullptr;
string TauLopParam::network = "";



// PRIVATE methods

TauLopParam::TauLopParam() {
        
    this->loadSizes();
    
    //private constructor
    this->v_chn[0] = new TaulopParamChannel (TauLopParam::network, 0, this->max_idx);
    this->v_chn[1] = new TaulopParamChannel (TauLopParam::network, 1, this->max_idx);
    
    this->max_tau = 8;  // TBD
    
    this->p2p_T0 = new double * [this->max_tau];
    
    for (int i = 0; i < this->max_tau; i++) {
        this->p2p_T0[i] = new double [this->max_idx];
    }

    this->p2p_T1 = new double * [this->max_tau];
    
    for (int i = 0; i < this->max_tau; i++) {
        this->p2p_T1[i] = new double [this->max_idx];
    }

    this->setP2P();
#if TLOP_DEBUG == 1
    show();
#endif
}



void TauLopParam::loadSizes () {
    
    ifstream ifs;
    
    string name = "sizes.txt";
    
    name = params_folder + TauLopParam::network + "/" + name;
    
    ifs.open(name.c_str());
    if (!ifs.is_open()) {
        cout << "ERROR: file SIZES " << name << " not opened" << endl;
        return;
    }
    
    string line;
    
    // 1. Count lines. First size will be always m=1.
    int num_sizes  = 1;
    
    // Comment line (tau line ??)  TBD
    //getline(ifs, line);
    
    while (!ifs.eof()) {
        
        getline(ifs, line);
        if (ifs.eof()) break;
        
        if (atoi(line.c_str()) == 1)  continue;
        
        num_sizes++;
    }
    
    ifs.close();
    ifs.open(name.c_str());
    
    // 3. Read sizes
    int idx = 1;
    
    // 3a. Make room for sizes vector
    this->sizes = new long [num_sizes];
    this->max_idx = num_sizes;
    this->sizes[0] = 1;
    
    // Comment line (tau line ??? )  TBD
    //getline(ifs, line);
    
    while (!ifs.eof()) {
        
        getline(ifs, line);
        if (ifs.eof()) break;
        
        long n = atoi(line.c_str());
        
        if (n == 1)  continue;
        
        this->sizes[idx++] = n;
    }
    
    ifs.close();
}



void TauLopParam::setP2P  () {
    
    double T = 0.0;
    
    double o0 = 0.0, o1 = 0.0;
    double L0 = 0.0, L1 = 0.0;
    
    int  chn;
    int  tau;
    
    chn = 0;
    
    for (tau = 0; tau < this->max_tau; tau++) {
        
        for (int idx = 0; idx < this->max_idx; idx++) {
            
            o0 = this->v_chn[chn]->getO(idx);
            L0 = this->v_chn[chn]->getL(idx, tau);
            
            T = o0 + (2 * L0);
            this->p2p_T0[tau][idx] = T;
        }
    }
    
    
    chn = 1;
    
    for (tau = 0; tau < this->max_tau; tau++) {
        
        for (int idx = 0; idx < this->max_idx; idx++) {
            
            o1 = this->v_chn[chn]->getO(idx);
            L0 = this->v_chn[0]->getL(idx, tau);
            L1 = this->v_chn[chn]->getL(idx, tau);
            
            if (TauLopParam::network == TCP_NET) {
                //T = o1 + (2 * L0) + L1;  // RICO: PROB. MENSAJES PEQUEÑOS
                T = (2 * L0) + L1;
            } else if (TauLopParam::network == IB_NET) {
                //T = o1 + L1;
                T = L1;  // RICO: PROB. MENSAJES PEQUEÑOS
            } else {
                cerr << "ERROR: no network specified (matrix_config.h)" << endl;
            }
            this->p2p_T1[tau][idx] = T;
        }
    }
}



// PUBLIC interface

TauLopParam::~TauLopParam () {
    
    TauLopParam::single = nullptr;
    
    delete [] this->v_chn[0];
    delete [] this->v_chn[1];
    
    for (int i = 0; i < this->max_tau; i++) {
        delete [] this->p2p_T0[i];
    }
    delete [] this->p2p_T0;

    for (int i = 0; i < this->max_tau; i++) {
        delete [] this->p2p_T1[i];
    }
    delete [] this->p2p_T1;

}



void TauLopParam::setInstance(string network) {
    
    if (!TauLopParam::single) {
        TauLopParam::network = network;
        TauLopParam::single = new TauLopParam();
    } else {
        cerr << "ERROR: network parameters already loaded from " << TauLopParam::network << endl;
    }
}


TauLopParam* TauLopParam::getInstance() {
    
    if (!TauLopParam::single) {
        TauLopParam::single = new TauLopParam();
        return single;
    } else {
        return single;
    }
}



double TauLopParam::getTime  (long n, int tau, int chn) {
    
    double T = 0.0;
    double **p2p;
    int idx;

    // RICO: TBD -> Quitar esto, debe funcionar para mas de 8.
    if (tau >= 8) tau = 7;
    // ********************************************************
    
    if (chn == 0) {
        p2p = this->p2p_T0;
    } else {
        p2p = this->p2p_T1;
    }
    
    
    for (idx = 0; idx < this->max_idx; idx++) {
        if (this->sizes[idx] >= n) break;
    }
    
    if (this->sizes[idx] == n) {
        T = p2p[tau-1][idx];
    }
    
    else if (idx == 0) {
        T = p2p[tau-1][0];
    }
    
    else if (idx == this->max_idx) {
        T = p2p[tau-1][idx-1] + getTime(n - this->sizes[this->max_idx-1], tau, chn);
    }
    
    else {
        
        int idx_up = idx;
        int idx_dw = idx - 1;
        
        double t_up = p2p[tau-1][idx_up];
        double t_dw = p2p[tau-1][idx_dw];
        
        long n_up = this->sizes[idx_up];
        long n_dw = this->sizes[idx_dw];
        
        T = t_dw + ((n - n_dw) * (t_up - t_dw)) / (n_up - n_dw);
        
        // Security rule: if T < 0, t_up < t_dw, then use t_dw.
        if (T < 0) {
            T = t_dw;
        }
    }
    
    return T;
}


long TauLopParam::getBytes (double t, int tau, int chn) {
    
    long  b = 0;
    int   idx;
    
    double **p2p;

    // RICO: TBD -> Quitar esto, debe funcionar para mas de 8.
    if (tau >= 8) tau = 7;
    // ********************************************************

    if (chn == 0) {
        p2p = this->p2p_T0;
    } else {
        p2p = this->p2p_T1;
    }

    for (idx = 0; idx < this->max_idx; idx++) {
        if (p2p[tau-1][idx] > t) break;
    }
    
    if (idx == 0) {
        return 1; // Time short. Only 1 block.
    }
    
    if (idx == this->max_idx) {
        
        double t_max = p2p[tau-1][this->max_idx-1];
        b = this->sizes[this->max_idx-1] + getBytes(t - t_max, tau, chn);
        
    } else { // Time is in an interval
        
        long b_up = this->sizes[idx];
        double t_up = p2p[tau-1][idx];
        
        long b_dw = this->sizes[idx-1];
        double t_dw = p2p[tau-1][idx-1];
        
        b = b_dw + ceil((t - t_dw) * (b_up - b_dw)) / (t_up - t_dw);
        
        if (b < 1) {
            b = 1;
        }
    }
    
    return b;
}


void TauLopParam::show () {
    
    cout << "Sizes: ";
    for (int i = 0; i < this->max_idx; i++) {
        cout << sizes[i] << "  ";
    }
    cout << endl;
    
    
    cout << "P2P 0: " << endl;
    for (int i = 0; i < this->max_idx; i++) {
        for (int j = 0; j < this->max_tau; j++) {
            cout << this->p2p_T0[j][i] << "  ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "P2P 1: " << endl;
    for (int i = 0; i < this->max_idx; i++) {
        for (int j = 0; j < this->max_tau; j++) {
            cout << this->p2p_T1[j][i] << "  ";
        }
        cout << endl;
    }
    cout << endl;

}




