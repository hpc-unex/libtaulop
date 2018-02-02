//
//  taulop_params_chhannel.cpp
//  TauLopCost
//
//  Created by jarico on 18/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "taulop_params_channel.hpp"
#include "config_taulop.h"
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;


// Utility definitions
const int OUT_OF_RANGE = -1;

const int TAUS  = 8;


static string toString(int numero) {
    stringstream flujo;
    string cadena;
    
    flujo << numero;
    flujo >> cadena;
    
    return cadena;
}


// PRIVATE methods


void TaulopParamChannel::loadOs (string network) {
    
    ifstream ifs;
    
    string name = "o" + toString(this->channel_nr) + "_m.txt";
    
    name = params_folder + network + "/" + name;
    
    ifs.open(name.c_str());
    if (!ifs.is_open()) {
        cout << "ERROR: file OVERHEADS " << name << " not opened" << endl;
        return;
    }
    
    // 1. Read o
    //   File must include an entry for the size m=1
    int idx = 0;
    
    // 1a. Make room for sizes vector
    this->o = new double [this->num_sizes];
    
    
    string line;
    while (!ifs.eof()) {
        
        getline(ifs, line);
        if (ifs.eof()) break;
        
        // Value
        double val = atof(line.c_str());
        this->o[idx] = val;

        idx++;
    }
    
    ifs.close();
}


void TaulopParamChannel::loadLs (string network) {
    
    ifstream ifs;
    
    string name = "L" + toString(this->channel_nr) + "_m_t.txt";
    
    name = params_folder + network + "/" + name;
    
    ifs.open(name.c_str());
    if (!ifs.is_open()) {
        cout << "ERROR: file Transfer Times " << name << " not opened" << endl;
        return;
    }
    
    string line, token;
    string delimiter = "\t";
    int pos;
    
    
    // 1. Read o
    //    File must include an entry for m=1 (first line)
    int idx = 0;
    
    // 1a. Make room for sizes vector
    this->L = new double * [this->num_sizes];
    
    // 1b. Comment line
    getline(ifs, line);
    
    while (!ifs.eof()) {
        
        getline(ifs, line);
        if (ifs.eof()) break;
        
        int tau = 0;
        
        this->L[idx] = new double [TAUS];
        
        while (tau < TAUS) {
            
            pos = (unsigned int)line.find(delimiter);
            token = line.substr(0, pos);
            this->L[idx][tau] = atof(token.c_str());
            line.erase(0, pos + delimiter.length());
            
            tau++;
        }
        
        idx++;
        if (idx == this->num_sizes) break;
    }
    
    ifs.close();
}


// PUBLIC interface

TaulopParamChannel::TaulopParamChannel (string network, int channel_nr, int num_sizes) {
    
    this->channel_nr = channel_nr;
    this->num_sizes  = num_sizes;
    
    // The file has not a defined format. By now (TBD), file names are:
    //   L:  L<chn>_m_t.txt
    //   o:  o<chn>_m.txt
    // Hence, not supported more than one option by channel (for instance TCP and IB).
    // Format of the files:
    //   1. Lines starting with # are coments
    //   2. Times are expected in microseconds (not mandatory).
    //   3. Decimal numbers are separated by dots (.)
    // L (Transfer times)
    //   First line is a comment
    //   Rest of lines are:  L(size,tau_1) L(size,tau_2) . . . L(size,tau_n)
    //   Line field delimiter is \t
    // o (Overhead)
    //   Rest of lines are:  Size o(size)
    // Sizes in o and L along all channels must be the same
    // All tau values must be covered.
    // First times (first line) is for size m=1
    // Number of lines in the file must match the sizes in sizes.txt
    // TBD: What about H and S values ?
    
    
    // 1. Overhead values
    this->loadOs(network);
    
    // 2. Transfer Time values
    this->loadLs(network);
    
#if TLOP_DEBUG == 1
    show();
#endif
    
}

TaulopParamChannel::~TaulopParamChannel () {
    
    cout << "TBD: deletes in TaulopParamChannel destructor" << endl;
    
    delete [] this->o;
    
    for (int i = 0; i < this->num_sizes; i++) {
        delete [] this->L[i];
    }
    delete [] this->L;
}


double TaulopParamChannel::getO (int idx) {

    if (idx >= this->num_sizes)  return OUT_OF_RANGE;
    
    return this->o[idx];
}


double TaulopParamChannel::getL (int idx, int tau) {
    
    if (idx >= this->num_sizes)  return OUT_OF_RANGE;
    if (tau >  TAUS)             return OUT_OF_RANGE;
    
    return this->L[idx][tau];
}



void TaulopParamChannel::show () {
    
    cout << "Overhead (o): ";
    for (int i = 0; i < this->num_sizes; i++) {
        cout << o[i] << "  ";
    }
    cout << endl;
    
    cout << "Transfer times (L): " << endl;
    for (int i = 0; i < this->num_sizes; i++) {
        for (int j = 0; j < TAUS; j++) {
            cout << L[i][j] << "  ";
        }
        cout << endl;
    }
    cout << endl;
    
}


