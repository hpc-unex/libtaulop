//
//  taulop_params_channel.hpp
//  TauLopCost
//
//  Created by jarico on 18/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef taulop_params_channel_hpp
#define taulop_params_channel_hpp

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class TaulopParamChannel {
    
private:
    
    double        *o;       // Array of overhead values per message size
    double       **L;       // Matrix of transfer times per message size and tau
    vector<long>   S;       // Vetor of sizes
    
    int       num_m;        // Number of message size entries (lines in the file)
    int       num_tau;      // Number of tau entries (columns in the file)
    int       channel_nr;   // Channel number
    
    string    channel;      // Channel name
    
    
    // Load data from file with parameters
    void loadData  (string channel); // From file <channel.txt>
    
public:
    
     TaulopParamChannel (string channel, int channel_nr);
    ~TaulopParamChannel ();
    
    double       getO      (int idx);
    double       getL      (int idx, int tau);
    
    int          getNumM   ();
    int          getNumTau ();
    vector<long> getSizes  ();

    void  show ();
};

#endif /* taulop_params_channel_hpp */
