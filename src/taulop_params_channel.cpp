//
//  taulop_params_chhannel.cpp
//  TauLopCost
//
//  Created by jarico on 18/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "taulop_params_channel.hpp"
#include "taulop_config.h"
#include "config.h"

#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
using namespace std;


// Utility definitions
const int OUT_OF_RANGE = -1;

vector<string> split(string strToSplit, char delimeter) {
    
    stringstream   ss(strToSplit);
    string         item;
    vector<string> splittedStrings;
    
    while (getline(ss, item, delimeter)) {
        splittedStrings.push_back(item);
    }
    
    return splittedStrings;
}


// PRIVATE methods

void TaulopParamChannel::loadData (string channel) {
    
    ifstream       ifs;
    vector<string> v;
    
    char delimiter = '\t';
    
    string params_folder (PARAMS_FOLDER);
    string name = params_folder + "/" + channel + ".txt";
    
    ifs.open(name);
    if (!ifs.is_open()) {
        cout << "ERROR: unable to open file containing parameters: " << name << endl;
        return;
    }
    
    string line;
    
    // Read number of m    (first line)
    getline(ifs, line);
    v = split(line, delimiter);
    this->num_m = stoi(v[1]);
    
    // Read number of tau  (second line)
    getline(ifs, line);
    v = split(line, delimiter);
    this->num_tau = stoi(v[1]);
    
    
    // 1. Read parameters
    //    File must include an entry for m=1 (first line)
    
    // 1a. Make room for sizes vector
    this->o = new double [this->num_m];
    
    // 1b. Make room for sizes vector
    this->L = new double * [this->num_m];
    
    int idx = 0;
    while (!ifs.eof()) {
        
        getline(ifs, line);
        if (line[0] == '#') continue;
        
        this->L[idx] = new double [this->num_tau];
        
        v = split(line, delimiter);
        
        // Sizes
        this->S.push_back(stof(v[0]));
        
        // Overhead
        this->o[idx] = stof(v[1]);
        
        // Transfer Time
        for (int tau = 0; tau < this->num_tau; tau++) {
            
            string token = v[tau + 2];
            this->L[idx][tau] = stof(token);
        }
        
        idx++;
        if (idx == this->num_m) break;
    }
    
    if (idx != this->num_m) {
        cout << "ERROR: file corrupt reading parameters: " << name << endl;
    }
    
    ifs.close();
}







// PUBLIC interface

TaulopParamChannel::TaulopParamChannel (string channel, int channel_nr) {
    
    // The file has a defined format. By now, file name is:
    //   <channel>.txt
    //
    // Assumptions (to be solved if possible):
    //
    // - Not supported more than one network by channel (for instance TCP and IB cannot be use together).
    // - First line contains the number of messages sizes measured:
    //   #m NUM
    // - Second line contains the number of taus measured:
    //   #tau NUM
    // - Comments on files must have a '#' in the first column.
    // - Times are expected in microseconds (not mandatory).
    // - Decimal numbers are separated by dots (.)
    // - Number of "m" and "tau" on all communication channels must be the same.
    // - All tau values must be included.
    // - First times (first line) is for size m=1 byte.
    
    
    this->channel_nr = channel_nr;
    this->channel    = channel;
    
    this->loadData(channel);
    
#if TLOP_DEBUG == 1
    show();
#endif
    
}

TaulopParamChannel::~TaulopParamChannel () {
    
    cout << "TBD: deletes in TaulopParamChannel destructor" << endl;
    
    delete [] this->o;
    
    for (int i = 0; i < this->num_m; i++) {
        delete [] this->L[i];
    }
    delete [] this->L;
}


double TaulopParamChannel::getO (int idx) {

    if (idx >= this->num_m)     return OUT_OF_RANGE;
    
    return this->o[idx];
}


double TaulopParamChannel::getL (int idx, int tau) {
    
    if (idx >= this->num_m)     return OUT_OF_RANGE;
    if (tau >  this->num_tau)   return OUT_OF_RANGE;
    
    return this->L[idx][tau];
}



int TaulopParamChannel::getNumM () {
    return this->num_m;
}


int TaulopParamChannel::getNumTau () {
    return this->num_tau;
}

vector<long> TaulopParamChannel::getSizes () {
    return this->S;
}


void TaulopParamChannel::show () {
    
    cout << "Overhead (o): ";
    for (int i = 0; i < this->num_m; i++) {
        cout << this->S[i] << ":  " << this->o[i] << endl;
    }
    
    cout << "Transfer times (L): " << endl;
    for (int i = 0; i < this->num_m; i++) {
        cout << this->S[i] << ":  ";
        for (int j = 0; j < this->num_tau; j++) {
            cout << this->L[i][j] << " \t ";
        }
        cout << endl;
    }
    cout << endl;
    
}


